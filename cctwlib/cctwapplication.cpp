#include "cctwapplication.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwtransformer.h"
#include <QScriptValue>
#include "cctwscriptengine.h"
#include "qcepproperty.h"
#include "cctwdebug.h"
#include "qcepsettingssaver.h"
#include <QtConcurrentRun>
#include <QFile>
#include "cctwintvector3dproperty.h"
#include "cctwdoublevector3dproperty.h"
#include "cctwdoublematrix3x3property.h"
#include "cctwunitcellproperty.h"
#include "cctwthread.h"
#include "cctwdatachunk.h"

#ifdef WANT_IMPORT_COMMANDS
#include "qcepimagedataformatcbf.h"
#include "qcepimagedataformatmar345.h"
#include "qcepimagedataformattiff.h"
#endif

#include <hdf5.h>
#include "lzf_filter.h"

#ifdef Q_OS_UNIX
#include "getopt.h"
#endif

QcepSettingsSaverPtr g_Saver;

#ifdef WANT_IMPORT_COMMANDS
QcepImageDataFormatCBF<double> cbfImg("cbf");
QcepImageDataFormatTiff<double> tiffImg("tiff");
QcepImageDataFormatMar345<double> mar345Img("mar345");
#endif

enum {
  TransformMode = 1,
  MergeMode     = 2,
  NormMode      = 3,
  ProjectMode   = 4
};

CctwApplication::CctwApplication(int &argc, char *argv[])
#ifdef NO_GUI
  : QCoreApplication(argc, argv),
#else
  : QApplication(argc, argv),
#endif
  m_ObjectNamer(this, "cctw"),
#ifndef NO_GUI
  m_Window(NULL),
#endif
  m_Parameters(NULL),
#ifdef WANT_IMPORT_COMMANDS
  m_ImportData(NULL),
#endif
  m_CompareData(NULL),
  m_InputData(NULL),
  m_OutputData(NULL),
  m_Transform(NULL),
  m_Transformer(NULL),
  m_ScriptEngine(NULL),
  m_PEIngressCommand(NULL),
#ifdef NO_GUI
  m_Saver(NULL),
#else
  m_Saver(new QcepSettingsSaver(this)),
#endif
  m_GuiWanted(QcepSettingsSaverWPtr(), this, "guiWanted", true, "Is GUI wanted?"),
  m_Mode(QcepSettingsSaverWPtr(), this, "mode", 0, "Operation mode"),
  m_StartupCommands(QcepSettingsSaverWPtr(), this, "startupCommands", QcepStringList(), "Startup commands"),
  m_InputFiles(QcepSettingsSaverWPtr(), this, "inputFiles", QcepStringList(), "Input files"),
  m_OutputFile(QcepSettingsSaverWPtr(), this, "outputFile", "", "Output file"),
  m_MaskFile(QcepSettingsSaverWPtr(), this, "maskFile", "", "Mask file"),
  m_Mask3DFile(QcepSettingsSaverWPtr(), this, "mask3DFile", "", "3D-Mask file"),
  m_AnglesFile(QcepSettingsSaverWPtr(), this, "anglesFile", "", "Angles File"),
  m_WeightsFile(QcepSettingsSaverWPtr(), this, "weightsFile", "", "Weights File"),
  m_Debug(m_Saver, this, "debug", 0, "Debug Level"),
  m_Halting(QcepSettingsSaverWPtr(), this, "halting", false, "Set to halt operation in progress"),
  m_Progress(QcepSettingsSaverWPtr(), this, "progress", 0, "Progress completed"),
  m_ProgressLimit(QcepSettingsSaverWPtr(), this, "progressLimit", 100, "Progress limit"),
  m_SettingsPath(m_Saver, this, "settingsPath", "", "Settings saved in"),
  m_ScriptPath(m_Saver, this, "scriptPath", "", "Execute script from"),
  m_SpecDataFilePath(m_Saver, this, "specDataFilePath", "", "Pathname of spec data file"),
  m_MpiRank(QcepSettingsSaverWPtr(), this, "mpiRank", 0, "MPI Rank of process"),
  m_MpiSize(QcepSettingsSaverWPtr(), this, "mpiSize", -1, "MPI Size"),
  m_Verbosity(QcepSettingsSaverWPtr(), this, "verbosity", 0, "Output Verbosity"),
  m_ExitStatus(QcepSettingsSaverWPtr(), this, "exitStatus", 0, "Exit Status")
{
  QcepProperty::registerMetaTypes();
  CctwDoubleMatrix3x3Property::registerMetaTypes();
  CctwDoubleVector3DProperty::registerMetaTypes();
  CctwIntVector3DProperty::registerMetaTypes();
  CctwUnitCellProperty::registerMetaTypes();

  g_Saver = m_Saver;

  connect(prop_Debug(), SIGNAL(valueChanged(int,int)), this, SLOT(onDebugChanged(int)));
  connect(prop_Progress(), SIGNAL(valueChanged(int,int)), this, SLOT(onProgress(int)));

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(doAboutToQuit()));

  register_lzf();
}

QcepSettingsSaverWPtr CctwApplication::saver() const
{
  return m_Saver;
}

void CctwApplication::onDebugChanged(int dbg)
{
  if (g_DebugLevel) {
    g_DebugLevel->setDebugLevel(dbg);
  }
}

void CctwApplication::onProgress(int prg)
{
  int prog = (prg*100)/get_ProgressLimit();

  if (m_LastProgress.fetchAndStoreOrdered(prog) != prog) {
#ifdef NO_GUI
    if ((prog % 5 == 0)) {
//      printMessage(tr("%1% completed").arg(prog));
      printf("\r%d%% completed", prog);
      fflush(stdout);
      if (prog==100) {
        printf("\n");
        fflush(stdout);
      }
    }
#else
    if (m_Window == NULL && (prog % 5 == 0)) {
      printMessage(tr("%1% completed").arg(prog));
    }
#endif
  }
}

void CctwApplication::decodeCommandLineArgs(int &argc, char *argv[])
{
#ifdef Q_OS_UNIX
  decodeCommandLineArgsForUnix(argc, argv);
#endif

#ifdef Q_OS_WIN
  decodeCommandLineArgsForWindows(argc, argv);
#endif
}

void CctwApplication::startupCommand(QString cmd)
{
  prop_StartupCommands()->appendValue(cmd);
}

QString CctwApplication::addSlashes(QString str)
{

  QString newStr;

  for(int i=0;i<str.length();i++) {
    if(str[i] == '\0') {
      newStr.append('\\');
      newStr.append('0');
    } else if(str[i] == '\'') {
      newStr.append('\\');
      newStr.append('\'');
    } else if(str[i] == '\"') {
      newStr.append('\\');
      newStr.append('\"');
    } else if(str[i] == '\\') {
      newStr.append('\\');
      newStr.append('\\');
    } else {
      newStr.append(str[i]);
    }
  }

  return newStr;
}

void CctwApplication::decodeCommandLineArgsForUnix(int &argc, char *argv[])
{
#ifdef Q_OS_UNIX
  int c;

  enum {
    argInputChunks = 10,
    argVersion,
    argInputDataset,
    argOutputDims,
    argOutputChunks,
    argOutputDataset,
    argInputProject,
    argOutputProject,
    argProjectOutput,
    argMergeInput,
    argMergeOutput,
    argOmega,
    argTwoTheta,
    argPhi,
    argChi
  };

  int option_index = 0;

  while (1) {
    static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, argVersion},
      {"verbosity", optional_argument, 0, 'v'},
      {"threads", required_argument, 0, 'j'},
      {"input", required_argument, 0, 'i'},
      {"inputchunks", required_argument, 0, argInputChunks},
      {"inputdataset", required_argument, 0, argInputDataset},
      {"mask", required_argument, 0, 'm'},
      {"mask3d", required_argument, 0, 'M'},
      {"angles", required_argument, 0, 'a'},
      {"omega", required_argument, 0, argOmega},
      {"twotheta", required_argument, 0, argTwoTheta},
      {"phi", required_argument, 0, argPhi},
      {"chi", required_argument, 0, argChi},
      {"weights", required_argument, 0, 'w'},
      {"output", required_argument, 0, 'o'},
      {"outputdims", required_argument, 0, argOutputDims},
      {"outputchunks", required_argument, 0, argOutputChunks},
      {"outputdataset", required_argument, 0, argOutputDataset},
      {"normalization", required_argument, 0, 'N'},
      {"compression", required_argument, 0, 'z'},
      {"subset", required_argument, 0, 'S'},
      {"transform", no_argument, 0, 't'},
      {"debug", required_argument, 0, 'D'},
      {"preferences", required_argument, 0, 'p'},
      {"gui", no_argument, 0, 'g'},
      {"nogui", no_argument, 0, 'n'},
      {"command", required_argument, 0, 'c'},
      {"script", required_argument, 0, 's'},
      {"iproject", optional_argument, 0, argInputProject},
      {"oproject", optional_argument, 0, argOutputProject},
      {"projectout", required_argument, 0, argProjectOutput},
      {"mergein", required_argument, 0, argMergeInput},
      {"mergeout", required_argument, 0, argMergeOutput},
      {0,0,0,0}
    };

    c = getopt_long(argc, argv, "hv::j:i:m:M:a:o:N:S:tdxD:p:gnc:w::s:z:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
    case 'h':
      startupCommand("showHelp();");
      break;

    case argVersion:
      startupCommand("showVersion();");
      break;

    case 'v':
      if (optarg) {
        set_Verbosity(atoi(optarg));
      } else {
        prop_Verbosity()->incValue(1);
      }
      break;

    case 'j':
      startupCommand(tr("setThreads(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'i':
      pushInputFile(optarg);
      break;

    case argInputChunks:
      startupCommand(tr("setInputChunks(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argInputDataset:
      startupCommand(tr("setInputDataset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'm':
      startupCommand(tr("setMask(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'M':
      startupCommand(tr("setMask3D(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'a':
      startupCommand(tr("setAngles(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argOmega:
      startupCommand(tr("setOmega(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argTwoTheta:
      startupCommand(tr("setTwoTheta(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argPhi:
      startupCommand(tr("setPhi(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argChi:
      startupCommand(tr("setChi(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'w':
      startupCommand(tr("setWeights(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'o':
      set_OutputFile(optarg);
      break;

    case 'S':
      startupCommand(tr("setSubset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argOutputDims:
      startupCommand(tr("setOutputDims(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argOutputChunks:
      startupCommand(tr("setOutputChunks(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argOutputDataset:
      startupCommand(tr("setOutputDataset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 't':
      startupCommand(tr("doTransform(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'N':
      startupCommand(tr("normalization(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'z':
      startupCommand(tr("compression(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argInputProject:
      startupCommand(tr("inputProject(%1);").arg(optarg?atoi(optarg):7));
      break;

    case argOutputProject:
      startupCommand(tr("outputProject(%1);").arg(optarg?atoi(optarg):7));
      break;

    case argProjectOutput:
      startupCommand(tr("setProjectOutput(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'g': /* want gui */
      set_GuiWanted(true);
      break;

    case 'n': /* no gui */
      set_GuiWanted(false);
      break;

    case 'c': /* command line command */
      startupCommand(tr("%1;").arg(optarg));
      break;

    case 's': /* script file */
      startupCommand(tr("executeScriptFile(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'p': /* preferences file */
      startupCommand(tr("loadPreferences(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argMergeInput:
      pushInputFile(optarg);
      break;

    case argMergeOutput:
      set_OutputFile(optarg);
      break;

    case 'D': /* change debug level */
      {
        char *a = optarg;
        int lvl = atoi(a);
        printf("set debug level to %d\n", lvl);
        if (g_DebugLevel) {
          g_DebugLevel->setDebugLevel(lvl);
        }
      }
      break;

    case '?': /* unknown option, or missing optional argument */
      printMessage("Unrecognized command line option");
      break;

    default:
      break;
    }
  }

  int first=true;

  while (optind < argc) {
    if (first) {
      first = false;

      if (strcasecmp(argv[optind], "transform") == 0) {
        set_Mode(TransformMode);
      } else if (strcasecmp(argv[optind], "merge") == 0) {
        set_Mode(MergeMode);
      } else if (strcasecmp(argv[optind], "norm") == 0) {
        set_Mode(NormMode);
      } else if (strcasecmp(argv[optind], "project") == 0) {
        set_Mode(ProjectMode);
      } else {
        pushInputFile(argv[optind]);
      }
    } else {
      pushInputFile(argv[optind]);
    }

    optind++;
  }
#endif
}

void CctwApplication::decodeCommandLineArgsForWindows(int &argc, char *argv[])
{
}

static herr_t cctwH5print(unsigned n, const H5E_error2_t *eptr, void *data)
{
  CctwApplication *app = (CctwApplication*) data;

  if (app) {
    app->printMessage(QObject::tr("%1:%2 : %3").arg(eptr->file_name).arg(eptr->line).arg(eptr->desc));
  }

  return 0;
}

void CctwApplication::printHDF5errors()
{
  printMessage("HDF5 Error occurred");

  H5Ewalk(H5E_DEFAULT, H5E_WALK_DOWNWARD, &cctwH5print, this);
}

static herr_t cctwH5error(hid_t h, void *data)
{
  CctwApplication *app = (CctwApplication*) data;

  if (app) {
    app->printHDF5errors();
  }

  return 0;
}

void CctwApplication::installHDF5ErrorHandler()
{
  H5Eset_auto(H5E_DEFAULT, &cctwH5error, (void*) this);
}

void CctwApplication::initialize(int &argc, char *argv[])
{
  installHDF5ErrorHandler();

  decodeCommandLineArgs(argc, argv);

#ifdef WANT_IMPORT_COMMANDS
  m_ImportData       = new CctwImporter(this, "importData", this);
#endif

  m_CompareData      = new CctwComparer(this, "compareData", this);

  m_Parameters       = new CctwCrystalCoordinateParameters("parameters", this);

  m_InputData        = new CctwChunkedData(this,
                                           CctwIntVector3D(256,256,256),
                                           CctwIntVector3D(32, 32, 32),
                                            true,
                                           "inputData",
                                           this);
  m_InputData        -> allocateChunks();

  m_OutputData       = new CctwChunkedData(this,
                                           CctwIntVector3D(256,256,256),
                                           CctwIntVector3D(32, 32, 32),
                                           false,
                                           "outputData",
                                           this);
  m_OutputData       -> allocateChunks();

  m_Transform        = new CctwCrystalCoordinateTransform(m_Parameters, "transform", NULL, this);

  m_Transformer      = new CctwTransformer(this,
                                           m_InputData,
                                           m_OutputData,
                                           m_Transform, /*1, 1, 1,*/
                                           "transformer",
                                           this);

  m_PEIngressCommand = new CctwPEIngressCommand(this, "peingress", this);

  m_ScriptEngine     = new CctwScriptEngine(this, this);

#ifdef WANT_IMPORT_COMMANDS
  m_ScriptEngine->globalObject().setProperty("importData", m_ScriptEngine->newQObject(m_ImportData));
#endif

  m_ScriptEngine->globalObject().setProperty("compareData", m_ScriptEngine->newQObject(m_CompareData));
  m_ScriptEngine->globalObject().setProperty("inputData", m_ScriptEngine->newQObject(m_InputData));
  m_ScriptEngine->globalObject().setProperty("outputData", m_ScriptEngine->newQObject(m_OutputData));
  m_ScriptEngine->globalObject().setProperty("parameters", m_ScriptEngine->newQObject(m_Parameters));
  m_ScriptEngine->globalObject().setProperty("transform", m_ScriptEngine->newQObject(m_Transform));
  m_ScriptEngine->globalObject().setProperty("transformer", m_ScriptEngine->newQObject(m_Transformer));
  m_ScriptEngine->globalObject().setProperty("peingress", m_ScriptEngine->newQObject(m_PEIngressCommand));
  m_ScriptEngine->globalObject().setProperty("application", m_ScriptEngine->newQObject(this));
  m_ScriptEngine->globalObject().setProperty("globals", m_ScriptEngine->globalObject());

#ifndef NO_GUI
  readSettings();
#endif

  if (m_Saver) {
    m_Saver->start();
  }

#ifdef NO_GUI
  set_GuiWanted(false);
#else
  if (get_GuiWanted()) {
    m_Window                  = new CctwqtMainWindow(this);
  }

  if (m_Window) {
    m_Window->show();
  }
#endif

  preStartup();

  foreach(QString cmd, get_StartupCommands()) {
    QMetaObject::invokeMethod(this, "evaluateStartupCommand", Qt::QueuedConnection, Q_ARG(QString, cmd));
  }

  postStartup();

  if (!get_GuiWanted()) {
    QMetaObject::invokeMethod(this, "execute", Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "quit", Qt::QueuedConnection);
  }
}

void CctwApplication::doAboutToQuit()
{
#ifndef NO_GUI
  writeSettings();
#endif
}

void CctwApplication::printLine(QString msg)
{
#ifdef NO_GUI
  printf("%s\n", qPrintable(msg));
#else
  if (m_Window) {
    m_Window->printLine(msg);
  } else {
    printf("%s\n", qPrintable(msg));
  }
#endif
}

void CctwApplication::printMessage(QString msg, QDateTime dt)
{
#ifdef NO_GUI
  printf("%s\n", qPrintable(msg));
  fflush(stdout);
#else
  if (m_Window) {
    m_Window->printMessage(msg, dt);
  } else {
    printf("%s\n", qPrintable(msg));
  }
#endif
}

void CctwApplication::wait(QString msg)
{
  waitCompleted();

//  printMessage(tr("Wait: %1").arg(msg));
}

void CctwApplication::evaluateStartupCommand(QString cmd)
{
  if (m_ScriptEngine) {
    QScriptValue val = m_ScriptEngine->evaluate(cmd);

    m_ScriptEngine->checkForExceptions();

    if (!val.isUndefined()) {
      printMessage(tr("%1").arg(val.toString()));
    }
  }
}

void CctwApplication::evaluateCommand(QString cmd)
{
  if (m_ScriptEngine) {
    QScriptValue val = m_ScriptEngine->evaluate(cmd);

    m_ScriptEngine->checkForExceptions();

    if (val.isUndefined()) {
      printMessage(tr("%1").arg(cmd));
    } else {
      printMessage(tr("%1 -> %2").arg(cmd).arg(val.toString()));
    }
  }
}

QScriptValue CctwApplication::evaluate(QString cmd)
{
  if (m_ScriptEngine) {
    QScriptValue result = m_ScriptEngine->evaluate(cmd);

    m_ScriptEngine->checkForExceptions();

    return result;
  } else {
    return QScriptValue();
  }
}

void CctwApplication::executeScriptFile(QString path)
{
  if (m_ScriptEngine) {
    QFile f(path);

    if (f.open(QIODevice::ReadOnly)) {
      QString script = f.readAll();

      QScriptValue val = m_ScriptEngine->evaluate(script, path, 1);

      m_ScriptEngine->checkForExceptions();

      printMessage(tr("Result -> %1").arg(val.toString()));
    }
  }
}

void CctwApplication::showHelp(QString about)
{
  printLine(tr(
              "\n"
              "usage: %1 transform|merge|norm|project <inputs> <options>\n"
              "\n"
              "where inputs are one or more hdf dataset refs (in url format)\n"
              "the file part of the URL gives the hdf/nexus file, and the 'fragment'\n"
              "(separated by a # character) gives the hdf dataset path\n"
              "\n"
              "where options are:\n"
              "\n"
              "--help, -h                       display this text\n"
              "--version                        display version info\n"
              "--verbosity[=n], -v[=n]          set verbosity to 'n', or increment it if 'n' not given\n"
              "--threads <n>, -j <n>            set number of worker threads\n"
              "--input <f>, -i <f>              specify input data (url format)\n"
              "--inputchunks <cks>              specify input chunk size (e.g. 32x32x32 or 32)\n"
              "--inputdataset <dsn>             specify input dataset path\n"
              "--mask <f>, -m <f>               specify mask data (url format)\n"
              "--maskdataset <dsn>              specify mask dataset path\n"
              "--mask3d <f>, -M <f>             specify 3D mask data (url format)\n"
              "--angles <f>, -a <f>             specify angles data (url format\n"
              "--anglesdataset <f>              specify angles dataset path\n"
              "--omega <angs>                   specify omega angles (url-angles format\n"
              "--twotheta <angs>                specify two theta angles (url-angles format\n"
              "--chi <angs>                     specify chi angles (url-angles format\n"
              "--phi <angs>                     specify phi angles (url-angles format\n"
              "--weights <f>, -w <f>            specify weights data (url format)\n"
              "--output <f>, -o <f>             specify output data (url format)\n"
              "--outputdims <dims>              specify output dimensions (e.g. 2048x2048x2048 or 2048)\n"
              "--outputchunks <cks>             specify output chunk size (e.g. 32x32x32 or 32)\n"
              "--outputdataset <dsn>            specify output dataset path\n"
              "--normalization <n>, -N <n>      normalize output file(s) (0=no norm, 1=norm)\n"
              "--compression <n>, -z <n>        compress output files (-ve = LZF, 0 = none, 1-9 = GZIP\n"
              "--subset <n/m>, -S <n/m>         specify subset of input data to operate on (or all if blank)\n"
              "--transform, -t                  transform all or part of the data\n"
              "--debug <n>, -D <n>              set debug level\n"
              "--preferences <f>, -p <f>        read settings from file <f>\n"
              "--gui, -g                        use GUI interface if available\n"
              "--nogui, -n                      no GUI interface\n"
              "--command <cmd>, -c <cmd>        execute command <cmd>\n"
              "--script <f>, -s <f>             run script in file <f>\n"
              "--iproject {=n}                  project input dataset (along x=1,y=2,z=4 axes)\n"
              "--oproject {=n}                  project output dataset (along x=1,y=2,z=4 axes)\n"
              "--projectout <p>                 prefix for projected output files (add .x.tif, .y.tif or .z.tif)\n"
              "--mergein <f>                    specify an input dataset to merge (url format)\n"
              "--mergeout <f>                   merge (previously specified) datasets into output (url format)\n"
              "\n"
              "Where <angs> may be:\n"
              "A single numeric value           set the angle to the given value\n"
              "A comma separated number pair    set starting angle and step\n"
//              "A ref to an HDF scalar value     set the angle to the value\n"
//              "A ref to an HDF group containing 'start' and 'step' - set start and step accordingly\n"
              "A ref to an HDF array            set angles to values from array\n"
              "\n"
              "Examples:\n"
              "cctwcli transform file1.nxs\\#/data/entry/v -o xform.nxs\\#/data/entry/v\n"
              "cctwcli merge file1.nxs\\#/data/entry/v file2.nxs\\#/data/entry/v -o merge.nxs\\#/data/entry/v\n"
              ).arg(arguments().at(0)));
}

void CctwApplication::showVersion()
{
  printLine(tr("cctw   version " STR(CCTW_VERSION)));
  printLine(tr("qt     version %1").arg(qVersion()));
  printLine(tr("ceplib version " STR(QCEPLIB_VERSION)));
  printLine(tr("hdf5   version " STR(QCEPLIB_HDF5_VERSION)));
#ifndef NO_GUI
  printLine(tr("qwt    version " STR(QCEPLIB_QWT_VERSION)));
#endif
  printLine(tr("tiff   version " STR(QCEPLIB_TIFF_VERSION)));
  printLine(tr("cbf    version " STR(QCEPLIB_CBF_VERSION)));
}

void CctwApplication::setThreads(QString desc)
{
  printMessage(tr("Set number of pool threads to %1").arg(desc));

  bool ok;

  int n = desc.toInt(&ok);

  if (ok) {
    if (n == 0) {
      QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());
    } else {
      QThreadPool::globalInstance()->setMaxThreadCount(n);
    }
  }
}

void CctwApplication::setInputData(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set input data to %1").arg(data));

    m_InputData->setDataSource(data);
  }
}

void CctwApplication::setInputChunks(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set input chunk size to %1").arg(data));

    m_InputData->setChunks(data);
  }
}

void CctwApplication::setInputDataset(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set input data set name to %1").arg(data));

    m_InputData->setDataset(data);
  }
}

void CctwApplication::setMaskData(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set mask to %1").arg(data));

    set_MaskFile(data);
  }
}

void CctwApplication::setMaskDataset(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set mask dataset to %1").arg(data));

    m_InputData->setMaskDataset(data);
  }
}

void CctwApplication::setMask3DData(QString data)
{
  printMessage(tr("Set 3D mask to %1").arg(data));

  set_Mask3DFile(data);
}

void CctwApplication::pushInputFile(QString path)
{
  prop_InputFiles()->appendValue(path);
}

void CctwApplication::setAnglesData(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set Angles to %1").arg(data));

    m_InputData->setAnglesSource(data);
  }
}

void CctwApplication::setAnglesDataset(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set Angles dataset to %1").arg(data));

    m_InputData->setAnglesDataset(data);
  }
}

void CctwApplication::setOutputData(QString data)
{
  if (m_OutputData) {
    printMessage(tr("Set output data to %1").arg(data));

    m_OutputData->setDataSource(data);
  }
}

void CctwApplication::setOutputDims(QString data)
{
  if (m_OutputData) {
    printMessage(tr("Set output dataset dimensions to %1").arg(data));

    m_OutputData->setDims(data);
  }
}

void CctwApplication::setOutputChunks(QString data)
{
  if (m_OutputData) {
    printMessage(tr("Set output chunk size to %1").arg(data));

    m_OutputData->setChunks(data);
  }
}

void CctwApplication::setOutputDataset(QString data)
{
  if (m_OutputData) {
    printMessage(tr("Set output data set name to %1").arg(data));

    m_OutputData->setDataset(data);
  }
}

void CctwApplication::setSubset(QString desc)
{
  if (m_Transformer) {
    m_Transformer->set_Subset(desc);
  }
}

void CctwApplication::transform(QString desc)
{
//  printMessage(tr("Partial transform of %1").arg(desc));

  if (m_Transformer) {
    m_Transformer->transform();
  }
}

void CctwApplication::setOmega(QString data)
{
  if(m_Parameters) {
    printMessage(tr("Set Omega Values to %1").arg(data));

    m_Parameters->setOmega(data);
  }
}

void CctwApplication::setTwoTheta(QString data)
{
  if(m_Parameters) {
    printMessage(tr("Set Two Theta Values to %1").arg(data));

    m_Parameters->setTwoTheta(data);
  }
}

void CctwApplication::setPhi(QString data)
{
  if(m_Parameters) {
    printMessage(tr("Set Phi Values to %1").arg(data));

    m_Parameters->setPhi(data);
  }
}

void CctwApplication::setChi(QString data)
{
  if(m_Parameters) {
    printMessage(tr("Set Chi Values to %1").arg(data));

    m_Parameters->setChi(data);
  }
}

void CctwApplication::readSettings()
{
  QSettings settings("xray.aps.anl.gov", "cctw");

  readSettings(&settings);
}

void CctwApplication::readSettings(QString path)
{
  QSettings settings(path, QSettings::IniFormat);

  printMessage(tr("Reading settings from %1").arg(path));

  readSettings(&settings);
}

void CctwApplication::readSettings(QSettings *settings)
{
  QcepProperty::readSettings(this, &staticMetaObject, "cctw", settings, true);

  if (m_Parameters) {
    m_Parameters->readSettings(settings, "parameters");
  }

#ifdef WANT_IMPORT_COMMANDS
  if (m_ImportData) {
    m_ImportData->readSettings(settings, "importData");
  }
#endif

  if (m_CompareData) {
    m_CompareData->readSettings(settings, "compareData");
  }

  if (m_InputData) {
    m_InputData->readSettings(settings, "inputData");
  }

  if (m_OutputData) {
    m_OutputData->readSettings(settings, "outputData");
  }

  if (m_Transform) {
    m_Transform->readSettings(settings, "transform");
  }

  if (m_Transformer) {
    m_Transformer->readSettings(settings, "transformer");
  }

  if (m_PEIngressCommand) {
    m_PEIngressCommand->readSettings(settings, "peingress");
  }
}

void CctwApplication::writeSettings()
{
  QSettings settings("xray.aps.anl.gov", "cctw");

  printMessage(tr("Writing default settings"));

  writeSettings(&settings);
}

void CctwApplication::clearWriteSettings()
{
  QSettings settings("xray.aps.anl.gov", "cctw");

  printMessage(tr("Writing default settings"));

  settings.clear();

  writeSettings(&settings);
}

void CctwApplication::writeSettings(QString path)
{
  QSettings settings(path, QSettings::IniFormat);

  printMessage(tr("Writing settings to %1").arg(path));

  writeSettings(&settings);
}

void CctwApplication::writeSettings(QSettings *settings)
{
  QcepProperty::writeSettings(this, &staticMetaObject, "cctw", settings, true);

  if (m_Parameters) {
    m_Parameters->writeSettings(settings, "parameters");
  }

#ifdef WANT_IMPORT_COMMANDS
  if (m_ImportData) {
    m_ImportData->writeSettings(settings, "importData");
  }
#endif

  if (m_CompareData) {
    m_CompareData->writeSettings(settings, "compareData");
  }

  if (m_InputData) {
    m_InputData->writeSettings(settings, "inputData");
  }

  if (m_OutputData) {
    m_OutputData->writeSettings(settings, "outputData");
  }

  if (m_Transform) {
    m_Transform->writeSettings(settings, "transform");
  }

  if (m_Transformer) {
    m_Transformer->writeSettings(settings, "transformer");
  }

  if (m_PEIngressCommand) {
    m_PEIngressCommand->writeSettings(settings, "peingress");
  }
}

QString CctwApplication::settingsScript()
{
  QString res;

  if (m_Parameters) {
    res += m_Parameters->settingsScript();
  }

//  if (m_ImportData) {
//    res += m_ImportData->settingsScript();
//  }

//  if (m_CompareData) {
//    res += m_CompareData->settingsScript();
//  }

  if (m_InputData) {
    res += m_InputData->settingsScript();
  }

  if (m_OutputData) {
    res += m_OutputData->settingsScript();
  }

  if (m_Transform) {
    res += m_Transform->settingsScript();
  }

  if (m_Transformer) {
    res += m_Transformer->settingsScript();
  }

  if (m_PEIngressCommand) {
    res += m_PEIngressCommand->settingsScript();
  }

  return res;
}

CctwCrystalCoordinateParameters *CctwApplication::parameters() const
{
  return m_Parameters;
}

void CctwApplication::addWorkOutstanding(int amt)
{
  m_WorkOutstanding.fetchAndAddOrdered(amt);
}

void CctwApplication::workCompleted(int amt)
{
  m_WorkOutstanding.fetchAndAddOrdered(-amt);
}

void CctwApplication::waitCompleted()
{
  while (m_WorkOutstanding.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(100);
    processEvents(QEventLoop::ExcludeUserInputEvents);
  }
}

int  CctwApplication::workOutstanding()
{
  return m_WorkOutstanding.fetchAndAddOrdered(0);
}

void CctwApplication::analyzePEMetaData(QString path)
{
  set_SpecDataFilePath(path);

  QtConcurrent::run(m_PEIngressCommand, &CctwPEIngressCommand::analyzePEMetaData, path);
}

void CctwApplication::analyzeSpecDataFile(QString path)
{
  set_SpecDataFilePath(path);

  QtConcurrent::run(m_PEIngressCommand, &CctwPEIngressCommand::analyzeSpecDataFile, path);
}

#ifndef NO_GUI
void CctwApplication::plotCurves(QwtPlotCurve *c1, QwtPlotCurve *c2, QwtPlotCurve *c3, QwtPlotCurve *c4)
{
  if (m_Window) {
    m_Window->plotCurves(c1, c2, c3, c4);
  }
}
#endif

int CctwApplication::inputChunkCount()
{
  return m_InputData->chunkCount().volume();
}

int CctwApplication::outputChunkCount()
{
  return m_OutputData->chunkCount().volume();
}

void CctwApplication::testing()
{
  CctwDoubleVector3D dv1(1.2,2.2,3.2), dv2(-1.2,-2.2,-3.2);

  CctwIntVector3D iv1(dv1), iv2(dv2);

  printMessage(tr("iv1 = %1, iv2 = %2").arg(iv1.toString()).arg(iv2.toString()));
}

void CctwApplication::setNormalization(QString data)
{
  int v = data.toInt();

  if (m_Transformer) {
    printMessage(tr("Set normalization to %1").arg(v));

    m_Transformer->set_Normalization(v);
    m_OutputData->set_Normalization(v);
  }
}

void CctwApplication::setCompression(QString data)
{
  int v = data.toInt();

  if (m_Transformer) {
    printMessage(tr("Set compression to %1").arg(v));

    m_Transformer->set_Compression(v);
    m_OutputData->set_Compression(v);
  }
}

void CctwApplication::inputProject(int axes)
{
  if (m_Transformer) {
    m_Transformer->inputProject(m_Transformer->get_ProjectDestination(), axes);
  }
}

void CctwApplication::outputProject(int axes)
{
  if (m_Transformer) {
    m_Transformer->outputProject(m_Transformer->get_ProjectDestination(), axes);
  }
}

void CctwApplication::setProjectOutput(QString dir)
{
  if (m_Transformer) {
    m_Transformer->set_ProjectDestination(dir);
  }
}

void CctwApplication::mergeInput(QString path)
{
  pushInputFile(path);
}

void CctwApplication::mergeOutput(QString path)
{
  setOutputData(path);

  QFuture<void> f = QtConcurrent::run(this, &CctwApplication::runMerge);

  while (!f.isFinished()) {
    CctwThread::msleep(100);
    processEvents(QEventLoop::ExcludeUserInputEvents);
  }
}

void CctwApplication::runTransform()
{
  m_InputData->setDataSource(get_InputFiles().at(0));
  m_InputData->setMaskSource(get_MaskFile());
  m_InputData->setMask3DSource(get_Mask3DFile());
  m_InputData->setAnglesSource(get_AnglesFile());
  m_InputData->setWeightsSource(get_WeightsFile());

  autoOutputFile(".nxs");

  m_OutputData->setDataSource(get_OutputFile());

  transform("");
}

void CctwApplication::runMerge()
{
  QVector < CctwChunkedData* > inputFiles;

  CctwChunkedData* outputFile = NULL;

  CctwIntVector3D hdfChunkSize(0,0,0);
  CctwIntVector3D dims(0,0,0);

  if (get_InputFiles().count() < 1) {
    printMessage("No merge inputs given");
    goto exit;
  }

  foreach(QString path, get_InputFiles()) {
    CctwChunkedData* inputFile = new CctwChunkedData(this, CctwIntVector3D(0,0,0), CctwIntVector3D(10,10,10), true, path, NULL);

    if (inputFile) {
      inputFile->setDataSource(path);
      if (inputFile->openInputFile()) {
        printMessage(tr("Opened %1 successfully").arg(path));
        inputFile->setChunkSize(inputFile->get_HDFChunkSize());
      } else {
        delete inputFile;
        printMessage(tr("Failed to open %1").arg(path));
        goto exit;
      }
    }

    inputFiles.append(inputFile);
  }

  hdfChunkSize = inputFiles[0]->get_HDFChunkSize();
  dims         = inputFiles[0]->dimensions();

  foreach(CctwChunkedData* inputFile, inputFiles) {
    if (inputFile->get_HDFChunkSize() != hdfChunkSize) {
      printMessage("Input datasets do not have the same HDF chunk sizes");
      goto exit;
    }

    if (inputFile->dimensions() != dims) {
      printMessage("Input datasets do not have the same dimensions");
      goto exit;
    }

    inputFile->setChunkSize(hdfChunkSize);
  }

  autoOutputFile(".mrg.nxs#/entry/data");
  autoChunkSizes();

  m_OutputData->setDataSource(get_OutputFile());
  m_OutputData->set_HDFChunkSize(hdfChunkSize);
  m_OutputData->set_Compression(m_Transformer->get_Compression());
  m_OutputData->setDimensions(dims);
  m_OutputData->setChunkSize(hdfChunkSize);
  m_OutputData->set_Normalization(m_Transformer->get_Normalization());

  if (m_OutputData->openOutputFile()) {
    int nchunks = m_OutputData->chunkCount().volume();

    set_Progress(0);
    set_ProgressLimit(nchunks);

    for (int i=0; i<nchunks; i++) {
      set_Progress(i);

      foreach(CctwChunkedData* inputFile, inputFiles) {
        CctwDataChunk *inChunk = inputFile->readChunk(i);
        m_OutputData->mergeChunk(inChunk);

        delete inChunk;
      }

      m_OutputData->writeChunk(i);
    }
  } else {
    printMessage(tr("Could not open %1 for output").arg(get_OutputFile()));
    goto exit;
  }

exit:

  foreach(CctwChunkedData* inputFile, inputFiles) {
    if (inputFile) {
      delete inputFile;
    }
  }
}

void CctwApplication::runNorm()
{
  runMerge();
}

void CctwApplication::runProject()
{
  m_InputData->setDataSource(get_InputFiles().at(0));

  if (m_Transformer) {
    m_Transformer->inputProject(get_OutputFile(), 7);
  }
}

void CctwApplication::executeTransform()
{
  QStringList f = get_InputFiles();

  if (f.count() != 1) {
    printMessage(tr("cctw transform needs exactly 1 input file, rather than %1").arg(f.count()));
  } else {

    QFuture<void> f = QtConcurrent::run(this, &CctwApplication::runTransform);

    while (!f.isFinished()) {
      CctwThread::msleep(100);
      processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}

void CctwApplication::executeMerge()
{
  QStringList f = get_InputFiles();

  if (f.count() < 1) {
    printMessage(tr("cctw merge needs 1 or more input files, rather than %1").arg(f.count()));
  } else {

    QFuture<void> f = QtConcurrent::run(this, &CctwApplication::runMerge);

    while (!f.isFinished()) {
      CctwThread::msleep(100);
      processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}

void CctwApplication::executeNorm()
{
  QStringList f = get_InputFiles();

  if (f.count() != 1) {
    printMessage(tr("cctw norm needs exactly 1 input file, rather than %1").arg(f.count()));
  } else {

    QFuture<void> f = QtConcurrent::run(this, &CctwApplication::runNorm);

    while (!f.isFinished()) {
      CctwThread::msleep(100);
      processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}

void CctwApplication::executeProject()
{
  QStringList f = get_InputFiles();

  if (f.count() != 1) {
    printMessage(tr("cctw project needs exactly 1 input file, rather than %1").arg(f.count()));
  } else {

    QFuture<void> f = QtConcurrent::run(this, &CctwApplication::runProject);

    while (!f.isFinished()) {
      CctwThread::msleep(100);
      processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}

void CctwApplication::execute()
{
  int m = get_Mode();

  switch (m) {
  case 0:
    break;

  case TransformMode:
    executeTransform();
    break;

  case MergeMode:
    executeMerge();
    break;

  case NormMode:
    executeNorm();
    break;

  case ProjectMode:
    executeProject();
    break;
  }

//  printMessage(tr("Mode : %1").arg(m));

//  foreach (QString s, get_InputFiles()) {
//    printMessage(tr("File: \"%1\"").arg(s));
//  }
}

void CctwApplication::preStartup()
{
  int m = get_Mode();

  if (m == TransformMode) {
    m_InputData -> setDataSource(get_InputFiles().at(0));
    m_OutputData -> setDataSource(get_OutputFile());
  }

  if (m == MergeMode) {
    m_OutputData->set_Normalization(0);
  }

  if (m == NormMode) {
    m_OutputData->set_Normalization(1);
  }
}

void CctwApplication::postStartup()
{
}

void CctwApplication::autoOutputFile(QString suffix)
{
  if (get_OutputFile().length() == 0) {
    QString commonBase = "";
    int first = true;

    foreach(QString f, get_InputFiles()) {
      QUrl u(f);
      QString fn = u.fileName();
      QFileInfo fi(fn);
      QString bn = fi.completeBaseName();

      if (first) {
        commonBase = bn;
        first = false;
      } else {
        QString a;

        for (int i=0; i<bn.length(); i++) {
          if (commonBase[i] == bn[i]) {
            a.append(bn[i]);
          } else break;
        }

        if (a.length() >= 1) {
          commonBase = a;
        }
      }

      printMessage(tr("Common basename %1").arg(commonBase));
    }

    QString res = commonBase + suffix;

    set_OutputFile(res);
  }
}

void CctwApplication::autoChunkSizes()
{
}

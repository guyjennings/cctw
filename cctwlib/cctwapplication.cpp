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
  m_StartupCommands(QcepSettingsSaverWPtr(), this, "startupCommands", QcepStringList(), "Startup commands"),
  m_Debug(m_Saver, this, "debug", 0, "Debug Level"),
  m_Halting(QcepSettingsSaverWPtr(), this, "halting", false, "Set to halt operation in progress"),
  m_Progress(QcepSettingsSaverWPtr(), this, "progress", 0, "Progress completed"),
  m_ProgressLimit(QcepSettingsSaverWPtr(), this, "progressLimit", 100, "Progress limit"),
  m_DependenciesPath(m_Saver, this, "dependenciesPath", "", "Dependencies saved in"),
  m_SettingsPath(m_Saver, this, "settingsPath", "", "Settings saved in"),
  m_SpecDataFilePath(m_Saver, this, "specDataFilePath", "", "Pathname of spec data file"),
  m_MpiRank(QcepSettingsSaverWPtr(), this, "mpiRank", 0, "MPI Rank of process"),
  m_MpiSize(QcepSettingsSaverWPtr(), this, "mpiSize", -1, "MPI Size")
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
      printMessage(tr("%1% completed").arg(prog));
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
    argInputDataset,
    argMaskDataset,
    argAnglesDataset,
    argOutputDims,
    argOutputChunks,
    argOutputDataset
  };

  while (1) {
    static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'v'},
      {"threads", required_argument, 0, 'j'},
      {"input", required_argument, 0, 'i'},
      {"inputchunks", required_argument, 0, argInputChunks},
      {"inputdataset", required_argument, 0, argInputDataset},
      {"mask", required_argument, 0, 'm'},
      {"maskdataset", required_argument, 0, argMaskDataset},
      {"angles", required_argument, 0, 'a'},
      {"anglesdataset", required_argument, 0, argAnglesDataset},
      {"output", required_argument, 0, 'o'},
      {"outputdims", required_argument, 0, argOutputDims},
      {"outputchunks", required_argument, 0, argOutputChunks},
      {"outputdataset", required_argument, 0, argOutputDataset},
      {"normalization", optional_argument, 0, 'N'},
      {"transform", optional_argument, 0, 't'},
      {"depends", optional_argument, 0, 'd'},
      {"debug", required_argument, 0, 'D'},
      {"preferences", required_argument, 0, 'p'},
      {"gui", no_argument, 0, 'g'},
      {"nogui", no_argument, 0, 'n'},
      {"command", required_argument, 0, 'c'},
      {"wait", required_argument, 0, 'w'},
      {"script", required_argument, 0, 's'},
      {0,0,0,0}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "hvj:i:m:a:o:N:t::d::D:p:gnc:w:s:", long_options, &option_index);

    if (c == -1) {
      break;
    }

    switch (c) {
    case 'h':
      startupCommand("showHelp();");
      break;

    case 'v':
      startupCommand("showVersion();");
      break;

    case 'j':
      startupCommand(tr("setThreads(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'i':
      startupCommand(tr("setInputData(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argInputChunks:
      startupCommand(tr("setInputChunks(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argInputDataset:
      startupCommand(tr("setInputDataset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'm':
      startupCommand(tr("setMaskData(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argMaskDataset:
      startupCommand(tr("setMaskDataset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'a':
      startupCommand(tr("setAnglesData(\"%1\");").arg(addSlashes(optarg)));
      break;

    case argAnglesDataset:
      startupCommand(tr("setAnglesDataset(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'o':
      startupCommand(tr("setOutputData(\"%1\");").arg(addSlashes(optarg)));
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
      startupCommand(tr("partialTransform(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'd':
      startupCommand(tr("partialDependencies(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'N':
      startupCommand(tr("normalization(\"%1\");").arg(addSlashes(optarg)));
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

    case 'w': /* wait */
      startupCommand(tr("wait(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 's': /* script file */
      startupCommand(tr("executeScriptFile(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'p': /* preferences file */
      startupCommand(tr("loadPreferences(\"%1\");").arg(addSlashes(optarg)));
      break;

    case 'D': /* change debug level */
      {
        char *a = optarg;
        int lvl = atoi(a);
        set_Debug(lvl);
      }
      break;

    case '?': /* unknown option, or missing optional argument */
      break;

    default:
      break;
    }
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
  m_OutputData              -> allocateChunks();

  m_Transform        = new CctwCrystalCoordinateTransform(m_Parameters, "transform", this);

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
#endif

#ifndef NO_GUI
  if (m_Window) {
    m_Window->show();
  }
#endif

  foreach(QString cmd, get_StartupCommands()) {
    QMetaObject::invokeMethod(this, "evaluateStartupCommand", Qt::QueuedConnection, Q_ARG(QString, cmd));
  }

  if (!get_GuiWanted()) {
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
              "usage: %1 <options>\n"
              "\n"
              "where options are:\n"
              "\n"
              "--help, -h                       display this text\n"
              "--version, -v                    display version info\n"
              "--threads <n>, -j <n>            set number of worker threads\n"
              "--input <f>, -i <f>              specify input data (url format)\n"
              "--inputchunks <cks>              specify input chunk size (e.g. 32x32x32 or 32)\n"
              "--inputdataset <dsn>             specify input dataset path\n"
              "--mask <f>, -m <f>               specify mask data (url format)\n"
              "--maskdataset <dsn>              specify mask dataset path\n"
              "--angles <f>, -a <f>             specify angles data (url format\n"
              "--anglesdataset <f>              specify angles dataset path\n"
              "--output <f>, -o <f>             specify output data (url format)\n"
              "--outputdims <dims>              specify output dimensions (e.g. 2048x2048x2048 or 2048)\n"
              "--outputchunks <cks>             specify output chunk size (e.g. 32x32x32 or 32)\n"
              "--outputdataset <dsn>            specify output dataset path\n"
              "--transform {<n/m>}, -t {<n/m>}  transform all or part of the data\n"
              "--depends {<n/m>}, -d {<n/m>}    calculate dependencies for all or part of the data\n"
              "--debug <n>, -D <n>              set debug level\n"
              "--preferences <f>, -p <f>        read settings from file <f>\n"
              "--gui, -g                        use GUI interface if available\n"
              "--nogui, -n                      no GUI interface\n"
              "--command <cmd>, -c <cmd>        execute command <cmd>\n"
              "--wait <msg>, -w                 wait for previous commands to finish\n"
              "--script <f>, -s <f>             run script in file <f>\n"
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

    m_InputData->setMaskSource(data);
  }
}

void CctwApplication::setMaskDataset(QString data)
{
  if (m_InputData) {
    printMessage(tr("Set mask dataset to %1").arg(data));

    m_InputData->setMaskDataset(data);
  }
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

void CctwApplication::partialTransform(QString desc)
{
//  printMessage(tr("Partial transform of %1").arg(desc));

  if (m_Transformer) {
    m_Transformer->transform();
  }
}

void CctwApplication::partialDependencies(QString desc)
{
//  printMessage(tr("Partial dependencies of %1").arg(desc));

  calculateDependencies();
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

void CctwApplication::calculateChunkDependencies(int n)
{
  if (!get_Halting()) {
    CctwCrystalCoordinateTransform transform(m_Parameters, tr("transform-%1").arg(n), NULL);

    //    printMessage(tr("Calculate Chunk Dependencies for chunk [%1,%2,%3]").arg(idx.x()).arg(idx.y()).arg(idx.z()));

    CctwDataChunk  *chunk   = m_InputData->chunk(n);

    if (chunk) {
      CctwIntVector3D chStart = chunk->chunkStart();
      CctwIntVector3D chSize  = chunk->chunkSize();
      CctwDoubleVector3D dblStart(chStart.x(), chStart.y(), chStart.z());
      int lastChunk = -1;

      int osx = m_Transformer->get_OversampleX();
      int osy = m_Transformer->get_OversampleY();
      int osz = m_Transformer->get_OversampleZ();

      double osxstp = osx >= 1 ? 1.0/osx : 0;
      double osystp = osy >= 1 ? 1.0/osy : 0;
      double oszstp = osz >= 1 ? 1.0/osz : 0;

      for (int z=0; z<chSize.z(); z++) {
        for (int oz=0; oz<osz; oz++) {
          for (int y=0; y<chSize.y(); y++) {
            for (int oy=0; oy<osy; oy++) {
              for (int x=0; x<chSize.x(); x++) {
                for (int ox=0; ox<osx; ox++) {
                  CctwDoubleVector3D coords = dblStart+CctwDoubleVector3D(x+ox*osxstp, y+oy*osystp, z+oz*oszstp);

                  CctwDoubleVector3D xfmcoord = transform.forward(coords);

                  CctwIntVector3D    pixels(xfmcoord);

                  if (m_OutputData->containsPixel(pixels)) {
                    int opchunk  = m_OutputData->chunkContaining(pixels);

                    if (opchunk != lastChunk) {
                      lastChunk = opchunk;
                      //              m_InputData->addDependency(n, opchunk);
                      //              m_OutputData->addDependency(opchunk, n);
                      m_Transformer->addDependency(n, opchunk);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

//    printMessage(tr("Finished Chunk Dependencies for chunk [%1,%2,%3]").arg(idx.x()).arg(idx.y()).arg(idx.z()));

    m_DependencyCounter.fetchAndAddOrdered(-1);
  } else {
    m_DependencyCounter.fetchAndStoreOrdered(0);
  }

  prop_Progress()->incValue(1);

  workCompleted(1);
}

void CctwApplication::calculateDependencies()
{
//  QVector < QFuture < void > > futures;
  waitCompleted();

//  m_InputData->clearDependencies();
//  m_OutputData->clearDependencies();

  m_Transformer->clearDependencies();

  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  QTime startAt;

  startAt.start();

  printMessage("Starting calculate dependencies");

  m_DependencyCounter.fetchAndStoreOrdered(0);

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
//        while ((!get_Halting()) && (m_DependencyCounter.fetchAndAddOrdered(0) > 32)) {
////          CctwqtThread::msleep(10);
//          processEvents();
//        }

        if (get_Halting()) {
          m_DependencyCounter.fetchAndStoreOrdered(0);
          goto abort;
        } else {
          CctwIntVector3D idx(x,y,z);
          int n = m_InputData->chunkNumberFromIndex(idx);

          m_DependencyCounter.fetchAndAddOrdered(1);

          addWorkOutstanding(1);
//          futures.append(
                QtConcurrent::run(this, &CctwApplication::calculateChunkDependencies, n)/*)*/;
//          calculateChunkDependencies(idx);
        }
      }
    }
  }

abort:
  while (m_DependencyCounter.fetchAndAddOrdered(0) > 0) {
    CctwThread::msleep(10);
    processEvents();
  }

//  foreach (QFuture<void> f, futures) {
//    f.waitForFinished();
//  }

//  m_Transformer -> completedDependencies();

  int msec = startAt.elapsed();

  printMessage(tr("finished calculate dependencies after %1 msec").arg(msec));
}

void CctwApplication::saveDependencies(QString path)
{
  if (m_Transformer) {
    m_Transformer->saveDependencies(path);
  }
}

void CctwApplication::loadDependencies(QString path)
{
  if (m_Transformer) {
    m_Transformer->loadDependencies(path);
  }
}

void CctwApplication::reportDependencies()
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);

  printMessage(tr("Check mapping"));

  for (int n=0; n<chunks.volume(); n++) {
    if (get_Halting()) {
      break;
    } else {
      CctwIntVector3D idx = m_InputData->chunkIndexFromNumber(n);
      int nn = m_InputData->chunkNumberFromIndex(idx);

      if (nn != n) {
        printMessage(tr("Problem: %1 => [%2,%3,%4] => %5").arg(n).arg(idx.x()).arg(idx.y()).arg(idx.z()).arg(nn));
      }
    }
  }

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);
          int n = m_InputData->chunkNumberFromIndex(idx);
          CctwIntVector3D idx2 = m_InputData->chunkIndexFromNumber(n);

          if (idx != idx2) {
            printMessage(tr("Problem: [%1,%2,%3] => %4 => [%5,%6,%7]")
                         .arg(idx.x()).arg(idx.y()).arg(idx.z())
                         .arg(n)
                         .arg(idx2.x()).arg(idx2.y()).arg(idx2.z()));
          }
        }
      }
    }
  }

  printMessage(tr("Input Data Dependencies"));

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          CctwDataChunk *chunk = m_InputData -> chunk(idx);

          if (chunk) {
            chunk->reportDependencies();
          }
        }
      }
    }
  }
}

void CctwApplication::reportOutputDependencies()
{
  CctwIntVector3D chunks = m_OutputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          reportOutputDependencies(idx);
        }
      }
    }
  }
}

void CctwApplication::reportOutputDependencies(CctwIntVector3D idx)
{
  CctwDataChunk *chunk = m_OutputData->chunk(idx);

  if (chunk) {
    chunk->reportDependencies();
  }
}

void CctwApplication::reportInputDependencies()
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          reportInputDependencies(idx);
        }
      }
    }
  }
}

void CctwApplication::reportInputDependencies(CctwIntVector3D idx)
{
  CctwDataChunk *chunk = m_InputData->chunk(idx);

  if (chunk) {
    chunk->reportDependencies();
  }
}

void CctwApplication::reportInputChunkCounts()
{
  CctwIntVector3D chunks = m_InputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      QString aLine;

      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          CctwDataChunk *chunk = m_InputData->chunk(idx);

          if (chunk) {
            int nDeps = chunk->dependencyCount();

            if (nDeps > 9) {
              aLine += "*";
            } else if (nDeps >= 1) {
              aLine += tr("%1").arg(nDeps);
            } else {
              aLine += "0";
            }
          } else {
            aLine += "0";
          }
        }
      }

      printMessage(aLine);
    }

    printMessage("");
  }
}

void CctwApplication::reportOutputChunkCounts()
{
  CctwIntVector3D chunks = m_OutputData->chunkCount();

  set_Halting(false);
  set_Progress(0);
  set_ProgressLimit(chunks.volume());

  for (int z=0; z<chunks.z(); z++) {
    for (int y=0; y<chunks.y(); y++) {
      QString aLine;

      for (int x=0; x<chunks.x(); x++) {
        if (get_Halting()) {
          break;
        } else {
          CctwIntVector3D idx(x,y,z);

          CctwDataChunk *chunk = m_OutputData->chunk(idx);

          if (chunk) {
            int nDeps = chunk->dependencyCount();

            if (nDeps > 9) {
              aLine += "*";
            } else if (nDeps >= 1) {
              aLine += tr("%1").arg(nDeps);
            } else {
              aLine += "0";
            }
          } else {
            aLine += "0";
          }
        }
      }

      printMessage(aLine);
    }

    printMessage("");
  }
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

QcepIntList CctwApplication::dependencies(int chunkIdx)
{
  return m_Transformer->dependencies(chunkIdx);
}

QList<CctwIntVector3D> CctwApplication::dependencies(int cx, int cy, int cz)
{
  return m_Transformer->dependencies(cx, cy, cz);
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
  }
}

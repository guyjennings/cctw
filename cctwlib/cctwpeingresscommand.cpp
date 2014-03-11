#include "cctwpeingresscommand.h"
#include "cctwapplication.h"
#include "cctwlinearfitter.h"

#ifndef NO_GUI
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#endif

#include <QFile>
#include <math.h>

CctwPEIngressCommand::CctwPEIngressCommand(CctwApplication *app, QString name, QObject *parent) :
  CctwObject(name, parent),
  m_Application(app)
{
}

void CctwPEIngressCommand::testSlot()
{
}

void CctwPEIngressCommand::analyzePEMetaData(QString path)
{
}

void CctwPEIngressCommand::analyzeSpecDataFile(QString path)
{
  m_Application->set_Halting(false);

  printMessage(tr("Analyzing PE MetaData from %1").arg(path));

  QFile file(path);

  QVector<double> xData, y1Data, y2Data, y3Data, y4Data;

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QSharedPointer<CctwLinearFitter> fit1(new CctwLinearFitter("fit1"));
    QSharedPointer<CctwLinearFitter> fit2(new CctwLinearFitter("fit2"));
    QSharedPointer<CctwLinearFitter> fit3(new CctwLinearFitter("fit3"));
    QSharedPointer<CctwLinearFitter> fit4(new CctwLinearFitter("fit4"));

    int ncols = 0;
    int line1 = true;
    int scanNumber = -1;
    int rowNum = -1;
    QString scanCmd;
    int isRotScan = false;

    QRegExp scanRE("^#S\\s*(\\d+)\\s+(\\w+)");
    QRegExp ncolsRE("^#N\\s*(\\d+)");

    while(!file.atEnd() && !m_Application->get_Halting()) {
      QString line = file.readLine();

      if (line.startsWith("#S")) {
        if (line.contains(scanRE)) {
          if (isRotScan && (rowNum >= 1200)) {
            fit1->performFit(10, 80);
            fit2->performFit(10, 80);
            fit3->performFit(10, 80);
            fit4->performFit(10, 80);
            printLine(tr("%1  %2  %3 %4 %5, %6 %7 %8")
                      .arg(rowNum)
                      .arg(scanNumber, 6)
                      .arg(fit1->slope(), 20, 'f', 14)
                      .arg(fit1->intercept(), 20, 'f', 14)
                      .arg(fit1->rSquared(), 20, 'f', 14)
                      .arg(fit2->slope(), 20, 'f', 14)
                      .arg(fit2->intercept(), 20, 'f', 14)
                      .arg(fit2->rSquared(), 20, 'f', 14) +
                      tr(", %1 %2 %3")
                      .arg(fit3->slope(), 20, 'f', 14)
                      .arg(fit3->intercept(), 20, 'f', 14)
                      .arg(fit3->rSquared(), 20, 'f', 14) +
                      tr(", %1 %2 %3")
                      .arg(fit4->slope(), 20, 'f', 14)
                      .arg(fit4->intercept(), 20, 'f', 14)
                      .arg(fit4->rSquared(), 20, 'f', 14));
            //          printLine(tr("Scan %1, cmd %2").arg(scanRE.cap(1)).arg(scanRE.cap(2)));
            //          fit2->performFit(10, 10);
            //          fit3->performFit(10, 10);
            //          fit4->performFit(10, 10);

            xData.append(scanNumber);
            y1Data.append(normalizedValue(fit1->slope()));
            y2Data.append(normalizedValue(fit2->slope()));
            y3Data.append(normalizedValue(fit3->slope()));
            y4Data.append(normalizedValue(fit4->slope()));
          }

          scanNumber = scanRE.cap(1).toInt();
          scanCmd    = scanRE.cap(2);
          isRotScan  = (scanCmd == "rotscan") && line.contains("auxth");
          rowNum     = 0;
        }
        line1 = true;
        // scan start
      } else if (line.startsWith("#D")) {
        // scan date
      } else if (line.startsWith("#L")) {
        // scan headings
      } else if (line.startsWith("#N")) {
        if (line.contains(ncolsRE)) {
          QString f = ncolsRE.cap(1);

          //          printLine(tr("Ncols = %1").arg(f));

          ncols = f.toInt();
        }
        // number of columns
      } else if (line.startsWith("#")) {
        // other comment
      } else if (isRotScan) {
        // data line
        rowNum++;

        QStringList fields = line.split(QRegExp("\\s+"));

        if (fields.count() >= ncols) {
          double x1 = fields[1].toDouble();
          double y1 = fields[0].toDouble();

          double x2 = fields[1].toDouble() + fields[2].toDouble();
          double y2 = fields[0].toDouble();

          double x3 = fields[1].toDouble()/* + fields[2].toDouble()*/;
          double y3 = fields[3].toDouble();

          double x4 = fields[3].toDouble();
          double y4 = fields[0].toDouble();

          if (line1) {
            line1 = false;
            fit1->startNewFit();
            fit2->startNewFit();
            fit3->startNewFit();
            fit4->startNewFit();
          }

          fit1->appendPoint(x1, y1);
          fit2->appendPoint(x2, y2);
          fit3->appendPoint(x3, y3);
          fit4->appendPoint(x4, y4);
        }
      }
    }
  }

#ifndef NO_GUI
  QwtPlotCurve *pc1 = new QwtPlotCurve("auxth vs spec epoch");

  pc1->setSamples(xData, y1Data);

  QwtPlotCurve *pc2 = new QwtPlotCurve("auxth vs det epoch");

  pc2->setSamples(xData, y2Data);
  //    pc1->attach(graph);

  QwtPlotCurve *pc3 = new QwtPlotCurve("index vs spec epoch");

  pc3->setSamples(xData, y3Data);

  QwtPlotCurve *pc4 = new QwtPlotCurve("auxth vs index");

  pc4->setSamples(xData, y4Data);

  m_Application->plotCurves(pc1, pc2, pc3, pc4);
#endif
}

double CctwPEIngressCommand::normalizedValue(double v)
{
  double r = fabs(v);

  if (r > 7) {
    r = r / 10.0;
  } else if (r > 4) {
    r = r / 5.0;
  } else if (r > 0.8) {

  } else if (r > 0.4) {
    r = r * 2;
  } else {
    r = r * 10;
  }

  return r;
}

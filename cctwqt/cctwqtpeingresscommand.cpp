#include "cctwqtpeingresscommand.h"
#include "cctwqtapplication.h"
#include "cctwqtlinearfitter.h"
#include "qwt_plot.h"
#include <QFile>

CctwqtPEIngressCommand::CctwqtPEIngressCommand(CctwqtApplication *app, QObject *parent) :
  CctwqtObject(parent),
  m_Application(app)
{
}

void CctwqtPEIngressCommand::testSlot()
{
}

void CctwqtPEIngressCommand::analyzePEMetaData(QString path)
{
}

void CctwqtPEIngressCommand::analyzeSpecDataFile(QString path, QwtPlot *graph)
{
  m_Application->set_Halting(false);

  printMessage(tr("Analyzing PE MetaData from %1").arg(path));

  QFile file(path);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QSharedPointer<CctwqtLinearFitter> fit1(new CctwqtLinearFitter());
//    QSharedPointer<CctwqtLinearFitter> fit2(new CctwqtLinearFitter());
//    QSharedPointer<CctwqtLinearFitter> fit3(new CctwqtLinearFitter());
//    QSharedPointer<CctwqtLinearFitter> fit4(new CctwqtLinearFitter());

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
            printLine(tr("%1  %2  %3  %4 %5")
                      .arg(rowNum)
                      .arg(scanNumber, 6)
                      .arg(fit1->slope(), 20, 'f', 14)
                      .arg(fit1->intercept(), 20, 'f', 14)
                      .arg(fit1->rSquared(), 20, 'f', 14));
            //          printLine(tr("Scan %1, cmd %2").arg(scanRE.cap(1)).arg(scanRE.cap(2)));
            //          fit2->performFit(10, 10);
            //          fit3->performFit(10, 10);
            //          fit4->performFit(10, 10);
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
          double x = fields[0].toDouble();
          double y = fields[1].toDouble();

          if (line1) {
            line1 = false;
            fit1->startNewFit();
          }

          fit1->appendPoint(x, y);
        }
      }
    }
  }
}

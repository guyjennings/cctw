#include "cctwqtpeingresscommand.h"
#include "cctwqtapplication.h"
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
  m_Application->set_Halting(false);

  printMessage(tr("Analyzing PE MetaData from %1").arg(path));

  QFile file(path);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    int ncols = 0;
    int line1 = true;
    int scanNumber = -1;
    int rowNum = -1;
    QString scanCmd;
    int isRotScan = false;

    double x0 = 0, y0 = 0;

    long double sumn, sumx, sumy, sumxy, sumxx, sumn10, sumx10, sumy10, sumxy10, sumxx10;
    sumn = sumx = sumy = sumxy = sumxx = sumn10 = sumx10 = sumy10 = sumxy10 = sumxx10 = 0;

    QRegExp scanRE("^#S\\s*(\\d+)\\s+(\\w+)");
    QRegExp ncolsRE("^#N\\s*(\\d+)");

    while(!file.atEnd() && !m_Application->get_Halting()) {
      QString line = file.readLine();

      if (line.startsWith("#S")) {
        if (line.contains(scanRE)) {
          if (sumn > 0) {
            double grad = (sumxy - sumx*sumy/sumn)/(sumxx - sumx*sumx/sumn);
            double intcp = sumy/sumn - grad*sumx/sumn + y0;

            if (sumn10 > 1000) {
              double grad10 = (sumxy10 - sumx10*sumy10/sumn10)/(sumxx10 - sumx10*sumx10/sumn10);
              double intcp10 = sumy10/sumn10 - grad10*sumx10/sumn10 + y0;
              printLine(tr("%1  %2  %3  %4  %5")
                        .arg(scanNumber, 6)
                        .arg(grad, 20, 'f', 14)
                        .arg(intcp, 20, 'f', 14)
                        .arg(grad10, 20, 'f', 14)
                        .arg(intcp10, 20, 'f', 14));
            } else {
//              printLine(tr("Slope: %1 Intercept: %2").arg(double(grad)).arg(double(intcp)));
            }
          }
//          printLine(tr("Scan %1, cmd %2").arg(scanRE.cap(1)).arg(scanRE.cap(2)));

          scanNumber = scanRE.cap(1).toInt();
          scanCmd    = scanRE.cap(2);
          isRotScan  = (scanCmd == "rotscan");

          sumn = sumx = sumy = sumxy = sumxx = sumn10 = sumx10 = sumy10 = sumxy10 = sumxx10 = 0;
          rowNum = 0;
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
            x0 = x;
            y0 = y;
          }

          x -= x0;
          y -= y0;

          sumn += 1;
          sumx += x;
          sumy += y;
          sumxx += x*x;
          sumxy += x*y;

          if (rowNum >= 10) {
            sumn10 += 1;
            sumx10 += x;
            sumy10 += y;
            sumxx10 += x*x;
            sumxy10 += x*y;
          }
        }
      }
    }
  }
}

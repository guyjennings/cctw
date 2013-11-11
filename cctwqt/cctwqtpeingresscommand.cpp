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
  printMessage(tr("Analyzing PE MetaData from %1").arg(path));

  QFile file(path);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    int ncols = 0;
    int line1 = true;

    QRegExp scanRE("^#S\\W*(\\d+)\\W+(\\w+)");
    QRegExp ncolsRE("^#N\\W*(\\d+)");

    while(!file.atEnd()) {
      QString line = file.readLine();

      if (line.startsWith("#S")) {
        if (line.contains(scanRE)) {
          printMessage(tr("Scan %1, cmd %2").arg(scanRE.cap(1)).arg(scanRE.cap(2)));
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

          printMessage(tr("Ncols = %1").arg(f));
        }
        // number of columns
      } else if (line.startsWith("#")) {
        // other comment
      } else {
        // data line
        if (line1) {
          line1 = false;
          QStringList fields = line.split(QRegExp("\\W"));

          printMessage(tr("%1 input fields").arg(fields.count()));
        }
      }
    }
  }
}

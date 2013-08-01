#include "cctwqtapplication.h"
#include "cctwqtmainwindow.h"
#include "cctwqtinputdata.h"
#include "cctwqtoutputdata.h"
#include "cctwcrystalcoordinatetransform.h"
#include "cctwtransformer.h"

CctwqtApplication::CctwqtApplication(int argc, char *argv[]) :
  QApplication(argc, argv),
  m_Window(new CctwqtMainWindow()),
  m_InputData(new CctwqtInputData(this)),
  m_OutputData(new CctwqtOutputData(this)),
  m_Transform(new CctwCrystalCoordinateTransform()),
  m_Transformer(new CctwTransformer(m_InputData, m_OutputData, m_Transform, 1, 1, 1, 0))
{
  m_Window->show();
}


/**
   Similar to cctwtcltest-1.tcl but does 2 concurrent transforms
   Use turbine -n 4 to get 2 workers
*/

import cctw;

main
{
  code =
"""
  outputData.dimensions = [128*3, 128*6, 128*4]
  outputDataManager.filePath = "./h5test.h5"
  cctw.calculateDependencies()
  transformer.transform()
""";
    
  foreach i in [0,1]
  {
    cctw(code);
  }
}

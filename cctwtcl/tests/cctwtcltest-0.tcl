# package ifneeded cctw 0.1 "load ./libCctwTcl.so cctw"

package require cctw

set result [ cctw { 42 } ] 

exit [ expr ! ( $result == 42 ) ]

# cctw {
#   outputData.dimensions = [128*3, 128*6, 128*4]
#   outputDataManager.filePath = "./h5test.h5"
#   cctw.calculateDependencies()
#   transformer.transform()
# }

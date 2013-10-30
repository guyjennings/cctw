
package require cctw

set chunk [ cctw_chunk_create 128 ]
cctw_chunk_read "pznpt4_0070.tif" pointer length

puts "pointer: $pointer length: $length"

# cctw {
#   outputData.dimensions = [128*3, 128*6, 128*4]
#   outputDataManager.filePath = "./h5test.h5"
#   cctw.calculateDependencies()
#   transformer.transform()
# }

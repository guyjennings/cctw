
package require cctw

puts "This is currently just a sketch."
exit 0

# Initialize CCTW
cctw_init

# Query CCTW for the number of chunks
set maxChunk [ cctw_maxchunks ]

for { set chunkID 0 } { $chunkID < $maxChunks } { incr chunkID } { 

    # Set the current chunk
    cctw_chunk $chunkID
    cctw {
        outputData.dimensions = [128*3, 128*6, 128*4]
        outputDataManager.filePath = "./h5test.h5"
        cctw.calculateDependencies()
        transformer.transform()
    }
}

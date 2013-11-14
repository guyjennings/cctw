
import string;

@dispatch=WORKER
(string result) cctw(string code) "cctw" "0.1"
[ "set <<result>> [ cctw <<code>> ]" ];

(int result) cctw_dimension(int i)
{
  string t = sprintf("inputData.dimensions[%i]", i);
  result = toint(cctw(t));
}

// Number of chunks along dimension i
(int result) cctw_chunks(int i)
{
  string t = sprintf("inputData.chunkCount[%i]", i);
  result = toint(cctw(t));
}

// Number of pixels in chunk along dimension i
(int result) cctw_chunk_size(int i)
{
  string t = sprintf("inputData.chunkSize[%i]", i);
  result = toint(cctw(t));
}

@dispatch=WORKER
(blob result) cctw_chunk_read(string filename) "cctw" "0.1"
[ "set <<result>> [ cctw::cctwswift_chunk_read <<filename>> ]" ];

#!/bin/zsh -ef

# Copy chunk.tiff to chunk-x-y-z.tiff

MAX_X=10
MAX_Y=10
MAX_Z=10

TOTAL=$(( MAX_X * MAX_Y * MAX_Z ))
declare TOTAL

for x in {0..$((${MAX_X}-1))}
do
  for y in {0..$((${MAX_Y}-1))}
  do
    for z in {0..$((${MAX_Z}-1))}
    do
      cp chunk.tiff chunk-${x}-${y}-${z}.tiff
    done
  done
done

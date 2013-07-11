#!/bin/zsh -ef

# Copy chunk.tiff to chunk-x-y-z.tiff

cd $(dirname $0)

pwd

MAX_X=3
MAX_Y=3
MAX_Z=3

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

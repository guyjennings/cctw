CCTW=../../cctw-release-5.2/bin/cctwcli

all:	project project-0 project-1 project-2 project-3

xf1-0.nxs:
	rm -rf xf1-0.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-0.nxs\#/entry/data/v  -c outputData.dimensions=[1000,1000,600] -c outputData.chunkSize=[100,100,60] \
	--compression 2 -c outputData.hdfChunkSize=[50,50,30] \
	--normalization 0 \
	--subset 0/4

xf1-1.nxs:
	rm -rf xf1-1.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-1.nxs\#/entry/data/v  -c outputData.dimensions=[1000,1000,600] -c outputData.chunkSize=[100,100,60] \
	--compression 2 -c outputData.hdfChunkSize=[50,50,30] \
	--normalization 0 \
	--subset 1/4

xf1-2.nxs:
	rm -rf xf1-2.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-2.nxs\#/entry/data/v  -c outputData.dimensions=[1000,1000,600] -c outputData.chunkSize=[100,100,60] \
	--compression 2 -c outputData.hdfChunkSize=[50,50,30] \
	--normalization 0 \
	--subset 2/4

xf1-3.nxs:
	rm -rf xf1-3.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-3.nxs\#/entry/data/v  -c outputData.dimensions=[1000,1000,600] -c outputData.chunkSize=[100,100,60] \
	--compression 2 -c outputData.hdfChunkSize=[50,50,30] \
	--normalization 0 \
	--subset 3/4

xf1-mrg.nxs: xf1-0.nxs xf1-1.nxs xf1-2.nxs xf1-3.nxs
	rm -rf xf1-mrg.nxs
	${CCTW} merge \
	xf1-0.nxs\#/entry/data/v \
	xf1-1.nxs\#/entry/data/v \
	xf1-2.nxs\#/entry/data/v \
	xf1-3.nxs\#/entry/data/v \
	--normalization 0 \
	--compression 2 \
	-o xf1-mrg.nxs\#/entry/data/v

xf1-mrg-norm.nxs: xf1-mrg.nxs
	rm -rf xf1-mrg-norm.nxs
	${CCTW} norm \
	xf1-mrg.nxs\#/entry/data/v \
	-o xf1-mrg-norm.nxs\#/entry/data/v

xf1-norm.nxs:
	rm -rf xf1-norm.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	-o xf1-norm.nxs\#/entry/data/v  -c outputData.dimensions=[1000,1000,600] -c outputData.chunkSize=[100,100,60] \
	-z2 -c outputData.hdfChunkSize=[50,50,30] \
	-N0

project-0: xf1-0.nxs
	rm -rf xf1-0*.tif*
	${CCTW} project xf1-0.nxs\#/entry/data/v -o xf1-0 --iproject=7

project-1: xf1-1.nxs
	rm -rf xf1-1*.tif*
	${CCTW} project xf1-1.nxs\#/entry/data/v -o xf1-1 --iproject=7

project-2: xf1-2.nxs
	rm -rf xf1-2*.tif*
	${CCTW} project xf1-2.nxs\#/entry/data/v -o xf1-2 --iproject=7

project-3: xf1-3.nxs
	rm -rf xf1-3*.tif*
	${CCTW} project xf1-3.nxs\#/entry/data/v -o xf1-3 --iproject=7

project: xf1-mrg-norm.nxs xf1-norm.nxs
	rm -rf xf1*.tif xf1*.tif.metadata
	${CCTW} project xf1-mrg-norm.nxs\#/entry/data/v -o xf1-mrg --iproject=7
	${CCTW} project xf1-norm.nxs\#/entry/data/v -o xf1 --iproject=7

clean:
	rm -rf xf1*


CCTW=../../cctw-release-5.2/bin/cctw

all:	project project-mrg project-0 project-1 project-2 project-3

testmask:  bfap00.pars
	rm -rf testmask.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/mask \
	--output testmask.nxs\#/entry/data/v \
	--compression 2 \
	--normalization 0 \
	--subset 0/100

testnomask:  bfap00.pars
	rm -rf testnomask.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--output testnomask.nxs\#/entry/data/v \
	--compression 2 \
	--normalization 0 \
	--subset 0/100

xf1-0.nxs: bfap00.pars
	rm -rf xf1-0.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-0.nxs\#/entry/data/v \
	--compression 2 \
	--normalization 0 \
	--subset 0/4

xf1-1.nxs: bfap00.pars
	rm -rf xf1-1.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-1.nxs\#/entry/data/v \
	--compression 2 \
	--normalization 0 \
	--subset 1/4

xf1-2.nxs: bfap00.pars
	rm -rf xf1-2.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-2.nxs\#/entry/data/v \
	--compression 2 \
	--normalization 0 \
	--subset 2/4

xf1-3.nxs: bfap00.pars
	rm -rf xf1-3.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	--output xf1-3.nxs\#/entry/data/v \
	--compression 2 \
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

xf1.nxs: bfap00.pars
	rm -rf xf1.nxs
	${CCTW} transform --script bfap00.pars \
	/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs\#/f1/data/v \
	--mask /home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask \
	-c inputData.chunkSize=[94,106,114] \
	-o xf1.nxs\#/entry/data/v \
	-z2 \
	-N0

xf1-norm.nxs: xf1.nxs
	rm -rf xf1-norm.nxs
	${CCTW} norm xf1.nxs\#/entry/data/v \
	-o xf1-norm.nxs\#/entry/data/v

project-0: xf1-0.nxs
	rm -rf xf1-0.[xyz].tif*
	${CCTW} project xf1-0.nxs\#/entry/data/v -o xf1-0

project-1: xf1-1.nxs
	rm -rf xf1-1.[xyz].tif*
	${CCTW} project xf1-1.nxs\#/entry/data/v -o xf1-1

project-2: xf1-2.nxs
	rm -rf xf1-2.[xyz].tif*
	${CCTW} project xf1-2.nxs\#/entry/data/v -o xf1-2

project-3: xf1-3.nxs
	rm -rf xf1-3.[xyz].tif*
	${CCTW} project xf1-3.nxs\#/entry/data/v -o xf1-3

project: xf1-norm.nxs
	rm -rf xf1.[xyz].tif*
	${CCTW} project xf1-norm.nxs\#/entry/data/v -o xf1

project-mrg: xf1-mrg-norm.nxs
	rm -rf xf1-mrg.[xyz].tif*
	${CCTW} project xf1-mrg-norm.nxs\#/entry/data/v -o xf1-mrg


clean:
	rm -rf xf1*


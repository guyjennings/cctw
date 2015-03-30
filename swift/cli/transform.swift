global const string CCTW = "../../cctw-release-5.2/bin/cctw";

// all:
file project<"project">;
file project_mrg<"project_mfg">;
file project[];

typedef nxs    file;
typedef xf_nxs file;
typedef pars   file;

xf_nxs xf1[];

app (xf_nxs xf) cctw_transform(nxs data, string dataset, pars p, string mask, int i)
{
  // rm -rf xf1-0.nxs
  CCTW "transform" "--script" p (filename(data)+"#"+dataset)
    "--mask" mask
    "-c" "inputData.chunkSize=[94,106,114]"
    "--output" (filename(xf)+"#/entry/data/v")
    "--compression"   "2"
    "--normalization" "0"
    "--subset" (toint(i)+"/4")
}

nxs data = input("/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs");
pars p = input("bfap00.pars");
mask = "/home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs\#/entry/mask";
n = 4;
foreach i in [0:n-1]
{
  xf_nxs xf = cctw_transform(data, "/f1/data/v", mask, i);
  xf1[i] = xf; 
}

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


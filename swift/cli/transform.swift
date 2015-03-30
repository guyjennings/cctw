global const string CCTW = "../../cctw-release-5.2/bin/cctw";

// all:
file project<"project">;
file project_mrg<"project_mfg">;
file projects[];

typedef nxs    file;
typedef xf_nxs file;
typedef pars   file;

xf_nxs xf1[];

app (xf_nxs xf) cctw_transform_subset(nxs data, string dataset_in, string dataset_out, pars p, string mask, int subset)
{
  // rm -rf xf1-0.nxs
  CCTW "transform" "--script" p (filename(data)+"#"+dataset_in)
    "--mask" mask
    "-c" "inputData.chunkSize=[94,106,114]"
    "--output" (filename(xf)+"#"+dataset_out)
    "--compression"   "2"
    "--normalization" "0"
    "--subset" (toString(subset)+"/4")
}

nxs data = input("/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs");
pars p = input("bfap00.pars");
mask = "/home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs#/entry/mask";
dataset_in = "/f1/data/v";
dataset_out = "/entry/data/v";
n = 4;
// Parallel loop
foreach i in [0:n-1]
{
  xf_nxs xf = cctw_transform_subset(data, dataset_in, dataset_out, p, mask, i);
  xf1[i] = xf; 
}

app (nxs mrg) cctw_merge(xf_nxs data[], string dataset)
{
  // rm -rf xf1-mrg.nxs
  CCTW "merge" data dataset
    "--normalization" "0"
    "--compression"   "2"
    "-o" (filename(mrg)+"#"+dataset);
}

file xf1_mrg<"xf1-mrg.nxs"> = cctw_merge(xf1, dataset_out);

app (nxs norm) cctw_norm(nxs data, string dataset)
{
  // rm -rf xf1-mrg-norm.nxs
  CCTW "norm" (filename(data)+"#"+dataset)
    "-o" (filename(norm)+"#"+dataset);
}

nxs xf1_mrg_norm<"xf1-mrg-norm.nxs"> = cctw_norm(xf1_mrg, dataset_out);

app (xf_nxs xf) cctw_transform(nxs data, string dataset_in, string dataset_out, pars p, string mask)
{
  // rm -rf xf1-0.nxs
  CCTW "transform" "--script" p (filename(data)+"#"+dataset_in)
    "--mask" mask
    "-c" "inputData.chunkSize=[94,106,114]"
    "--output" (filename(xf)+"#"+dataset_out)
    "--compression"   "2"
    "--normalization" "0"
}

nxs xf1_nxs<"xf1.nxs"> = cctw_transform(data, dataset_in, dataset_out, p, mask);

file xf1_norm<"xf1-norm.nxs"> = cctw_norm(xf1_nxs, dataset_out);

#if 0 

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

#endif

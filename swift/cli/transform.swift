
import sys;

typedef nxs file;

#ifdef DRYRUN
global const string CCTW = "./cctw-dryrun.sh";
nxs data = input("bfap00_170K.nxs");
#else
global const string CCTW = "../../cctw-release-5.2/bin/cctw";
nxs data = input("/home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00/kt0012a_11/bfap00_170K.nxs");
#endif

// all:
file project<"project">;
file project_mrg<"project_mfg">;
file projects[];

typedef xf_nxs file;
typedef pars   file;

xf_nxs xf1[];

app (xf_nxs xf) cctw_transform_subset(nxs data, string dataset_in, string dataset_out, pars p, string mask, int subset, int n)
{
  // rm -rf xf1-0.nxs
  CCTW "transform" "--script" p (filename(data)+"#"+dataset_in)
    "--mask" mask
    "-c" "inputData.chunkSize=[94,106,114]"
    "--output" (filename(xf)+"#"+dataset_out)
    "--compression"   "2"
    "--normalization" "0"
    "--subset" ("%i/%i"%(subset,n))
}

pars parameters = input("bfap00.pars");
mask = "/home/bessrc/sharedbigdata/data1/osborn-2014-1/pilatus_mask.nxs#/entry/mask";
dataset_in = "/f1/data/v";
dataset_out = "/entry/data/v";
// n = 4;
n = toint(argv("n"));
// Parallel loop
foreach i in [0:n-1]
{
  xf_nxs xf<"xf-%i.nxs"%i> = cctw_transform_subset(data, dataset_in, dataset_out, parameters, mask, i, n);
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

nxs xf1_nxs<"xf1.nxs"> = cctw_transform(data, dataset_in, dataset_out, parameters, mask);

nxs xf1_norm<"xf1-norm.nxs"> = cctw_norm(xf1_nxs, dataset_out);

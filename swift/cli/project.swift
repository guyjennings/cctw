
typedef projection file;

app (projection o) cctw_project(nxs data, string dataset)
{
  // rm -rf xf1-0.[xyz].tif*
  CCTW "project" (filename(data)+"#"+dataset)
    "-o" o;
}

foreach i in [0:n-1]
{
  projection p<"project-%i"%i> = cctw_project(xf1[i], dataset_out);
}

projection xf1_prj<"xf1"> = cctw_project(xf1_norm, dataset_out);

projection xf1_mrg_prj<"xf1-mrg"> = cctw_project(xf1_mrg_norm, dataset_out);

#!/bin/bash
set -eu

# Tukey:
CCTW="nice ${HOME}/proj/x86_64/cctw/bin/cctw"
DATA=${HOME}/cctw-tests
OUTPUT_FILE=${DATA}/mullite_run1_transform.nxs
OUTPUT=${OUTPUT_FILE}\#/entry/data

rm -fv ${OUTPUT_FILE}

${CCTW} transform \
  --script ${DATA}/mullite_run1_transform.pars \
  --mask   ${DATA}/mask_mullite_run1.nxs\#/entry/mask \
  ${DATA}/mullite.nxs\#/mullite_run1/data/data \
  -o ${OUTPUT} \
  --normalization 0

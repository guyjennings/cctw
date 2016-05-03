TEMPLATE=subdirs

QMAKE_EXTRA_TARGETS += f1_transform f1_transform_masked

CCTW                 = ../bin/cctw
F1DATAFILE           = ../../cctw-data/lsmo54_100K.nxs
F1DATASET            = /f1/data/data
F1PARS               = ../../cctw-data/100K/f1_transform.pars
F1MASKFILE           = ../../cctw-data/lsmo_mask.nxs
F1MASKDATASET        = /mask

#f1_transform.target   = transform.nxs
f1_transform.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -o f1_transform.nxs\\$${LITERAL_HASH}transform | tee f1_transform.log

f1_transform_masked.depends = $${F1DATAFILE} $${F1MASKFILE} $${F1PARS}
f1_transform_masked.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -M $${F1MASKFILE}\\$$LITERAL_HASH$${F1MASKDATASET} -o f1_transform_masked.nxs\\$${LITERAL_HASH}transform | tee f1_transform.log

TEMPLATE=subdirs

CCTW                 = ../bin/cctw
F1DATAFILE           = ../../cctw-data/lsmo54_100K.nxs
F1DATASET            = /f1/data/data
F1PARS               = ../../cctw-data/100K/f1_transform.pars
F1MASKFILE           = ../../cctw-data/lsmo_mask.nxs
F1MASKDATASET        = /mask

QMAKE_EXTRA_TARGETS += f1_transform
f1_transform.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -o f1_transform.nxs\\$${LITERAL_HASH}transform | tee f1_transform.log

QMAKE_EXTRA_TARGETS += f1_transform_masked
f1_transform_masked.depends = $${F1DATAFILE} $${F1MASKFILE} $${F1PARS}
f1_transform_masked.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -M $${F1MASKFILE}\\$$LITERAL_HASH$${F1MASKDATASET} -o f1_transform_masked.nxs\\$${LITERAL_HASH}transform | tee f1_transform.log

QMAKE_EXTRA_TARGETS += f1_transform_0
f1_transform_0.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform_0.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -S 0/4 -o f1_transform_0.nxs\\$${LITERAL_HASH}transform | tee f1_transform_0.log

QMAKE_EXTRA_TARGETS += f1_transform_1
f1_transform_1.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform_1.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -S 1/4 -o f1_transform_1.nxs\\$${LITERAL_HASH}transform | tee f1_transform_1.log

QMAKE_EXTRA_TARGETS += f1_transform_2
f1_transform_2.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform_2.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -S 2/4 -o f1_transform_2.nxs\\$${LITERAL_HASH}transform | tee f1_transform_2.log

QMAKE_EXTRA_TARGETS += f1_transform_3
f1_transform_3.depends   = $${F1DATAFILE} $${F1PARS}
f1_transform_3.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -S 3/4 -o f1_transform_3.nxs\\$${LITERAL_HASH}transform | tee f1_transform_3.log

QMAKE_EXTRA_TARGETS += f1_transform_merged
f1_transform_merged.depends = f1_transform_0.nxs f1_transform_1.nxs f1_transform_2.nxs f1_transform_3.nxs
f1_transform_merged.commands += $${CCTW} merge f1_transform_0.nxs\\$${LITERAL_HASH}transform f1_transform_1.nxs\\$${LITERAL_HASH}transform f1_transform_2.nxs\\$${LITERAL_HASH}transform f1_transform_3.nxs\\$${LITERAL_HASH}transform --normalization 0 --compression 2 -o f1_transform_merged.nxs\\$${LITERAL_HASH}merged | tee f1_transform_merged.log

QMAKE_EXTRA_TARGETS += f1_project_0
f1_project_0.depends = f1_transform_0.nxs
f1_project_0.commands += $${CCTW} project f1_transform_0.nxs\\$${LITERAL_HASH}transform -o f1_transform_0

QMAKE_EXTRA_TARGETS += f1_project_1
f1_project_1.depends = f1_transform_1.nxs
f1_project_1.commands += $${CCTW} project f1_transform_1.nxs\\$${LITERAL_HASH}transform -o f1_transform_1

QMAKE_EXTRA_TARGETS += f1_project_2
f1_project_2.depends = f1_transform_2.nxs
f1_project_2.commands += $${CCTW} project f1_transform_2.nxs\\$${LITERAL_HASH}transform -o f1_transform_2

QMAKE_EXTRA_TARGETS += f1_project_3
f1_project_3.depends = f1_transform_3.nxs
f1_project_3.commands += $${CCTW} project f1_transform_3.nxs\\$${LITERAL_HASH}transform -o f1_transform_3

QMAKE_EXTRA_TARGETS += f1_project_merged
f1_project_merged.depends = f1_transform_merged.nxs
f1_project_merged.commands += $${CCTW} project f1_transform_merged.nxs\\$${LITERAL_HASH}merged -o f1_transform_merged

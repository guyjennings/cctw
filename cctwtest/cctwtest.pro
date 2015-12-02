TEMPLATE=subdirs

QMAKE_EXTRA_TARGETS += f1_transform

CCTW                 = ../bin/cctw
F1DATAFILE           = ../../cctw-data/lsmo54_100K.nxs
F1DATASET            = /f1/data/data
F1PARS               = ../../cctw-data/100K/f1_transform.pars

#f1_transform.target   = transform.nxs
f1_transform.depends   = $${DATAFILE} $${PARS}
f1_transform.commands += $${CCTW} transform --script $${F1PARS} $${F1DATAFILE}\\$$LITERAL_HASH$${F1DATASET} -o f1_transform.nxs\\$${LITERAL_HASH}transform | tee f1_transform.log

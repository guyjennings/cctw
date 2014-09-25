#
# Default build configuration - to make a custom build, copy this file
# to cctw-config-local.pri and comment out unwanted options
#

# Control which build subprojects are built...

CONFIG += BUILD_CCTWCLI
CONFIG += BUILD_CCTWQT
CONFIG += BUILD_CCTWTCL
CONFIG += BUILD_CCTWLIB
CONFIG += BUILD_CCTWMPI

# Subsidiary options

CONFIG += INCLUDE_IMPORT_COMMANDS
CONFIG += INCLUDE_ANALYSIS_COMMANDS


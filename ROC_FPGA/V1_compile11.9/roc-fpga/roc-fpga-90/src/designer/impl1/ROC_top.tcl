# Created by Microsemi Libero Software 11.9.0.4
# Wed Jun 02 22:40:49 2021

# (OPEN DESIGN)

open_design "ROC_top.adb"

# set default back-annotation base-name
set_defvar "BA_NAME" "ROC_top_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {C:\Users\curlywei\Documents\roc-fpga-old\src_old\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"


layout -timing_driven
report -type "status" {ROC_top_place_and_route_report.txt}
report -type "globalnet" {ROC_top_globalnet_report.txt}
report -type "globalusage" {ROC_top_globalusage_report.txt}
report -type "iobank" {ROC_top_iobank_report.txt}
report -type "pin" -listby "name" {ROC_top_report_pin_byname.txt}
report -type "pin" -listby "number" {ROC_top_report_pin_bynumber.txt}

save_design

scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer.cpp .
scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer.h .
scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer_run.cpp .
scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer_run_multi.sh .

scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer_calib_packv5_040523_0434.cpp .
scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer_multi.cpp .

 scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/analyzer_run_readmap.cpp .

#root -l ladder_cali_test.cpp

#rm test_folder/*

#root -l -b -q ladder_cali_test.cpp

#root -l -b -q analyzer_run.cpp

#ladder_cali.cpp
#ladder_cali.h
#ladder_cali_test.cpp

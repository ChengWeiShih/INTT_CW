scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/ladder_cali.cpp .
scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/ladder_cali.h .
#scp phnxrc@opc0.sphenix.bnl.gov:/home/phnxrc/INTT/cwshih/cali_analyzer/ladder_cali_test.cpp .

#root -l ladder_cali_test.cpp

#rm test_folder/*

#root -l -b -q ladder_cali_test.cpp

#ladder_cali.cpp
#ladder_cali.h
#ladder_cali_test.cpp

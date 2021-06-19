=======================================================================================
INTT Channel classification V3, 2021/06/19

new feature  : 
	formate of input file : .dat (not .root)
	root version : 6.06/01
	new plot     : Summary_average_entry.pdf
	final good channel ratio calculation is added.

=======================================================================================
INTT Channel classification V2, 2021/06/10

root version : 5.34

new feature : input file formate : .dat, not .root

add the limit of Z-axis of final plot 

running procedure become eaiser

in the run.sh, I add "sleep 15" aiming to make the code more stable when running


=======================================================================================
INTT Channel classification V1, 2021/06/07
This macro requires 4 files to run.
1. calibration_ana_code_multi.c
2. summary_plot.c
3. run.sh
4. total_file.txt

For file 4, it can be created by "ls fphx_raw_*.root > total_file.txt"
Please delete the text ".root" in this file.





For more detailed introduction, please check "Channel classification instruction.pdf"

=======================================================================================


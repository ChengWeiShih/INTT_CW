################################################################################
#  SDC WRITER VERSION "3.1";
#  DESIGN "ROC_top";
#  Timing constraints scenario: "Primary";
#  DATE "Sun Jul 31 08:33:33 2011";
#  VENDOR "Actel";
#  PROGRAM "Actel Designer Software Release v9.1";
#  VERSION "9.1.0.18"  Copyright (C) 1989-2010 Actel Corp. 
################################################################################


set sdc_version 1.7


########  Clock Constraints  ########

create_clock  -name { OUT_CLK_n } -period 3.333 -waveform { 0.000 1.667  }  { OUT_CLK_n  } 

create_clock  -name { BCO_CLK_n } -period 33.333 -waveform { 0.000 16.667  }  { BCO_CLK_n  } 



########  Generated Clock Constraints  ########





########  Clock Source Latency Constraints #########



########  Input Delay Constraints  ########



########  Output Delay Constraints  ########



########   Delay Constraints  ########



########   Delay Constraints  ########



########   Multicycle Constraints  ########



########   False Path Constraints  ########



########   Output load Constraints  ########



########  Disable Timing Constraints #########



########  Clock Uncertainty Constraints #########




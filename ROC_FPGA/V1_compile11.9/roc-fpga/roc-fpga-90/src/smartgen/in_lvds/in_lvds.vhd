-- Version: 7.3 7.3.0.29

library ieee;
use ieee.std_logic_1164.all;
library proasic3e;
use proasic3e.all;

entity in_lvds is 
    port(PADP, PADN : in std_logic;  Y : out std_logic) ;
end in_lvds;


architecture DEF_ARCH of  in_lvds is

    component INBUF_LVDS
        port(PADP, PADN : in std_logic := 'U'; Y : out std_logic
        ) ;
    end component;

begin   

    INBUF_LVDS_0_inst : INBUF_LVDS
      port map(PADP => PADP, PADN => PADN, Y => Y);
end DEF_ARCH;

-- _Disclaimer: Please leave the following comments in the file, they are for internal purposes only._


-- _GEN_File_Contents_

-- Version:7.3.0.29
-- ACTGENU_CALL:1
-- BATCH:T
-- FAM:ProASIC3E
-- OUTFORMAT:VHDL
-- LPMTYPE:LPM_IO_INBUF
-- LPM_HINT:INBUF_SP
-- INSERT_PAD:NO
-- INSERT_IOREG:NO
-- GEN_BHV_VHDL_VAL:F
-- GEN_BHV_VERILOG_VAL:F
-- MGNTIMER:F
-- MGNCMPL:F
-- "DESDIR:D:/Anuj/daq/roc_fem_8chip/smartgen\in_lvds"
-- GEN_BEHV_MODULE:T
-- SMARTGEN_DIE:IT6X6M2
-- SMARTGEN_PACKAGE:pq208
--  WIDTH:1
--  TYPE:LVDS
--  TRIEN_POLARITY:0
--  CLR_POLARITY:0

-- _End_Comments_


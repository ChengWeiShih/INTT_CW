--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    11:02:00 03/13/07
-- Design Name:    
-- Module Name:    ROC_top - RTL
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;
library proasic3e;
use proasic3e.all;
library synplify;
use synplify.attributes.all; 

entity ROC_top is
    Port (  RST 							   : in std_logic;
   	      LATCH_0 							: in std_logic;
   	      LATCH_1 							: in std_logic;
   	      SEND_DATA_0 					: in std_logic;
   	      SEND_DATA_1 					: in std_logic;
   		   DATA_IN_0_n 					: in std_logic_vector(71 downto 0);
   		   DATA_IN_0_p 					: in std_logic_vector(71 downto 0);
   		   DATA_IN_1_n 					: in std_logic_vector(103 downto 0);
   		   DATA_IN_1_p 					: in std_logic_vector(103 downto 0);
            OUT_CLK_n					   : in std_logic;
            OUT_CLK_p					   : in std_logic;
            BCO_CLK_n					   : in std_logic;
            BCO_CLK_p					   : in std_logic;
            SHIFT_IN                   : in std_logic;
            SHIFT_EN                   : in std_logic;
            SHIFT_OUT                  : out std_logic;
            TKMSB_A00H                 : out std_logic;
            TKMSB_A00L                 : out std_logic;
            TKMSB_A01H                 : out std_logic;
            TKMSB_A01L                 : out std_logic;
            TKMSB_A10H                 : out std_logic;
            TKMSB_A10L                 : out std_logic;
            TKMSB_A11H                 : out std_logic;
            TKMSB_A11L                 : out std_logic;
            TKLSB_A00H                 : out std_logic;
            TKLSB_A00L                 : out std_logic;
            TKLSB_A01H                 : out std_logic;
            TKLSB_A01L                 : out std_logic;
            TKLSB_A10H                 : out std_logic;
            TKLSB_A10L                 : out std_logic;
            TKLSB_A11H                 : out std_logic;
            TKLSB_A11L                 : out std_logic;
            DATA_OUT_0_0 					: out std_logic_vector(31 downto 0);
            DATA_OUT_0_1 					: out std_logic_vector(31 downto 0);
            DATA_OUT_1_0 					: out std_logic_vector(31 downto 0);
            DATA_OUT_1_1 					: out std_logic_vector(31 downto 0)
         );
end ROC_top;

architecture RTL of ROC_top is
 
  component in_lvds 
    port(PADP, PADN : in std_logic;  Y : out std_logic) ;
  end component;

  component out_lvds
    port(Data : in std_logic; PADP, PADN : out std_logic);
  end component;

  component roc_dcm
    port(POWERDOWN	: in std_logic;
	 CLKA		: in std_logic;  
	 LOCK		: out std_logic;
	 GLA		: out std_logic;
	 GLB		: out std_logic
	 ) ;
  end component;

   component roc_block is
    Port ( 	
       RST : in std_logic;
       BCO_CLK : in std_logic;
       CLK : in std_logic;
       CLK90 : in std_logic;
       OUT_CLK : in std_logic;
       LATCH : in std_logic;
       SEND_DATA_0 : in std_logic;
       SEND_DATA_1 : in std_logic;
       DATA_IN_0 : in std_logic_vector(71 downto 0);
       DATA_IN_1 : in std_logic_vector(103 downto 0);
       SHIFT_IN : in std_logic;
       SHIFT_EN : in std_logic;
       SHIFT_OUT : out std_logic;
       LSB : out std_logic_vector(7 downto 0);
       MSB : out std_logic_vector(7 downto 0);
       DATA_OUT_0 : out std_logic_vector(31 downto 0);
       DATA_OUT_1 : out std_logic_vector(31 downto 0);
       DATA_OUT_2 : out std_logic_vector(31 downto 0);
       DATA_OUT_3 : out std_logic_vector(31 downto 0);
       TEST_OUT : out std_logic_vector(7 downto 0));
   end component;

   component BUFD
     port(A: in std_logic := 'U'; Y : out std_logic) ;
   end component;

--   signal WRITE_CLK : std_logic;
   signal BCO_CLK : std_logic;
   signal OUT_CLK : std_logic;
   signal CLK : std_logic;
   signal CLK90 : std_logic;
   signal LOCKED_int : std_logic;
   signal GLOBAL_RST : std_logic;
   signal DATA_IN_0: std_logic_vector(71 downto 0);
   signal DATA_IN_1: std_logic_vector(103 downto 0);
   signal DATA_IN_0_tmp: std_logic_vector(71 downto 0);
   signal DATA_IN_1_tmp: std_logic_vector(103 downto 0);
   signal TEST_OUT: std_logic_vector(1 downto 0);

   signal LSB : std_logic_vector(7 downto 0);
   signal LSB_tmp0 : std_logic_vector(7 downto 0);
   signal LSB_tmp1 : std_logic_vector(7 downto 0);

   signal MSB : std_logic_vector(7 downto 0);
   signal MSB_tmp0 : std_logic_vector(7 downto 0);
   signal MSB_tmp1 : std_logic_vector(7 downto 0);

   signal DATA_OUT_0_0_tmp : std_logic_vector(31 downto 0);
   signal DATA_OUT_0_1_tmp : std_logic_vector(31 downto 0);
   signal DATA_OUT_1_0_tmp : std_logic_vector(31 downto 0);
   signal DATA_OUT_1_1_tmp : std_logic_vector(31 downto 0);

   signal DATA_OUT_0_0_int : std_logic_vector(31 downto 0);
   signal DATA_OUT_0_1_int : std_logic_vector(31 downto 0);
   signal DATA_OUT_1_0_int : std_logic_vector(31 downto 0);
   signal DATA_OUT_1_1_int : std_logic_vector(31 downto 0);


begin

process (OUT_CLK)
begin 
   if rising_edge(OUT_CLK) then
      GLOBAL_RST <= RST or not(LOCKED_int);
	end if;
end process;


--input_arr_0: for index in 0 to 71 generate 
--begin
--    in_data_0 : in_lvds
--    port map(PADP => DATA_IN_0_p(index),
--             PADN => DATA_IN_0_n(index),
--             Y => DATA_IN_0(index)
--             );
--end generate;
--
--input_arr_1: for index in 0 to 103 generate 
--begin
--    in_data_1 : in_lvds
--    port map(PADP => DATA_IN_1_p(index),
--             PADN => DATA_IN_1_n(index),
--             Y => DATA_IN_1(index)
--             );
--end generate;

--For ROC-II testing, _n and _p are swapped on connectors
input_arr_0: for index in 0 to 71 generate 
begin
    in_data_0 : in_lvds
    port map(PADP => DATA_IN_0_p(index),
             PADN => DATA_IN_0_n(index),
             Y => DATA_IN_0_tmp(index)
             );
end generate;

--Determined that data_sta0_wedge1_side1_chip05_out1 and data_sta0_wedge1_side1_chip04_out1
--are inverted in layout. Invert them here:


DATA_IN_0(16 downto 0) <= DATA_IN_0_tmp(16 downto 0);
DATA_IN_0(17) <= not DATA_IN_0_tmp(17); --not DATA_IN_0_tmp(17); --this is correct
--Try inverting 18 even though layout does not indicate it (because chip 5 seems to be missing 1/2 of its data):
DATA_IN_0(18) <= DATA_IN_0_tmp(18); --not DATA_IN_0_tmp(18); --this is correct
DATA_IN_0(19) <= DATA_IN_0_tmp(19); --not DATA_IN_0_tmp(19); --this is correct
DATA_IN_0(63 downto 20) <= DATA_IN_0_tmp(63 downto 20);
DATA_IN_0(64) <= not DATA_IN_0_tmp(64); --test (65 swap did not work)
DATA_IN_0(71 downto 65) <= DATA_IN_0_tmp(71 downto 65);

input_arr_1: for index in 0 to 103 generate 
begin
    in_data_1 : in_lvds
    port map(PADP => DATA_IN_1_p(index),
             PADN => DATA_IN_1_n(index),
             Y => DATA_IN_1_tmp(index)
             );
end generate;

--Determined that data_sta3_wedge1_side1_chip07_out1 
--is inverted in layout. Invert it here:

DATA_IN_1(43 downto 0)  <= DATA_IN_1_tmp(43 downto 0);
DATA_IN_1(44)  <= not DATA_IN_1_tmp(44); --44 is correct (inverting 45 was incorrect)
DATA_IN_1(95 downto 45)  <= DATA_IN_1_tmp(95 downto 45);
DATA_IN_1(96) <= not DATA_IN_1_tmp(96); --this is correct
DATA_IN_1(103 downto 97)  <= DATA_IN_1_tmp(103 downto 97);

  Inst_roc_dcm: roc_dcm
    PORT MAP(powerdown => '1',
             CLKA => BCO_CLK,
             LOCK => LOCKED_int,
             GLA => CLK,
             GLB => CLK90
             );

  Inst_clk : in_lvds
    port map(PADP => BCO_CLK_p,
             PADN => BCO_CLK_n,
             Y => BCO_CLK
             );

  Inst_out_clk : in_lvds
    port map(PADP => OUT_CLK_p,
             PADN => OUT_CLK_n,
             Y => OUT_CLK
             );


  --Note: FPGA "B" needs to use LATCH_1 rather than LATCH_0 because LATCH_B0 has been used by
  --SC FPGA to send LSB to SC TLK.
  Inst_roc_block : roc_block
    port map(
             RST => GLOBAL_RST,
             BCO_CLK => BCO_CLK,
             CLK => CLK,
             CLK90 => CLK90,
             OUT_CLK => OUT_CLK,
             DATA_IN_0 => DATA_IN_0,
             DATA_IN_1 => DATA_IN_1,
             LATCH => LATCH_1,
             SEND_DATA_0 => '1', --SEND_DATA_0,
             SEND_DATA_1 => '1', --SEND_DATA_1,
             SHIFT_EN => SHIFT_EN,
             SHIFT_IN => SHIFT_IN,
             SHIFT_OUT => open, --SHIFT_OUT,
             LSB => LSB,
             MSB => MSB,
             DATA_OUT_0 => DATA_OUT_1_0_int,
             DATA_OUT_1 => DATA_OUT_1_1_int,
             DATA_OUT_2 => DATA_OUT_0_0_int,
             DATA_OUT_3 => DATA_OUT_0_1_int,
             TEST_OUT => open
             );

  SHIFT_OUT <= LATCH_1;--DATA_OUT_1_0_tmp(0); --LATCH_0;
  --SHIFT_EN <= DATA_IN_0_tmp(0); --TEST_OUT(1);


process (OUT_CLK,GLOBAL_RST)
begin 
	if (GLOBAL_RST = '1') then
      TKLSB_A00H <= '0';
      TKLSB_A00L <= '0';
      TKLSB_A01H <= '0';
      TKLSB_A01L <= '0';
      TKLSB_A10H <= '0';
      TKLSB_A10L <= '0';
      TKLSB_A11H <= '0';
      TKLSB_A11L <= '0';
      TKMSB_A00H <= '0';
      TKMSB_A00L <= '0';
      TKMSB_A01H <= '0';
      TKMSB_A01L <= '0';
      TKMSB_A10H <= '0';
      TKMSB_A10L <= '0';
      TKMSB_A11H <= '0';
      TKMSB_A11L <= '0';
      DATA_OUT_0_0_tmp <= (others => '0');
      DATA_OUT_0_1_tmp <= (others => '0');
      DATA_OUT_1_0_tmp <= (others => '0');
      DATA_OUT_1_1_tmp <= (others => '0');
      DATA_OUT_0_0 <= (others => '0');
      DATA_OUT_0_1 <= (others => '0');
      DATA_OUT_1_0 <= (others => '0');
      DATA_OUT_1_1 <= (others => '0');
      LSB_tmp0 <= (others => '0');
      LSB_tmp1 <= (others => '0');
      MSB_tmp0 <= (others => '0');
      MSB_tmp1 <= (others => '0');
	elsif rising_edge(OUT_CLK) then  --default
	--elsif falling_edge(OUT_CLK) then --this does not seem to work at all
      LSB_tmp0 <= LSB;
      LSB_tmp1 <= LSB_tmp0;
      MSB_tmp0 <= MSB;
      MSB_tmp1 <= MSB_tmp0;
      -- see excell spreadsheet for exact mapping
      TKLSB_A00H <= LSB_tmp1(5);
      TKLSB_A00L <= LSB_tmp1(4);
      TKLSB_A01H <= LSB_tmp1(7);
      TKLSB_A01L <= LSB_tmp1(6);
      TKLSB_A10H <= LSB_tmp1(0);
      TKLSB_A10L <= LSB_tmp1(1);
      TKLSB_A11H <= LSB_tmp1(2);
      TKLSB_A11L <= LSB_tmp1(3);

      TKMSB_A00H <= MSB_tmp1(5);
      TKMSB_A00L <= MSB_tmp1(4);
      TKMSB_A01H <= MSB_tmp1(7);
      TKMSB_A01L <= MSB_tmp1(6);
      TKMSB_A10H <= MSB_tmp1(0);
      TKMSB_A10L <= MSB_tmp1(1);
      TKMSB_A11H <= MSB_tmp1(2);
      TKMSB_A11L <= MSB_tmp1(3);

      DATA_OUT_0_0_tmp <= DATA_OUT_0_0_int;
      DATA_OUT_0_1_tmp <= DATA_OUT_0_1_int;
      DATA_OUT_1_0_tmp <= DATA_OUT_1_0_int;
      DATA_OUT_1_1_tmp <= DATA_OUT_1_1_int;
      --DATA_OUT need one extra clock delay to be in time with LSBs:
      DATA_OUT_0_0 <= DATA_OUT_0_0_tmp;
      DATA_OUT_0_1 <= DATA_OUT_0_1_tmp;
      --Station 3 needs to have 16 bit chunks inverted:
      DATA_OUT_1_0(31 downto 16) <= DATA_OUT_1_0_tmp(15 downto 0);
      DATA_OUT_1_0(15 downto 0) <= DATA_OUT_1_0_tmp(31 downto 16);
      --DATA_OUT_1_0_int <= DATA_OUT_1_0_tmp;
      --Test inverting 16 bit chunks for station 4:
      DATA_OUT_1_1(31 downto 16) <= DATA_OUT_1_1_tmp(15 downto 0);
      DATA_OUT_1_1(15 downto 0) <= DATA_OUT_1_1_tmp(31 downto 16);
      --DATA_OUT_1_1_int <= DATA_OUT_1_1_tmp;
	end if;
end process;

end RTL;


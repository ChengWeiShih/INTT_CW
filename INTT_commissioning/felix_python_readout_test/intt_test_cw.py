import time

RESET_FPHX = 0x0B
LATCH_FPGA = 0X09
CALIB = 0X05
FPHX_COMMAND = 0X03

def load_datansize(d):
	d.reg.roc_wildcard = 1<<1
	d.reg.roc_wildcard = 0
 
def start_sm(d):
	d.reg.roc_wildcard = 1
	d.reg.roc_wildcard = 0

def reset_fpga(d):
	write_scparam(d, 0x04, 0x00, 0xff, 0xff)
	load_datansize(d)
	start_sm(d)

def reset_fphx(d):
	write_scparam(d, 0x0b, 0xff, 0xff, 0xff)
	load_datansize(d)
	start_sm(d)

# note : send a pre-defined word from ROC to Felix through the optical fiber driver.
# note : therefore the Felix can know what is going on.
# note : the latch can do the sync, and enable the Felix data taking.
def latch_fpga(d):
	write_scparam(d, 0x09, 0xff, 0xff, 0xff)
	load_datansize(d)
	start_sm(d)

def calib(d):
	write_scparam(d, 0x05, 0x00, 0x03, 0xff)
	load_datansize(d)
	time.sleep(1)
	start_sm(d)

# note : the EnableRO is moved out from the default setting list
# note : regID 2
def enable_ro(d):
	send_fphx_cmd(d, 0xCF511070)

def enable_channel(d, chan):
	rd = d.reg.enable
	d.reg.enable = rd | (1<<chan)

def disable_channel(d, chan):
	rd = d.reg.enable
	d.reg.enable = rd & ~(1<<chan)

def macro_calib(d, spacing =1199, n_pulses =10, n_ampl_steps =63, ampl_step =1):
	reset_fpga(d)
	reset_fphx(d)
	ld_fphxparam(d)
	enable_ro(d)
	latch_fpga(d)

	send_calib_param(d, spacing, n_pulses, n_ampl_steps, ampl_step)

	return

def send_calib_param(d, spacing, n_pulses, n_ampl_steps, ampl_step):

	word = 0x0
	word |= (0xffff & spacing) << (48-16)
	word |= (0xff & n_pulses) << (48-16-8)
	word |= (0xffff & n_ampl_steps) << (48-16-8-16)
	word |= (0xff & ampl_step)

	comm_parsed = [ ((word >> (48-8-i)) & 0x000000000FF) for i in range(0,48,8)]

	for item in comm_parsed:
		write_scparam(d, 0x0c, 0xff, item, 0xff)
		load_datansize(d)
		time.sleep(.001)

	start_sm(d)

# note : send the slow control command
# note : wedge : 0 ~ 31, or 0xff for all the wedges
def send_fphx_cmd(d, fphx_word, wedge=0xff):
	comm_parsed = [ ((fphx_word >> (32-8-i)) & 0x000000FF) for i in range(0,32,8)]

	for item in comm_parsed:
		write_scparam(d, 0x03, wedge, item, 0xff)
		load_datansize(d)
#		time.sleep(.001)

	start_sm(d)

# note : this is the function to generate the slow control parameter word.
# note : select chipID : 1 ~ 13, or 21 for all 13 chips.
# note : the chipID is from 1 ~ 13, each half-ladder connects to two wedges.
# note : Each wedge is responsible for 13 chips, (one side)
# note : therefore, one of our job is to find out which wedge corresponds to whcih side of the half-ladder. 
# note : select regsiterID : there are 18 registers, can be checked by the FPHX document
# note : cmd = 1 (I don't know why)
# note : data : the value you want to set, doesn't need to convert it to bit, just the value.
def make_fphx_cmd(chipId, regId, cmd, data):
	header = 0x67
	trailer = 0x0
	word = int(0)
	word = header << (32-7)
	word |= (0x1f & chipId) << (32-12)
	word |= (0x1f & regId) << (32-17)
	word |= (0x7 & cmd) << (32-20)
	word |= (0xff & data) << (32-28)
	word |= (0xf & trailer)
	return word

# note : the following is the dead setting. the default setting
# note : there are 18 registers in total, but only 17 are weitten in the follwing. 
# note : The second one, the "digital control (EnableRO)" (RO stands for Read Out) is in the list.
# note : but the enable_ro overweite it.

# note : the 18th regisiter is missing, and the 
# note : the default setting is updated, customized for INTT, done by CW, Oct/17/2022
def ld_fphxparam(d, cheat=None):
	cheat = [
	0xCF50A800, # mask channel, = make_fphx_cmd(21,1,2,128)
	0XCF511050, # Digital control (enable RO), will be overwritten
	0XCF519010,
	0xCF5210F0, # DAC0 15
	0xCF529130, # DAC1 19
	0xCF531170, # DAC2 23
	0xCF5391B0, # DAC3 27
	0xCF5411F0, # DAC4 31
	0xCF549230, # DAC5 35
	0xCF551270, # DAC6 39
	0xCF5592B0, # DAC7 43
	0XCF561460,
	0XCF569040,
	0XCF571400,
	0XCF579420, # Gain and BandWidth selections
	0XCF581050,
	0XCF5893F0, # the LVDS
	]

	for item in cheat:
		send_fphx_cmd(d, item)
		time.sleep(.001)

#note : 'command' is the data word from Felix to ROC
# note : 'dataword' is from ROC to FPHX chips, but is packed in the final word
# note : just like a package in a ship.
# note : wedge is the ROC ports, from 0 ~ 31. all ports : 0xFF
def write_scparam(d, command, wedge, dataword, sizeword):
	d.reg.roc_command = command
	d.reg.roc_wedge = wedge
	d.reg.roc_dataword = dataword
	d.reg.roc_sizeword = sizeword

# note : the read backer, the last bit information is missing, due to the longer data transmission line.
def read_fphx(d, chip, register, wedge):
	send_fphx_cmd(d, make_fphx_cmd(chip, register, 0, 0), wedge)
	readback = d.reg.fphx_readback
	output = [None,None]

	binary = bin(readback)
	reverse = binary[-1:1:-1]
	reverse = reverse + (8 - len(reverse))*'0'

	output[0] = int(reverse[:-1],2)
	output[1] = int(reverse[:-1], 2) + 128

	return output

def write_fphx(d, chip, register, wedge, value, cmd=1):
	fphx_word = make_fphx_cmd(chip, register, cmd, value)
	send_fphx_cmd(d, fphx_word, wedge)


def cold_start(d):
	if d.reg.si5345_pll.nLOL == 0:
		d.reg.si5345_pll.program = 1
		time.sleep(5)

	d.reg.clks_ctl.rst_clksgth = 1 #melhorar a geracao de clock
	time.sleep(1)
	d.reg.clks_ctl.rst_clksgth = 0 #melhorar a geracao de clock
	time.sleep(2)
	d.reg.rst_gth = 1

	d.reg.rst_gth = 0
	time.sleep(1)
	reset_fpga(d)
	time.sleep(1)
	d.reg.latch_sync_rst = 1
	d.reg.latch_sync_rst = 0
	d.reg.rst=1
	d.reg.rst=0
	time.sleep(1)
	latch_fpga(d)
	time.sleep(1)
	print("Slow Control Sync: " + str(d.reg.sync_ok))
	print("Latch FPGA: " + str(d.reg.latch_sync))
	reset_fphx(d)
	if (d.reg.sync_ok == 1 and d.reg.latch_sync == 1):
		return 1
	else:
		return 0



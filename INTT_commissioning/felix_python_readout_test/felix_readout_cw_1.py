import numpy as np
from dam import *
import time
import datetime

import intt

d = dam()
d.reset()

today = datetime.datetime.now()
filename = ("/home/intt/dam_pcie/python/data/"+"calib_"+today.strftime("%m%d%y_%H%M"))
# filename = ("/home/intt/dam_pcie/python/data/"+"calib_"+today.strftime("%y%m%d_%H%M")) #better format for us (2022/11/1 Genki)

# note : the setting function from intt.py
intt.macro_calib(d)
#calib(d)

# note : open the channels of felix, enable to take the data
d.reg.ldd_enable = 1

# note : send the signal to the ROC, to ask ROC generate the test pulses.
d.reg.clks_ctl.calib_stclk = 1
d.reg.clks_ctl.calib_stclk = 0 # note : once the signal is sent, stop sending the signal

start = time.time()
end = time.time()

a = [] # the list to hold the data
b = []
PEDACO = 10000000
MINUTES = 4
SECONDS = 0
TIME = MINUTES*60 + SECONDS # [s]

while (end - start < TIME):
	rd = d.read(PEDACO) # note : rd is the calibration data, not just one value, it should be also a list.
	if len(rd) == 0:
		d.reset()
	else:
		a.append(rd)

	if len(rd) < int(PEDACO/2) and len(rd) != 0:
		time.sleep(0.1)
		print(len(rd))
	if len(rd) == int(PEDACO/2):
		print("NO LIMITE ----------")
	end = time.time()

# note : after 4 mins, we know that the ROC finishs the pulse generation, close the Felix channel
d.reg.ldd_enable = 0

# note : save the data to npy file.
if len(a) != 0:
	np.save(filename,a)


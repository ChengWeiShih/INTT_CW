import tkinter as tk
import datetime 
from datetime import date

# Create the GUI window
window = tk.Tk()
window.title("ELOG calibration test update")
window.geometry("1000x300")

n_row = 1

# Create the cells
roc_id_label = tk.Label(window, text="ROC ID : ")
roc_id_label.grid(row=n_row, column=1)

roc_id_entry = tk.Entry(window,width = 25)
roc_id_entry.grid(row=n_row, column=2, columnspan = 2)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= 

n_row += 1

roc_volt1_label = tk.Label(window, text="ROC Voltage 1 (V) : ")
roc_volt1_label.grid(row=n_row, column=1)

roc_volt1_entry = tk.Entry(window,width=5,textvariable=tk.StringVar(value="5.8"))
roc_volt1_entry.grid(row=n_row, column=2)


roc_curr1_label = tk.Label(window, text="ROC current 1 (A) : ")
roc_curr1_label.grid(row=n_row, column=3)

roc_curr1_entry = tk.Entry(window,width=5)
roc_curr1_entry.grid(row=n_row, column=4)


roc_volt2_label = tk.Label(window, text="ROC Voltage 2 (V) : ")
roc_volt2_label.grid(row=n_row, column=5)

roc_volt2_entry = tk.Entry(window,width=5, textvariable=tk.StringVar(value="5.0"))
roc_volt2_entry.grid(row=n_row, column=6)


roc_curr2_label = tk.Label(window, text="ROC current 2 (A) : ")
roc_curr2_label.grid(row=n_row, column=7)

roc_curr2_entry = tk.Entry(window,width=5)
roc_curr2_entry.grid(row=n_row, column=8)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= 
n_row += 1
separate_bar1 = tk.Label(window, text="------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar1.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= 

n_row += 1

ladder_id_label = tk.Label(window, text="Ladder ID : ")
ladder_id_label.grid(row=n_row, column=1)

ladder_id_entry = tk.Entry(window,width=10,textvariable=tk.StringVar(value=""))
ladder_id_entry.grid(row=n_row, column=2)


MCC_id_label = tk.Label(window, text="MCC ID : ")
MCC_id_label.grid(row=n_row, column=3)

MCC_id_entry = tk.Entry(window,width=5)
MCC_id_entry.grid(row=n_row, column=4)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= x
n_row += 1

chip_vol_label = tk.Label(window, text="Chip Voltage (V) : ")
chip_vol_label.grid(row=n_row, column=1)

chip_vol_entry = tk.Entry(window,width=5,textvariable=tk.StringVar(value="5.8"))
chip_vol_entry.grid(row=n_row, column=2)


chip_current_label = tk.Label(window, text="Chip current (A) : ")
chip_current_label.grid(row=n_row, column=3)

chip_current_entry = tk.Entry(window,width=8, textvariable=tk.StringVar(value="Current non-accessible"))
chip_current_entry.grid(row=n_row, column=4)


bias_vol_label = tk.Label(window, text="Bias Voltage (V) : ")
bias_vol_label.grid(row=n_row, column=5)

bias_vol_entry = tk.Entry(window,width=5, textvariable=tk.StringVar(value="100"))
bias_vol_entry.grid(row=n_row, column=6)


bias_current_1_label = tk.Label(window, text="Bias current (nA) : ")
bias_current_1_label.grid(row=n_row, column=7)

bias_current_1_entry = tk.Entry(window,width=5)
bias_current_1_entry.grid(row=n_row, column=8)

bias_current_2_label = tk.Label(window, text=", ")
bias_current_2_label.grid(row=n_row, column=9)

bias_current_2_entry = tk.Entry(window,width=5)
bias_current_2_entry.grid(row=n_row, column=10)
# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========
n_row += 1

port_id_label = tk.Label(window, text="Port : ")
port_id_label.grid(row=n_row, column=1)

port_id_entry = tk.Entry(window,width=8,textvariable=tk.StringVar(value=""))
port_id_entry.grid(row=n_row, column=2)

file_name_label = tk.Label(window, text="File name : ")
file_name_label.grid(row=n_row, column=3)

file_name_start = datetime.datetime.today().strftime("%m%d-")
file_name_entry = tk.Entry(window,width=10,textvariable=tk.StringVar(value=file_name_start))
file_name_entry.grid(row=n_row, column=4)

module_id_label = tk.Label(window, text="Module ID : ")
module_id_label.grid(row=n_row, column=5)

module_id_entry = tk.Entry(window,width=5)
module_id_entry.grid(row=n_row, column=6)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
separate_bar2 = tk.Label(window, text="------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========
n_row += 1

result_label = tk.Label(window, text="Result : ")
result_label.grid(row=n_row, column=1)

result_entry = tk.Entry(window,width=90,textvariable=tk.StringVar(value=""))
result_entry.grid(row=n_row, column=2, columnspan = 9)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= x
n_row += 1

note_label = tk.Label(window, text="Note : ")
note_label.grid(row=n_row, column=1)

note_entry = tk.Entry(window,width=90,textvariable=tk.StringVar(value=""))
note_entry.grid(row=n_row, column=2, columnspan = 9)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= x



n_row += 1

# Create the button
def print_all():

    print("--------------------------------------------------------------------------------")
    print("Print time :", datetime.datetime.now() )
    print('ROC ID :',roc_id_entry.get(), "ROC power :",roc_volt1_entry.get(),"[V], ",roc_curr1_entry.get(),"[A] / ",roc_volt2_entry.get(),"[V],",roc_curr2_entry.get(),"[A]")
    print("Chip power :",chip_vol_entry.get(),"[V], ",chip_current_entry.get(),"[A]")
    print("Bias :",bias_vol_entry.get(),"[V],",bias_current_1_entry.get(),"[nA] &",bias_current_2_entry.get(),"[nA]")
    print('Ladder ID :',ladder_id_entry.get(), ' MCC ID :',MCC_id_entry.get())
    print('Port ID :',port_id_entry.get(),' File_name :',file_name_entry.get(),' Module ID :',module_id_entry.get())
    print('Result :',result_entry.get())
    print('Note :',note_entry.get())

def simple_clear():
    roc_curr1_entry.delete(0, 'end')
    roc_curr2_entry.delete(0, 'end')
    MCC_id_entry.delete(0,'end')
    bias_current_1_entry.delete(0,'end')
    bias_current_2_entry.delete(0,'end')
    port_id_entry.delete(0,'end')
    file_name_entry.delete(5,'end')
    module_id_entry.delete(0,'end')
    result_entry.delete(0,'end')
    note_entry.delete(0,'end')

def all_clear():
    roc_curr1_entry.delete(0, 'end')
    roc_curr2_entry.delete(0, 'end')
    MCC_id_entry.delete(0,'end')
    bias_current_1_entry.delete(0,'end')
    bias_current_2_entry.delete(0,'end')
    port_id_entry.delete(0,'end')
    file_name_entry.delete(0,'end')
    module_id_entry.delete(0,'end')
    result_entry.delete(0,'end')
    note_entry.delete(0,'end')

    roc_id_entry.delete(0,'end')
    roc_volt1_entry.delete(0,'end')
    roc_volt2_entry.delete(0,'end')
    bias_vol_entry.delete(0,'end')
    chip_vol_entry.delete(0,'end')
    chip_current_entry.delete(0,'end')
    

    # / chip voltage : 5.0 V (from rack) / Bias voltage : 100 V (from rack) 
    # With the FEM DATA light condition : 0 ,1 and 6 didn't light up. (only B3 is latched)
    
    # 
    # 
    # 
    #   
    # 

    # roc_id = roc_id_entry.get()
    # roc_volt1 = roc_volt1_entry.get()
    # roc_curr1 = roc_curr1_entry.get()
    # roc_volt2 = roc_volt2_entry.get()
    # roc_curr2 = roc_curr2_entry.get()
    
    # print("ROC ID:", roc_id)
    # print("ROC Voltage 1:", roc_volt1)
    # print("ROC current 1:", roc_curr1)
    # print("ROC Voltage 2:", roc_volt2)
    # print("ROC current 2:", roc_curr2)

button = tk.Button(window, text="Print", command=print_all)
button.grid(row=n_row, column=8 )

clear_button = tk.Button(window, text="Calib_clear", fg='red', command=simple_clear )
clear_button.grid(row=n_row, column=1 )

clear_button = tk.Button(window, text="ALL_CLEAR", fg='blue', command=all_clear )
clear_button.grid(row=n_row, column=4 )

# Run the GUI
window.mainloop()

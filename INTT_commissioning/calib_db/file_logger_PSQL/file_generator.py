
# note : note : this code is for generating the txt map file only for the INTT barrel testing (in commissioning)

import tkinter as tk
import glob, os
from tkinter import filedialog
import datetime 

check_print_tag = 0

def print_selection():
    print("Selected number:", var_ladder.get())

def simple_print(*args,var_barrel):
    print("aaa",var_barrel)


def update_dropdown(*args,var_barrel,var_ladder,dropdown_ladder):
    print("button feedback : ",var_barrel)
    if (var_barrel == "B0L0" or var_barrel == "B0L1" or var_barrel == "NaN ") :
        opt_ladder = ["00", "01", "02","03","04","05","06","07","08","09","10","11"]
    else:
        opt_ladder = ["00", "01", "02","03","04","05","06","07","08","09","10","11","12","13","14","15"]
    var_ladder.set(opt_ladder[0])
    
    # note : so it deletes the items in the list, and use the for loop to add them back, one by one.
    dropdown_ladder['menu'].delete(0, 'end')
    for option in opt_ladder:
        dropdown_ladder['menu'].add_command(label=option, command=tk._setit(var_ladder, option))


def print_all():

    ROC1_check_tag = 0
    ROC2_check_tag = 0

    print("====================================================================================")
    print("File :", file_name_entry.get())
    print("Side :", var_NS.get())
    
    for i in range (len(ladder_text)) :
        if (var_barrel[i].get() != "NaN "):
            ROC1_check_tag = 1
            break

    for i in range (len(ladder_text)) :
        if (var_barrel_2[i].get() != "NaN "):
            ROC2_check_tag = 1
            break

    if (ROC1_check_tag == 1):
        for i in range (len(ladder_text)) :
            if (var_barrel[i].get() != "NaN ") :
                print("ROC :",ROC1_entry.get(),"ID :",i+1,"Ladder :", var_barrel[i].get()+var_ladder[i].get(),"moduleID :",var_module[i].get(),"port :",var_port[i].get())

    
    if (ROC2_check_tag == 1):
        print(" ")
        for i in range (len(ladder_text)) :
            if (var_barrel_2[i].get() != "NaN ") :
                print("ROC :",ROC2_entry.get(),"ID :",i+1,"Ladder :", var_barrel_2[i].get()+var_ladder_2[i].get(),"moduleID :",var_module_2[i].get(),"port :",var_port_2[i].get())

    if (note1_entry.get() != ""):
        print(" ")
        print(note1_entry.get())
    if (note2_entry.get() != ""):
        print(" ")
        print(note2_entry.get())
    if (note3_entry.get() != ""):
        print(" ")
        print(note3_entry.get())
    # for i in var_barrel :
    #     print(i.get())
    
    # for il in var_ladder :
    #     print(il.get())

def print_map():
    global check_print_tag
    check_print_tag = 1

    ROC1_check_tag = 0
    ROC2_check_tag = 0

    print("====================================================================================")
    
    if( file_name_entry.get()[ len(file_name_entry.get())-4 : ] != '.txt' ) :
        print('check the output file name')
        return

    # todo : maybe not need it !
    if( os.path.isfile(file_name_entry.get()) ):
        print('File already exists')
        return

    for i in range (len(ladder_text)) :
        if (var_barrel[i].get() != "NaN "):
            ROC1_check_tag = 1
            break

    for i in range (len(ladder_text)) :
        if (var_barrel_2[i].get() != "NaN "):
            ROC2_check_tag = 1
            break

    if (ROC1_check_tag + ROC2_check_tag == 0):
        print('Nothing filled !!!')
        check_print_tag = 0
        return

    if(var_NS.get() == opt_NS[0]) : # note : NaN
        print('Wrong NS side !!!')
        check_print_tag = 0
        return

    if (ROC1_entry.get() + ROC2_entry.get() == ''):
        print("both ROC1 and ROC2 no entry ? ")
        return

    if ( ROC1_check_tag == 1 and ROC1_entry.get() == '' ):
        print('Felix ch. 0 should not be empty !')
        return

    if ( ROC2_check_tag == 1 and ROC2_entry.get() == '' ):
        print('Felix ch. 1 should not be empty !')
        return
    
    print("File :", file_name_entry.get())
    print('#felix_ch       ROC_port        Ladder')

    if (ROC1_check_tag == 1):
        for i in range (len(ladder_text)) :
            if (var_barrel[i].get() != "NaN ") :
                if (var_sharp[i].get() == False) :
                    print('%s   %s  %s%s%s'%( var_module[i].get(), var_port[i].get(), var_barrel[i].get(),var_ladder[i].get(),var_NS.get()))
                elif (var_sharp[i].get() == True):
                    print('# %s   %s  %s%s%s'%( var_module[i].get(), var_port[i].get(), var_barrel[i].get(),var_ladder[i].get(),var_NS.get()))
    
    if (ROC2_check_tag == 1):
        print(" ")
        for i in range (len(ladder_text)) :
            if (var_barrel_2[i].get() != "NaN ") :
                if (var_sharp_2[i].get() == False) :
                    print('%s   %s  %s%s%s'%( var_module_2[i].get(), var_port_2[i].get(), var_barrel_2[i].get(),var_ladder_2[i].get(),var_NS.get()))
                elif (var_sharp_2[i].get() == True) :
                    print('# %s   %s  %s%s%s'%( var_module_2[i].get(), var_port_2[i].get(), var_barrel_2[i].get(),var_ladder_2[i].get(),var_NS.get()))

    if (ROC1_entry.get() !=""):
        print('# Felix ch. 0 ',ROC1_entry.get())
    if (ROC2_entry.get() !=""):
        print('# Felix ch. 1',ROC2_entry.get())
    if (note1_entry.get() != ""):
        print('#',note1_entry.get())
    if (note2_entry.get() != ""):
        print('#',note2_entry.get())
    if (note3_entry.get() != ""):
        print('#',note3_entry.get())

def output_file():
    global check_print_tag
    if (check_print_tag == 0) :
        print('Please check the print first')
        return
    
    check_print_tag = 0

    ROC1_check_tag = 0
    ROC2_check_tag = 0  

    text_content = []  
    
    for i in range (len(ladder_text)) :
        if (var_barrel[i].get() != "NaN "):
            ROC1_check_tag = 1
            break

    for i in range (len(ladder_text)) :
        if (var_barrel_2[i].get() != "NaN "):
            ROC2_check_tag = 1
            break

    text_content.append('#felix_ch       ROC_port        Ladder')

    if (ROC1_check_tag == 1):
        for i in range (len(ladder_text)) :
            if (var_barrel[i].get() != "NaN ") :
                if (var_sharp[i].get() == False):
                    text_content.append('%s   %s  %s%s%s'%( var_module[i].get(), var_port[i].get(), var_barrel[i].get(),var_ladder[i].get(),var_NS.get()))
                elif (var_sharp[i].get() == True):
                    text_content.append('# %s   %s  %s%s%s'%( var_module[i].get(), var_port[i].get(), var_barrel[i].get(),var_ladder[i].get(),var_NS.get()))
    
    text_content.append('')

    if (ROC2_check_tag == 1):
        for i in range (len(ladder_text)) :
            if (var_barrel_2[i].get() != "NaN ") :
                if (var_sharp_2[i].get() == False):
                    text_content.append('%s   %s  %s%s%s'%( var_module_2[i].get(), var_port_2[i].get(), var_barrel_2[i].get(),var_ladder_2[i].get(),var_NS.get()))
                elif (var_sharp_2[i].get() == True):
                    text_content.append('# %s   %s  %s%s%s'%( var_module_2[i].get(), var_port_2[i].get(), var_barrel_2[i].get(),var_ladder_2[i].get(),var_NS.get()))

    text_content.append('')
    text_content.append("# File : %s"%file_name_entry.get())
    text_content.append('# Time : %s'%datetime.datetime.now())
    if (ROC1_entry.get() != ""):
        text_content.append('# ROC1 : %s'%ROC1_entry.get())
    if (ROC2_entry.get() != ""):
        text_content.append('# ROC2 : %s'%ROC2_entry.get())
    if (note1_entry.get() != ""):
        text_content.append('# log1 : %s'%note1_entry.get())
    if (note2_entry.get() != ""):
        text_content.append('# log2 : %s'%note2_entry.get())
    if (note3_entry.get() != ""):
        text_content.append('# log3 : %s'%note3_entry.get())
    if (map_name_entry.get() != ""):
        text_content.append('# map source : %s'%map_name_entry.get())

    with open(file_name_entry.get(), 'w') as f:
        for line in text_content:
            f.write(line)
            f.write('\n')
    
    f.close()

    print('file generated ')

    file_name_entry.delete(0, 'end')
    file_name_entry.insert(0, file_default_directory)

def reset_ladder_part():
    # note : reset NS
    var_NS.set(opt_NS[0])
    dropdown_NS['menu'].delete(0, 'end')
    for option_NS in opt_NS :
        dropdown_NS['menu'].add_command(label=option_NS, command=tk._setit(var_NS, option_NS))

    # note : reset ladder related thing
    for i in range (len(ladder_text)) :
        # note : reset barrel
        var_barrel[i].set(opt_barrrel[0])
        dropdown_barrel_list[i]['menu'].delete(0, 'end')

        var_barrel_2[i].set(opt_barrrel[0])
        dropdown_barrel_list_2[i]['menu'].delete(0, 'end')

        for option_barrel in opt_barrrel:
            dropdown_barrel_list  [i]['menu'].add_command(label=option_barrel, command=tk._setit(var_barrel[i], option_barrel,lambda value, i=i: update_dropdown(var_barrel = value, var_ladder = var_ladder[i],dropdown_ladder=dropdown_ladder_list[i])))
            dropdown_barrel_list_2[i]['menu'].add_command(label=option_barrel, command=tk._setit(var_barrel_2[i], option_barrel, lambda value, i=i: update_dropdown(var_barrel = value, var_ladder = var_ladder_2[i],dropdown_ladder=dropdown_ladder_list_2[i])))
            # dropdown_barrel_2['menu'].add_command(command=lambda value_2, i=i: update_dropdown(var_barrel = value_2, var_ladder = var_ladder_2[i],dropdown_ladder=dropdown_ladder_list_2[i]))
        
        # note : reset ladder 
        var_ladder[i].set(opt_ladder[0])
        dropdown_ladder_list[i]['menu'].delete(0, 'end')

        var_ladder_2[i].set(opt_ladder[0])
        dropdown_ladder_list_2[i]['menu'].delete(0, 'end')

        for option_ladder in opt_ladder:
            dropdown_ladder_list  [i]['menu'].add_command(label=option_ladder, command=tk._setit(var_ladder[i], option_ladder))
            dropdown_ladder_list_2[i]['menu'].add_command(label=option_ladder, command=tk._setit(var_ladder_2[i], option_ladder))
            
        # note : reset module 
        var_module[i].set(opt_module[0])
        dropdown_module_list[i]['menu'].delete(0, 'end')

        var_module_2[i].set(opt_module_2[0])
        dropdown_module_list_2[i]['menu'].delete(0, 'end')

        for option_module in opt_module:
            dropdown_module_list[i]['menu'].add_command(label=option_module, command=tk._setit(var_module[i], option_module))
        
        for option_module in opt_module_2:
            dropdown_module_list_2[i]['menu'].add_command(label=option_module, command=tk._setit(var_module_2[i], option_module))


        # note : reset port 
        var_port[i].set(opt_port[0])
        dropdown_port_list[i]['menu'].delete(0, 'end')

        var_port_2[i].set(opt_port[0])
        dropdown_port_list_2[i]['menu'].delete(0, 'end')

        for option_port in opt_port:
            dropdown_port_list[i]['menu'].add_command(label=option_port, command=tk._setit(var_port[i], option_port))
            dropdown_port_list_2[i]['menu'].add_command(label=option_port, command=tk._setit(var_port_2[i], option_port))


        # note : reset sharp '#'
        var_sharp[i].set(value = False)
        var_sharp_2[i].set(value = False)

def reset_all():
    global check_print_tag
    check_print_tag = 0

    file_name_entry.delete(0, 'end')
    file_name_entry.insert(0, file_default_directory)
    ROC1_entry.delete(0, 'end')
    ROC2_entry.delete(0, 'end')
    note1_entry.delete(0,'end')
    note2_entry.delete(0,'end')
    note3_entry.delete(0,'end')
    map_name_entry.delete(0,'end')

    reset_ladder_part()

def reset_file_name():
    file_name_entry.delete(0, 'end')
    file_name_entry.insert(0, file_default_directory)

def find_latest_file():
    # file_directory = "/home/inttdev/work/cwshih/calib_db/fake_folder/"
    list_of_files = glob.glob(file_default_directory+"/*.root")
    latest_file = max(list_of_files, key=os.path.getctime)
    # latest_file = latest_file.replace(file_directory,'')

    return latest_file

def update_file_latest ():
    file_name_entry.delete(0,'end')
    file_name_entry.insert('end', find_latest_file())

def clear_note_1():
    note1_entry.delete(0,'end')

def clear_note_2():
    note2_entry.delete(0,'end')

def clear_note_3():
    note3_entry.delete(0,'end')

# def fiber_swap ():

#     up_ROC_name = ROC1_entry.get()
#     down_ROC_name = ROC2.entry.get()

#     for i in range(len(ladder_text)):
        
        


def browse_file():
    file_path = filedialog.askopenfilename(initialdir=file_default_directory)
    file_name_entry.delete(0,'end')
    file_name_entry.insert('end', file_path)

def read_map_info_sharp(path_in):
    mapping = []
    sharp_code = []
    with open( path_in ) as file :
        for line in file :

            # for comment out
            if line[0] == "#" :
                if (line.find('B0L0') != -1 or line.find('B0L1') != -1 or line.find('B1L0') != -1 or line.find('B1L1') != -1 ):
                    if ( len(line.replace('#','').split()) == 3 ):
                        mapping.append( line.replace('#','').split() )
                        sharp_code.append(1)
                        continue
                    else :
                        continue

                else :
                    continue

            # for empty line, does it work?
            if line[0] == "" :
                continue

            # if the number of elements is smaller than 2 (at least Felix ch and ROC port should be), skip it
            if len( line ) < 2 :
                continue

            # print( line, line.split() )
            mapping.append( line.split() )
            sharp_code.append(0)

    print("----------------------------- The selected map ----------------------------------")
    print("!!!! You are using the # included map, it may not work, please check !!!!")
    for i in range (len(mapping)) :
        if (sharp_code[i] == 1) : 
            print('#',mapping[i],'< -- with #')
        else :
            print(mapping[i])

    return mapping, sharp_code


def read_map_info(path_in):
    mapping = []
    with open( path_in ) as file :
        for line in file :

            # for comment out
            if line[0] == "#" :
                continue

            # for empty line, does it work?
            if line[0] == "" :
                continue

            # if the number of elements is smaller than 2 (at least Felix ch and ROC port should be), skip it
            if len( line ) < 2 :
                continue

            # print( line, line.split() )
            mapping.append( line.split() )

    print("----------------------------- The selected map ----------------------------------")
    for data in  mapping :
        print(data)

    return mapping


def browse_map_file():
    map_path = filedialog.askopenfilename(initialdir=file_default_directory)
    map_name_entry.delete(0,'end')
    map_name_entry.insert('end', map_path)

    # note : print the map content 
    # read_map_info(map_name_entry.get())

# note : the function to find the file for map
# note : ultra stupid 
def find_latest_file_map():
    # file_directory = "/home/inttdev/work/cwshih/calib_db/fake_folder/"
    list_of_files = glob.glob(file_default_directory+"/*.txt")
    latest_file = max(list_of_files, key=os.path.getctime)
    # latest_file = latest_file.replace(file_directory,'')

    return latest_file

def update_file_latest_map ():
    map_name_entry.delete(0,'end')
    map_name_entry.insert('end', find_latest_file_map())
    
    # note : print the map content 
    # read_map_info(map_name_entry.get())

def load_map_print ():
    read_map_info(map_name_entry.get())

def load_map_print_sharp ():
    read_map_info_sharp(map_name_entry.get())


def apply_map ():
    # note : reset the ladder part first
    reset_ladder_part()

    inputmap = read_map_info(map_name_entry.get())
    
    ROC1_index = 0
    ROC2_index = 0

    # note : set the N or S based on the first entry
    if (inputmap[0][2][6] == "N") : 
        var_NS.set(opt_NS[1])
    elif (inputmap[0][2][6] == "S") : 
        var_NS.set(opt_NS[2])
    else : # note : unknown character
        var_NS.set(opt_NS[0])

    # note : SL 0 : module ID
    # note : SL 1 : port ID
    # note : SL 2 : ladder and NS
    for SL in inputmap : 
        if ( -1 < int(SL[0])  < 7 ) : # note : ROC1
            var_module[ROC1_index].set(SL[0]) # note : 0 ~ 6
            var_port[ROC1_index].set(SL[1])   # note : A1 ~ D3, it's not in the list, let's try
            var_barrel[ROC1_index].set(SL[2][0:4]) # note : B0L0, for example
            var_ladder[ROC1_index].set(SL[2][4:6]) # note : 06, for example 
            ROC1_index += 1
        elif ( 6 < int(SL[0])  < 14 ) : # note : ROC2
            var_module_2[ROC2_index].set(SL[0]) # note : 7 ~ 13
            var_port_2[ROC2_index].set(SL[1])   # note : A1 ~ D3, it's not in the list, let's try
            var_barrel_2[ROC2_index].set(SL[2][0:4]) # note : B0L0, for example
            var_ladder_2[ROC2_index].set(SL[2][4:6]) # note : 06, for example 
            ROC2_index += 1

def empty_map_slot():
    map_name_entry.delete(0,'end')

def apply_map_sharp ():
    # note : reset the ladder part first
    reset_ladder_part()
    
    inputmap, input_sharp_code = read_map_info_sharp(map_name_entry.get())
    
    ROC1_index = 0
    ROC2_index = 0

    ROC1_index_sharp = 0
    ROC2_index_sharp = 0

    # note : set the N or S based on the first entry
    if (inputmap[0][2][6] == "N") : 
        var_NS.set(opt_NS[1])
    elif (inputmap[0][2][6] == "S") : 
        var_NS.set(opt_NS[2])
    else : # note : unknown character
        var_NS.set(opt_NS[0])

    # note : SL 0 : module ID
    # note : SL 1 : port ID
    # note : SL 2 : ladder and NS
    for SL in inputmap : 
        if ( -1 < int(SL[0])  < 7 ) : # note : ROC1
            var_module[ROC1_index].set(SL[0]) # note : 0 ~ 6
            var_port[ROC1_index].set(SL[1])   # note : A1 ~ D3, it's not in the list, let's try
            var_barrel[ROC1_index].set(SL[2][0:4]) # note : B0L0, for example
            var_ladder[ROC1_index].set(SL[2][4:6]) # note : 06, for example 
            ROC1_index += 1
        elif ( 6 < int(SL[0])  < 14 ) : # note : ROC2
            var_module_2[ROC2_index].set(SL[0]) # note : 7 ~ 13
            var_port_2[ROC2_index].set(SL[1])   # note : A1 ~ D3, it's not in the list, let's try
            var_barrel_2[ROC2_index].set(SL[2][0:4]) # note : B0L0, for example
            var_ladder_2[ROC2_index].set(SL[2][4:6]) # note : 06, for example 
            ROC2_index += 1

    # note : add the sharp
    for  i in range (len(inputmap)) :
        if ( -1 < int(inputmap[i][0])  < 7 ) : # note : ROC1
            if ( input_sharp_code[i] == 1 ) :
                var_sharp[ROC1_index_sharp].set(value=True)
            ROC1_index_sharp += 1
        elif ( 6 < int(inputmap[i][0])  < 14 ) : # note : ROC2
            if ( input_sharp_code[i] == 1 ) :
                var_sharp_2[ROC2_index_sharp].set(value=True)
            ROC2_index_sharp += 1
       
def close_win ():
    root.quit()

# note : create tkinter window
root = tk.Tk()
root.title("INTT Commissioning, map generator")
root.geometry("1000x820")





ladder_text = ["Ladder1", "Ladder2", "Ladder3", "Ladder4", "Ladder5", "Ladder6", "Ladder7"]
roc_text = ["Felix ch. 0", "Felix ch. 1"]
file_name_text = "Set map name : "
module_text = "module id "
NS_text = "North or South : "
map_name_text = "Load map : "
port_text = "port"
status_text = "status"
sharp_text  = "add #"

opt_barrrel = ["NaN ","B0L0","B0L1","B1L0","B1L1"]
opt_ladder  = ["00", "01", "02","03","04","05","06","07","08","09","10","11"]
opt_module  = [0,1,2,3,4,5,6]
opt_module_2  = [7,8,9,10,11,12,13]

opt_NS      = ["NaN","N","S"]
opt_port    = ["A1","A2","A3","B1","B2","B3","C1","C2","C3","D1","D2","D3"]

file_default_directory = "/home/inttdev/map/"
# file_default_directory = "/home/inttdev/work/cwshih/calib_db/fake_folder/"

dict_status = {
    'NC' : 'no-comment',
    'Good' : 'good',
    'HE' : 'half-entry',
    'DE' : 'double-entry',
    'Noisy' : 'noisy',
    'Weird' : 'Weird',
    'HRN' : 'half-row-nothing',
    'HCN' : 'half-channel-nothing',
    'Nothing' : 'nothing'}



opt_status = list(dict_status.keys())

var_barrel = [tk.StringVar(root) for i in range(len(ladder_text))]
var_ladder = [tk.StringVar(root) for i in range(len(ladder_text))]
var_module = [tk.StringVar(root) for i in range(len(ladder_text))]
var_port   = [tk.StringVar(root) for i in range(len(ladder_text))]
var_status = [tk.StringVar(root) for i in range(len(ladder_text))]
var_sharp  = [tk.IntVar(root) for i in range(len(ladder_text))]
dropdown_barrel_list = []
dropdown_module_list = []
dropdown_ladder_list = []
dropdown_port_list   = []
dropdown_status_list = []
CB_sharp_list        = []


var_barrel_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_ladder_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_module_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_port_2   = [tk.StringVar(root) for i in range(len(ladder_text))]
var_status_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_sharp_2  = [tk.IntVar(root) for i in range(len(ladder_text))]
dropdown_barrel_list_2 = []
dropdown_module_list_2 = []
dropdown_ladder_list_2 = []
dropdown_port_list_2   = []
dropdown_status_list_2 = []
CB_sharp_list_2        = []




n_row = 1
# note : for file name 
file_name_label = tk.Label(root, text=file_name_text)
file_name_label.grid(row=n_row, column=1)

file_name_entry = tk.Entry(root,width=85,textvariable=tk.StringVar(value=file_default_directory))
file_name_entry.grid(row=n_row, column=2, columnspan = 7)

button_rename = tk.Button(root, text="Rename", width = 5,fg='red', command=reset_file_name)
button_rename.grid(row=n_row, column=9 )

# n_row += 1
# button_browse = tk.Button(root, text="Find", fg='black', command=browse_file)
# button_browse.grid(row=n_row, column=1 )

# button_latest = tk.Button(root, text="Latest", fg='black', command=update_file_latest)
# button_latest.grid(row=n_row, column=2 )


# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
separate_bar0 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar0.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========


n_row += 1
# note : for the map in
button_browse_map = tk.Button(root, text="Find", fg='black', command=browse_map_file)
button_browse_map.grid(row=n_row, column=1 )

button_latest_map = tk.Button(root, text="Latest", fg='black', command=update_file_latest_map)
button_latest_map.grid(row=n_row, column=2 )

button_print_map = tk.Button(root, text="Print", fg='blue', command=load_map_print)
button_print_map.grid(row=n_row, column=3 )

button_load_map = tk.Button(root, text="Load", fg='blue', command=apply_map)
button_load_map.grid(row=n_row, column=4 )

# note : for the sharp
button_print_map_sharp = tk.Button(root, text="Print#", fg='red', command=load_map_print_sharp)
button_print_map_sharp.grid(row=n_row, column=5 )
 
button_load_map_sharp = tk.Button(root, text="Load#", fg='red', command=apply_map_sharp)
button_load_map_sharp.grid(row=n_row, column=6 )


n_row += 1
map_name_label = tk.Label(root, text=map_name_text)
map_name_label.grid(row=n_row, column=1)

map_name_entry = tk.Entry(root,width=85,textvariable=tk.StringVar(value=""))
map_name_entry.grid(row=n_row, column=2, columnspan = 7)
# note : empty the map slot
button_map_empty = tk.Button(root, width = 5, text="Empty", fg='red', command=empty_map_slot)
button_map_empty.grid(row=n_row, column=9 )

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
separate_bar1 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar1.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# note : for the N or S
n_row += 1
NS_label = tk.Label(root, text=NS_text,fg='blue')
NS_label.grid(row=n_row, column=1)

var_NS = tk.StringVar(root)
var_NS.set(opt_NS[0])

dropdown_NS = tk.OptionMenu(root, var_NS, *opt_NS)
dropdown_NS.grid(row=n_row,column=2)


# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
separate_bar1 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar1.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# note : create 7 ladders selections
# note : ROC1
n_row += 1
n_column = 1

ROC1_label = tk.Label(root, text=roc_text[0])
ROC1_label.grid(row=n_row, column=n_column,padx=5)

ROC1_entry = tk.Entry(root,width=10,textvariable=tk.StringVar(value=""))
ROC1_entry.grid(row=n_row+1, column=n_column, rowspan = 2, padx=5)


# note : for the add #, ROC1
for i in range( len(ladder_text) ):
    var_sharp[i].set(value = False)
    CB_sharp = tk.Checkbutton(root, text=sharp_text, variable=var_sharp[i],onvalue=True, offvalue=False, activebackground='pink')
    CB_sharp.grid(row=n_row,column=n_column+i+2)
    CB_sharp_list.append(CB_sharp)


n_row += 1
# note : for the ladder, ROC1 
for i in range( len(ladder_text) ):

    var_ladder[i].set(opt_ladder[0])

    dropdown_ladder = tk.OptionMenu(root, var_ladder[i], *opt_ladder)
    dropdown_ladder.grid(row=n_row+2,column=n_column+i+2)
    dropdown_ladder_list.append(dropdown_ladder)

# note : for the barrel, ROC1
for i in range( len(ladder_text) ):
    
    label_ID = tk.Label(root, fg = '#00A2FF',text=ladder_text[i])
    label_ID.grid(row=n_row,column=n_column+i+2)

    var_barrel[i].set(opt_barrrel[0])

    dropdown_barrel = tk.OptionMenu(root, var_barrel[i], *opt_barrrel,command=lambda value, i=i: update_dropdown(var_barrel = value, var_ladder = var_ladder[i],dropdown_ladder=dropdown_ladder_list[i]))
    dropdown_barrel.grid(row=n_row+1,column=n_column+i+2)
    dropdown_barrel_list.append(dropdown_barrel)

# note : for the module, ROC1
n_row += 3
for i in range( len(ladder_text) ):

    label_module = tk.Label(root, text=module_text)
    label_module.grid(row=n_row,column=n_column+i+2)

    var_module[i].set(opt_module[0])

    dropdown_module = tk.OptionMenu(root, var_module[i], *opt_module)
    dropdown_module.grid(row=n_row+1,column=n_column+i+2)
    dropdown_module_list.append(dropdown_module)

# note : for the port, ROC1
n_row += 2
for i in range( len(ladder_text) ):

    label_port = tk.Label(root, text=port_text)
    label_port.grid(row=n_row,column=n_column+i+2)

    var_port[i].set(opt_port[0])

    dropdown_port = tk.OptionMenu(root, var_port[i], *opt_port)
    dropdown_port.grid(row=n_row+1,column=n_column+i+2)
    dropdown_port_list.append(dropdown_port)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 2
separate_bar2 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========


# note : ==================================================================================================================================

# note : for the ROC2
n_row += 2

ROC2_label = tk.Label(root, text=roc_text[1])
ROC2_label.grid(row=n_row, column=n_column,padx=5)
ROC2_entry = tk.Entry(root,width=10,textvariable=tk.StringVar(value=""))
ROC2_entry.grid(row=n_row+1, column=n_column, rowspan = 2, padx=5)


# note : for the add #
for i in range( len(ladder_text) ):
    var_sharp_2[i].set(value = False)
    CB_sharp_2 = tk.Checkbutton(root, text=sharp_text, variable=var_sharp_2[i],onvalue=True, offvalue=False, activebackground='pink')
    CB_sharp_2.grid(row=n_row,column=n_column+i+2)
    CB_sharp_list_2.append(CB_sharp_2)



n_row += 1
# note : for the ladder, ROC2
for i in range( len(ladder_text) ):

    var_ladder_2[i].set(opt_ladder[0])
    dropdown_ladder_2 = tk.OptionMenu(root, var_ladder_2[i], *opt_ladder)
    dropdown_ladder_2.grid(row=n_row+2,column=n_column+i+2)
    dropdown_ladder_list_2.append(dropdown_ladder_2)

# note : for the barrel, ROC2
for i in range( len(ladder_text) ):

    label_ID_2 = tk.Label(root, fg = '#00A2FF', text=ladder_text[i])
    label_ID_2.grid(row=n_row,column=n_column+i+2)

    var_barrel_2[i].set(opt_barrrel[0])
    dropdown_barrel_2 = tk.OptionMenu(root, var_barrel_2[i], *opt_barrrel,command=lambda value_2, i=i: update_dropdown(var_barrel = value_2, var_ladder = var_ladder_2[i],dropdown_ladder=dropdown_ladder_list_2[i]))
    dropdown_barrel_2.grid(row=n_row+1,column=n_column+i+2)
    dropdown_barrel_list_2.append(dropdown_barrel_2)

# note : for the module, ROC2
n_row += 3
for i in range( len(ladder_text) ):

    label_module_2 = tk.Label(root, text=module_text)
    label_module_2.grid(row=n_row,column=n_column+i+2)

    var_module_2[i].set(opt_module_2[0])
    dropdown_module_2 = tk.OptionMenu(root, var_module_2[i], *opt_module_2)
    dropdown_module_2.grid(row=n_row+1,column=n_column+i+2)
    dropdown_module_list_2.append(dropdown_module_2)

# note : for the port, ROC2
n_row += 2
for i in range( len(ladder_text) ):

    label_port_2 = tk.Label(root, text=port_text)
    label_port_2.grid(row=n_row,column=n_column+i+2)

    var_port_2[i].set(opt_port[0])

    dropdown_port_2 = tk.OptionMenu(root, var_port_2[i], *opt_port)
    dropdown_port_2.grid(row=n_row+1,column=n_column+i+2)
    dropdown_port_list_2.append(dropdown_port_2)



# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 2
separate_bar3 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar3.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# note : for the note 
n_row += 3
note1_button = tk.Button(root, text="Note 1 : ",command=clear_note_1)
note1_button.grid(row=n_row, column=1)

note1_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note1_entry.grid(row=n_row, column=2, columnspan = 8)

n_row += 1
note2_button = tk.Button(root, text="Note 2 : ",command=clear_note_2)
note2_button.grid(row=n_row, column=1)

note2_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note2_entry.grid(row=n_row, column=2, columnspan = 8)

n_row += 1
note3_button = tk.Button(root, text="Note 3 : ",command=clear_note_3)
note3_button.grid(row=n_row, column=1)

note3_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note3_entry.grid(row=n_row, column=2, columnspan = 8)

# note : for the print
n_row += 1
button_print = tk.Button(root, text="Print", command=print_map)
button_print.grid(row=n_row, column=1 )

# note : for the output file
button_out = tk.Button(root, width = 15,text="Generate file", command=output_file)
button_out.grid(row=n_row, column=5, columnspan = 2 )

# note : for rest
button_reset = tk.Button(root, text="Reset", fg='red', command=reset_all)
button_reset.grid(row=n_row, column=8 )

# note : for close the window
button_close = tk.Button(root, text="Close", fg='red', command=close_win)
button_close.grid(row=n_row, column=9 )


root.mainloop()

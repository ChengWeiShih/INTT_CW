
# note : this v2 has the PSQL function
# note : the pkl is used

import tkinter as tk
import glob, os
from tkinter import filedialog
import PSQL_for_cabling_cali as PCC

hostname = '/var/run/postgresql'
database = 'cw_calib_test'
username = 'inttdev'
pwd      = 'admin' # note : not pretty sure it is correct or not.
port_id  = '5432'

PCC.hostname = hostname
PCC.database = database
PCC.username = username
PCC.pwd = pwd
PCC.port_id = port_id

# note cehck the used parameters for linking the database
PCC.print_DB_link_par()

# note : initialize the psql
cur,conn = PCC.initialize_psql_server()



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

    global check_print_tag
    ROC1_check_tag = 0
    ROC2_check_tag = 0

    print("====================================================================================")
    
    for i in range (len(ladder_text)) :
        if (var_barrel[i].get() != "NaN "):
            ROC1_check_tag = 1
            break

    for i in range (len(ladder_text)) :
        if (var_barrel_2[i].get() != "NaN "):
            ROC2_check_tag = 1
            break
    

    if (ROC1_check_tag + ROC2_check_tag == 0):
        print('No ladder filled !!')
        return
    
    if (var_server.get() == "NaN"):
        print('no server !!!')
        return

    if (var_NS.get() == "NaN"):
        print(" No NS !!!")
        return

    if( file_name_entry.get()[ len(file_name_entry.get())-4 : ] != '.pkl' ) :
        print('check the output file name')
        return
    
    if (ROC1_entry.get() + ROC2_entry.get() == ''):
        print("both ROC1 and ROC2 no entry ? ")
        return

    if ( ROC1_check_tag == 1 and ROC1_entry.get() == '' ):
        print('ROC1 should not be empty !')
        return

    if ( ROC2_check_tag == 1 and ROC2_entry.get() == '' ):
        print('ROC2 should not be empty !')
        return

    check_print_tag = 1

    print("Server :",var_server.get())
    print("File :", file_name_entry.get())
    print("Map  : ", map_name_entry.get())
    print("Side :", var_NS.get())
    

    if (ROC1_check_tag == 1):
        for i in range (len(ladder_text)) :
            if (var_barrel[i].get() != "NaN ") :
                print("ROC :",ROC1_entry.get(),"ID :",i+1,"Ladder :", var_barrel[i].get()+var_ladder[i].get(),"moduleID :",var_module[i].get(),"port :",var_port[i].get(),"status :",dict_status[var_status[i].get()])

    
    if (ROC2_check_tag == 1):
        print(" ")
        for i in range (len(ladder_text)) :
            if (var_barrel_2[i].get() != "NaN ") :
                print("ROC :",ROC2_entry.get(),"ID :",i+1,"Ladder :", var_barrel_2[i].get()+var_ladder_2[i].get(),"moduleID :",var_module_2[i].get(),"port :",var_port_2[i].get(),"status :",dict_status[var_status_2[i].get()])

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


def reset_ladder_part ():
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

        

        # note : reset status 
        var_status[i].set(opt_status[0])
        dropdown_status_list[i]['menu'].delete(0, 'end')

        var_status_2[i].set(opt_status[0])
        dropdown_status_list_2[i]['menu'].delete(0, 'end')

        for option_status in opt_status:
            dropdown_status_list[i]['menu'].add_command(label=option_status, command=tk._setit(var_status[i], option_status))
            dropdown_status_list_2[i]['menu'].add_command(label=option_status, command=tk._setit(var_status_2[i], option_status))



def reset_all():
    file_name_entry.delete(0, 'end')
    map_name_entry.delete(0, 'end')
    ROC1_entry.delete(0, 'end')
    ROC2_entry.delete(0, 'end')
    note1_entry.delete(0,'end')
    note2_entry.delete(0,'end')
    note3_entry.delete(0,'end')

    global check_print_tag
    check_print_tag = 0

    # note : reset data_server
    var_server.set(opt_server[0])
    dropdown_server['menu'].delete(0, 'end')
    for option_server in opt_server :
        dropdown_server['menu'].add_command(label=option_server, command=tk._setit(var_server, option_server))

    # note : reset NS
    var_NS.set(opt_NS[0])
    dropdown_NS['menu'].delete(0, 'end')
    for option_NS in opt_NS :
        dropdown_NS['menu'].add_command(label=option_NS, command=tk._setit(var_NS, option_NS))

    # note : reset the ladder part info.
    reset_ladder_part()

def find_latest_file():
    # file_directory = "/home/inttdev/work/cwshih/calib_db/fake_folder/"
    # note : now move to .pkl file
    list_of_files = glob.glob(file_default_directory+"/*.pkl")
    latest_file = max(list_of_files, key=os.path.getctime)
    # latest_file = latest_file.replace(file_directory,'')

    return latest_file

def update_file_latest ():
    file_name_entry.delete(0,'end')
    file_name_entry.insert('end', find_latest_file())



def browse_file():
    file_path = filedialog.askopenfilename(initialdir=file_default_directory)
    file_name_entry.delete(0,'end')
    file_name_entry.insert('end', file_path)

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
    map_path = filedialog.askopenfilename(initialdir=map_default_directory)
    map_name_entry.delete(0,'end')
    map_name_entry.insert('end', map_path)

    # note : print the map content 
    read_map_info(map_name_entry.get())

# note : the function to find the file for map
# note : ultra stupid 
def find_latest_file_map():
    # file_directory = "/home/inttdev/work/cwshih/calib_db/fake_folder/"
    list_of_files = glob.glob(map_default_directory+"/*.txt")
    latest_file = max(list_of_files, key=os.path.getctime)
    # latest_file = latest_file.replace(file_directory,'')

    return latest_file

def update_file_latest_map ():
    map_name_entry.delete(0,'end')
    map_name_entry.insert('end', find_latest_file_map())
    
    # note : print the map content 
    read_map_info(map_name_entry.get())


def apply_map ():

    # note : reset the ladder part info.
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

def get_tail_name (string_in):
    find_slash = string_in.rfind('/')
    return string_in[find_slash+1:] 

def fill_psql ():

    global check_print_tag
    if (check_print_tag == 0):
        print("!!! Check print first !!!")
        return

    table_name = PCC.cali_info_table_name
    # file_name  = get_tail_name(file_name_entry.get())
    # map_file   = get_tail_name(map_name_entry.get())
    file_name  = file_name_entry.get() # note : now let's try to save the whole directory as well
    map_file   = map_name_entry.get()  # note : now let's try to save the whole directory as well
    N_or_S     = var_NS.get()
    data_server = var_server.get()
    ladder_pos_1 = [] # note : for the ROC 1
    module_id_1  = []
    port_1       = []
    status_1     = []
    ladder_pos_2 = [] # note : for the ROC 2
    module_id_2  = []
    port_2       = [] 
    status_2     = []

    # note : check the entry
    ROC1_check_tag = 0
    ROC2_check_tag = 0

    for i in range (len(ladder_text)) :
        if (var_barrel[i].get() != "NaN "):
            ROC1_check_tag = 1
            break

    for i in range (len(ladder_text)) :
        if (var_barrel_2[i].get() != "NaN "):
            ROC2_check_tag = 1
            break


    if (ROC1_check_tag + ROC2_check_tag == 0) :
        print('no entry, no entry filled to the PSQL')
        return

    if (ROC1_check_tag == 1) :
        ROC = ROC1_entry.get()

        for i in range (len(ladder_text)) :
            if (var_barrel[i].get() != "NaN "):
                ladder_pos_1.append(var_barrel[i].get()+var_ladder[i].get()) 
                module_id_1.append(var_module[i].get())  
                port_1.append(var_port[i].get())
                status_1.append(dict_status[ var_status[i].get() ])      

        PCC.fill_calib_data (cur, conn, table_name, data_server, file_name, map_file, N_or_S, ROC, ladder_pos_1, module_id_1, port_1, status_1)

    
    if (ROC2_check_tag == 1) :
        ROC = ROC2_entry.get()

        for i in range (len(ladder_text)) :
            if (var_barrel_2[i].get() != "NaN "):
                ladder_pos_2.append(var_barrel_2[i].get()+var_ladder_2[i].get()) 
                module_id_2.append(var_module_2[i].get())  
                port_2.append(var_port_2[i].get())
                status_2.append(dict_status[ var_status_2[i].get() ])       

        PCC.fill_calib_data (cur, conn, table_name, data_server, file_name, map_file, N_or_S, ROC, ladder_pos_2, module_id_2, port_2, status_2)
    
    check_print_tag = 0
    print('PSQL updated')
            
def close_win ():
    PCC.close_psql_server(cur,conn)
    root.quit()

# note : create tkinter window
root = tk.Tk()
root.title("INTT Commissioning, Barrel calibration test")
root.geometry("1000x790")


check_print_tag = 0


ladder_text = ["Ladder1", "Ladder2", "Ladder3", "Ladder4", "Ladder5", "Ladder6", "Ladder7"]
roc_text = ["ROC1", "ROC2"]
file_name_text = "File name : "
module_text = "module id "
NS_text = "North or South : "
map_name_text = "Calib map : "
port_text = "port"
status_text = "status"
server_text = "data_server"

opt_barrrel = ["NaN ","B0L0","B0L1","B1L0","B1L1"]
opt_ladder  = ["00", "01", "02","03","04","05","06","07","08","09","10","11"]
opt_module  = [0,1,2,3,4,5,6]
opt_module_2  = [7,8,9,10,11,12,13]

opt_NS      = ["NaN","N","S"]
opt_port    = ["A1","A2","A3","B1","B2","B3","C1","C2","C3","D1","D2","D3"]

opt_server = ["NaN", "inttdaq", "test_server"]

file_default_directory = "/home/inttdev/data/IR"
map_default_directory  = "/home/inttdev/map" 

dict_status = {
    'NC' : 'no-comment',
    'Good' : 'good',
    'HE' : 'half-entry',
    'DE' : 'double-entry',
    'Noisy' : 'noisy',
    'Weird' : 'Weird',
    'HRN' : 'half-row-nothing',
    'HCN' : 'half-channel-nothing',
    'Nothing' : 'nothing',
    'test' : 'code-testing'}

opt_status = list(dict_status.keys())

var_barrel = [tk.StringVar(root) for i in range(len(ladder_text))]
var_ladder = [tk.StringVar(root) for i in range(len(ladder_text))]
var_module = [tk.StringVar(root) for i in range(len(ladder_text))]
var_port   = [tk.StringVar(root) for i in range(len(ladder_text))]
var_status = [tk.StringVar(root) for i in range(len(ladder_text))]
dropdown_barrel_list = []
dropdown_module_list = []
dropdown_ladder_list = []
dropdown_port_list   = []
dropdown_status_list = []


var_barrel_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_ladder_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_module_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
var_port_2   = [tk.StringVar(root) for i in range(len(ladder_text))]
var_status_2 = [tk.StringVar(root) for i in range(len(ladder_text))]
dropdown_barrel_list_2 = []
dropdown_module_list_2 = []
dropdown_ladder_list_2 = []
dropdown_port_list_2   = []
dropdown_status_list_2 = []




n_row = 1
# note : for file name 
file_name_label = tk.Label(root, text=file_name_text)
file_name_label.grid(row=n_row, column=1)

file_name_entry = tk.Entry(root,width=65,textvariable=tk.StringVar(value=""))
file_name_entry.grid(row=n_row, column=2, columnspan = 7)


button_browse = tk.Button(root, text="Find", fg='black', command=browse_file)
button_browse.grid(row=n_row, column=8 )

button_latest = tk.Button(root, text="Latest", fg='black', command=update_file_latest)
button_latest.grid(row=n_row, column=9 )



# note : for the map in
n_row += 1
map_name_label = tk.Label(root, text=map_name_text)
map_name_label.grid(row=n_row, column=1)

map_name_entry = tk.Entry(root,width=50,textvariable=tk.StringVar(value=""))
map_name_entry.grid(row=n_row, column=2, columnspan = 6)

button_browse_map = tk.Button(root, text="Find", fg='black', command=browse_map_file)
button_browse_map.grid(row=n_row, column=7 )

button_latest_map = tk.Button(root, text="Latest", fg='black', command=update_file_latest_map)
button_latest_map.grid(row=n_row, column=8 )

button_load_map = tk.Button(root, text="Load", fg='red', command=apply_map)
button_load_map.grid(row=n_row, column=9 )

# note : for the data_server
n_row += 1
server_label = tk.Label(root, text=server_text)
server_label.grid(row=n_row, column=1)

var_server = tk.StringVar(root)
var_server.set(opt_server[0])

dropdown_server = tk.OptionMenu(root, var_server, *opt_server)
dropdown_server.grid(row=n_row,column=2)

# note : for the N or S
NS_label = tk.Label(root, text=NS_text)
NS_label.grid(row=n_row, column=3)

var_NS = tk.StringVar(root)
var_NS.set(opt_NS[0])

dropdown_NS = tk.OptionMenu(root, var_NS, *opt_NS)
dropdown_NS.grid(row=n_row,column=4)


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
ROC1_label.grid(row=n_row, column=n_column,padx=10)

ROC1_entry = tk.Entry(root,width=10,textvariable=tk.StringVar(value=""))
ROC1_entry.grid(row=n_row+1, column=n_column, rowspan = 2, padx=10)

# note : for the ladder, ROC1 
for i in range( len(ladder_text) ):

    var_ladder[i].set(opt_ladder[0])

    dropdown_ladder = tk.OptionMenu(root, var_ladder[i], *opt_ladder)
    dropdown_ladder.grid(row=n_row+2,column=n_column+i+2)
    dropdown_ladder_list.append(dropdown_ladder)

# note : for the barrel, ROC1
for i in range( len(ladder_text) ):
    
    label_ID = tk.Label(root, fg = '#00A2FF', text=ladder_text[i])
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

# note : for the status, ROC1
n_row += 2
for i in range( len(ladder_text) ):

    label_status = tk.Label(root, text=status_text)
    label_status.grid(row=n_row,column=n_column+i+2)

    var_status[i].set(opt_status[0])

    dropdown_status = tk.OptionMenu(root, var_status[i], *opt_status)
    dropdown_status.grid(row=n_row+1,column=n_column+i+2)
    dropdown_status_list.append(dropdown_status)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 2
separate_bar2 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========


# note : ==================================================================================================================================

# note : for the ROC2
n_row += 2

ROC2_label = tk.Label(root, text=roc_text[1])
ROC2_label.grid(row=n_row, column=n_column,padx=10)
ROC2_entry = tk.Entry(root,width=10,textvariable=tk.StringVar(value=""))
ROC2_entry.grid(row=n_row+1, column=n_column, rowspan = 2, padx=10)

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


# note : for the status, ROC2
n_row += 2
for i in range( len(ladder_text) ):

    label_status_2 = tk.Label(root, text=status_text)
    label_status_2.grid(row=n_row,column=n_column+i+2)

    var_status_2[i].set(opt_status[0])

    dropdown_status_2 = tk.OptionMenu(root, var_status_2[i], *opt_status)
    dropdown_status_2.grid(row=n_row+1,column=n_column+i+2)
    dropdown_status_list_2.append(dropdown_status_2)


# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 2
separate_bar3 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar3.grid(row=n_row, column = 1, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========


# note : for the note 
n_row += 3
note1_label = tk.Label(root, text="Note 1 : ")
note1_label.grid(row=n_row, column=1)

note1_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note1_entry.grid(row=n_row, column=2, columnspan = 8)

n_row += 1
note2_label = tk.Label(root, text="Note 2 : ")
note2_label.grid(row=n_row, column=1)

note2_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note2_entry.grid(row=n_row, column=2, columnspan = 8)

n_row += 1
note3_label = tk.Label(root, text="Note 3 : ")
note3_label.grid(row=n_row, column=1)

note3_entry = tk.Entry(root,width=95,textvariable=tk.StringVar(value=""))
note3_entry.grid(row=n_row, column=2, columnspan = 8)

# note : for the print
n_row += 1
button_print = tk.Button(root, text="Print", command=print_all)
button_print.grid(row=n_row, column=1 )

# note : fill to psql
button_psql = tk.Button(root, text="Fill PSQL", command=fill_psql)
button_psql.grid(row=n_row, column=4 )

# note : for rest
button_reset = tk.Button(root, text="Reset", fg='red', command=reset_all)
button_reset.grid(row=n_row, column=8 )

# note : for close the window
button_close = tk.Button(root, text="Close", fg='red', command=close_win)
button_close.grid(row=n_row, column=9 )


root.mainloop()

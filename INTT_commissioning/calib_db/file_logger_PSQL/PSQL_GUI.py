import tkinter as tk
import pandas as pd
import PSQL_for_cabling_cali as PCC

# note : psql initial value reset to default
def func_ini_reset ():
    for i in range (len(psql_ini_name)):
        var_psql_ini[i].set(value=psql_ini_info_def[i])

# note : update the column list 
def func_update_col ():

    global column_list
    global column_list_expand
    input_str = var_table.get()
    input_str = input_str.replace(',','')
    input_str = input_str.replace('(','')
    input_str = input_str.replace(')','')
    input_str = input_str.replace('\'','')
    # print('->',input_str,'<-')

    cur,conn = PCC.initialize_psql_server()
    input_column = PCC.count_N_column (cur,conn,name_of_table = input_str)

    dropdown_column['menu'].delete(0, 'end')
    for option in column_list:
        dropdown_column['menu'].add_command(label=option, command= tk._setit(var_column, option))

    for i in range(len(input_column)):
        dropdown_column['menu'].add_command(label=i+1, command=tk._setit(var_column, i+1))
    
    var_column.set(column_list[0])

    column_list_expand = input_column

# note : to use those value
def func_ini_load ():

    global table_list

    print( ' ============= the used value declaration : =============')
    print( 'hostname =', var_psql_ini[0].get() )
    print( 'database =', var_psql_ini[1].get() )
    print( 'username =', var_psql_ini[2].get() )
    print( 'pwd      =', var_psql_ini[3].get() ) 
    print( 'port_id  =', var_psql_ini[4].get() )
    print( '============= please re-declare them if needed =============')

    PCC.hostname = var_psql_ini[0].get()
    PCC.database = var_psql_ini[1].get()
    PCC.username = var_psql_ini[2].get()
    PCC.pwd = var_psql_ini[3].get()
    PCC.port_id = var_psql_ini[4].get()

    cur,conn = PCC.initialize_psql_server()

    table_list = PCC.print_table_in_db(cur,conn)
    # print('print the table list : ',table_list)

    # note : so it deletes the items in the list, and use the for loop to add them back, one by one.
    dropdown_table['menu'].delete(0, 'end')
    dropdown_table['menu'].add_command(label='NaN', command=tk._setit(var_table, 'NaN'))
    for option in table_list:
        dropdown_table['menu'].add_command(label=option, command= tk._setit(var_table, option))

    var_table.set("NaN")

# note : dynamic checkmenu for the column
def create_option_menus():

    var_order_who.set('NaN')
    dropdown_order_who['menu'].delete(0, 'end')
    dropdown_order_who['menu'].add_command(label='NaN', command=tk._setit(var_order_who, 'NaN'))
    for option in column_list_expand:
        dropdown_order_who['menu'].add_command(label=option, command= tk._setit(var_order_who, option))
    
    global n_row
    num_option_menus = 0
    
    if (var_column.get() == column_list[0]):
        print('wrong number')
        for option_menu in option_menus:
            option_menu.grid_forget()
        option_menus.clear()
        return

    elif (var_column.get() == column_list[1]):
        num_option_menus = 1

    else :
        num_option_menus = int(var_column.get())

    # Remove existing OptionMenus
    for option_menu in option_menus:
        option_menu.grid_forget()
    option_menus.clear()

    # Create new OptionMenus based on selection in first row
    for i in range(num_option_menus):
        var = tk.StringVar(root)
        var.set('')
        option_menu = tk.OptionMenu(root, var, *column_list_expand)
        option_menu.grid(row=5, column=i)
        option_menus.append(option_menu)

def print_table():
    
    print_string = 'select '
    
    # Print selected values from OptionMenus in second row
    selected_values = []
    for option_menu in option_menus:
        selected_value = option_menu.cget('text')
        selected_values.append(selected_value)
    print(selected_values)

    cur,conn = PCC.initialize_psql_server()

    # note : all
    if (var_column.get() == column_list[0]) :
        print_string += '* from '

    # note : distinct
    elif (var_column.get() == column_list[1]):
        print_string += 'distinct '
        print_string += selected_values[0]
        print_string += ' from '
    else :
        for i in range (len(selected_values)) : 
            print_string += selected_values[i]
            if ( i != len(selected_values) - 1):
                print_string += ', '
        print_string += ' from '
    
    input_str = var_table.get()
    input_str = input_str.replace(',','')
    input_str = input_str.replace('(','')
    input_str = input_str.replace(')','')
    input_str = input_str.replace('\'','')

    print_string += input_str

    if(note3_entry.get() != ''):
        print_string += ' '
    print_string += note3_entry.get()

    # print('test : ',note3_entry.get())

    # note : ASC   
    if (var_order.get() == order_list[1]):
        print_string += ' order by '
        print_string += var_order_who.get()
        print_string += ' '
        print_string += order_list[1]
    # note : DESC
    elif (var_order.get() == order_list[2]):
        print_string += ' order by '
        print_string += var_order_who.get()
        print_string += ' '
        print_string += order_list[2]
    
    print_string += ';'

    print('output command : ', print_string)

    cur.execute(print_string)
    colnames = [desc[0] for desc in cur.description]
    df = pd.DataFrame(data = cur.fetchall(), columns = colnames)
    pd.options.display.max_colwidth = 1000
    pd.set_option('display.max_columns', None)
    print(df.to_string())

    conn.commit()

def clear_note_3():
    note3_entry.delete(0,'end')   

def close_win ():
    # cur,conn = PCC.initialize_psql_server()
    # PCC.close_psql_server(cur,conn)
    root.quit()




# hostname = 'localhost'
# database = 'intt_cali_cabling_test_1'
# username = 'chengweishi'
# pwd      = 'admin' # note : not pretty sure it is correct or not.
# port_id  = '5432'

# PCC.hostname = hostname
# PCC.database = database
# PCC.username = username
# PCC.pwd = pwd
# PCC.port_id = port_id

# # note cehck the used parameters for linking the database
# PCC.print_DB_link_par()

# # note : initialize the psql
# cur,conn = PCC.initialize_psql_server()


root = tk.Tk()

# app = App(root)

root.title("General purpose, PSQL interface")
root.geometry("1150x300")

psql_ini_name = ['hostname','database','username','pwd','port id']
psql_ini_info_def = ['/var/run/postgresql', 'cw_calib_test', 'inttdev', 'admin', '5432']
var_psql_ini = [tk.StringVar(root,value=psql_ini_info_def[i]) for i in range(len(psql_ini_name))]

table_list = ['NaN']
column_list = ['all','distinct']
order_list = ['NaN','ASC','DESC']
order_who_list = ['NaN']
column_list_expand = []
option_menus = []

n_row = 0
# note : first row, to initiate the database
for i in range (len(psql_ini_name)):
    psql_ini_label = tk.Label(root, text=psql_ini_name[i])
    psql_ini_label.grid(row=0,column=i)


# note : reset the value
psql_ini_reset_button = tk.Button(root, text="reset",fg='red',command=func_ini_reset)
psql_ini_reset_button.grid(row=0, column=len(psql_ini_name))

# note : second row, to fill the value
n_row += 1
for i in range (len(psql_ini_name)):    
    psql_ini_entry = tk.Entry(root,textvariable=var_psql_ini[i])
    psql_ini_entry.grid(row=1, column=i)


# note : check the value 
psql_ini_button = tk.Button(root, text="Load",command=func_ini_load)
psql_ini_button.grid(row=1, column=len(psql_ini_name))

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# n_row += 2
separate_bar2 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=2, column = 0, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# note : third row, select table
n_row += 1

var_table = tk.StringVar(root)
var_table.set(table_list[0])
dropdown_table = tk.OptionMenu(root, var_table, *table_list)
dropdown_table.grid(row=3,column=0)

var_column = tk.StringVar(root)
var_column.set(column_list[0])
dropdown_column = tk.OptionMenu(root, var_column, *column_list)
dropdown_column.grid(row=3,column=1)

var_order = tk.StringVar(root)
var_order.set(order_list[0])
dropdown_order = tk.OptionMenu(root, var_order, *order_list)
dropdown_order.grid(row=3,column=2)

n_row += 1

table_button = tk.Button(root, text='Select table : ',command = func_update_col)
table_button.grid(row=4,column=0)

column_button = tk.Button(root, text='checked column : ',command = create_option_menus)
column_button.grid(row=4,column=1)

var_order_who = tk.StringVar(root)
var_order_who.set('NaN')
dropdown_order_who = tk.OptionMenu(root, var_order_who, *order_who_list)
dropdown_order_who.grid(row=4,column=2)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# n_row += 2
separate_bar2 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=6, column = 0, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
note3_button = tk.Button(root, text="additional : ",command=clear_note_3)
note3_button.grid(row=7, column=0)

note3_entry = tk.Entry(root,width=85,textvariable=tk.StringVar(value=""))
note3_entry.grid(row=7, column=1, columnspan = 8)


# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

# n_row += 2
separate_bar2 = tk.Label(root, text="-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
separate_bar2.grid(row=8, column = 0, columnspan = 10)

# ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= ========= =========

n_row += 1
final_button = tk.Button(root, text='Print table ', fg = 'red',command = print_table)
final_button.grid(row=9,column=0)

# note : for close the window
button_close = tk.Button(root, text="Close", fg='red', command=close_win)
button_close.grid(row=9, column=5 )

root.mainloop()

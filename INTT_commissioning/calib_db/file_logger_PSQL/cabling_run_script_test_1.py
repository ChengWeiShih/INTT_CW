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

# # note : create the table for the ladder info
# # todo : just do this for the first time 
#ladder_CVS_directory = '/home/inttdev/work/cwshih/calib_db/Barrel_ladder_info.csv'
#PCC.create_table(cur, conn, PCC.ladder_info_table_name, PCC.header_name_ladder, PCC.condition_ladder)
# note : dump the ladder information from the cvs
#PCC.dump_the_cvs(cur,conn,PCC.ladder_info_table_name,['Side', 'Position', 'Ladder_name'],ladder_CVS_directory)

# note : create the table for the cabling calibration database
PCC.create_table(cur, conn, PCC.cali_info_table_name, PCC.header_name_cali, PCC.condition_cali_info)

#ladder_pos = ['B0L110','B1L010','B0L104','B1L108']
#N_or_S = 'S'
#module_id = [9,9,1,2]
#cali_file = 'cali_test_23'

#PCC.fill_calib_data (cur, conn, PCC.cali_info_table_name, cali_file, ladder_pos, N_or_S, module_id)

# note : close the PSQL
PCC.close_psql_server(cur,conn)


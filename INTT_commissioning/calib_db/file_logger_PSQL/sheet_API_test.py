from googleapiclient.discovery import build # note : for google sheet API
from google.oauth2 import service_account   # note : for google sheet API
import numpy as np

import pandas as pd

import time



SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']
SERVICE_ACCOUNT_FILE = "/home/inttdev/work/cwshih/calib_db/key.json" # note : the key for access
creds = None
# note : The ID spreadsheet.
SAMPLE_SPREADSHEET_ID = '19PSvkhAQA8rnkP0z_wpN87CWQ6O-f3bT' # note : the id of the spreadsheet, from the address


# note : the functions for sheet to pd
def initialize_sheet_to_pd () :
    
    creds = service_account.Credentials.from_service_account_file(SERVICE_ACCOUNT_FILE, scopes = SCOPES)

    # it seems to be the version of API function
    service = build('sheets', 'v4', credentials=creds)

    # get sheet information
    sheet = service.spreadsheets()

    return sheet

# note : return value : dict of pandas dataframes
def sheet_to_pd(sheet) :

    get_sheet_info = sheet.get(spreadsheetId=SAMPLE_SPREADSHEET_ID ).execute().get('sheets', '')
    print("number of the sheets : ", len(get_sheet_info))

    # note : a list contains all the names of the sheets
    table_name_list = []
    for i in range (len(get_sheet_info)):
        table_name_list.append(get_sheet_info[i].get("properties", {}).get("title", "Sheet1"))
    print('names of sheets : ',table_name_list)

    # note : save the values of the sheets
    # todo : the 'A:W' may need to be changed, it dependes the size of the table. 
    
    # note : the version of list
    # sheet_value_pd_list = []
    # for i in range (len(table_name_list)):
    #     sheet_value_pd_list.append(pd.DataFrame(sheet.values().get(spreadsheetId=SAMPLE_SPREADSHEET_ID, range="%s!A:W"% (table_name_list[i]) ).execute().get('values', [])))

    # note : the version of dict
    sheet_value_pd_list = {}
    for i in range (len(table_name_list)):
        sheet_value_pd_list[table_name_list[i]] = pd.DataFrame(sheet.values().get(spreadsheetId=SAMPLE_SPREADSHEET_ID, range="%s!A:W"% (table_name_list[i]) ).execute().get('values', []))
    

    return sheet_value_pd_list

def name_of_tables (sheet):
    get_sheet_info = sheet.get(spreadsheetId=SAMPLE_SPREADSHEET_ID ).execute().get('sheets', '')
    print("number of the sheet : ", len(get_sheet_info))

    # a list contains all the names of the sheets
    table_name_list = []
    for i in range (len(get_sheet_info)):
        table_name_list.append(get_sheet_info[i].get("properties", {}).get("title", "Sheet1"))
    print('name of sheets : ',table_name_list)


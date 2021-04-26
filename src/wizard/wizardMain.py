# img_viewer.py

import PySimpleGUI as sg
import os.path
import esptool
from serialWiz import * 
from loayoutWiz import * 
from datetime import datetime
import sys
from io import StringIO 
from contextlib import redirect_stdout
import re

def log(b, window):
    now = datetime.now()
    time = "["+now.strftime("%H:%M:%S")+"] "
    window["CONSOLE"].update(disabled=False)
    txt = window.FindElement('CONSOLE').Widget
    txt.insert("end", time + b)
    window["CONSOLE"].update(disabled=True)
    window.refresh()

class customIO(StringIO):
    window = None
    p = re.compile(r"(\d?\d?\d) %")
    def __init__(self,window):
        self.window = window
        super().__init__()
    def write(self, b):
        now = datetime.now()
        time = "["+now.strftime("%H:%M:%S")+"] "
        temp = b
        if not temp.strip() == "":
            self.window["CONSOLE"].update(disabled=False)
            txt = self.window.FindElement('CONSOLE').Widget
            txt.insert("end", time + b+"\n")
            self.window["CONSOLE"].update(disabled=True)
            result = self.p.search(b)
            if result:
                progress = result.group(1)
                self.window['PROGRESS'].update(current_count=progress)
            self.window.refresh()
        super(customIO,self).write(b)

def landingPage(window):
    window["LANDING_COL"].update(visible=True)
    window["SERIAL_COL"].update(visible=False)
    window["IMAGE_COL"].update(visible=True)
    window["GOBACK_SECTION"].update(visible=False)
    window["PROGRESS_SECTION"].update(visible=False)

    window["LAND_HELP"].update(visible=True)
    window["PORT_HELP"].update(visible=False)
    window["UPLOAD_HELP"].update(visible=False)
    window["ERASE_HELP"].update(visible=False)
    window["WIFI_HELP"].update(visible=False)
    window["COMPLETE_HELP"].update(visible=False)

    return "landing"

def uploadPage(window,selectedPort):

    window["LAND_HELP"].update(visible=False)
    window["PORT_HELP"].update(visible=False)
    window["UPLOAD_HELP"].update(visible=False)
    window["ERASE_HELP"].update(visible=False)
    window["WIFI_HELP"].update(visible=False)
    window["COMPLETE_HELP"].update(visible=False)

    if selectedPort == "":
        window['PORT_SELECT_SECTION'].update(visible=True)
        window['SELECTED_PORT_SECTION'].update(visible=False)
        window["PORT_HELP"].update(visible=True)
    else:
        window['PORT_SELECT_SECTION'].update(visible=False)
        window['SELECTED_PORT_SECTION'].update(visible=True)
        window["UPLOAD_HELP"].update(visible=True)
    window["LANDING_COL"].update(visible=False)
    window["SERIAL_COL"].update(visible=True)
    window["IMAGE_COL"].update(visible=True)
    window["GOBACK_SECTION"].update(visible=True)
    window["CMD_SECTION"].update(visible=False)
    window["PROGRESS_SECTION"].update(visible=False)
    window["SSID_SECTION"].update(visible=False)
    window["PASSWORD_SECTION"].update(visible=False)
    window["WIFI_BUTTON_SECTION"].update(visible=False)
    return "upload"

def erasePage(window,selectedPort):
    window["LAND_HELP"].update(visible=False)
    window["PORT_HELP"].update(visible=False)
    window["UPLOAD_HELP"].update(visible=False)
    window["ERASE_HELP"].update(visible=False)
    window["WIFI_HELP"].update(visible=False)
    window["COMPLETE_HELP"].update(visible=False)

    if selectedPort == "":
        window['PORT_SELECT_SECTION'].update(visible=True)
        window['SELECTED_PORT_SECTION'].update(visible=False)
        window["PORT_HELP"].update(visible=True)
    else:
        window['PORT_SELECT_SECTION'].update(visible=False)
        window['SELECTED_PORT_SECTION'].update(visible=True)
        window["ERASE_HELP"].update(visible=True)
    window["LANDING_COL"].update(visible=False)
    window["SERIAL_COL"].update(visible=True)
    window["IMAGE_COL"].update(visible=True)
    window["GOBACK_SECTION"].update(visible=True)
    window["CMD_SECTION"].update(visible=False)
    window["PROGRESS_SECTION"].update(visible=False)
    window["SSID_SECTION"].update(visible=False)
    window["PASSWORD_SECTION"].update(visible=False)
    window["WIFI_BUTTON_SECTION"].update(visible=False)
    return "erase"

def serialPage(window,selectedPort):
    window["LAND_HELP"].update(visible=False)
    window["PORT_HELP"].update(visible=False)
    window["UPLOAD_HELP"].update(visible=False)
    window["ERASE_HELP"].update(visible=False)
    window["WIFI_HELP"].update(visible=False)
    window["COMPLETE_HELP"].update(visible=False)

    if selectedPort == "":
        window['PORT_SELECT_SECTION'].update(visible=True)
        window['SELECTED_PORT_SECTION'].update(visible=False)
        window["PORT_HELP"].update(visible=True)
        window["CMD_SECTION"].update(visible=False)
    else:
        window['PORT_SELECT_SECTION'].update(visible=False)
        window['SELECTED_PORT_SECTION'].update(visible=True)
        window["COMPLETE_HELP"].update(visible=True)
        window["CMD_SECTION"].update(visible=True)
    window["LANDING_COL"].update(visible=False)
    window["SERIAL_COL"].update(visible=True)
    window["IMAGE_COL"].update(visible=True)
    window["GOBACK_SECTION"].update(visible=True)
    window["PROGRESS_SECTION"].update(visible=False)
    window["SSID_SECTION"].update(visible=False)
    window["PASSWORD_SECTION"].update(visible=False)
    window["WIFI_BUTTON_SECTION"].update(visible=False)
    return "serial"

def wifiPage(window,selectedPort):
    window["LAND_HELP"].update(visible=False)
    window["PORT_HELP"].update(visible=False)
    window["UPLOAD_HELP"].update(visible=False)
    window["ERASE_HELP"].update(visible=False)
    window["WIFI_HELP"].update(visible=False)
    window["COMPLETE_HELP"].update(visible=False)

    if selectedPort == "":
        window['PORT_SELECT_SECTION'].update(visible=True)
        window['SELECTED_PORT_SECTION'].update(visible=False)
        window["PORT_HELP"].update(visible=True)
        window["SSID_SECTION"].update(visible=False)
        window["PASSWORD_SECTION"].update(visible=False)
        window["WIFI_BUTTON_SECTION"].update(visible=False)
    else:
        window['PORT_SELECT_SECTION'].update(visible=False)
        window['SELECTED_PORT_SECTION'].update(visible=True)
        window["WIFI_HELP"].update(visible=True)
        window["SSID_SECTION"].update(visible=True)
        window["PASSWORD_SECTION"].update(visible=True)
        window["WIFI_BUTTON_SECTION"].update(visible=True)
    window["LANDING_COL"].update(visible=False)
    window["SERIAL_COL"].update(visible=True)
    window["IMAGE_COL"].update(visible=True)
    window["GOBACK_SECTION"].update(visible=True)
    window["CMD_SECTION"].update(visible=False)
    window["PROGRESS_SECTION"].update(visible=False)
    return "wifi"

def main():
    sg.theme('Dark Grey 12')
    #tray = sg.SystemTray(filename=abs_file_path)
    script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in
    rel_path = "./res/hat.png"
    abs_file_path = os.path.join(script_dir, rel_path)
    window = sg.Window("Shroom Sense Wizard", uiLayout, icon=abs_file_path)
    selectedPort = ""

    window.read(timeout=1)
    page = landingPage(window)

    serial = None

    # Run the Event Loop
    while True:
        if serial and serial.in_waiting:
            try:
                val = str(serial.readline().decode().replace('\r\n','\n'))
                if val:
                    log(val,window)
            except Exception:
                # TODO deal with errors here
                pass
        event, values = window.read(timeout=50)
        if event == "Exit" or event == sg.WIN_CLOSED:
            break
        #landing interface
        elif event == "LAND_UPLOAD":
            page = uploadPage(window,selectedPort)
        elif event == "LAND_SERIAL":
            page = serialPage(window,selectedPort)
        elif event == "LAND_WIFI":
            page = wifiPage(window,selectedPort)
        elif event == "LAND_ERASE":
            page = erasePage(window,selectedPort)
        elif event == "GOTOLAND":
            selectedPort = ""
            if not serial == None:
                serial.close()
            serial = None 
            page = landingPage(window)
        #WIFI Interface
        elif event == "SKIPWIFI":
            page = serialPage(window,selectedPort)
        elif event == "SENDWIFI":
            ssid = values['SSID']
            password = values['PASSWORD']
            serial.write(("<CONNECT_WIFI:"+ssid+","+password+">").encode())
        #Serial Interface
        elif event == "REFRESH":
            window["PORTS"].update(values=serial_ports())
        elif event == "SENDCMD":
            if serial:
                serial.write(("<"+values['CMD']+">").encode())
        elif event == "START" and page == "wifi":
            selectedPort = values['PORTS']
            window['SELECTED_PORT'].update(value = selectedPort)
            if not selectedPort == "":
                window['PORT_SELECT_SECTION'].update(visible=False)
                window['SELECTED_PORT_SECTION'].update(visible=True)
                window["PORT_HELP"].update(visible=False)
                window["WIFI_HELP"].update(visible=True)
                window["SSID_SECTION"].update(visible=True)
                window["PASSWORD_SECTION"].update(visible=True)
                window["WIFI_BUTTON_SECTION"].update(visible=True)
                serial = openSerial(selectedPort)
        elif event == "START" and page == "serial":
            selectedPort = values['PORTS']
            window['SELECTED_PORT'].update(value = selectedPort)
            if not selectedPort == "":
                window['PORT_SELECT_SECTION'].update(visible=False)
                window['SELECTED_PORT_SECTION'].update(visible=True)
                window["PORT_HELP"].update(visible=False)
                window["CMD_SECTION"].update(visible=True)
                serial = openSerial(selectedPort)
        elif event == "START" and page == "erase":
            selectedPort = values['PORTS']
            window['SELECTED_PORT'].update(value = selectedPort)
            if not selectedPort == "":
                window["GOBACK_SECTION"].update(visible=False)
                window['PORT_SELECT_SECTION'].update(visible=False)
                window['SELECTED_PORT_SECTION'].update(visible=True)
                window["PORT_HELP"].update(visible=False)
                window["ERASE_HELP"].update(visible=True)
                try:
                    f = customIO(window)
                    with redirect_stdout(f):
                        esptool.main(['--port', selectedPort, 'erase_flash'])
                    log("Board Memory Erased\n",window)
                    window["GOBACK_SECTION"].update(visible=True)
                except Exception:
                    # TODO deal with errors here
                    log("Welp Thats not right...\n",window)
                    log("Looks like there was a problem uploading to the board\n",window)
                    log("Make sure you've got the right port selected\n",window)
                    f = customIO(window)
                    with redirect_stdout(f):
                        print(Exception)
                    pass
        elif event == "START" and page == "upload":
            selectedPort = values['PORTS']
            window['SELECTED_PORT'].update(value = selectedPort)
            if not selectedPort == "":
                window["GOBACK_SECTION"].update(visible=False)
                window['PORT_SELECT_SECTION'].update(visible=False)
                window['SELECTED_PORT_SECTION'].update(visible=True)
                window['PROGRESS_SECTION'].update(visible=True)
                window["PORT_HELP"].update(visible=False)
                window["UPLOAD_HELP"].update(visible=True)
                window['PROGRESS'].update(current_count=0)
                try:
                    f = customIO(window)
                    with redirect_stdout(f):
                        script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in
                        rel_path = "./res/MushroomSense.bin"
                        abs_file_path = os.path.join(script_dir, rel_path)
                        esptool.main(['--port', selectedPort, 'write_flash', '0x0000', abs_file_path] )
                    window['PROGRESS'].update(current_count=100)
                    log("Connecting Serial Monitor\n\n",window)
                    serial = openSerial(selectedPort)
                    log("================= Serial Monitor Output =================\n",window)
                    page = wifiPage(window,selectedPort)
                except Exception:
                    # TODO deal with errors here
                    log("Welp Thats not right...\n",window)
                    log("Looks like there was a problem uploading to the board\n",window)
                    log("Make sure you've got the right port selected\n",window)
                    window['PORT_SELECT_SECTION'].update(visible=True)
                    window['SELECTED_PORT_SECTION'].update(visible=False)
                    f = customIO(window)
                    with redirect_stdout(f):
                        print(Exception)
                    pass

    window.close()

import PySimpleGUI as sg
import os.path
from serialWiz import * 
from datetime import datetime
import sys
from io import StringIO 
from contextlib import redirect_stdout
import re

sg.theme('Dark Grey 12')


def collapse(layout, key, visible=True):
    """
    Helper function that creates a Column that can be later made hidden, thus appearing "collapsed"
    :param layout: The layout for the section
    :param key: Key used to make this seciton visible / invisible
    :return: A pinned column that can be placed directly into your layout
    :rtype: sg.pin
    """
    return sg.pin(sg.Column(layout, key=key, visible=visible))

serialMonitorLayoutColumn = [
    [ collapse([[
        sg.Text("Port: ", font=('Helvetica 20')),
        sg.Combo(values=serial_ports(), default_value=None, key="PORTS", font=('Helvetica 20'), readonly=True),
        sg.Button("Refresh Ports", key="REFRESH", font=('Helvetica 20')),
        sg.Button("Start", key="START", font=('Helvetica 20'))
        ]], "PORT_SELECT_SECTION")
    ],
    [collapse([[
        sg.Text("Selected Port: ", font=('Helvetica 20')),
        sg.Text("", font=('Helvetica 20'), key="SELECTED_PORT", size=(50,1))
    ]], "SELECTED_PORT_SECTION")
    ],
    [collapse([[
        sg.Multiline('', size=(70,20), key="CONSOLE", disabled=True, autoscroll=True, font=('Helvetica 20'))
    ]],"CONSOLE_SECTION")
    ],
    [collapse([[
        sg.In('', size=(60,1), key="CMD", font=('Helvetica 20')),
        sg.Button("Send", key="SENDCMD", font=('Helvetica 20'))
    ]],"CMD_SECTION")
    ],
    [collapse([[
        sg.Text("SSID: ", font=('Helvetica 20'), key="SSID_LABEL"),
        sg.In('', size=(30,1), key="SSID", font=('Helvetica 20')),
    ]],"SSID_SECTION")
    ],
    [collapse([[
        sg.Text("Password: ", font=('Helvetica 20'), key="PASSWORD_LABEL"),
        sg.In('', size=(30,1), key="PASSWORD", font=('Helvetica 20'))
    ]],"PASSWORD_SECTION")
    ],
    [collapse([[
        sg.Button("Connect WiFi", key="SENDWIFI", font=('Helvetica 20')),
        sg.Button("Skip", key="SKIPWIFI", font=('Helvetica 20'))
    ]],"WIFI_BUTTON_SECTION")   
    ]
]

script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in
rel_path = "./res/hermit1.png"
abs_file_path = os.path.join(script_dir, rel_path)

imageColumn = [
    [collapse([[
        sg.Progress(max_value=100, key="PROGRESS", size=(45,20), bar_color=("#8b9fde","#313641"))
    ]],"PROGRESS_SECTION")
    ],
    [collapse([[
        sg.Text("If you are setting up a new board click Uplaod", size=(45,2), font=('Helvetica 14')),
    ]],"LAND_HELP")
    ],
    [collapse([[
        sg.Text("Select your boards port from the dropdown and click Start. Not sure which port to select? "+
        "Try disconecting your bord and clicking Refresh Ports then connect your board and click Refresh Ports. "+
        "take note of the ports avalible in the dropwon after each refresh. "+
        "Your bord is on the port that appears appears on the second list and not the first."
        , size=(45,8), font=('Helvetica 14')),
    ]],"PORT_HELP")
    ],
    [collapse([[
        sg.Text("Hang tight, uploading Mushroom Sense to your board.", size=(45,2), font=('Helvetica 14')),
    ]],"UPLOAD_HELP")
    ],
    [collapse([[
        sg.Text("Erasing Your Board. Yeah there's no progress bar. sorry. Practice some meditation while you wait.", size=(45,3), font=('Helvetica 14')),
    ]],"ERASE_HELP")
    ],
    [collapse([[
        sg.Text("Connect your board to WiFi by typing your network name in the SSID field and password in the "+
        "Password field. Then Click Connect WiFi. You can all so skip this step and set it up later.", size=(45,5), font=('Helvetica 14')),
    ]],"WIFI_HELP")
    ],
    [collapse([[
        sg.Text("All done! If everything went as planned your should see the sensor data display ever couple seconds.", size=(45,3), font=('Helvetica 14')),
    ]],"COMPLETE_HELP")
    ],
    [
        sg.Image(filename=abs_file_path)
    ],
    [collapse([[
        sg.Button("⇽ Do Something Else", key="GOTOLAND", font=('Helvetica 20')),
    ]],"GOBACK_SECTION")
    ],
]

landingColumn =[
            [
                sg.Button("Upload", key="LAND_UPLOAD", font=('Helvetica 40')),
               
            ],
            [
                sg.Button("Configure WiFi", key="LAND_WIFI", font=('Helvetica 20')),
            ],
            [
                sg.Button("Serial Monitor", key="LAND_SERIAL", font=('Helvetica 20'))
            ],
            [
                sg.Button("Erase Board", key="LAND_ERASE", font=('Helvetica 20'))
            ]
        ]


# ----- Full layout -----
uiLayout = [
    [
        collapse(imageColumn, key="IMAGE_COL", visible=False),
        collapse(landingColumn, "LANDING_COL", visible=False),
        collapse(serialMonitorLayoutColumn,"SERIAL_COL", visible=False),
    ]
]
from tkinter import *
from tkinter import messagebox
from tkinter.colorchooser import askcolor

import serial
import serial.tools.list_ports
import time

MAX_BDS = 146
FIRST_YEAR = 1900

CTABLE_LEN = 24
INVALID = CTABLE_LEN - 1
cTable = "abcdefghijklmnoprstuvz ."

def find_arduino(ser, element, colors, info):
    ports = list(serial.tools.list_ports.comports())
    ser = None
    for p in ports:
        try:
            ser = serial.Serial(p.device, 9600, timeout=1, write_timeout=1, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)  # Establish the connection on a specific port
            time.sleep(2)
            ser.write(b'_') #clear buffer
            ser.write(b'test')  # Send a byte string to the Arduino
            time.sleep(0.1)
            while(1):
                response = ser.readline().strip().decode("utf-8")
                if(response != b""):
                    break
            if response == "test":
                break
        except:
            pass
    if(ser is not None):
        element.config(background=colors[0])
        info.config(text="Koledar je povezan!")
    else:
        element.config(background=colors[1])
        info.config(text="Povezava ni bila uspešna.")
    return ser

def callback_prikazi(ser, b_r, b_g, b_b, t_r, t_g, t_b, brightness, info, save=False, reset=False):
    if ser is None:
        info.config(text="Koledar ni povezan!")
        return
    
    ser.write(b'_') #clear buffer

    if(save == False):
        ser.write(b'showprofile')
    elif(reset == True):
        ser.write(b'reset')
    else:
        ser.write(b'writeprofile')

    send_text = "{:03d}{:03d}{:03d}{:03d}{:03d}{:03d}".format(b_r, b_g, b_b, t_r, t_g, t_b)
    ser.write(send_text.encode())
    ser.write(b'.')
    time.sleep(0.1)
    send_text = int((brightness/100)*255)
    ser.write(str(send_text).encode())
    ser.write(b'.')
    time.sleep(0.1)
    while(1):
        response = ser.readline().strip().decode("utf-8")
        if(response != b""):
            break
    response = ser.readline().strip().decode("utf-8")
    info.config(text=response)

def write_BD_callback(ser, info_text, bd_text):
    if ser is None:
        info_text.config(text="Koledar ni povezan.")
        return
    bds = [bd for bd in bd_text.split("\n") if bd]
    if len(bds) > MAX_BDS:
        info_text.config(text="Preveč oseb, vpiši jih največ 146.")
        return
    elif len(bds) == 0:
        info_text.config(text="Ni vpisanih oseb.")
        return
    
    final = []
    for bd in bds:
        bd = bd.strip()
        if len(bd) == 0:
            continue
        name_surname = ""
        index = 0
        while index < len(bd) and (bd[index] < '0' or bd[index] > '9'):
            crka = bd[index].lower()
            if(crka == 'č'):
                name_surname += "\1"
            elif(crka == 'š'):
                name_surname += "\2"
            elif(crka == 'ž'):
                name_surname += "\3"
            elif(crka == ' '):
                name_surname += " "
            elif(crka >= 'a' and crka <= 'z'):
                name_surname += crka
            else:
                info_text.config(text="Neveljaven znak \""+crka+"\": " + bd[:15]+" ...")
                return
            index += 1
        name_surname = name_surname.strip()
        # Spliti name_surname na 2 dela (potem omogoči tudi da je več imen/priimkov)
        # + poglej ali je ime/priimek predolg
        date = bd[index:].strip().split(".")
        if(len(date) == 3 and len(name_surname) > 0):
            dan = int(date[0])
            mesec = int(date[1])
            leto = int(date[2])
            if(dan < 1 or dan > 31 or mesec < 1 or mesec > 12 or leto < FIRST_YEAR or leto > FIRST_YEAR+255):
                info_text.config(text="Napaka v datumu: " + bd[:15]+" ...")
                return
            encoded = (encode_text(name_surname))
            if(encoded is None):
                info_text.config(text="Predolgo ime: " + bd[:15]+" ...")
                return
            final.append((encoded, dan, mesec, leto-FIRST_YEAR))
        else:
            info_text.config(text="Napaka v formatu: " + bd[:15]+" ...")
            return

    ser.write(b'_') #clear buffer
    ser.write(b'writebd')
    #time.sleep(0.1)
    ser.write(str(len(final)).encode()) # number of bds
    ser.write(b'.')
    #time.sleep(0.1)
    for bd in final:
        ser.write(bd[0].encode()) #encoded name and surname
        ser.write(b'.')
        #time.sleep(0.1)
        send_text = "{:02d}{:02d}{:03d}".format(bd[1], bd[2], bd[3])
        ser.write(send_text.encode()) #day
        ser.write(b'.')
        time.sleep(0.05)
    while(1):
        response = ser.readline().strip().decode("utf-8")
        if(response != b""):
            break
    response = ser.readline().strip().decode("utf-8")
    info_text.config(text=response)

def cElementIndex(cEl):
    for i in range(CTABLE_LEN):
        if (cTable[i] == cEl):
            return i
    print("INDEX EROR")
    print(cEl)
    return -1

def encode_text(name_surname):
    name_surname_split = name_surname.split(" ")
    correct_split = []
    # Remove elements from extra spaces
    for i in name_surname_split:
        if i != '':
            correct_split.append(i)
    final_string = ""
    for title in correct_split:
        final_string += title + " "
    final_string = final_string.strip()
    if(len(final_string) > 40):
        return None
    final_string = final_string.ljust(40, '-')

    return final_string
#Encoding for future use
"""
    byPrev_bits = 0
    byBits = None
    cbOut = 0
    byCurr = None
    byStep = None
    byIndex = None

    return_array = [0] * 25

    i = 0
    while(i < 40):
        byBits = 0
        byCurr = 0
        byStep = 0
        if (byPrev_bits != 0):
            byIndex = cElementIndex(name_surname[i - 1])
            byCurr += ((byIndex << byPrev_bits) & 0x1F) << 3
            byBits += 5 - byPrev_bits
        if (byBits + 5 <= 8):
            byIndex = cElementIndex(name_surname[i])
            if (byPrev_bits != 0):
                byCurr += byIndex << (3 - (5 - byPrev_bits))
            else:
                byCurr += byIndex << 3
            byBits += 5
            byPrev_bits = 8 - byBits
            if (byPrev_bits != 0):
                byIndex = cElementIndex(name_surname[i + 1])
                byCurr += byIndex >> (5 - byPrev_bits)
            else:
                byStep = -1
        else:
            byStep = -1
            byPrev_bits = 8 - byBits
            byIndex = cElementIndex(name_surname[i])
            byCurr += byIndex >> (5 - byPrev_bits)

        return_array[cbOut] = byCurr
        cbOut += 1
        i += byStep
        i += 2
        print(i)
    print(return_array)
    return return_array
"""

def read_callback(ser, info_text, display_text_box):
    final_string = ""
    if ser is None:
        info_text.config(text="Koledar ni povezan!")
        return
    ser.write(b'_') #clear buffer
    ser.write(b'read')

    while(1):
        response = ser.readline().strip().decode("utf-8")
        if(response != b''):
            break
    try:
        kolicina = int(response)
    except:
        info_text.config(text="Napaka: izključi in priključi Koledar.")
        return
    for i in range(kolicina):
        while True:
            response = ser.readline().strip().decode("utf-8")
            if(response != b""):
                break
        
        response = response.replace('\1', 'č')
        response = response.replace('\2', 'š')
        response = response.replace('\3', 'ž')
        final_string += response + '\n'
    if(len(final_string) != 0):
        info_text.config(text="Prebrano iz Koledarja!")
        display_text_box.delete('1.0', END)
        display_text_box.insert(INSERT, final_string)
    else:
        info_text.config(text="Na Koledarju ni rojstnih dni.")

def confirm_select(text_body, dialog_icon="question"):
    return messagebox.askquestion("POZOR", text_body, icon=dialog_icon)

def color_select(object, current_color):
    if(current_color != "#000000" and current_color != "#FFFFFF"):
        colors = askcolor(current_color, title="Izberi barvo")
    else:
        colors = askcolor(title="Izberi barvo")
    return colors
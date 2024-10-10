from tkinter import *

from methods import *

from data import *

import os

def resource_path(relative_path):
    try:
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

def set_bg_color(element, curr_hex):
    global color_bg_rgb
    global color_bg_hex
    global text_bd_color_select
    colors = color_select(element, curr_hex)
    if(colors[0] is not None and colors[1] is not None):
        color_bg_rgb = colors[0]
        color_bg_hex = colors[1]
        text_bd_color_select.config(background=color_bg_hex)

def set_text_color(element, curr_hex):
    global color_text_rgb
    global color_text_hex
    global text_text_color_select
    colors = color_select(element, curr_hex)
    if(colors[0] is not None and colors[1] is not None):
        color_text_rgb = colors[0]
        color_text_hex = colors[1]
        text_text_color_select.config(background=color_text_hex)

def find_arduino_set_serial():
    global ser
    global btn_connect
    global COLOR_GREEN
    global COLOR_RED
    global info_text
    ser = find_arduino(ser, btn_connect,(COLOR_GREEN, COLOR_RED), info_text)

def save_profile(text_body, dialog_icon="info"):
    global ser
    global info_text
    if(ser is None):
        info_text.config(text="Arduino ni povezan!")
        return
    if(confirm_select(text_body, dialog_icon) == "yes"):
        global color_bg_rgb
        global color_text_rgb
        global slider_brightness
        callback_prikazi(ser, color_bg_rgb[0],color_bg_rgb[1],color_bg_rgb[2],
                            color_text_rgb[0],color_text_rgb[1],color_text_rgb[2],
                            slider_brightness.get(),info_text, True)

def reset_data(text_body, dialog_icon="info"):
    global ser
    global info_text
    if(ser is None):
        info_text.config(text="Arduino ni povezan!")
        return
    if(confirm_select(text_body, dialog_icon) == "yes"):
        callback_prikazi(ser, 0, 0, 0, 255, 255, 255, 100, info_text, True, True)

def write_bds(text_body, dialog_icon="info"):
    global ser
    global info_text
    if(ser is None):
        info_text.config(text="Arduino ni povezan!")
        return
    if(confirm_select(text_body, dialog_icon) == "yes"):
        global input_bd
        write_BD_callback(ser, info_text, input_bd.get(1.0, "end-1c"))

root = Tk()

root.geometry(str(SCREEN_W)+"x"+str(SCREEN_H))
root.configure(bg=COLOR_BG)
root.title("KOLEDAR GUI 2.24")
root.option_add("*Background", COLOR_BG)
photo = PhotoImage(file = resource_path("./icon.png"))
root.iconphoto(False, photo)


# ---------------- TOP ----------------

title = Text(root, foreground="white", background=COLOR_BG, font=("Monospace", 40, "bold"), height=1,	relief=FLAT,
highlightthickness=0)
title.insert(INSERT, "KOLEDAR GUI v2.24")
title.config(state=DISABLED)
title.place(x=10,y=10)

info_text = Label(root, foreground="white", background=COLOR_BG, font=("Monospace", 15),
relief=FLAT, highlightthickness=0, justify="right", text="Informacije o prenosu se bodo izpisale tukaj...")
info_text.place(x=SCREEN_W-650,y=45, width=640, height=30)

START = 75


# ---------------- FRAME ----------------

left_frame = Frame(root)
left_frame.configure(bg=COLOR_BG)
#left_frame.pack(side=LEFT, fill="both", expand=True)
left_frame.place(x=0,y=START, width=LEFT_W, height=SCREEN_H)
left_frame.propagate = False

right_frame = Frame(root)
right_frame.configure(bg=COLOR_BG)
#right_frame.pack(side=RIGHT, fill="both", expand=True)
right_frame.place(x=LEFT_W,y=START, width=RIGHT_W, height=SCREEN_H)
right_frame.propagate = False


# ---------------- LEFT SIDE ----------------

btn_connect = Button(root, text="POVEŽI SE", justify=CENTER, background=COLOR_RED,
relief=STYLE_BTN_DEF, font=FONT_BTN_BIG, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: find_arduino_set_serial())
btn_connect.place(x=10,y=START+10, width=LEFT_W-20, height=60)

btn_read = Button(root, text="PREBERI", justify=CENTER, background=COLOR_BTN_DEF,
relief=STYLE_BTN_DEF, font=FONT_BTN_BIG, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: read_callback(ser, info_text, input_bd))
btn_read.place(x=10,y=START+10+60+10, width=LEFT_W/2-20, height=60)

btn_write = Button(root, text="VPIŠI", justify=CENTER, background=COLOR_BTN_DEF,
relief=STYLE_BTN_DEF, font=FONT_BTN_BIG, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: write_bds("Ali ste prepričani, da želite vnesti nove rojstne dneve in izbrisati obstoječe podatke?"))
btn_write.place(x=10 + LEFT_W/2 - 20 + 20,y=START+10+60+10, width=LEFT_W/2-20, height=60)

input_bd = Text(root, foreground=COLOR_TEXT_INPUT_DEF, background=COLOR_INPUT_DEF, font=("Monospace", 20),
relief=STYLE_TEXT_INPUT_DEF, highlightthickness=0, padx=10, pady=10)
input_bd.insert(INSERT, "Vpiši rojstne dneve tukaj...")
#input_bd.insert(INSERT, "")
input_bd.place(x=10,y=START+10+60+10+60+10, width=LEFT_W-20, height=SCREEN_H-(START+10+60+10+60+10)-10)


# ---------------- RIGHT SIDE ----------------

text_bd_color_select = Text(root, background=color_bg_hex, relief=GROOVE, highlightthickness=0)
text_bd_color_select.config(state=DISABLED)
text_bd_color_select.place(x=LEFT_W+10,y=START+10, width=RIGHT_W-20, height=125)

top = START + 10 + 125

btn_bg_color_secect = Button(root, text="IZBERI BARVO OZADJA", justify=CENTER,
background=COLOR_BTN_DEF, relief=STYLE_BTN_DEF, font=FONT_BTN_SMALL, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: set_bg_color(text_bd_color_select, color_bg_hex))
btn_bg_color_secect.place(x=LEFT_W+10,y=top+10, width=RIGHT_W-20, height=60)

top = top + 10 + 60

text_text_color_select = Text(root, background=color_text_hex, relief=GROOVE,
highlightthickness=0)
text_text_color_select.config(state=DISABLED)
text_text_color_select.place(x=LEFT_W+10,y=top+10, width=RIGHT_W-20, height=125)

top = top + 10 + 125

btn_text_color_select = Button(root, text="IZBERI BARVO TEKSTA", justify=CENTER,
background=COLOR_BTN_DEF, relief=STYLE_BTN_DEF, font=FONT_BTN_SMALL, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: set_text_color(text_text_color_select, color_text_hex))
btn_text_color_select.place(x=LEFT_W+10,y=top+10, width=RIGHT_W-20, height=60)

top = top + 10 + 60

slider_brightness = Scale(root, from_=20, to=100, orient=HORIZONTAL, label="SVETLOST ZASLONA",
resolution=10, background=COLOR_BTN_DEF,highlightthickness=0, relief=STYLE_BTN_DEF, font=FONT_BTN_SMALL,
foreground=COLOR_TEXT_BTN_DEF)
slider_brightness.set(100)
slider_brightness.place(x=LEFT_W+10, y=top+10, width=RIGHT_W-20, height=75)

top = top+10+75

btn_show_display_profile = Button(root, text="PRIKAŽI\nPROFIL",
justify=CENTER, background=COLOR_BTN_DEF, relief=STYLE_BTN_DEF, font=FONT_BTN_BIG, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: callback_prikazi(ser, color_bg_rgb[0],color_bg_rgb[1],color_bg_rgb[2],
color_text_rgb[0],color_text_rgb[1],color_text_rgb[2], slider_brightness.get(),info_text, False))
btn_show_display_profile.place(x=LEFT_W+10,y=top+10, width=RIGHT_W/2-15, height=150)

btn_save_profile = Button(root, text="VPIŠI\nPROFIL",
justify=CENTER, background=COLOR_BTN_DEF, relief=STYLE_BTN_DEF, font=FONT_BTN_BIG, foreground=COLOR_TEXT_BTN_DEF,
command=lambda: save_profile("Ali ste prepričani, da želite vnesti nove barve in svetlost in izbrisati obstoječe podatke?"))
btn_save_profile.place(x=LEFT_W+10+RIGHT_W/2-15+10,y=top+10, width=RIGHT_W/2-15, height=150)

top = top+10+150

btn_clear_settings = Button(root, text="IZBRIŠI NASTAVITVE IN ROJSTNE DNEVE",
justify=CENTER, background=COLOR_RED, relief=STYLE_BTN_DEF,
font=("Monospace", 10, "bold"), foreground=COLOR_TEXT_BTN_DEF,
command=lambda: reset_data("Ali ste prepričani, da želite izbrisati vse nastavitve in rojstne dneve?", "warning"))
btn_clear_settings.place(x=LEFT_W+10,y=top+10, width=RIGHT_W-20, height=50)

root.resizable(False,False)
root.mainloop()

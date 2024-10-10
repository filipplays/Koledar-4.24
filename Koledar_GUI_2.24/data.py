from tkinter import *

SCREEN_W = 1200
SCREEN_H = 800

COLS = 50
ROWS = 50

LEFT_W = int(0.7*SCREEN_W) #840
RIGHT_W = SCREEN_W - LEFT_W #360
#2e2e2e
COLOR_BG = "#252526"
COLOR_RED = "#ff3131"
COLOR_GREEN = "#31ff31"
COLOR_BTN_DEF = "#404040"
COLOR_INPUT_DEF = "#404040"

COLOR_TEXT_BTN_DEF = "#FFFFFF"
COLOR_TEXT_INPUT_DEF = "#FFFFFF"

STYLE_BTN_DEF = RIDGE
STYLE_TEXT_INPUT_DEF = RIDGE
FONT_BTN_BIG = ("Monospace", 30, "bold")
FONT_BTN_SMALL = ("Monospace", 15, "bold")

color_bg_rgb = (0,0,0)
color_bg_hex = "#000000"

color_text_rgb = (255,255,255)
color_text_hex = "#FFFFFF"

btn_connect = None
slider_brightness = None

ser = None
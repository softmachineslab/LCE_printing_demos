# Michael Vinciguerra
# 11/5/21
# decode_morse.py
# Python script for reading the data from Arduino during execution
# of IR Breakbeam demo with LCEs. For better visualization purposes.
# Majority of actual decoding occurs in .ino file.
#********************************************************************

# Go to https://pythonforundergradengineers.com/python-arduino-potentiometer.html
# for more details on how this works if anything is confusing.

# Make sure the appropriate libraries are installed in Anaconda Prompt
# before running this script
# https://docs.python.org/3/library/tkinter.html
import tkinter
import serial # PySerial Library v3.5
import time # for delaying data collection
from tkinter import * # Library for GUI
from tkinter import ttk # Provides modern themed widget set and API

# make sure the 'COM#' is set according to the Arduino we want to read from
# on the Windows Device Manager
port = 'COM7'
brate = 9800 # baud rate for serial communication
ser = serial.Serial(port, brate, timeout=None) # no timeout
time.sleep(2) # wait 2 seconds

# Instantiate the GUI
root = Tk()
root.title("Decoding Morse Code...")

# Add a scroll bar for longer messages
# https://python-course.eu/tkinter_text_widget.php
# Problem - Scrollbar requires use of mainloop() because of user interaction with window, so ignore this
#scroll = Scrollbar(root)
#scroll.pack(side=RIGHT, fill=Y)

# Instantiate our message holders
# This line must come AFTER we instantiate the root window above
# This is because not setting the "container" attribute in the below calls
# will cause them to default to the root window, so if the root window
# doesn't exist, then these calls will error out!
# Once you instantiate these, we can update the displays with "set()"
# DEPRECATED: only used in the 'Label' widgets
#above = StringVar()
#side = StringVar()

# Instantiate our two frames with no text in either of them
# Labels vs. Text widgets - Label widget does not allow for use of Scrollbar easily, and Text is more easily updateable
#frame_above = Text(root, textvariable=above, bg="#CC002B", fg="black", padx=30, pady=30, font='Times_New_Roman', justify=LEFT, width=50)
#frame_above.grid(row=0, column=0)
frame_above = Text(root, height=10, width=50, bg="#CC002B", fg="black", font='Times_New_Roman')
frame_above.pack() # To display immediately

#frame_side = Text(root, textvariable=side, bg='#12DC00', padx=30, pady=30, font='Times_New_Roman', width=50)
#frame_side.grid(row = 0, column=1)
frame_side = Text(root, height=10, width=50, bg='#12DC00', font='Times_New_Roman')
frame_side.pack() # To display immediately

# Finish setting up the scroll bar
# Deprecated - no use of Scrollbar any more
#scroll.config(command=frame_above.yview)
#frame_above.config(yscrollcommand=scroll.set)

#root.mainloop() # Displays text, then blocks for user input (not good)
root.update() # Displays changes to window, then allows program to continue (needed to update text)

# Keep receiving data while Arduino is open
# Either close the COM port or unplug the Arduino to gracefully exit this program
while (ser.is_open):
    line = None
    try:
        line = ser.readline() # read line from Serial Monitor output
    except:
        break # If we error out, just close the port and exit the program

    if line:
        txt = line.decode() # convert the byte string to a unicode string
        txt = txt.strip() # removes the '\r\n' from end of string

        # Values will be output as "a/s,<value>"
        # a = above sensor, s = side sensor
        # We are going to split the output according to which sensor is
        # receiving data, then print out the value
        vals = txt.split(',')

        # Replace troublesome characters that couldn't be handled by Arduino (spaces)
        if (vals[1] == '-'):
            vals[1] = ' ' 

        if (vals[0] == 'a'):
            # Print value to appropriate display
            # DEPRECATED - Used for 'Label' Widget
            #above_text = above.get() # Get previous text to add onto
            #above.set(above_text + vals[1])

            frame_above.insert(END, vals[1]) # inserts at end of text
        else:
            # Print value to appropriate display
            # DEPRECATED - Used for 'Label' Widget
            #side_text = side.get() # Get previous text to add onto
            #side.set(side_text + vals[1])

            frame_side.insert(END, vals[1]) # inserts at end of text
    
    root.update()

ser.cBeautilose() # close the serial port to prevent any resource issues
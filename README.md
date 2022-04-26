# Multi-Material Printing of Liquid Crystal Elastomers with Integrated Stretchable Electronics

## Infrared (IR) Communication Demonstration Code

### Michael R. Vinciguerra, Dinesh K. Patel, Wuzhou Zu, Carmel Majidi, Lining Yao

To run the code, you are going to need two Arduino boards. Both can be connected to the same computer, but the one running the decoding file needs to be the one communicating with the Arduino IDE. Make sure you are using Python3.

Make sure there is a resistor in series with the IR LED to ensure that the forward voltage and current requirements are met. [Here are the specifications](http://www.qt-brightek.com/datasheet/QBLP613-IR1.pdf) of the LED used in the original study. Here are the [IR Breakbeam sensors](https://www.adafruit.com/product/2168?gclid=CjwKCAjwsJ6TBhAIEiwAfl4TWDnGmqlhaK2zbcryaZW5WscjHcbEf1iTkGCxFPaxC6S_h1dksP4xiBoC9yUQAvD_BwE) used as receivers. If you are confused as to how to use the breakbeam sensors, follow the tutorial that is on the product page.

To run the display code, you can follow [this tutorial](https://pythonforundergradengineers.com/python-arduino-potentiometer.html). It makes use of the `tkinter` Python library to capture and display the information from the serial monitor.

If you want to change the message, you can directly edit the message in the `IR_Morse.ino` file.

import serial
import time

from ST7567Controller import ST7567Controller
from icon_parser import parse_header_bitmaps

SERIAL_PORT = 'COM3'

ALIGN_LEFT = 0
ALIGN_RIGHT = -1
ALIGN_CENTER = -2

SET = 1
RESET = 0
INVERT = 2


if __name__ == "__main__":
    # Replace 'COM3' with '/dev/ttyUSB0' or similar on Linux/Mac
    try:
        icons = parse_header_bitmaps("python_icon.h")
        python_icon = icons['python_icon']

        lcd = ST7567Controller(port=SERIAL_PORT, baudrate=115200)
        lcd.cls()
        lcd.setFont(0)
        lcd.printStr(ALIGN_CENTER, 10, "Hello from Python!", SET)
        lcd.drawRectD(0,0,128,64,1)
        lcd.drawRect(4,5,127-4*2,17,1)
        print("Sending picture...")
        lcd.drawBitmap(python_icon['payload'], ALIGN_CENTER, 26, python_icon['w'], python_icon['h'])
        lcd.display()

        lcd.close()
        print("UI successfully sent to STM32!")
        
    except Exception as e:
        print(f"Error connecting to serial port: {e}")
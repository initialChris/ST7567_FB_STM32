import serial
import time

from ST7567Controller import ST7567Controller

ALIGN_LEFT = 0
ALIGN_RIGHT = -1
ALIGN_CENTER = -2

SET = 1
RESET = 0
INVERT = 2
# ==========================================
# TEST SCRIPT FOR DSX80 CONTROLLER UI
# ==========================================

def drawPowerBar(lcd : ST7567Controller, start_x : int, start_y : int, w : int, h : int, gap : int, count : int, current_pwr : int):
    for i in range(count):
        x = start_x + (i * (w + gap))
        
        if i < current_pwr:
            # Full solid block for active power
            lcd.fill_rect(x, start_y, w, h, SET)
        else:
            # "Open" segment: Top, Bottom, and Right side only
            # Top line
            lcd.draw_hline(x, x + w - 1, start_y, 1)
            # Bottom line
            lcd.draw_hline(x, x + w - 1, start_y + h - 1, 1)
            # Right vertical line
            lcd.draw_vline(x + w - 1, start_y, start_y + h - 1, 1)
            
            # Optional: Ensure the inside and left side are clear (color 0)
            # lcd.draw_vline(x, start_y, start_y + h - 1, 0)

def drawUi(lcd : ST7567Controller):
    lcd.clear()
    lcd.draw_line(0, 9, 127, 9)
    lcd.draw_line(59,9,59,52)
    lcd.draw_line(59,36,127,36)
    lcd.draw_line(0, 52, 127, 52)

    lcd.set_font(0)
    lcd.print_str(1, 1, "DSX80", 2)
    lcd.print_str(-1, 1,"10:22", 2)

    lcd.set_font(3)
    lcd.print_str(4, 13, "260;", 2)

    lcd.set_font(0)
    lcd.print_str(5, 41, "ACTUAL")   

    lcd.print_str(65, 13, "SET:")
    lcd.print_str(64, 25, "AMB:")
    lcd.print_str(-1, 13, "270 C")
    lcd.print_str(-1, 25, "32 C")
    lcd.draw_circle(119,13, 1, 1) 
    lcd.draw_circle(119,25, 1, 1) 

    lcd.fill_rect(60,37,128,15)
    lcd.print_str(76, 41, "SLEEP", INVERT)
    lcd.print_str(0, 56, "POW:")
    lcd.print_str(104, 56, "80W")

    for current_pwr in range (0, 11):
        drawPowerBar(lcd, 28, 56, 5, 7, 2, 10, current_pwr)
        lcd.update()
        time.sleep(0.04)        

def drawMenu(lcd : ST7567Controller, index : int):
    lcd.clear()
    #lcd.fill_rect(0,0,128,9)
    lcd.draw_rect(0,0,128,11)


    lcd.set_font(0)
    lcd.print_str(ALIGN_CENTER, 2, "Settings", INVERT)
    if (index >= 0 and index < 5):
        lcd.print_str(1, 14, "Idle Temperature")
        lcd.print_str(1, 24, "Idle timeout")
        lcd.print_str(1, 34, "Lcd Brightness")
        lcd.print_str(1, 44, "Ambient mode")
        lcd.print_str(1, 54, "PID Calibration")
    elif (index >= 5 and index < 10):
        lcd.print_str(1, 14, "Entry 6")
        lcd.print_str(1, 24, "Entry 7")
        lcd.print_str(1, 34, "Entry 8")
        lcd.print_str(1, 44, "Entry 9")
        lcd.print_str(1, 54, "Entry 10")

    padding = 5  # pixel di margine dalle punte della linea
    num_entries = 10
    y_start = 13
    y_end = 62
    y_min = y_start + padding
    y_max = y_end - padding

    step = (y_max - y_min) / (num_entries - 1)
    y_pos = int(y_min + (step * index))

    lcd.draw_line(125, y_start, 125, y_end) # La barra fissa
    lcd.fill_circle(125, y_pos, 2)           # Il pallino mobile
    
    lcd.fill_rect(0,13 + 10*(index%5),120,9, INVERT)
    time.sleep(0.2)
    lcd.update()

if __name__ == "__main__":
    # Replace 'COM3' with '/dev/ttyUSB0' or similar on Linux/Mac
    try:
        lcd = ST7567Controller(port='COM3', baudrate=115200)
        #drawUi(lcd)
        for i in range(0, 10):
            drawMenu(lcd, i)
            time.sleep(0.2)

        # Send the buffer to the display
        
        print("UI successfully sent to STM32!")
        lcd.update()
        lcd.close()
        
    except Exception as e:
        print(f"Error connecting to serial port: {e}")
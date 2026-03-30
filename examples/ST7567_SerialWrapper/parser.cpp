#include <ST7567_FB.h>

#include "fonts/fonts_all.h"


#include "serial.h"
#include "commands.h"

#define MAX_STR_LEN 64
#define BITMAP_BUFFER_SIZE 1024

uint8_t globalBitmapBuffer[BITMAP_BUFFER_SIZE];

void parseSerialCommand(ST7567_FB &lcd) {
    uint8_t cmdByte = serialRead();
    const CommandInfo* currentCmd = nullptr;

    // Read command identifier
    for (uint8_t i = 0; i < COMMAND_MAP_SIZE; i++) {
        if (COMMAND_MAP[i].id == cmdByte) {
            currentCmd = &COMMAND_MAP[i];
            break;
        }
    }

    if (currentCmd == nullptr) return; // If not valid command, ignore

    // 2. Read fixed parameters (up to 7 bytes)
    uint8_t buffer[16];
    if (currentCmd->argCount > 0) {
        if (!serialReadArgs(buffer, currentCmd->argCount)) return; // Serial error or timeout
    }

    // 3. Esecuzione
    switch (currentCmd->id) {
        // --- SYSTEM ---
        case CMD_INIT:          lcd.init(buffer[0]); break;
        case CMD_DISPLAY:       lcd.display(); break;
        case CMD_CLS:           lcd.cls(); break;
        case CMD_SLEEP:         lcd.sleep(buffer[0] > 0); break;
        case CMD_SET_CONTRAST:  lcd.setContrast(buffer[0]); break;
        case CMD_SET_SCROLL:    lcd.setScroll(buffer[0]); break;
        case CMD_INVERT:        lcd.displayInvert(buffer[0] > 0); break;
        case CMD_DISPLAY_ON:    lcd.displayOn(buffer[0] > 0); break;
        case CMD_DISPLAY_MODE:  lcd.displayMode(buffer[0]); break;
        case CMD_SET_ROTATION:  lcd.setRotation(buffer[0]); break;

        // --- DRAWING ---
        case CMD_DRAW_PIXEL:    lcd.drawPixel(buffer[0], buffer[1], buffer[2]); break;
        case CMD_DRAW_LINE:     lcd.drawLine((int8_t)buffer[0], (int8_t)buffer[1], (int8_t)buffer[2], (int8_t)buffer[3], buffer[4]); break;
        case CMD_DRAW_LINE_H:   lcd.drawLineH(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_DRAW_LINE_V:   lcd.drawLineV(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_DRAW_LINE_VFAST: lcd.drawLineVfast(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_DRAW_LINE_HFAST: lcd.drawLineHfast(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_DRAW_LINE_VFAST_D: lcd.drawLineVfastD(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_DRAW_LINE_HFAST_D: lcd.drawLineHfastD(buffer[0], buffer[1], buffer[2], buffer[3]); break;

        // --- SHAPES ---
        case CMD_DRAW_RECT:     lcd.drawRect(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]); break;
        case CMD_FILL_RECT:     lcd.fillRect(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]); break;
        case CMD_DRAW_CIRCLE:   lcd.drawCircle(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_FILL_CIRCLE:   lcd.fillCircle(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        
        case CMD_DRAW_TRIANGLE: 
            lcd.drawTriangle((int8_t)buffer[0], (int8_t)buffer[1], (int8_t)buffer[2], (int8_t)buffer[3], (int8_t)buffer[4], (int8_t)buffer[5], buffer[6]); 
            break;
        case CMD_FILL_TRIANGLE: 
            lcd.fillTriangle((int8_t)buffer[0], (int8_t)buffer[1], (int8_t)buffer[2], (int8_t)buffer[3], (int8_t)buffer[4], (int8_t)buffer[5], buffer[6]); 
            break;
        
        case CMD_DRAW_RECT_D:   lcd.drawRectD(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]); break;
        case CMD_FILL_RECT_D:   lcd.fillRectD(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]); break;
        case CMD_FILL_CIRCLE_D: lcd.fillCircleD(buffer[0], buffer[1], buffer[2], buffer[3]); break;
        case CMD_FILL_TRIANGLE_D: 
            lcd.fillTriangleD((int8_t)buffer[0], (int8_t)buffer[1], (int8_t)buffer[2], (int8_t)buffer[3], (int8_t)buffer[4], (int8_t)buffer[5], buffer[6]); 
            break;

        // --- TEXT ---
        case CMD_PRINT_CHAR:    lcd.printChar((int16_t)buffer[0], (int16_t)buffer[1], buffer[2], buffer[3]); break;
        case CMD_SET_CR:        lcd.setCR(buffer[0]); break;
        case CMD_SET_INV_CH:    lcd.setInvert(buffer[0]); break;
        case CMD_SET_MIN_WD:    lcd.setFontMinWd(buffer[0]); break;

        case CMD_PRINT_STR: {
            // Buffer contains: [x, y, col, len]
            uint8_t len = buffer[3];   
            if (len > 0 && len < MAX_STR_LEN) {
                char str[MAX_STR_LEN];
                if (serialReadArgs((uint8_t*)str, len)) { // Read the string
                    str[len] = '\0';
                    lcd.printStr((int8_t)buffer[0], (int8_t)buffer[1], str, buffer[2]);
                }
            }
            break;
        }

        // --- ADVANCED ---
        case CMD_SET_DITHER:    lcd.setDither((int8_t)buffer[0]); break;
        

        case CMD_SET_FONT:     
             // Fonts are selected by index  
            switch (buffer[0]) {
                case 0: lcd.setFont(c64enh); break;
                case 1: lcd.setFont(amstrad); break;
                case 2: lcd.setFont(term11x16); break;
                // Insert here case for other fonts
                default: lcd.setFont(c64enh); break;
            }
            break;

        case CMD_DRAW_BITMAP: { 
            uint8_t w = buffer[2];
            uint8_t h = buffer[3];
            
            uint16_t requiredSize = w * ((h + 7) / 8);

            if (requiredSize > 0 && requiredSize <= BITMAP_BUFFER_SIZE) {
                if (serialReadArgs(globalBitmapBuffer, requiredSize, 1000)) {
                    lcd.drawBitmap(globalBitmapBuffer, (int8_t)buffer[0], (int8_t)buffer[1], w, h);
                }
            } else {
                
                uint8_t trash;
                serialReadArgs(&trash, requiredSize, 1000); 
            }
            break;
        }
        
    }
}
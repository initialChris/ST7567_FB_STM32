#include <ST7567_FB.h>

#include "fonts/fonts_all.h"


#include "serial.h"

#define MAX_STR_LEN 64

extern ST7567_FB lcd;

void parseSerialCommand()
{

    uint8_t cmd = serialRead();
    uint8_t buffer[16]; // Buffer for arguments

    switch(cmd) {

        // --- DISPLAY CONTROL ---

        case 'C': // Clear screen
            lcd.cls();
            break;

        case 'D': // Update display (Send buffer via SPI)
            lcd.display();
            break;

        case 'K': // Set Contrast (1 byte: val)
            if (serialReadArgs(buffer, 1)) {
                lcd.setContrast(buffer[0]);
            }
            break;

        case 'N': // Display Invert (1 byte: 0 or 1)
            if (serialReadArgs(buffer, 1)) {
                lcd.displayInvert(buffer[0] > 0);
            }
            break;

        // --- BASIC DRAWING ---

        case 'P': // Draw Pixel (3 bytes: x, y, color)
            if (serialReadArgs(buffer, 3)) {
                lcd.drawPixel(buffer[0], buffer[1], buffer[2]);
            }
            break;

        case 'L': // Draw Line (5 bytes: x0, y0, x1, y1, color)
            if (serialReadArgs(buffer, 5)) {
                lcd.drawLine((int8_t)buffer[0], (int8_t)buffer[1],
                             (int8_t)buffer[2], (int8_t)buffer[3], buffer[4]);
            }
            break;

        case 'H': // Draw Horizontal Line (4 bytes: x0, x1, y, color)
            if (serialReadArgs(buffer, 4)) {
                lcd.drawLineH(buffer[0], buffer[1], buffer[2], buffer[3]);
            }
            break;

        case 'V': // Draw Vertical Line (4 bytes: x, y0, y1, color)
            if (serialReadArgs(buffer, 4)) {
                lcd.drawLineV(buffer[0], buffer[1], buffer[2], buffer[3]);
            }
            break;

        // --- SHAPES ---

        case 'R': // Draw Rectangle (5 bytes: x, y, w, h, color)
            if (serialReadArgs(buffer, 5)) {
                lcd.drawRect(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
            }
            break;

        case 'F': // Fill Rectangle (5 bytes: x, y, w, h, color)
            if (serialReadArgs(buffer, 5)) {
                lcd.fillRect(buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
            }
            break;

        case 'O': // Draw Circle (4 bytes: x, y, r, color)
            if (serialReadArgs(buffer, 4)) {
                lcd.drawCircle(buffer[0], buffer[1], buffer[2], buffer[3]);
            }
            break;

        case 'Q': // Fill Circle (4 bytes: x, y, r, color)
            if (serialReadArgs(buffer, 4)) {
                lcd.fillCircle(buffer[0], buffer[1], buffer[2], buffer[3]);
            }
            break;

        case 'A': // Draw Triangle (7 bytes: x0, y0, x1, y1, x2, y2, color)
            if (serialReadArgs(buffer, 7)) {
                lcd.drawTriangle((int16_t)buffer[0], (int16_t)buffer[1],
                                 (int16_t)buffer[2], (int16_t)buffer[3],
                                 (int16_t)buffer[4], (int16_t)buffer[5], buffer[6]);
            }
            break;

        case 'W': // Fill Triangle (7 bytes: x0, y0, x1, y1, x2, y2, color)
            if (serialReadArgs(buffer, 7)) {
                lcd.fillTriangle((int16_t)buffer[0], (int16_t)buffer[1],
                                 (int16_t)buffer[2], (int16_t)buffer[3],
                                 (int16_t)buffer[4], (int16_t)buffer[5], buffer[6]);
            }
            break;

        // --- TEXT ---

        case 'T': // Print String (3 bytes + N bytes: x, y, len, string_data)
            if (serialReadArgs(buffer, 4)) {
                uint8_t x = buffer[0];
                uint8_t y = buffer[1];
                uint8_t len = buffer[2];
                uint8_t col = buffer[3];

                if (len > 0 && len < MAX_STR_LEN) {
                    char str[MAX_STR_LEN];

                    // Read the actual string characters
                    if (serialReadArgs((uint8_t*)str, len)) {
                        str[len] = '\0'; // Null-terminate for C-string compatibility

                        // Handling negative coordinates (e.g. 255 -> -1 for right align)
                        int16_t posX = (x > 127) ? (int16_t)x - 256 : x;
                        int16_t posY = (y > 127) ? (int16_t)y - 256 : y;

                        lcd.printStr(posX, posY, str, col);
                    }
                }
            }
            break;

        case 'S': // Select Font (1 byte: index)
			if (serialReadArgs(buffer, 1)) {
				uint8_t fontIndex = buffer[0];

				switch(fontIndex) {
					case 0:
						lcd.setFont(c64enh); // Default/System font
						break;
					case 1:
						lcd.setFont(Seg7_12x21);
						break;
					case 2:
						lcd.setFont(Seg7_12x23b);
						break;
					case 3:
						lcd.setFont(Seg7_12x23);
						break;
					case 4:
						lcd.setFont(Seg7_12x31);
						break;
					case 5:
						lcd.setFont(Seg7_13x23b);
						break;
					case 6:
						lcd.setFont(Seg7_14x31b);
						break;
					case 7:
						lcd.setFont(Seg7_14x31);
						break;
					case 8:
						lcd.setFont(seg7_dig_12x32);
						break;
					case 9:
						lcd.setFont(seg7_dig_15x32);
						break;
					case 10:
						lcd.setFont(Seg7f_12x23);
						break;

					default:
						lcd.setFont(c64enh); // Default safety
						break;
				}
			}
			break;
    }
}

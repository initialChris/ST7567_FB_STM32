# ST7567_FB - STM32 Port (C++)

**Fast SPI graphics library for ST7567 128x64 LCD with frame buffer.**

This is a fork of the original library created by [cbm80amiga](https://github.com/cbm80amiga/ST7567_FB).

If you want to support the original author: [https://www.paypal.me/cbm80amiga](https://www.paypal.me/cbm80amiga)

The included fonts are adapted from [PropFonts](https://github.com/cbm80amiga/PropFonts).

The library has been tested with the STM32F401RET and an ST7567 128x64 LCD (e.g., OPEN-SMART 2.6" display module).

### Hardware Configuration
The library utilizes the **STM32 HAL** and requires the SPI peripheral to be configured as follows:
- **Mode:** Half-Duplex Master (or Full-Duplex Master).
- **Clock Speed:** The SPI baudrate should be set via prescaler to **not exceed 20MHz**.
- **Data Size:** 8 Bits.
- **MSB First**.

### Constructor
To initialize the display, pass the SPI handler and the GPIO pins for DC, CS, and Reset:

```cpp
ST7567_FB lcd(&hspi1, 
              LCD_DC_GPIO_Port, LCD_DC_Pin, 
              LCD_CS_GPIO_Port, LCD_CS_Pin, 
              LCD_RST_GPIO_Port, LCD_RST_Pin);
```

### Examples

Some of the original examples have been rewritten to work on the STM32 and are available in the `examples` directory.

The `tests/F401` folder contains a sandbox to test the examples on the **Nucleo F401RE** using the following pin mapping. You can use the presets contained in the `CMakePresets.json` file to build each individual example.


| Signal    | STM32 Pin | Arduino Header |
|:----------|:---------:|:--------------:|
| SPI1_SCK  | **PA5** | D13            |
| SPI1_MOSI | **PA7** | D11            |
| LCD_DC    | **PA6** | D12            |
| LCD_CS    | **PB6** | D10            |
| LCD_BCKL  | **PC7** | D9             |
| LCD_RST   | **PA9** | D8             |

The `examples/ST7567_SerialWrapper` is particularly useful for sending commands to the display via UART. This allows testing commands and drawing custom UIs in real-time with Python, avoiding the need to flash the firmware every time. It supports all commands described in the header using the same format, except for the `setFont` command which accepts the index of the font to use. The font list must be configured in `examples/ST7567_SerialWrapper/parser.cpp` within the following section:

```cpp
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
```

## Tools:
### Image converter
The `tools/image_converter/image_converter.py` helps to convert images into a format compatible with the ST7567 display. It requires the Pillow library to be installed.

### Font formatter
The `tools/font_formatter/formatter.py` script can be used to format the header files of the included fonts. It removes the `PROGMEM` keyword and casts the first negative number to `(uint8_t)` from the fonts contained in [PropFonts](https://github.com/cbm80amiga/PropFonts) repository.

### Font converter
TODO


## TODO
- [ ] Clean up the code (there is some inconsistency in the code style. Some sections should be declared private).
- [ ] Implement DMA support for non-blocking `display()` updates.
- [ ] Add the ability to send fonts via UART.
- [ ] Create the font generator tool.

## License
Refer to the original repository for licensing information.
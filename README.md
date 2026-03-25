# ST7567_FB - STM32 Port (C++)

**Fast SPI graphics library for ST7567 128x64 LCD with frame buffer.**

This is a port for **STM32 (HAL)** of the original library created by [cbm80amiga](https://github.com/cbm80amiga/ST7567_FB). It is designed for high-performance graphics on small monochrome displays using the STM32 ecosystem and C++.


## 🚀 Features
- **Frame Buffer:** All drawing operations are performed in RAM for maximum speed.
- **Proportional Fonts:** Built-in support (compatible with [PropFonts](https://github.com/cbm80amiga/PropFonts)).
- **Primitives:** - Pixels, lines, rectangles, circles, triangles (both outline and filled).
- **Optimized Drawing:** - Ultra-fast horizontal/vertical line drawing.
  - 17 ordered dithering patterns.
  - Bitmap drawing support.


## 🛠 Tested Hardware
- **MCU:** STM32F401RET6.
- **Display:** ST7567 128x64 LCD (e.g., OPEN-SMART 2.6" display module).

## 🔧 Implementation Details (STM32)

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

## 📋 TODO
- [ ] Clean up the code (there is some inconsistency in the code style. Some sections should be declared private).
- [ ] Implement **DMA** support for non-blocking `display()` updates.


## 📜 Original Credits
This library is a fork and port of the work by **cbm80amiga**. 

- **Original Repository:** [cbm80amiga/ST7567_FB](https://github.com/cbm80amiga/ST7567_FB)
- **Original YouTube Video:** [Watch here](https://youtu.be/YC-wqZnCGQ4)

If you find the original logic of this library useful and want to support the original author:
- **PayPal:** [https://www.paypal.me/cbm80amiga](https://www.paypal.me/cbm80amiga)



## License
Refer to the original repository for licensing information.
import serial
import time
MAX_STR_LEN = 64

class Cmd:
    # --- SYSTEM & CONTROL ---
    INIT            = 0x01
    DISPLAY         = 0x02
    CLS             = 0x03
    SLEEP           = 0x04
    SET_CONTRAST    = 0x05
    SET_SCROLL      = 0x06
    INVERT          = 0x07
    DISPLAY_ON      = 0x08
    DISPLAY_MODE    = 0x09
    SET_ROTATION    = 0x0A

    # --- BASIC DRAWING ---
    DRAW_PIXEL        = 0x10
    DRAW_LINE         = 0x11
    DRAW_LINE_H       = 0x12
    DRAW_LINE_V       = 0x13
    DRAW_LINE_VFAST   = 0x14
    DRAW_LINE_HFAST   = 0x15
    DRAW_LINE_VFAST_D = 0x16
    DRAW_LINE_HFAST_D = 0x17

    # --- SHAPES ---
    DRAW_RECT       = 0x20
    FILL_RECT       = 0x21
    DRAW_CIRCLE     = 0x22
    FILL_CIRCLE     = 0x23
    DRAW_TRIANGLE   = 0x24
    FILL_TRIANGLE   = 0x25
    DRAW_RECT_D     = 0x26
    FILL_RECT_D     = 0x27
    FILL_CIRCLE_D   = 0x28
    FILL_TRIANGLE_D = 0x29

    # --- TEXT & FONTS ---
    SET_FONT        = 0x30
    PRINT_CHAR      = 0x31
    PRINT_STR       = 0x32
    SET_CR          = 0x33
    SET_INV_CH      = 0x34
    SET_MIN_WD      = 0x35

    # --- ADVANCED & BITMAP ---
    SET_DITHER      = 0x40
    DRAW_BITMAP     = 0x41


class ST7567Controller:
    def __init__(self, port: str, baudrate: int = 115200):
        self.ser = serial.Serial(port, baudrate, timeout=1)
        # time.sleep(0.5) # If nucleo restart on serial open

    def _send_cmd(self, cmd_id: int, *args):
        payload = bytearray([cmd_id])
        for val in args:
            # bool are converted to 1/0
            if isinstance(val, bool):
                payload.append(1 if val else 0)
            else:
                # Handle negative numbers (-1 -> 255)
                payload.append(val & 0xFF)
        self.ser.write(payload)

    
    def _write_chunked(self, payload: bytearray, chunk_size: int = 64):
        # Avoid overflowing the STM32 UART buffer.
        for i in range(0, len(payload), chunk_size):
            chunk = payload[i:i+chunk_size]
            self.ser.write(chunk)
            self.ser.flush()  # Force the OS to send the buffer immediately
            time.sleep(0.01)

    # ==========================================
    # SYSTEM & CONTROL
    # ==========================================
    def init(self, contrast: int = 4):
        self._send_cmd(Cmd.INIT, contrast)

    def begin(self):
        self.init()

    def display(self):
        self._send_cmd(Cmd.DISPLAY)

    def cls(self):
        self._send_cmd(Cmd.CLS)

    def clearDisplay(self):
        self.cls()

    def sleep(self, mode: bool = True):
        self._send_cmd(Cmd.SLEEP, mode)

    def setContrast(self, val: int):
        self._send_cmd(Cmd.SET_CONTRAST, val)

    def setScroll(self, val: int):
        self._send_cmd(Cmd.SET_SCROLL, val)

    def displayInvert(self, mode: bool):
        self._send_cmd(Cmd.INVERT, mode)

    def displayOn(self, mode: bool):
        self._send_cmd(Cmd.DISPLAY_ON, mode)

    def displayMode(self, val: int):
        self._send_cmd(Cmd.DISPLAY_MODE, val)

    def setRotation(self, mode: int):
        self._send_cmd(Cmd.SET_ROTATION, mode)

    # ==========================================
    # BASIC DRAWING
    # ==========================================
    def drawPixel(self, x: int, y: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_PIXEL, x, y, col)

    def drawLine(self, x0: int, y0: int, x1: int, y1: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE, x0, y0, x1, y1, col)

    def drawLineH(self, x0: int, x1: int, y: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_H, x0, x1, y, col)

    def drawLineV(self, x: int, y0: int, y1: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_V, x, y0, y1, col)

    def drawLineVfast(self, x: int, y0: int, y1: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_VFAST, x, y0, y1, col)

    def drawLineHfast(self, x0: int, x1: int, y: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_HFAST, x0, x1, y, col)

    def drawLineVfastD(self, x: int, y0: int, y1: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_VFAST_D, x, y0, y1, col)
    
    def drawLineHfastD(self, x0: int, x1: int, y: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_LINE_HFAST_D, x0, x1, y, col)

    # ==========================================
    # SHAPES
    # ==========================================
    def drawRect(self, x0: int, y0: int, w: int, h: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_RECT, x0, y0, w, h, col)

    def fillRect(self, x0: int, y0: int, w: int, h: int, col: int = 1):
        self._send_cmd(Cmd.FILL_RECT, x0, y0, w, h, col)

    def drawCircle(self, x0: int, y0: int, radius: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_CIRCLE, x0, y0, radius, col)

    def fillCircle(self, x0: int, y0: int, r: int, col: int = 1):
        self._send_cmd(Cmd.FILL_CIRCLE, x0, y0, r, col)

    def drawTriangle(self, x0: int, y0: int, x1: int, y1: int, x2: int, y2: int, color: int = 1):
        self._send_cmd(Cmd.DRAW_TRIANGLE, x0, y0, x1, y1, x2, y2, color)

    def fillTriangle(self, x0: int, y0: int, x1: int, y1: int, x2: int, y2: int, color: int = 1):
        self._send_cmd(Cmd.FILL_TRIANGLE, x0, y0, x1, y1, x2, y2, color)

    def drawRectD(self, x0: int, y0: int, w: int, h: int, col: int = 1):
        self._send_cmd(Cmd.DRAW_RECT_D, x0, y0, w, h, col)

    def fillRectD(self, x0: int, y0: int, w: int, h: int, col: int = 1):
        self._send_cmd(Cmd.FILL_RECT_D, x0, y0, w, h, col)

    def fillCircleD(self, x0: int, y0: int, r: int, col: int = 1):
        self._send_cmd(Cmd.FILL_CIRCLE_D, x0, y0, r, col)

    def fillTriangleD(self, x0: int, y0: int, x1: int, y1: int, x2: int, y2: int, color: int = 1):
        self._send_cmd(Cmd.FILL_TRIANGLE_D, x0, y0, x1, y1, x2, y2, color)

    # ==========================================
    # TEXT & FONTS
    # ==========================================
    def setFont(self, f_index: int):
        self._send_cmd(Cmd.SET_FONT, f_index)

    def setCR(self, _cr: int):
        self._send_cmd(Cmd.SET_CR, _cr)

    def setInvert(self, _inv: int):
        self._send_cmd(Cmd.SET_INV_CH, _inv)

    def setFontMinWd(self, wd: int):
        self._send_cmd(Cmd.SET_MIN_WD, wd)

    def setCharMinWd(self, wd: int):
        self.setFontMinWd(wd)  # Alias per comodità

    def printChar(self, xpos: int, ypos: int, c: str, col: int = 1):
        char_val = ord(c[0]) if isinstance(c, str) else c
        self._send_cmd(Cmd.PRINT_CHAR, xpos, ypos, char_val, col)

    def printStr(self, xpos: int, ypos: int, text: str, col: int = 1):
        clean_text = text.encode('ascii', 'ignore')
        length = len(clean_text)
        
        # Limit string length based parser buffer size
        if length > MAX_STR_LEN - 1:
            clean_text = clean_text[:MAX_STR_LEN - 1]
            length = MAX_STR_LEN - 1
            
        payload = bytearray([Cmd.PRINT_STR, xpos & 0xFF, ypos & 0xFF, col & 0xFF, length])
        payload.extend(clean_text)
        
        self.ser.write(payload)

    # ==========================================
    # ADVANCED & BITMAP
    # ==========================================
    def setDither(self, s: int):
        self._send_cmd(Cmd.SET_DITHER, s)

    def drawBitmap(self, data: bytes, x: int, y: int, w: int, h: int):

        expected_size = w * ((h + 7) // 8)
        if len(data) != expected_size:
            raise ValueError(f"Data length mismatch. Expected {expected_size} bytes, got {len(data)}.")
            
        payload = bytearray([Cmd.DRAW_BITMAP, x & 0xFF, y & 0xFF, w & 0xFF, h & 0xFF])
        payload.extend(data)
        self._write_chunked(payload, 32)

    def close(self):
        self.ser.close()
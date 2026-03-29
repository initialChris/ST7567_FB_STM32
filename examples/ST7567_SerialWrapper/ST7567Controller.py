import serial
MAX_STR_LEN = 64
class ST7567Controller:
    
    def __init__(self, port: str, baudrate: int = 115200):
        # Initialize the serial connection
        self.ser = serial.Serial(port, baudrate, timeout=1)
        # Wait a moment for the STM32 to reset and initialize
        #time.sleep(0.5)

    def _send_cmd(self, cmd_char: str, *args):
        # Build the byte array starting with the command character
        payload = bytearray(cmd_char.encode('ascii'))
        for val in args:
            # Ensure values are within 8-bit unsigned range (0-255)
            # This automatically handles -1 as 255 (0xFF) using bitwise AND
            payload.append(val & 0xFF)
        self.ser.write(payload)
        #time.sleep(0.1)

    # --- DISPLAY CONTROL ---

    def clear(self):
        # Clear the frame buffer
        self.ser.write(b'C')
        #time.sleep(0.1)

    def update(self):
        # Send the frame buffer to the display via SPI/DMA
        self.ser.write(b'D')
        

    def set_contrast(self, value: int):
        # Set contrast level
        self._send_cmd('K', value)

    def invert_display(self, invert: bool):
        # Invert display colors (True/False)
        self._send_cmd('N', 1 if invert else 0)

    # --- BASIC DRAWING ---

    def draw_pixel(self, x: int, y: int, color: int = 1):
        self._send_cmd('P', x, y, color)

    def draw_line(self, x0: int, y0: int, x1: int, y1: int, color: int = 1):
        self._send_cmd('L', x0, y0, x1, y1, color)

    def draw_hline(self, x0: int, x1: int, y: int, color: int = 1):
        self._send_cmd('H', x0, x1, y, color)

    def draw_vline(self, x: int, y0: int, y1: int, color: int = 1):
        self._send_cmd('V', x, y0, y1, color)

    # --- SHAPES ---

    def draw_rect(self, x: int, y: int, w: int, h: int, color: int = 1):
        self._send_cmd('R', x, y, w, h, color)

    def fill_rect(self, x: int, y: int, w: int, h: int, color: int = 1):
        self._send_cmd('F', x, y, w, h, color)

    def draw_circle(self, x: int, y: int, r: int, color: int = 1):
        self._send_cmd('O', x, y, r, color)

    def fill_circle(self, x: int, y: int, r: int, color: int = 1):
        self._send_cmd('Q', x, y, r, color)

    def draw_triangle(self, x0: int, y0: int, x1: int, y1: int, x2: int, y2: int, color: int = 1):
        self._send_cmd('A', x0, y0, x1, y1, x2, y2, color)

    def fill_triangle(self, x0: int, y0: int, x1: int, y1: int, x2: int, y2: int, color: int = 1):
        self._send_cmd('W', x0, y0, x1, y1, x2, y2, color)

    # --- TEXT ---
    def set_font(self, font_index: int):
        """Sets the active font on the STM32 by index."""
        self._send_cmd('S', font_index)
        
    def print_str(self, x: int, y: int, text: str, color: int = 1):
        # Clean text to ascii to avoid serialization errors
        clean_text = text.encode('ascii', 'ignore')
        length = len(clean_text)
        
        # Limit string length to our C++ parser buffer size
        if length > MAX_STR_LEN-1:
            clean_text = clean_text[:MAX_STR_LEN-1]
            length = MAX_STR_LEN-1
            
        payload = bytearray(b'T')
        payload.append(x & 0xFF)
        payload.append(y & 0xFF)
        payload.append(length)
        payload.append(color & 0xFF)
        payload.extend(clean_text)
        
        self.ser.write(payload)

    def close(self):
        # Close the serial port connection
        self.ser.close()
import argparse
import os
import sys
from PIL import Image

def convertImageToSt7567Bytes(imagePath: str, invert: bool = False, threshold: int = 128) -> tuple[int, int, bytes]:
    try:
        # Open image, convert to grayscale
        img = Image.open(imagePath).convert('L')
    except FileNotFoundError:
        print(f"[ERROR] File '{imagePath}' not found.")
        sys.exit(1)

    width, height = img.size

    # Calcola l'altezza arrotondata al multiplo di 8 superiore (padding)
    # Es: se height è 45, (45 + 7) // 8 = 52 // 8 = 6 pagine. 6 * 8 = 48 pixel.
    padded_height = ((height + 7) // 8) * 8
    pages = padded_height // 8

    byte_data = bytearray()

    # Process image page by page (8 vertical pixels at a time)
    for page in range(pages):
        for x in range(width):
            column_byte = 0
            for y_offset in range(8):
                y = (page * 8) + y_offset
                
                # Leggiamo il pixel solo se siamo dentro i limiti reali dell'immagine
                if y < height:
                    # Get brightness (0 = black, 255 = white)
                    pixel_brightness = img.getpixel((x, y))
                    
                    # Check pixel intensity against threshold
                    if invert:
                        is_pixel_on = pixel_brightness >= threshold
                    else:
                        is_pixel_on = pixel_brightness < threshold
                else:
                    # Se y supera l'altezza reale (zona di padding), il pixel resta spento
                    is_pixel_on = False

                if is_pixel_on:
                    column_byte |= (1 << y_offset)
                    
            byte_data.append(column_byte)

    # Ritorniamo le dimensioni originali (non quelle paddate), ma l'array include il padding
    return width, height, bytes(byte_data)

def formatHexList(payload: bytes) -> list[str]:
    return [f"0x{b:02X}" for b in payload]

def generateHeaderFile(var_name: str, w: int, h: int, payload: bytes) -> str:
    filename = f"{var_name}.h"
    hex_list = formatHexList(payload)
    
    with open(filename, 'w') as f:
        f.write(f"/* Auto-generated ST7567 Bitmap: {w}x{h} */\n")
        f.write(f"#include <stdint.h>\n\n")
        f.write(f"const uint8_t {var_name} [] = {{ {w}, {h},\n")
        for i in range(0, len(hex_list), 16):
            chunk = ", ".join(hex_list[i:i+16])
            f.write(f"    {chunk},\n")
        f.write("};\n")
    return filename


def main():
    # Setup argparse (disable default add_help to free up '-h' flag)
    parser = argparse.ArgumentParser(description="Convert an image to ST7567 byte array format.", add_help=True)
    
    # Define positional arguments
    parser.add_argument("image_path", help="Path to the source image (e.g. icon.png)")
    parser.add_argument("var_name", help="Name for the output variable and files")
    
    # Define optional flags
    parser.add_argument("-s", dest="stats", action="store_true", help="Print only stats")
    parser.add_argument("-i", dest="invert", action="store_true", help="Invert image colors (black to white)")
    
    args = parser.parse_args()

    print("\n--- ST7567 Image Converter ---")
    print(f"[*] Processing file : {args.image_path}")
    print(f"[*] Variable name   : {args.var_name}")
    print(f"[*] Invert mode     : {'ON' if args.invert else 'OFF'}")

    # Process image
    w, h, payload = convertImageToSt7567Bytes(args.image_path, invert=args.invert)
    array_size = len(payload)

    print("\n--- Results ---")
    print(f"[+] Dimensions      : {w}x{h} pixels")
    print(f"[+] Payload size    : {array_size} bytes (plus 2 bytes metadata)")

    if not args.stats:
        h_file = generateHeaderFile(args.var_name, w, h, payload)
        print(f"[+] Created header  : {h_file}")
        

    print("------------------------------\n")

if __name__ == "__main__":
    main()
import os
import re
import glob

def process_header_files():
    """
    Finds all .h files in the current directory, removes 'PROGMEM',
    and casts the first negative number to (uint8_t).
    """
    # Get all .h files in the current working directory
    h_files = glob.glob('*.h')
    
    if not h_files:
        print("No .h files found in the current directory.")
        return

    for file_path in h_files:
        try:
            with open(file_path, 'r', encoding='utf-8') as file:
                content = file.read()
            
            # Remove 'PROGMEM' (checking with and without leading space to keep formatting clean)
            modified_content = content.replace(' PROGMEM', '')
            modified_content = modified_content.replace('#include "Arduino.h"', '')
            modified_content = modified_content.replace('PROGMEM', '')

            # Use regex to find the first negative number and prepend (uint8_t)
            # (?<!\(uint8_t\)) is a negative lookbehind: it prevents adding the cast
            # again if the script is executed multiple times on the same file.
            # (-\d+) matches the minus sign followed by digits.
            # count=1 ensures only the first occurrence in the file is replaced.
            modified_content = re.sub(r'(?<!\(uint8_t\))(-\d+)', r'(uint8_t)\1', modified_content, count=1)
            
            # Save the modified content back to the same file
            with open(file_path, 'w', encoding='utf-8') as file:
                file.write(modified_content)
                
            print(f"Successfully processed: {file_path}")
            
        except Exception as e:
            print(f"Error processing {file_path}: {e}")

if __name__ == "__main__":
    process_header_files()
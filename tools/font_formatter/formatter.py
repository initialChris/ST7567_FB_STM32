import os
import re
import glob

def process_header_files(input_dir='input', output_dir='output'):
    """
    Finds all .h files in the 'input' directory, removes 'PROGMEM' and Arduino includes,
    casts the first negative number to (uint8_t), and saves them to the 'output' directory.
    """

    # Create the output directory if it does not exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Search for all .h files specifically in the input directory
    search_pattern = os.path.join(input_dir, '*.h')
    h_files = glob.glob(search_pattern)
    
    if not h_files:
        print(f"No .h files found in the '{input_dir}' directory.")
        return

    for file_path in h_files:
        try:
            # Read the original file
            with open(file_path, 'r', encoding='utf-8') as file:
                content = file.read()
            
            # Remove 'PROGMEM' and Arduino.h includes
            modified_content = content.replace(' PROGMEM', '')
            modified_content = modified_content.replace('#include "Arduino.h"', '')
            modified_content = modified_content.replace('PROGMEM', '')

            # Use regex to find the first negative number and prepend (uint8_t)
            modified_content = re.sub(r'(?<!\(uint8_t\))(-\d+)', r'\t(uint8_t)\1', modified_content, count=1)
            
            # Extract just the filename from the path (e.g., 'input/font.h' -> 'font.h')
            base_name = os.path.basename(file_path)
            
            # Construct the new path for the output file
            output_path = os.path.join(output_dir, base_name)

            # Save the modified content to the output directory
            with open(output_path, 'w', encoding='utf-8') as file:
                file.write(modified_content)
                
            print(f"Successfully processed: {base_name} -> saved to {output_path}")
            
        except Exception as e:
            print(f"Error processing {file_path}: {e}")

if __name__ == "__main__":
    process_header_files()
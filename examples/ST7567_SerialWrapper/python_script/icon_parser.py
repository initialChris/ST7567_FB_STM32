import re
import os

def parse_header_bitmaps(filepath: str) -> dict:
    """
    Parses a C/C++ .h file to extract bitmap arrays.
    Returns a dict with name, width, height, and byte payload.
    """
    if not os.path.exists(filepath):
        raise FileNotFoundError(f"File not found: {filepath}")

    with open(filepath, 'r') as file:
        content = file.read()

    # Remove C/C++ comments to prevent false matches
    content = re.sub(r'//.*', '', content)
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

    # Match pattern: const uint8_t VAR_NAME [] = { DATA };
    pattern = re.compile(
        r'const\s+(?:uint8_t|unsigned\s+char)\s+(\w+)\s*\[.*?\]\s*=\s*\{([^}]*)\};', 
        re.MULTILINE | re.DOTALL
    )
    
    matches = pattern.findall(content)
    bitmaps = {}

    for var_name, data_string in matches:
        raw_items = data_string.split(',')
        values = []
        
        for item in raw_items:
            item = item.strip()
            if item:
                try:
                    # Parse string to int (auto-detects base 10/16)
                    values.append(int(item, 0))
                except ValueError:
                    pass # Ignore non-numeric artifacts
        
        # Check for at least width and height bytes
        if len(values) >= 2:
            w = values[0]
            h = values[1]
            payload = bytes(values[2:]) 
            
            bitmaps[var_name] = {
                'w': w,
                'h': h,
                'payload': payload
            }
            
    return bitmaps
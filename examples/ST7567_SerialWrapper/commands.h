#ifndef _COMMANDS_H
#define _COMMANDS_H

// Command IDs
enum CommandID : uint8_t {
    // --- SYSTEM & CONTROL (0x01 - 0x0F) ---
    CMD_INIT            = 0x01,
    CMD_DISPLAY         = 0x02,
    CMD_CLS             = 0x03,
    CMD_SLEEP           = 0x04,
    CMD_SET_CONTRAST    = 0x05,
    CMD_SET_SCROLL      = 0x06,
    CMD_INVERT          = 0x07,
    CMD_DISPLAY_ON      = 0x08,
    CMD_DISPLAY_MODE    = 0x09,
    CMD_SET_ROTATION    = 0x0A,

    // --- BASIC DRAWING (0x10 - 0x1F) ---
    CMD_DRAW_PIXEL        = 0x10,
    CMD_DRAW_LINE         = 0x11,
    CMD_DRAW_LINE_H       = 0x12,
    CMD_DRAW_LINE_V       = 0x13,
    CMD_DRAW_LINE_VFAST   = 0x14,
    CMD_DRAW_LINE_HFAST   = 0x15,
    CMD_DRAW_LINE_VFAST_D = 0x16,
    CMD_DRAW_LINE_HFAST_D = 0x17,

    // --- SHAPES (0x20 - 0x2F) ---
    CMD_DRAW_RECT       = 0x20,
    CMD_FILL_RECT       = 0x21,
    CMD_DRAW_CIRCLE     = 0x22,
    CMD_FILL_CIRCLE     = 0x23,
    CMD_DRAW_TRIANGLE   = 0x24,
    CMD_FILL_TRIANGLE   = 0x25,
    CMD_DRAW_RECT_D     = 0x26,
    CMD_FILL_RECT_D     = 0x27,
    CMD_FILL_CIRCLE_D   = 0x28,
    CMD_FILL_TRIANGLE_D = 0x29,

    // --- TEXT & FONTS (0x30 - 0x3F) ---
    CMD_SET_FONT        = 0x30,
    CMD_PRINT_CHAR      = 0x31,
    CMD_PRINT_STR       = 0x32, // Variable length
    CMD_SET_CR          = 0x33,
    CMD_SET_INV_CH      = 0x34,
    CMD_SET_MIN_WD      = 0x35,

    // --- ADVANCED & BITMAP (0x40 - 0x4F) ---
    CMD_SET_DITHER      = 0x40,
    CMD_DRAW_BITMAP     = 0x41  // Variable length
};


struct CommandInfo {
    CommandID id;
    uint8_t argCount; // Fixed number of bytes to read
};

// Command map (Allocated in Flash)
static const CommandInfo COMMAND_MAP[] = {
    { CMD_INIT,              1 }, // 1 byte: contrast
    { CMD_DISPLAY,           0 },
    { CMD_CLS,               0 },
    { CMD_SLEEP,             1 }, // 1 byte: mode (0/1)
    { CMD_SET_CONTRAST,      1 },
    { CMD_SET_SCROLL,        1 },
    { CMD_INVERT,            1 },
    { CMD_DISPLAY_ON,        1 },
    { CMD_DISPLAY_MODE,      1 },
    { CMD_SET_ROTATION,      1 },
  
    { CMD_DRAW_PIXEL,        3 }, // x, y, col
    { CMD_DRAW_LINE,         5 }, // x0, y0, x1, y1, col
    { CMD_DRAW_LINE_H,       4 }, // x0, x1, y, col
    { CMD_DRAW_LINE_V,       4 }, // x, y0, y1, col
    { CMD_DRAW_LINE_VFAST,   4 }, 
    { CMD_DRAW_LINE_HFAST,   4 }, 
    { CMD_DRAW_LINE_VFAST_D, 4 },
    { CMD_DRAW_LINE_HFAST_D, 4 },

    { CMD_DRAW_RECT,         5 }, // x, y, w, h, col
    { CMD_FILL_RECT,         5 },
    { CMD_DRAW_CIRCLE,       4 }, // x, y, r, col
    { CMD_FILL_CIRCLE,       4 },
    { CMD_DRAW_TRIANGLE,     7 }, // x0, y0, x1, y1, x2, y2, col
    { CMD_FILL_TRIANGLE,     7 },
    { CMD_DRAW_RECT_D,       5 },
    { CMD_FILL_RECT_D,       5 },
    { CMD_FILL_CIRCLE_D,     4 },
    { CMD_FILL_TRIANGLE_D,   7 },
  
    { CMD_SET_FONT,          1 }, // font index
    { CMD_PRINT_CHAR,        4 }, // x, y, char, col
    { CMD_PRINT_STR,         4 }, // x, y, col, len (variable cmd length)
    { CMD_SET_CR,            1 },
    { CMD_SET_INV_CH,        1 },
    { CMD_SET_MIN_WD,        1 },
  
    { CMD_SET_DITHER,        1 },
    { CMD_DRAW_BITMAP,       4 }  // x, y, w, h, (bitmap_data)
};

#define COMMAND_MAP_SIZE (sizeof(COMMAND_MAP) / sizeof(CommandInfo))

#endif
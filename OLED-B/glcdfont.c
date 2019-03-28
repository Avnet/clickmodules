// This is the 'classic' fixed-space bitmap font for Adafruit_GFX since 1.0.
// See gfxfont.h for newer custom bitmap font info.

#ifndef FONT5X7_H
#define FONT5X7_H

// Standard ASCII 5x7 font

static const unsigned char font[] = {

                                   // Decimal Hex   Description                                         Symbol
    0x00, 0x00, 0x00, 0x00, 0x00,  //   0      0    NUL (Null)                                         // ☺ 
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E,  //   1      1    SOH (Start of Header)                              // ☻ 
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E,  //   2      2    STX (Start of Text)                                // ♥ 
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C,  //   3      3    ETX (End of Text)                                  // ♦
    0x18, 0x3C, 0x7E, 0x3C, 0x18,  //   4      4    EOT (End of Transmission)                          // ♣
    0x1C, 0x57, 0x7D, 0x57, 0x1C,  //   5      5    ENQ (Enquiry)                                      // ♠
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C,  //   6      6    ACK (Acknowledge)                                  // •
    0x00, 0x18, 0x3C, 0x18, 0x00,  //   7      7    BEL (Bell)                                         // ◘
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF,  //   8      8    BS (BackSpace)                                     // ○
    0x00, 0x18, 0x24, 0x18, 0x00,  //   9      9    HT (Horizontal Tabulation)                         // ◙
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF,  //  10      A    LF (Line Feed)                                     // ♂
    0x30, 0x48, 0x3A, 0x06, 0x0E,  //  11      B    VT (Vertical Tabulation)                           // ♀
    0x26, 0x29, 0x79, 0x29, 0x26,  //  12      C    FF (Form Feed)                                     // ♪
    0x40, 0x7F, 0x05, 0x05, 0x07,  //  13      D    CR (Carriage Return)                               // ♫
    0x40, 0x7F, 0x05, 0x25, 0x3F,  //  14      E    SO (Shift Out)                                     // ☼
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A,  //  15      F    SI (Shift In)                                      //     
    0x7F, 0x3E, 0x1C, 0x1C, 0x08,  //  16     10    DLE (Data Link Escape)                             //     
    0x08, 0x1C, 0x1C, 0x3E, 0x7F,  //  17     11    DC1 (Device Control 1)                             // ►
    0x14, 0x22, 0x7F, 0x22, 0x14,  //  18     12    DC2 (Device Control 2)                             // ◄
    0x5F, 0x5F, 0x00, 0x5F, 0x5F,  //  19     13    DC3 (Device Control 3)                             // ↕
    0x06, 0x09, 0x7F, 0x01, 0x7F,  //  20     14    DC4 (Device Control 4)                             // ‼
    0x00, 0x66, 0x89, 0x95, 0x6A,  //  21     15    NAK (Negative Acknowledge)                         // ¶
    0x60, 0x60, 0x60, 0x60, 0x60,  //  22     16    SYN (Synchronous Idle)                             // §
    0x94, 0xA2, 0xFF, 0xA2, 0x94,  //  23     17    ETB (End of Transmission Block)                    // ▬
    0x08, 0x04, 0x7E, 0x04, 0x08,  //  24     18    CAN (Cancel)                                       // ↨
    0x10, 0x20, 0x7E, 0x20, 0x10,  //  25     19    EM (End of Medium)                                 // ↑
    0x08, 0x08, 0x2A, 0x1C, 0x08,  //  26     1A    SUB (Substitute)                                   // ↓
    0x08, 0x1C, 0x2A, 0x08, 0x08,  //  27     1B    ESC (Escape)                                       // →
    0x1E, 0x10, 0x10, 0x10, 0x10,  //  28     1C    FS (File Separator)                                // ←
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C,  //  29     1D    GS (Group Separator)                               // ∟
    0x30, 0x38, 0x3E, 0x38, 0x30,  //  30     1E    RS (Record Separator)                              // ↔
    0x06, 0x0E, 0x3E, 0x0E, 0x06,  //  31     1F    US (Unit Separator)                                // ▲
    0x00, 0x00, 0x00, 0x00, 0x00,  //  32     20    Space                                              // S
    0x00, 0x00, 0x5F, 0x00, 0x00,  //  33     21    Exclamation mark                                   // !
    0x00, 0x07, 0x00, 0x07, 0x00,  //  34     22    Quotation Mark                                     // 
    0x14, 0x7F, 0x14, 0x7F, 0x14,  //  35     23    Hash                                               // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12,  //  36     24    Dollar                                             // $
    0x23, 0x13, 0x08, 0x64, 0x62,  //  37     25    Percent                                            // %
    0x36, 0x49, 0x56, 0x20, 0x50,  //  38     26    Ampersand                                          // &
    0x00, 0x08, 0x07, 0x03, 0x00,  //  39     27    Apostrophe                                         //
    0x00, 0x1C, 0x22, 0x41, 0x00,  //  40     28    Open bracket                                       /( 
    0x00, 0x41, 0x22, 0x1C, 0x00,  //  41     29    Close bracket                                      // )
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A,  //  42     2A    Asterisk                                           // *
    0x08, 0x08, 0x3E, 0x08, 0x08,  //  43     2B    Plus                                               // +
    0x00, 0x80, 0x70, 0x30, 0x00,  //  44     2C    Comma                                              // ,
    0x08, 0x08, 0x08, 0x08, 0x08,  //  45     2D    Dash                                               // =
    0x00, 0x00, 0x60, 0x60, 0x00,  //  46     2E    Full stop                                          // .
    0x20, 0x10, 0x08, 0x04, 0x02,  //  47     2F    Slash                                              // /
    0x3E, 0x51, 0x49, 0x45, 0x3E,  //  48     30    Zero                                               // 0
    0x00, 0x42, 0x7F, 0x40, 0x00,  //  49     31    One                                                // 1
    0x72, 0x49, 0x49, 0x49, 0x46,  //  50     32    Two                                                // 2
    0x21, 0x41, 0x49, 0x4D, 0x33,  //  51     33    Three                                              // 3
    0x18, 0x14, 0x12, 0x7F, 0x10,  //  52     34    Four                                               // 4
    0x27, 0x45, 0x45, 0x45, 0x39,  //  53     35    Five                                               // 5
    0x3C, 0x4A, 0x49, 0x49, 0x31,  //  54     36    Six                                                // 6
    0x41, 0x21, 0x11, 0x09, 0x07,  //  55     37    Seven                                              // 7
    0x36, 0x49, 0x49, 0x49, 0x36,  //  56     38    Eight                                              // 8
    0x46, 0x49, 0x49, 0x29, 0x1E,  //  57     39    Nine                                               // 9
    0x00, 0x00, 0x14, 0x00, 0x00,  //  58     3A    Colon                                              // ;
    0x00, 0x40, 0x34, 0x00, 0x00,  //  59     3B    Semicolon                                          // <
    0x00, 0x08, 0x14, 0x22, 0x41,  //  60     3C    Less than                                          // =
    0x14, 0x14, 0x14, 0x14, 0x14,  //  61     3D    Equals sign                                        // >
    0x00, 0x41, 0x22, 0x14, 0x08,  //  62     3E    Greater than                                       // ?
    0x02, 0x01, 0x59, 0x09, 0x06,  //  63     3F    Question mark                                      //     
    0x3E, 0x41, 0x5D, 0x59, 0x4E,  //  64     40    At                                                 // @
    0x7C, 0x12, 0x11, 0x12, 0x7C,  //  65     41    Upper case A                                       // A
    0x7F, 0x49, 0x49, 0x49, 0x36,  //  66     42    Upper case B                                       // B
    0x3E, 0x41, 0x41, 0x41, 0x22,  //  67     43    Upper case C                                       // C
    0x7F, 0x41, 0x41, 0x41, 0x3E,  //  68     44    Upper case D                                       // D
    0x7F, 0x49, 0x49, 0x49, 0x41,  //  69     45    Upper case E                                       // E
    0x7F, 0x09, 0x09, 0x09, 0x01,  //  70     46    Upper case F                                       // F
    0x3E, 0x41, 0x41, 0x51, 0x73,  //  71     47    Upper case G                                       // G
    0x7F, 0x08, 0x08, 0x08, 0x7F,  //  72     48    Upper case H                                       // H
    0x00, 0x41, 0x7F, 0x41, 0x00,  //  73     49    Upper case I                                       // I
    0x20, 0x40, 0x41, 0x3F, 0x01,  //  74     4A    Upper case J                                       // J
    0x7F, 0x08, 0x14, 0x22, 0x41,  //  75     4B    Upper case K                                       // K
    0x7F, 0x40, 0x40, 0x40, 0x40,  //  76     4C    Upper case L                                       // L
    0x7F, 0x02, 0x1C, 0x02, 0x7F,  //  77     4D    Upper case M                                       // M
    0x7F, 0x04, 0x08, 0x10, 0x7F,  //  78     4E    Upper case N                                       // N
    0x3E, 0x41, 0x41, 0x41, 0x3E,  //  79     4F    Upper case O                                       // O
    0x7F, 0x09, 0x09, 0x09, 0x06,  //  80     50    Upper case P                                       // P
    0x3E, 0x41, 0x51, 0x21, 0x5E,  //  81     51    Upper case Q                                       // Q
    0x7F, 0x09, 0x19, 0x29, 0x46,  //  82     52    Upper case R                                       // R
    0x26, 0x49, 0x49, 0x49, 0x32,  //  83     53    Upper case S                                       // S
    0x03, 0x01, 0x7F, 0x01, 0x03,  //  84     54    Upper case T                                       // T
    0x3F, 0x40, 0x40, 0x40, 0x3F,  //  85     55    Upper case U                                       // U
    0x1F, 0x20, 0x40, 0x20, 0x1F,  //  86     56    Upper case V                                       // V
    0x3F, 0x40, 0x38, 0x40, 0x3F,  //  87     57    Upper case W                                       // W
    0x63, 0x14, 0x08, 0x14, 0x63,  //  88     58    Upper case X                                       // X
    0x03, 0x04, 0x78, 0x04, 0x03,  //  89     59    Upper case Y                                       // Y
    0x61, 0x59, 0x49, 0x4D, 0x43,  //  90     5A    Upper case Z                                       // Z
    0x00, 0x7F, 0x41, 0x41, 0x41,  //  91     5B    Open square bracket                                // [
    0x02, 0x04, 0x08, 0x10, 0x20,  //  92     5C    Backslash                                          // 
    0x00, 0x41, 0x41, 0x41, 0x7F,  //  93     5D    Close square bracket                               // ]
    0x04, 0x02, 0x01, 0x02, 0x04,  //  94     5E    Caret                                              // ^
    0x40, 0x40, 0x40, 0x40, 0x40,  //  95     5F    Underscore                                         // _
    0x00, 0x03, 0x07, 0x08, 0x00,  //  96     60    Grave accent                                       // `
    0x20, 0x54, 0x54, 0x78, 0x40,  //  97     61    Lower case a                                       // a
    0x7F, 0x28, 0x44, 0x44, 0x38,  //  98     62    Lower case b                                       // b
    0x38, 0x44, 0x44, 0x44, 0x28,  //  99     63    Lower case c                                       // c
    0x38, 0x44, 0x44, 0x28, 0x7F,  // 100     64    Lower case d                                       // d
    0x38, 0x54, 0x54, 0x54, 0x18,  // 101     65    Lower case e                                       // e
    0x00, 0x08, 0x7E, 0x09, 0x02,  // 102     66    Lower case f                                       // f
    0x18, 0xA4, 0xA4, 0x9C, 0x78,  // 103     67    Lower case g                                       // g
    0x7F, 0x08, 0x04, 0x04, 0x78,  // 104     68    Lower case h                                       // h
    0x00, 0x44, 0x7D, 0x40, 0x00,  // 105     69    Lower case i                                       // i
    0x20, 0x40, 0x40, 0x3D, 0x00,  // 106     6A    Lower case j                                       // j
    0x7F, 0x10, 0x28, 0x44, 0x00,  // 107     6B    Lower case k                                       // k
    0x00, 0x41, 0x7F, 0x40, 0x00,  // 108     6C    Lower case l                                       // l
    0x7C, 0x04, 0x78, 0x04, 0x78,  // 109     6D    Lower case m                                       // m
    0x7C, 0x08, 0x04, 0x04, 0x78,  // 110     6E    Lower case n                                       // n
    0x38, 0x44, 0x44, 0x44, 0x38,  // 111     6F    Lower case o                                       // o
    0xFC, 0x18, 0x24, 0x24, 0x18,  // 112     70    Lower case p                                       // p
    0x18, 0x24, 0x24, 0x18, 0xFC,  // 113     71    Lower case q                                       // q
    0x7C, 0x08, 0x04, 0x04, 0x08,  // 114     72    Lower case r                                       // r
    0x48, 0x54, 0x54, 0x54, 0x24,  // 115     73    Lower case s                                       // s
    0x04, 0x04, 0x3F, 0x44, 0x24,  // 116     74    Lower case t                                       // t
    0x3C, 0x40, 0x40, 0x20, 0x7C,  // 117     75    Lower case u                                       // u
    0x1C, 0x20, 0x40, 0x20, 0x1C,  // 118     76    Lower case v                                       // v
    0x3C, 0x40, 0x30, 0x40, 0x3C,  // 119     77    Lower case w                                       // w
    0x44, 0x28, 0x10, 0x28, 0x44,  // 120     78    Lower case x                                       // x
    0x4C, 0x90, 0x90, 0x90, 0x7C,  // 121     79    Lower case y                                       // y
    0x44, 0x64, 0x54, 0x4C, 0x44,  // 122     7A    Lower case z                                       // z
    0x00, 0x08, 0x36, 0x41, 0x00,  // 123     7B    Open brace                                         // {
    0x00, 0x00, 0x77, 0x00, 0x00,  // 124     7C    Pipe                                               // |
    0x00, 0x41, 0x36, 0x08, 0x00,  // 125     7D    Close brace                                        // }
    0x02, 0x01, 0x02, 0x04, 0x02,  // 126     7E    Tilde                                              // ~
    0x3C, 0x26, 0x23, 0x26, 0x3C,  // 127     7F    Delete                                             // 
    0x1E, 0xA1, 0xA1, 0x61, 0x12,  // 128     80    Upper case C with cedilla                          // Ç
    0x3A, 0x40, 0x40, 0x20, 0x7A,  // 129     81    Lower case u with diaeresis                        // ü
    0x38, 0x54, 0x54, 0x55, 0x59,  // 130     82    Lower case e with acute                            // é
    0x21, 0x55, 0x55, 0x79, 0x41,  // 131     83    Lower case a with circumflex                       // â
    0x22, 0x54, 0x54, 0x78, 0x42,  // 132     84    Lower case a with diaeresis                        // ä
    0x21, 0x55, 0x54, 0x78, 0x40,  // 133     85    Lower case a with grave                            // à
    0x20, 0x54, 0x55, 0x79, 0x40,  // 134     86    Lower case a with ring above                       // å
    0x0C, 0x1E, 0x52, 0x72, 0x12,  // 135     87    Lower case c with cedilla                          // ç
    0x39, 0x55, 0x55, 0x55, 0x59,  // 136     88    Lower case e with circumflex                       // ê
    0x39, 0x54, 0x54, 0x54, 0x59,  // 137     89    Lower case e with diaeresis                        // ë
    0x39, 0x55, 0x54, 0x54, 0x58,  // 138     8A    Lower case e with grave                            // è
    0x00, 0x00, 0x45, 0x7C, 0x41,  // 139     8B    Lower case i with diaeresis                        // ï
    0x00, 0x02, 0x45, 0x7D, 0x42,  // 140     8C    Lower case i with circumflex                       // î
    0x00, 0x01, 0x45, 0x7C, 0x40,  // 141     8D    Lower case i with grave                            // ì
    0x7D, 0x12, 0x11, 0x12, 0x7D,  // 142     8E    Upper case A with diaeresis                        // Ä
    0xF0, 0x28, 0x25, 0x28, 0xF0,  // 143     8F    Upper case A with ring above                       // Å
    0x7C, 0x54, 0x55, 0x45, 0x00,  // 144     90    Upper case E with acute                            // É
    0x20, 0x54, 0x54, 0x7C, 0x54,  // 145     91    Lower case ae                                      // æ
    0x7C, 0x0A, 0x09, 0x7F, 0x49,  // 146     92    Upper case AE                                      // Æ
    0x32, 0x49, 0x49, 0x49, 0x32,  // 147     93    Lower case o with circumflex                       // ô
    0x3A, 0x44, 0x44, 0x44, 0x3A,  // 148     94    Lower case o with diaeresis                        // ö
    0x32, 0x4A, 0x48, 0x48, 0x30,  // 149     95    Lower case o with grave                            // ò
    0x3A, 0x41, 0x41, 0x21, 0x7A,  // 150     96    Lower case u with circumflex                       // û
    0x3A, 0x42, 0x40, 0x20, 0x78,  // 151     97    Lower case u with grave                            // ù
    0x00, 0x9D, 0xA0, 0xA0, 0x7D,  // 152     98    Lower case y with diaeresis                        // ÿ
    0x3D, 0x42, 0x42, 0x42, 0x3D,  // 153     99    Upper case O with diaeresis                        // Ö
    0x3D, 0x40, 0x40, 0x40, 0x3D,  // 154     9A    Upper case U with diaeresis                        // Ü
    0x3C, 0x24, 0xFF, 0x24, 0x24,  // 155     9B    Cent sign                                          // ¢ 
    0x48, 0x7E, 0x49, 0x43, 0x66,  // 156     9C    Pound sign                                         // £
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B,  // 157     9D    Yen sign                                           // ¥
    0xFF, 0x09, 0x29, 0xF6, 0x20,  // 158     9E    Peseta sign                                        // ₧
    0xC0, 0x88, 0x7E, 0x09, 0x03,  // 159     9F    Lower case f with hook                             // ƒ
    0x20, 0x54, 0x54, 0x79, 0x41,  // 160     A0    Lower case a with acute                            // á
    0x00, 0x00, 0x44, 0x7D, 0x41,  // 161     A1    Lower case i with acute                            // í
    0x30, 0x48, 0x48, 0x4A, 0x32,  // 162     A2    Lower case o with acute                            // ó
    0x38, 0x40, 0x40, 0x22, 0x7A,  // 163     A3    Lower case u with acute                            // ú
    0x00, 0x7A, 0x0A, 0x0A, 0x72,  // 164     A4    Lower case n with tilde                            // ñ
    0x7D, 0x0D, 0x19, 0x31, 0x7D,  // 165     A5    Upper case N with tilde                            // Ñ
    0x26, 0x29, 0x29, 0x2F, 0x28,  // 166     A6    Feminine ordinal indicator                         // ª
    0x26, 0x29, 0x29, 0x29, 0x26,  // 167     A7    Masculine ordinal indicator                        // º
    0x30, 0x48, 0x4D, 0x40, 0x20,  // 168     A8    Inverted question mark                             // ¿
    0x38, 0x08, 0x08, 0x08, 0x08,  // 169     A9    Reversed not sign                                  // ⌐
    0x08, 0x08, 0x08, 0x08, 0x38,  // 170     AA    Not sign                                           // ¬
    0x2F, 0x10, 0xC8, 0xAC, 0xBA,  // 171     AB    Vulgar fraction one half                           // ½
    0x2F, 0x10, 0x28, 0x34, 0xFA,  // 172     AC    Vulgar fraction one quarter                        // ¼
    0x00, 0x00, 0x7B, 0x00, 0x00,  // 173     AD    Inverted exclamation mark                          // ¡
    0x08, 0x14, 0x2A, 0x14, 0x22,  // 174     AE    Left-pointing double angle quotation mark          // «
    0x22, 0x14, 0x2A, 0x14, 0x08,  // 175     AF    Right-pointing double angle quotation mark         // »
    0x55, 0x00, 0x55, 0x00, 0x55,  // 176     B0    Light shade                                        // ░
    0xAA, 0x55, 0xAA, 0x55, 0xAA,  // 177     B1    Medium shade                                       // ▒
    0xFF, 0x55, 0xFF, 0x55, 0xFF,  // 178     B2    Dark shade                                         // ▓
    0x00, 0x00, 0x00, 0xFF, 0x00,  // 179     B3    Box drawings light vertical                        // │
    0x10, 0x10, 0x10, 0xFF, 0x00,  // 180     B4    Box drawings light vertical and left               // ┤
    0x14, 0x14, 0x14, 0xFF, 0x00,  // 181     B5    Box drawings vertical single and left double       // ╡
    0x10, 0x10, 0xFF, 0x00, 0xFF,  // 182     B6    Box drawings vertical double and left single       // ╢
    0x10, 0x10, 0xF0, 0x10, 0xF0,  // 183     B7    Box drawings down double and left single           // ╖
    0x14, 0x14, 0x14, 0xFC, 0x00,  // 184     B8    Box drawings down single and left double           // ╕
    0x14, 0x14, 0xF7, 0x00, 0xFF,  // 185     B9    Box drawings double vertical and left              // ╣
    0x00, 0x00, 0xFF, 0x00, 0xFF,  // 186     BA    Box drawings double vertical                       // ║
    0x14, 0x14, 0xF4, 0x04, 0xFC,  // 187     BB    Box drawings double down and left                  // ╗
    0x14, 0x14, 0x17, 0x10, 0x1F,  // 188     BC    Box drawings up single and left double             // ╝
    0x10, 0x10, 0x1F, 0x10, 0x1F,  // 189     BD    Box drawings double up and left                    // ╜
    0x14, 0x14, 0x14, 0x1F, 0x00,  // 190     BE    Box drawings up double and left single             // ╛
    0x10, 0x10, 0x10, 0xF0, 0x00,  // 191     BF    Box drawings light down and left                   // ┐
    0x00, 0x00, 0x00, 0x1F, 0x10,  // 192     C0    Box drawings light up and right                    // └
    0x10, 0x10, 0x10, 0x1F, 0x10,  // 193     C1    Box drawings light up and horizontal               // ┴
    0x10, 0x10, 0x10, 0xF0, 0x10,  // 194     C2    Box drawings light down and horizontal             // ┬
    0x00, 0x00, 0x00, 0xFF, 0x10,  // 195     C3    Box drawings light vertical and right              // ├
    0x10, 0x10, 0x10, 0x10, 0x10,  // 196     C4    Box drawings light horizontal                      // ─
    0x10, 0x10, 0x10, 0xFF, 0x10,  // 197     C5    Box drawings light vertical and horizontal         // ┼
    0x00, 0x00, 0x00, 0xFF, 0x14,  // 198     C6    Box drawings vertical single and right double      // ╞
    0x00, 0x00, 0xFF, 0x00, 0xFF,  // 199     C7    Box drawings vertical double and right single      // ╟
    0x00, 0x00, 0x1F, 0x10, 0x17,  // 200     C8    Box drawings double up and right                   // ╚
    0x00, 0x00, 0xFC, 0x04, 0xF4,  // 201     C9    Box drawings double down and right                 // ╔
    0x14, 0x14, 0x17, 0x10, 0x17,  // 202     CA    Box drawings double up and horizontal              // ╩
    0x14, 0x14, 0xF4, 0x04, 0xF4,  // 203     CB    Box drawings double down and horizontal            // ╦
    0x00, 0x00, 0xFF, 0x00, 0xF7,  // 204     CC    Box drawings double vertical and right             // ╠
    0x14, 0x14, 0x14, 0x14, 0x14,  // 205     CD    Box drawings double horizontal                     // ═
    0x14, 0x14, 0xF7, 0x00, 0xF7,  // 206     CE    Box drawings double vertical and horizontal        // ╬
    0x14, 0x14, 0x14, 0x17, 0x14,  // 207     CF    Box drawings up single and horizontal double       // ╧
    0x10, 0x10, 0x1F, 0x10, 0x1F,  // 208     D0    Box drawings up double and horizontal single       // ╨
    0x14, 0x14, 0x14, 0xF4, 0x14,  // 209     D1    Box drawings down single and horizontal double     // ╤
    0x10, 0x10, 0xF0, 0x10, 0xF0,  // 210     D2    Box drawings down double and horizontal single     // ╥
    0x00, 0x00, 0x1F, 0x10, 0x1F,  // 211     D3    Box drawings up double and right single            // ╙
    0x00, 0x00, 0x00, 0x1F, 0x14,  // 212     D4    Box drawings up single and right double            // ╘
    0x00, 0x00, 0x00, 0xFC, 0x14,  // 213     D5    Box drawings down single and right double          // ╒
    0x00, 0x00, 0xF0, 0x10, 0xF0,  // 214     D6    Box drawings down double and right single          // ╓
    0x10, 0x10, 0xFF, 0x10, 0xFF,  // 215     D7    Box drawings vertical double and horizontal single // ╫   
    0x14, 0x14, 0x14, 0xFF, 0x14,  // 216     D8    Box drawings vertical single and horizontal double // ╪   
    0x10, 0x10, 0x10, 0x1F, 0x00,  // 217     D9    Box drawings light up and left                     // ┘   
    0x00, 0x00, 0x00, 0xF0, 0x10,  // 218     DA    Box drawings light down and right                  // ┌   
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 219     DB    Full block                                         // █   
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0,  // 220     DC    Lower half block                                   // ▄   
    0xFF, 0xFF, 0xFF, 0x00, 0x00,  // 221     DD    Left half block                                    // ▌   
    0x00, 0x00, 0x00, 0xFF, 0xFF,  // 222     DE    Right half block                                   // ▐   
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F,  // 223     DF    Upper half block                                   // ▀   
    0x38, 0x44, 0x44, 0x38, 0x44,  // 224     E0    Greek lower case alpha                             // α   
    0xFC, 0x4A, 0x4A, 0x4A, 0x34,  // 225     E1    Lower case sharp s                                 // ß[c]
    0x7E, 0x02, 0x02, 0x06, 0x06,  // 226     E2    Greek upper case letter gamma                      // Γ   
    0x02, 0x7E, 0x02, 0x7E, 0x02,  // 227     E3    Greek lower case pi                                // π[d]
    0x63, 0x55, 0x49, 0x41, 0x63,  // 228     E4    Greek upper case letter sigma                      // Σ[e]
    0x38, 0x44, 0x44, 0x3C, 0x04,  // 229     E5    Greek lower case sigma                             // σ   
    0x40, 0x7E, 0x20, 0x1E, 0x20,  // 230     E6    Micro sign                                         // µ[f]
    0x06, 0x02, 0x7E, 0x02, 0x02,  // 231     E7    Greek lower case tau                               // τ   
    0x99, 0xA5, 0xE7, 0xA5, 0x99,  // 232     E8    Greek upper case letter phi                        // Φ   
    0x1C, 0x2A, 0x49, 0x2A, 0x1C,  // 233     E9    Greek upper case letter theta                      // Θ   
    0x4C, 0x72, 0x01, 0x72, 0x4C,  // 234     EA    Greek upper case letter omega                      // Ω[g]
    0x30, 0x4A, 0x4D, 0x4D, 0x30,  // 235     EB    Greek lower case delta                             // δ[h]
    0x30, 0x48, 0x78, 0x48, 0x30,  // 236     EC    Infinity                                           // ∞   
    0xBC, 0x62, 0x5A, 0x46, 0x3D,  // 237     ED    Greek lower case phi                               // φ[i]
    0x3E, 0x49, 0x49, 0x49, 0x00,  // 238     EE    Greek lower case epsilon                           // ε[j]
    0x7E, 0x01, 0x01, 0x01, 0x7E,  // 239     EF    Intersection                                       // ∩   
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A,  // 240     F0    Identical to                                       // ≡   
    0x44, 0x44, 0x5F, 0x44, 0x44,  // 241     F1    Plus-minus sign                                    // ±   
    0x40, 0x51, 0x4A, 0x44, 0x40,  // 242     F2    Greater-than or equal to                           // ≥   
    0x40, 0x44, 0x4A, 0x51, 0x40,  // 243     F3    Less-than or equal to                              // ≤   
    0x00, 0x00, 0xFF, 0x01, 0x03,  // 244     F4    Top half integral                                  // ⌠   
    0xE0, 0x80, 0xFF, 0x00, 0x00,  // 245     F5    Bottom half integral                               // ⌡   
    0x08, 0x08, 0x6B, 0x6B, 0x08,  // 246     F6    Division sign                                      // ÷   
    0x36, 0x12, 0x36, 0x24, 0x36,  // 247     F7    Almost equal to                                    // ≈   
    0x06, 0x0F, 0x09, 0x0F, 0x06,  // 248     F8    Degree sign                                        // °   
    0x00, 0x00, 0x18, 0x18, 0x00,  // 249     F9    Bullet operator                                    // ∙   
    0x00, 0x00, 0x10, 0x10, 0x00,  // 250     FA    Middle dot                                         // ·   
    0x30, 0x40, 0xFF, 0x01, 0x01,  // 251     FB    Square root                                        // √[k]
    0x00, 0x1F, 0x01, 0x01, 0x1E,  // 252     FC    Superscript lower case n                           // ⁿ   
    0x00, 0x19, 0x1D, 0x17, 0x12,  // 253     FD    Superscript two                                    // ²   
    0x00, 0x3C, 0x3C, 0x3C, 0x3C,  // 254     FE    Black square                                       // ■   
    0x00, 0x00, 0x00, 0x00, 0x00   // 255     FF        
};

#endif // FONT5X7_H

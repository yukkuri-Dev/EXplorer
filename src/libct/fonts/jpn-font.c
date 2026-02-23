/* jpn-font.c
 * Japanese font stub based on text.c
 * Font bitmap data is intentionally omitted (placeholder only).
 */

#include <graphics/text.h>
#include <graphics/color.h>
#include <graphics/drawing.h>

/*
 * Placeholder font data slots for half-width katakana.
 * Mapping: Shift_JIS single byte 0xA1..0xDF -> index 0..(COUNT-1)
 * Unicode mapping: U+FF61 + index.
 * Example: 0xA7 (index 0xA7-0xA1 = 6) maps to U+FF67 'ｧ' (halfwidth small a),
 * which corresponds to fullwidth 'ァ' (U+30A1). Put the 8 bytes for that glyph
 * at the corresponding slot below (replace the 0x00 placeholders).
 */

#define HW_KATA_FIRST 0xA1
#define HW_KATA_LAST  0xDF
#define HW_KATA_COUNT (HW_KATA_LAST - HW_KATA_FIRST + 1)

/* Each glyph is 8x8: scanline = (width+7)/8 = 1, bytes_per_glyph = 8 */
const uint8_t japanese8x8[] = {
    /* Glyphs for 0xA1 .. 0xDF (HW_KATA_COUNT == 63) */
    /* For each glyph: 8 bytes (one per row). Replace 0x00 with bitmap bytes. */

    /* 0xA1 -> U+FF61 '｡' */
    0b00000000,
    0b00000000,
    0b00011100,
    0b00110110,
    0b00100010,
    0b00110110,
    0b00011100,
    0b00000000,
    /* 0xA2 -> U+FF62 '｢' */
    0b01111100,
    0b01111100,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b00000000,

    /* 0xA3 -> U+FF63 '｣' */
    0b00000000,
    0b00000000,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b01111100,
    0b01111100,

    /* 0xA4 -> U+FF64 '､' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100000,
    0b00011100,
    0b00001110,

    /* 0xA5 -> U+FF65 '･' */
    0b00000000,
    0b00000000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00000000,
    0b00000000,

    /* 0xA6 -> U+FF66 'ｦ' */
    0b00111110,
    0b00000010,
    0b00000010,
    0b00011110,
    0b00000010,
    0b00000100,
    0b00011000,
    0b00000000,

    /* 0xA7 -> U+FF67 'ｧ' (halfwidth small a) */
    /* ここに 'ｧ'（対応する全角 'ァ'）のビットマップを入れる */
    0b00000000,
    0b00000000,
    0b00111111,
    0b00000001,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00001000,

    /* 0xA8 -> U+FF68 'ｨ' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000100,
    0b00001100,
    0b00010100,
    0b00000100,


    /* 0xA9 -> U+FF69 'ｩ' */
    0b00000000,
    0b00000000,
    0b00001000,
    0b00111111,
    0b00100001,
    0b00000010,
    0b00000010,
    0b00000100,
    /* 0xAA -> U+FF6A 'ｪ' */
    0b00000000,
    0b00000000,
    0b00111111,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00111111,

    /* 0xAB -> U+FF6B 'ｫ' */
    0b00000000,
    0b00000000,
    0b00000010,
    0b00111111,
    0b00000110,
    0b00001010,
    0b00000010,
    0b00000010,

    /* 0xAC -> U+FF6C 'ｬ' */
    0b00000000,
    0b00000000,
    0b00010000,
    0b00111111,
    0b00010001,
    0b00010000,
    0b00001000,
    0b00001000,

    /* 0xAD -> U+FF6D 'ｭ' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011100,
    0b00000100,
    0b00000100,
    0b00111110,

    /* 0xAE -> U+FF6E 'ｮ' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b00000010,
    0b00011110,
    0b00000010,
    0b00111111,

    /* 0xAF -> U+FF6F 'ｯ' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010100,
    0b00010101,
    0b00000001,
    0b00000110,

    /* 0xB0 -> U+FF70 'ｰ' */
    0b00000000,
    0b00000000,
    0b00000000,
    0b01100000,
    0b00011110,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xB1 -> U+FF71 'ｱ' */
    0b00111110,
    0b00000010,
    0b00001010,
    0b00001100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00000000,

    /* 0xB2 -> U+FF72 'ｲ' */
    0b00000010,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00011000,
    0b00101000,
    0b00001000,
    0b00001000,

    /* 0xB3 -> U+FF73 'ｳ' */
    0b00001000,
    0b00111100,
    0b00100100,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00011000,
    0b00000000,

    /* 0xB4 -> U+FF74 'ｴ' */
    0b00111110,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00111110,

    /* 0xB5 -> U+FF75 'ｵ' */
    0b00000100,
    0b01111110,
    0b00001100,
    0b00010100,
    0b00100100,
    0b00001100,
    0b00000100,
    0b00000000,

    /* 0xB6 -> U+FF76 'ｶ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xB7 -> U+FF77 'ｷ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xB8 -> U+FF78 'ｸ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xB9 -> U+FF79 'ｹ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBA -> U+FF7A 'ｺ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBB -> U+FF7B 'ｻ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBC -> U+FF7C 'ｼ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBD -> U+FF7D 'ｽ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBE -> U+FF7E 'ｾ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xBF -> U+FF7F 'ｿ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC0 -> U+FF80 'ﾀ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC1 -> U+FF81 'ﾁ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC2 -> U+FF82 'ﾂ' */
    0b00000000,
    0b00000000,
    0b01001000,
    0b01001001,
    0b00000001,
    0b00000011,
    0b00000010,
    0b00011000,

    /* 0xC3 -> U+FF83 'ﾃ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC4 -> U+FF84 'ﾄ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC5 -> U+FF85 'ﾅ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC6 -> U+FF86 'ﾆ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC7 -> U+FF87 'ﾇ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC8 -> U+FF88 'ﾈ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xC9 -> U+FF89 'ﾉ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCA -> U+FF8A 'ﾊ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCB -> U+FF8B 'ﾋ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCC -> U+FF8C 'ﾌ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCD -> U+FF8D 'ﾍ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCE -> U+FF8E 'ﾎ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xCF -> U+FF8F 'ﾏ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD0 -> U+FF90 'ﾐ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD1 -> U+FF91 'ﾑ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD2 -> U+FF92 'ﾒ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD3 -> U+FF93 'ﾓ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD4 -> U+FF94 'ﾔ' */
    0b00100000,
    0b00100000,
    0b11111110,
    0b00100010,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,

    /* 0xD5 -> U+FF95 'ﾕ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD6 -> U+FF96 'ﾖ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD7 -> U+FF97 'ﾗ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD8 -> U+FF98 'ﾘ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xD9 -> U+FF99 'ﾙ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDA -> U+FF9A 'ﾚ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDB -> U+FF9B 'ﾛ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDC -> U+FF9C 'ﾜ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDD -> U+FF9D 'ﾝ' */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDE -> U+FF9E 'ﾞ' (voiced sound mark) */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    /* 0xDF -> U+FF9F 'ﾟ' (semi-voiced sound mark) */
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

};

/* Optional per-glyph widths. If you want variable widths, fill these
 * with values per glyph. For now they are all 8 (full-width of this
 * 8x8 font). */
static const uint8_t jpn_widths[HW_KATA_COUNT] = {
    /* 63 entries of 8 */
    /* You can edit individual entries if some glyphs should be narrower. */
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

/* Japanese font struct */
struct font jpn_system = {
    8, /* width */
    8, /* height */
    jpn_widths,
    japanese8x8,
};

/* Helper to return the japanese font if needed. */
struct font * get_jpn_font()
{
    return &jpn_system;
}

/*
 * Install the Japanese font as the active font. Returns the previously
 * active font (calls the generic `set_font`).
 */
struct font * set_jpn_font()
{
    return set_font(&jpn_system);
}

/*
 * Render a NUL-terminated string where single bytes in the range
 * 0xA1..0xDF are treated as Shift_JIS-style half-width katakana.
 *
 * Expectation: `japanese8x8` contains glyphs for half-width katakana
 * in the order 0xA1..0xDF (one glyph per byte). Each glyph is
 * `bytes_per_glyph` bytes long (scanline * height).
 */
void render_text_jpn_halfwidth(uint16_t x, uint16_t y, const uint8_t *text)
{
    const unsigned char * glyph;
    uint8_t b;
    uint8_t width = jpn_system.width;
    uint16_t scanline = (jpn_system.width + 7) / 8;
    uint16_t bytes_per_glyph = scanline * jpn_system.height;

    while (*text != '\0') {
        b = *text;
        if (b >= 0xA1 && b <= 0xDF) {
            /* half-width katakana: map directly into jpn glyph table */
            uint16_t idx = (uint16_t)(b - 0xA1);
            glyph = jpn_system.data + (idx * bytes_per_glyph);
            draw_glyph(x, y, width, jpn_system.height, scanline, glyph);
            x += width;
        } else {
            /* fallback: render ASCII using current font */
            struct font *prev = get_font();
            /* temporarily switch to system ASCII font if different */
            /* assume text.c's `system` font remains available as default */
            /* Use existing render_text for ASCII by casting away const */
            if (b >= 0x20 && b <= 0x7e) {
                /* render single ASCII char using current font */
                const unsigned char *asg; /* ascii glyph */
                uint16_t ascii_scan = (prev->width + 7) / 8;
                uint16_t ascii_bytes = ascii_scan * prev->height;
                uint8_t c = b;
                c -= 0x20;
                asg = prev->data + (c * ascii_bytes);
                draw_glyph(x, y, prev->width, prev->height, ascii_scan, asg);
                x += prev->width;
            }
        }
        ++text;
    }
}

/* Decode next UTF-8 codepoint. Advances *p and returns codepoint, or 0 on error/terminator. */
static uint32_t utf8_next(const char **p)
{
    const unsigned char *s = (const unsigned char *)*p;
    uint32_t cp;

    /* End of string */
    if (*s == '\0')
        return 0;

    /* ASCII fast path */
    if (*s < 0x80) {
        cp = *s;
        (*p)++;
        return cp;
    }

    /* 2-byte sequence: 110xxxxx 10xxxxxx */
    if ((s[0] & 0xE0) == 0xC0) {
        if (s[1] == '\0' || (s[1] & 0xC0) != 0x80) {
            /* Truncated or invalid continuation */
            (*p)++;
            return 0;
        }
        cp = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
        (*p) += 2;
        return cp;
    }

    /* 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx */
    if ((s[0] & 0xF0) == 0xE0) {
        if (s[1] == '\0' || s[2] == '\0' ||
            (s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80) {
            /* Truncated or invalid continuation */
            (*p)++;
            return 0;
        }
        cp = ((s[0] & 0x0F) << 12) |
             ((s[1] & 0x3F) << 6) |
             (s[2] & 0x3F);
        (*p) += 3;
        return cp;
    }

    /* 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    if ((s[0] & 0xF8) == 0xF0) {
        if (s[1] == '\0' || s[2] == '\0' || s[3] == '\0' ||
            (s[1] & 0xC0) != 0x80 ||
            (s[2] & 0xC0) != 0x80 ||
            (s[3] & 0xC0) != 0x80) {
            /* Truncated or invalid continuation */
            (*p)++;
            return 0;
        }
        cp = ((s[0] & 0x07) << 18) |
             ((s[1] & 0x3F) << 12) |
             ((s[2] & 0x3F) << 6) |
             (s[3] & 0x3F);
        (*p) += 4;
        return cp;
    }

    /* invalid leading byte: skip one */
    (*p)++;
    return 0;
}

/* Map a Unicode codepoint to a half-width table byte (0xA1..0xDF).
 * Returns 0 on failure (not mapped) — caller should skip or fallback. */
/* Fill `out` with 1..2 halfwidth bytes for the Unicode codepoint `cp`.
 * Returns number of bytes written (0 on unmapped). */
static int map_unicode_to_halfwidth_sequence(uint32_t cp, uint8_t *out)
{
    /* If already in halfwidth Unicode block U+FF61..U+FF9F */
    if (cp >= 0xFF61 && cp <= 0xFF9F) {
        out[0] = (uint8_t)(0xA1 + (cp - 0xFF61));
        return 1;
    }

    /* Fullwidth katakana and related mappings. Many fullwidth katakana
     * that include dakuten/handakuten are represented as base + mark.
     */
    switch (cp) {
    /* small kana */
    case 0x30A1: out[0] = 0xA7; return 1; /* ァ -> ｧ */
    case 0x30A3: out[0] = 0xA8; return 1; /* ィ -> ｨ */
    case 0x30A5: out[0] = 0xA9; return 1; /* ゥ -> ｩ */
    case 0x30A7: out[0] = 0xAA; return 1; /* ェ -> ｪ */
    case 0x30A9: out[0] = 0xAB; return 1; /* ォ -> ｫ */
    case 0x30E3: out[0] = 0xAC; return 1; /* ャ -> ｬ */
    case 0x30E5: out[0] = 0xAD; return 1; /* ュ -> ｭ */
    case 0x30E7: out[0] = 0xAE; return 1; /* ョ -> ｮ */
    case 0x30C3: out[0] = 0xAF; return 1; /* ッ -> ｯ */
    /* prolonged sound mark */
    case 0x30FC: out[0] = 0xB0; return 1; /* ー -> ｰ */

    /* vowels */
    case 0x30A2: out[0] = 0xB1; return 1; /* ア -> ｱ */
    case 0x30A4: out[0] = 0xB2; return 1; /* イ -> ｲ */
    case 0x30A6: out[0] = 0xB3; return 1; /* ウ -> ｳ */
    case 0x30A8: out[0] = 0xB4; return 1; /* エ -> ｴ */
    case 0x30AA: out[0] = 0xB5; return 1; /* オ -> ｵ */

    /* K row */
    case 0x30AB: out[0] = 0xB6; return 1; /* カ -> ｶ */
    case 0x30AD: out[0] = 0xB7; return 1; /* キ -> ｷ */
    case 0x30AF: out[0] = 0xB8; return 1; /* ク -> ｸ */
    case 0x30B1: out[0] = 0xB9; return 1; /* ケ -> ｹ */
    case 0x30B3: out[0] = 0xBA; return 1; /* コ -> ｺ */
    /* G (dakuten) */
    case 0x30AC: /* ガ */ out[0] = 0xB6; out[1] = 0xDE; return 2;
    case 0x30AE: /* ギ */ out[0] = 0xB7; out[1] = 0xDE; return 2;
    case 0x30B0: /* グ */ out[0] = 0xB8; out[1] = 0xDE; return 2;
    case 0x30B2: /* ゲ */ out[0] = 0xB9; out[1] = 0xDE; return 2;
    case 0x30B4: /* ゴ */ out[0] = 0xBA; out[1] = 0xDE; return 2;

    /* S row */
    case 0x30B5: out[0] = 0xBB; return 1; /* サ -> ｻ */
    case 0x30B7: out[0] = 0xBC; return 1; /* シ -> ｼ */
    case 0x30B9: out[0] = 0xBD; return 1; /* ス -> ｽ */
    case 0x30BB: out[0] = 0xBE; return 1; /* セ -> ｾ */
    case 0x30BD: out[0] = 0xBF; return 1; /* ソ -> ｿ */
    /* Z (dakuten) */
    case 0x30B6: out[0] = 0xBB; out[1] = 0xDE; return 2; /* ザ */
    case 0x30B8: out[0] = 0xBC; out[1] = 0xDE; return 2; /* ジ */
    case 0x30BA: out[0] = 0xBD; out[1] = 0xDE; return 2; /* ズ */
    case 0x30BC: out[0] = 0xBE; out[1] = 0xDE; return 2; /* ゼ */
    case 0x30BE: out[0] = 0xBF; out[1] = 0xDE; return 2; /* ゾ */

    /* T row */
    case 0x30BF: out[0] = 0xC0; return 1; /* タ -> ﾀ */
    case 0x30C1: out[0] = 0xC1; return 1; /* チ -> ﾁ */
    case 0x30C4: out[0] = 0xC2; return 1; /* ツ -> ﾂ */
    case 0x30C6: out[0] = 0xC3; return 1; /* テ -> ﾃ */
    case 0x30C8: out[0] = 0xC4; return 1; /* ト -> ﾄ */
    /* D (dakuten) */
    case 0x30C0: out[0] = 0xC0; out[1] = 0xDE; return 2; /* ダ */
    case 0x30C2: out[0] = 0xC1; out[1] = 0xDE; return 2; /* ヂ */
    case 0x30C5: out[0] = 0xC2; out[1] = 0xDE; return 2; /* ヅ */
    case 0x30C7: out[0] = 0xC3; out[1] = 0xDE; return 2; /* デ */
    case 0x30C9: out[0] = 0xC4; out[1] = 0xDE; return 2; /* ド */

    /* N row */
    case 0x30CA: out[0] = 0xC5; return 1; /* ナ -> ﾅ */
    case 0x30CB: out[0] = 0xC6; return 1; /* ニ -> ﾆ */
    case 0x30CC: out[0] = 0xC7; return 1; /* ヌ -> ﾇ */
    case 0x30CD: out[0] = 0xC8; return 1; /* ネ -> ﾈ */
    case 0x30CE: out[0] = 0xC9; return 1; /* ノ -> ﾉ */

    /* H row */
    case 0x30CF: out[0] = 0xCA; return 1; /* ハ -> ﾊ */
    case 0x30D2: out[0] = 0xCB; return 1; /* ヒ -> ﾋ */
    case 0x30D5: out[0] = 0xCC; return 1; /* フ -> ﾌ */
    case 0x30D8: out[0] = 0xCD; return 1; /* ヘ -> ﾍ */
    case 0x30DB: out[0] = 0xCE; return 1; /* ホ -> ﾎ */
    /* B (dakuten) */
    case 0x30D0: out[0] = 0xCA; out[1] = 0xDE; return 2; /* バ */
    case 0x30D3: out[0] = 0xCB; out[1] = 0xDE; return 2; /* ビ */
    case 0x30D6: out[0] = 0xCC; out[1] = 0xDE; return 2; /* ブ */
    case 0x30D9: out[0] = 0xCD; out[1] = 0xDE; return 2; /* ベ */
    case 0x30DC: out[0] = 0xCE; out[1] = 0xDE; return 2; /* ボ */
    /* P (handakuten) */
    case 0x30D1: out[0] = 0xCA; out[1] = 0xDF; return 2; /* パ */
    case 0x30D4: out[0] = 0xCB; out[1] = 0xDF; return 2; /* ピ */
    case 0x30D7: out[0] = 0xCC; out[1] = 0xDF; return 2; /* プ */
    case 0x30DA: out[0] = 0xCD; out[1] = 0xDF; return 2; /* ペ */
    case 0x30DD: out[0] = 0xCE; out[1] = 0xDF; return 2; /* ポ */

    /* M row */
    case 0x30DE: out[0] = 0xCF; return 1; /* マ -> ﾏ */
    case 0x30DF: out[0] = 0xD0; return 1; /* ミ -> ﾐ */
    case 0x30E0: out[0] = 0xD1; return 1; /* ム -> ﾑ */
    case 0x30E1: out[0] = 0xD2; return 1; /* メ -> ﾒ */
    case 0x30E2: out[0] = 0xD3; return 1; /* モ -> ﾓ */

    /* small ya/yu/yo handled above; large */
    case 0x30E4: out[0] = 0xD4; return 1; /* ヤ -> ﾔ */
    case 0x30E6: out[0] = 0xD5; return 1; /* ユ -> ﾕ */
    case 0x30E8: out[0] = 0xD6; return 1; /* ヨ -> ﾖ */

    /* R row */
    case 0x30E9: out[0] = 0xD7; return 1; /* ラ -> ﾗ */
    case 0x30EA: out[0] = 0xD8; return 1; /* リ -> ﾘ */
    case 0x30EB: out[0] = 0xD9; return 1; /* ル -> ﾙ */
    case 0x30EC: out[0] = 0xDA; return 1; /* レ -> ﾚ */
    case 0x30ED: out[0] = 0xDB; return 1; /* ロ -> ﾛ */

    /* small wa etc */
    case 0x30EE: out[0] = 0x00; return 0; /* ヮ - no halfwidth equivalent */
    case 0x30EF: out[0] = 0xDC; return 1; /* ワ -> ﾜ */
    case 0x30F2: out[0] = 0xA6; return 1; /* ヲ -> ｦ */
    case 0x30F3: out[0] = 0xDD; return 1; /* ン -> ﾝ */
    case 0x30F4: /* ヴ -> ウ + dakuten */ out[0] = 0xB3; out[1] = 0xDE; return 2;
    /* others (historic) - skip */
    default:
        return 0;
    }
}

/* UTF-8 aware renderer. Maps UTF-8 codepoints to halfwidth table where possible.
 * ASCII (0x20..0x7E) is rendered using the current ASCII font. */
void render_text_utf8_jpn(uint16_t x, uint16_t y, const char *utf8)
{
    const char *p = utf8;
    uint32_t cp;
    uint8_t byte;

    uint16_t scanline = (jpn_system.width + 7) / 8;
    uint16_t bytes_per_glyph = scanline * jpn_system.height;

    while ((cp = utf8_next(&p)) != 0) {
        if (cp <= 0x7F) {
            /* ASCII */
            if (cp >= 0x20 && cp <= 0x7E) {
                struct font *prev = get_font();
                uint16_t ascii_scan = (prev->width + 7) / 8;
                uint16_t ascii_bytes = ascii_scan * prev->height;
                uint8_t c = (uint8_t)cp;
                c -= 0x20;
                const unsigned char *asg = prev->data + (c * ascii_bytes);
                draw_glyph(x, y, prev->width, prev->height, ascii_scan, asg);
                x += prev->width;
            }
            continue;
        }

        {
            uint8_t seq[2];
            int n = map_unicode_to_halfwidth_sequence(cp, seq);
            for (int i = 0; i < n; ++i) {
                uint8_t hb = seq[i];
                if (hb >= HW_KATA_FIRST && hb <= HW_KATA_LAST) {
                    uint16_t idx = (uint16_t)(hb - HW_KATA_FIRST);
                    const unsigned char *glyph = jpn_system.data + (idx * bytes_per_glyph);
                    draw_glyph(x, y, jpn_system.width, jpn_system.height, scanline, glyph);
                    x += jpn_system.width;
                }
            }
        }
        /* unmapped codepoints are skipped */
    }
}

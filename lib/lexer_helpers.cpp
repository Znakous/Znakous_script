#include "lexer_helpers.h"

constexpr std::bitset<256> make_char_bitset(const char* str) {
    std::bitset<256> bits;
    for (; *str; ++str) {
        bits.set(static_cast<unsigned char>(*str));
    }
    return bits;
}


constexpr auto forbidden_chars_bitset = make_char_bitset("<>=)({}[];' ,.!@#%^&*/-\"+~");
constexpr auto forbidden_startchars_bitset = make_char_bitset("0123456789");

    
bool is_forbidden_char(char c) noexcept {
    return forbidden_chars_bitset.test(static_cast<unsigned char>(c));
}

bool is_forbidden_startchar(char c) noexcept {
    return forbidden_startchars_bitset.test(static_cast<unsigned char>(c)) || is_forbidden_char(c);
}

bool is_whitespace(char c) noexcept {
    return c == ' ' || c == '\n';
}

bool is_numeric(char c) noexcept {
    return c=='+' || c=='-' || c=='e' || std::isdigit(c);
}
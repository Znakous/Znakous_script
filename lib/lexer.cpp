#include "lexer.h"
#include "preprocess.h"
#include <bitset>

constexpr std::bitset<256> make_char_bitset(const char* str) {
    std::bitset<256> bits;
    for (; *str; ++str) {
        bits.set(static_cast<unsigned char>(*str));
    }
    return bits;
}

constexpr auto forbidden_chars_bitset = make_char_bitset("<>=)({};',.!@#%^&*-+~");

bool is_forbidden_char(char c) noexcept {
    return forbidden_chars_bitset.test(static_cast<unsigned char>(c));
}




Lexer::Lexer(const char* data) // null-terminated data
    : str_(Preprocess(data)), cur_pos_(0), read_pos_(0), derived_size(0) {
    data_ = str_.data();
    std::cout << "lexer go construct\n";
    std::cout << str_;
    std::cout << data_;
    std::cout << "lexer go construct\n";
    ReadOne();
    for (const auto& keyword : keywords) {
        keywords_.insert(keyword.first, keyword.second);
    }
    for (const auto& std_func : std_funcs) {
        keywords_.insert(std_func, {TokenType::stdfunc, std_func});
    }
    cur_token_ = NextToken();
    std::cout << "lexer constructed\n";
}

void Lexer::ReadOne() {
    if (data_[read_pos_] == '\0') {
        derived_size = read_pos_;
        cur_char_ = 0;
    } else {
        cur_char_ = data_[read_pos_];
    }
    cur_pos_ = read_pos_++;

}
Token Lexer::PeekToken() {
    return cur_token_;
}
Token Lexer::GetToken() {
    std::cout << "call get " << (int) cur_token_.type << "\n";
    if (cur_token_.type == TokenType::invalid) {
        std::cout << "invalid on ";
        std::cout << data_ +cur_pos_;
        return cur_token_;
    }
    Token t = cur_token_;
    cur_token_ = NextToken();
    std::cout << "get token ret " << (int) t.type << " next to be " << (int) cur_token_.type << "\n";
    return t;
}
void Lexer::FindNext(char symbol) {
    while(cur_char_ !=  symbol && cur_char_ != 0) {
        ReadOne();
    }
}
bool Lexer::IsIdentSymbol() {
    return !is_forbidden_char(cur_char_);
}
void Lexer::FindNumber(bool met_e=false, bool e_was_last=false) {
    ReadOne();
    std::cout << "find number\n";
    if (std::isdigit(cur_char_)) {
        FindNumber(met_e, false);
    } else if (cur_char_ == 'e' && std::isdigit(data_[read_pos_])) {
        if (met_e) {
            return;
        } else {
            FindNumber(true, true);
        }
    } else if (cur_char_ == '-' && e_was_last) {
        FindNumber(met_e, false);
    }
    return;
}

Token Lexer::NextToken() {
    std::cout << "getting token\n";
    uint32_t start = cur_pos_;
    auto resp = keywords_.get(data_ + start);
    auto resp_ident = identifiers_.get(data_ + start);
    std::cout << "ask for " << data_ + start << "\n";
    if (resp.has_value()) {
        std::cout << "keyword " << (int)resp.value().param.type << "\n";
        uint32_t size = resp.value().size;
        cur_pos_ += size;
        read_pos_ += size;
        cur_char_ = data_[cur_pos_];
        return (resp.value().param);
    } else if (resp_ident.has_value()) {
        std::cout << "really ident\n";
        uint32_t size = resp_ident.value().size;
        cur_pos_ += size;
        read_pos_ += size;
        cur_char_ = data_[cur_pos_];
        return (resp_ident.value().param);
    }
    else {
        // can be literal
        if (cur_char_ == '"') {
            uint32_t first_quote = cur_pos_;
            ReadOne();
            FindNext('"');
            uint32_t second_quote = cur_pos_;
            ReadOne();
            std::cout << "found literal\n";
            return Token(TokenType::stringliteral, std::string_view(data_ + first_quote + 1, data_ + second_quote));
        } else if (std::isdigit(cur_char_)) {
            std::cout << "found number\n";
            uint32_t first_digit = cur_pos_;
            FindNumber();
            std::cout << "found number " << std::string_view(data_ + first_digit, data_ + cur_pos_) << "\n";
            return Token(TokenType::intliteral, std::string_view(data_ + first_digit, data_ + cur_pos_));
        }
         else if (cur_char_ != 0 && IsIdentSymbol()){
            std::cout << "first time ident\n";
            // std::cout << "aaaaaaaaaaaaaaaaaaaaaaa wdf you doing\n";
            // std::cout << "found ident\n";
            uint32_t first_let = cur_pos_;
            while(cur_char_ != 0 &&  IsIdentSymbol()) {
                ReadOne();
            }
            identifiers_.insert(std::string_view(data_ + first_let, data_ + cur_pos_), Token(TokenType::ident, std::string_view(data_ + first_let, data_ + cur_pos_)));
            return Token(TokenType::ident, std::string_view(data_ + first_let, data_ + cur_pos_));
        }
    }
    std::cout << "nothing\n";
    return Token(TokenType::invalid);
}

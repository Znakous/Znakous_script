#include "lexer.h"

Lexer::Lexer(const char* data) // null-terminated data
    : data_(data), cur_pos_(0), read_pos_(0), derived_size(0) {
    ReadOne();
    for (const auto& keyword : keywords) {
        keywords_.insert(keyword.first, keyword.second);
    }
    for_peeks_ = NextToken();
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
    return for_peeks_;
}
Token Lexer::GetToken() {
    if (for_peeks_.type == TokenType::invalid) {
        return for_peeks_;
    }
    Token t = for_peeks_;
    for_peeks_ = NextToken();
    return t;
}
void Lexer::FindNext(char symbol) {
    while(cur_char_ !=  symbol && cur_char_ != 0) {
        ReadOne();
    }
}
void Lexer::FindNumber(bool met_e=false, bool e_was_last=false) {
    ReadOne();
    if (std::isdigit(cur_char_)) {
        FindNumber(met_e, false);
    } else if (cur_char_ == 'e') {
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
    if (resp.has_value()) {
        std::cout << "keyword\n";
        uint32_t size = resp.value().size;
        cur_pos_ += size;
        read_pos_ += size;
        cur_char_ = data_[cur_pos_];
        return *(resp.value().param_ptr);
    }
    else {
        // can be literal
        if (cur_char_ == '"') {
            uint32_t first_quote = cur_pos_;
            ReadOne();
            FindNext('"');
            uint32_t second_quote = cur_pos_;
            ReadOne();
            return Token(TokenType::stringliteral, std::string_view(data_ + first_quote + 1, data_ + second_quote));
        } else if (std::isdigit(cur_char_)) {
            uint32_t first_digit = cur_pos_;
            FindNumber();
            return Token(TokenType::intliteral, std::string_view(data_ + first_digit, data_ + cur_pos_));
        }
    }
    std::cout << "nothing\n";
    return Token(TokenType::invalid, std::nullopt);
}

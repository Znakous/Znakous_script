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

bool is_whitespace(char c) noexcept {
    return c == ' ' || c == '\n';
}

bool is_numeric(char c) noexcept {
    return c=='+' || c=='-' || c=='e' || std::isdigit(c);
}

Lexer::Lexer(const char* data, std::shared_ptr<logging::Logger> logger)
    : str_(Preprocess(data)), cur_pos_(0), read_pos_(0), derived_size(0), logger_(logger), in_(std::nullopt) {
    logger_->log("Lexer initializing with preprocessed data");
    ReadOne();
    for (const auto& keyword : keywords) {
        keywords_.insert(keyword.first, keyword.second);
    }
    for (const auto& std_func : std_funcs) {
        keywords_.insert(std_func, {TokenType::stdfunc, std_func});
    }
    cur_token_ = NextToken();
    logger_->log("Lexer initialization completed");
}

Lexer::Lexer(std::istream& in, std::shared_ptr<logging::Logger> logger)
    : logger_(logger), in_(in), str_(""), cur_pos_(0), read_pos_(0), derived_size(0) {
    logger_->log("Lexer initializing with input stream");
    ReadOne();
    for (const auto& keyword : keywords) {
        keywords_.insert(keyword.first, keyword.second);
    }
    for (const auto& std_func : std_funcs) {
        keywords_.insert(std_func, {TokenType::stdfunc, std_func});
    }
    cur_token_ = NextToken();
    logger_->log("Lexer initialization completed");
}

void Lexer::ReadOne() {
    if (in_.has_value()) {
        char buffer;
        logger_->log("ask for one from input stream");
        while (in_.value().get().get(buffer)) {
            logger_->log("Read one from input stream: ", buffer);
            if (!is_whitespace(buffer)) {
                str_.push_back(buffer);
            }
        }
    }
    if (str_.data()[read_pos_] == '\0') {
        derived_size = read_pos_;
        cur_char_ = 0;
    } else {
        cur_char_ = str_.data()[read_pos_];
    }
    cur_pos_ = read_pos_++;
}

Token Lexer::PeekToken() {
    return cur_token_;
}

Token Lexer::GetToken() {
    logger_->log("Getting token, current type: ", static_cast<int>(cur_token_.type));
    
    if (cur_token_.type == TokenType::invalid) {
        logger_->log("Invalid token at position: ", cur_pos_);
        return cur_token_;
    }
    
    Token t = cur_token_;
    cur_token_ = NextToken();
    logger_->log("Token returned: ", static_cast<int>(t.type), ", next token type: ", static_cast<int>(cur_token_.type));
    return t;
}

void Lexer::FindNext(char symbol) {
    while(cur_char_ != symbol && cur_char_ != 0) {
        ReadOne();
    }
}

bool Lexer::IsIdentSymbol() {
    return !is_forbidden_char(cur_char_);
}

void Lexer::FindNumber(bool met_e=false, bool e_was_last=false) {
    ReadOne();
    logger_->log("Finding number");
    
    if (std::isdigit(cur_char_)) {
        FindNumber(met_e, false);
    } else if (cur_char_ == 'e' && is_numeric(str_.data()[read_pos_])) {
        if (met_e) {
            return;
        } else {
            FindNumber(true, true);
        }
    } else if ((cur_char_ == '-' || cur_char_ == '+') && e_was_last) {
        FindNumber(met_e, false);
    }
    return;
}

Token Lexer::NextToken() {
    logger_->log("Getting next token at position: ", cur_pos_);
    uint32_t start = cur_pos_;
    
    auto resp = keywords_.get(str_.data() + start);
    auto resp_ident = identifiers_.get(str_.data() + start);
    
    if (resp.has_value()) {
        logger_->log("Found keyword, type: ", static_cast<int>(resp.value().param.type));
        uint32_t size = resp.value().size;
        cur_pos_ += size;
        read_pos_ += size;
        cur_char_ = str_.data()[cur_pos_];
        return (resp.value().param);
    } else if (resp_ident.has_value()) {
        logger_->log("Found identifier");
        uint32_t size = resp_ident.value().size;
        cur_pos_ += size;
        read_pos_ += size;
        cur_char_ = str_.data()[cur_pos_];
        return (resp_ident.value().param);
    } else {
        if (cur_char_ == '"') {
            uint32_t first_quote = cur_pos_;
            ReadOne();
            FindNext('"');
            uint32_t second_quote = cur_pos_;
            ReadOne();
            logger_->log("Found string literal");
            return Token(TokenType::stringliteral, std::string_view(str_.data() + first_quote + 1, str_.data() + second_quote));
        } else if (std::isdigit(cur_char_)) {
            logger_->log("Found number literal");
            uint32_t first_digit = cur_pos_;
            FindNumber();
            std::string_view number(str_.data() + first_digit, str_.data() + cur_pos_);
            logger_->log("Parsed number: ", number);
            return Token(TokenType::intliteral, number);
        } else if (cur_char_ != 0 && IsIdentSymbol()) {
            logger_->log("Found new identifier");
            uint32_t first_let = cur_pos_;
            while(cur_char_ != 0 && IsIdentSymbol()) {
                ReadOne();
            }
            std::string_view ident(str_.data() + first_let, str_.data() + cur_pos_);
            identifiers_.insert(ident, Token(TokenType::ident, ident));
            return Token(TokenType::ident, ident);
        }
    }
    
    logger_->log("No valid token found");
    return Token(TokenType::invalid);
}

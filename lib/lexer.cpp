#include "lexer.h"
#include "preprocess.h"
#include "lexer_helpers.h"
#include "std_function_fwd.h"


Lexer::Lexer(std::string&& data, std::shared_ptr<logging::Logger> logger)
 : str_(std::move(data)), cur_pos_(0), read_pos_(0), derived_size(0), logger_(logger) {
    for (const auto& keyword : keywords) {
        keywords_.insert(keyword.first, keyword.second);
    }
    for (const auto& std_func : std_funcs) {
        keywords_.insert(std_func, {TokenType::stdfunc, std_func});
    }
}

void WildLexer::ReadOne() {
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

TokenType Lexer::GetFunnyAssign(std::string_view op) {
    auto resp = keywords_.get(op.data());
    if (resp.has_value()) {
        return resp.value().param.type;
    }
    return TokenType::invalid;
}

Token WildLexer::NextToken() {
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

void NormalLexer::ReadOne() {
    if (str_.data()[read_pos_] == '\0') {
        derived_size = read_pos_;
        cur_char_ = 0;
    } else {
        cur_char_ = str_.data()[read_pos_];
    }
    cur_pos_ = read_pos_++;
}

Token NormalLexer::NextToken() {
    while (cur_char_ == ' ') {
        ReadOne();
    }
    logger_->log("NormalLexer: Getting next token at position: ", cur_pos_);
    logger_->log("NormalLexer: Current char: ", str_.data() + cur_pos_);
    auto resp = keywords_.get(str_.data() + cur_pos_);
    auto resp_ident = identifiers_.get(str_.data() + cur_pos_);
    auto ident = GetIdent();
    
    if (ident.size() > 0) {
        std::cout << "ident: " << ident << " symb " << str_.data()[cur_pos_] << " " << ident.size() << std::endl;
        if (resp.has_value()) {
            std::cout << "resp has value: " << (int) resp.value().param.type << " " << resp.value().size << std::endl;
        }
        if (resp.has_value() && resp.value().param.type == TokenType::ident && resp.value().size == ident.size()) {
            std::cout << "already known\n";
            return resp.value().param;
        } else if (resp.has_value() && resp.value().size == ident.size()) {
            std::cout << "keyword\n";
            return resp.value().param;
        }
        else {
            std::cout << "declare new ident " << ident << std::endl;
            identifiers_.insert(ident, Token(TokenType::ident, ident));
            return Token(TokenType::ident, ident);
        } 
        // else {
        //     logger_->log("the fuck is this?");
        //     throw std::runtime_error("the fuck is this?");
        // }
    }
    logger_->log("NormalLexer: not ident ", str_.data() + cur_pos_);
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

std::string_view NormalLexer::GetIdent() {
    uint32_t first_let = cur_pos_;
    if (is_forbidden_startchar(cur_char_)) {
        return std::string_view();
    }
    while(cur_char_ != 0 && !is_forbidden_char(cur_char_)) {
        ReadOne();
    }
    return std::string_view(str_.data() + first_let, str_.data() + cur_pos_);
}

WildLexer::WildLexer(const char* data, std::shared_ptr<logging::Logger> logger)
 : Lexer(PreprocessWild(data), logger) {
    logger_->log("Lexer initializing with preprocessed data");
    this->ReadOne();
    cur_token_ = NextToken();
    logger_->log("Lexer initialization completed");
}

WildLexer::WildLexer(std::istream& in, std::shared_ptr<logging::Logger> logger)
 : Lexer(PreprocessWild(in), logger) {
    logger_->log("Lexer initializing with input stream");
    ReadOne();
    cur_token_ = NextToken();
    logger_->log("Lexer initialization completed");
}
NormalLexer::NormalLexer(const char* data, std::shared_ptr<logging::Logger> logger)
 : Lexer(PreprocessNormal(data), logger) {
    logger_->log("Normal Lexer initializing with preprocessed data");
    this->ReadOne();
    cur_token_ = NextToken();
    logger_->log("Normal Lexer initialization completed");
}

NormalLexer::NormalLexer(std::istream& in, std::shared_ptr<logging::Logger> logger)
 : Lexer(PreprocessNormal(in), logger) {
    logger_->log("Normal Lexer initializing with input stream");
    ReadOne();
    cur_token_ = NextToken();
    logger_->log("Normal Lexer initialization completed");
}

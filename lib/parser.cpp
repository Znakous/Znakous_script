#include "parser.h"

Parser::Parser(const char* data, std::shared_ptr<logging::Logger> log, bool wild) 
    : logger_(log)
{   
    if (wild) {
        lexer_ = std::make_unique<WildLexer>(data, log);
    } else {
        lexer_ = std::make_unique<NormalLexer>(data, log);
    }
    cur_token_ = lexer_->GetToken();
    peek_token_ = lexer_->GetToken();
    logger_->log("Parser initialized with tokens: ", static_cast<int>(cur_token_.type), 
                 " and ", static_cast<int>(peek_token_.type));
}

Parser::Parser(std::istream& in, std::shared_ptr<logging::Logger> log, bool wild)
    : logger_(log)
{    
    if (wild) {
        lexer_ =  std::make_unique<WildLexer>(in, log);
    } else {
        lexer_ = std::make_unique<NormalLexer>(in, log);
    }
    cur_token_ = lexer_->GetToken();
    peek_token_ = lexer_->GetToken();
    logger_->log("Parser initialized with tokens: ", static_cast<int>(cur_token_.type), 
                    " and ", static_cast<int>(peek_token_.type));
}


void Parser::AdvanceTokens() {
    logger_->log("Advancing tokens from: ", static_cast<int>(cur_token_.type), 
                 " and ", static_cast<int>(peek_token_.type));
    cur_token_ = peek_token_;
    peek_token_ = lexer_->GetToken();
    logger_->log("Advanced to: ", static_cast<int>(cur_token_.type), 
                 " and ", static_cast<int>(peek_token_.type));
}

Statement Parser::ParseStatement() {
    logger_->log("Parsing statement, current token: ", static_cast<int>(cur_token_.type),
                 ", peek token: ", static_cast<int>(peek_token_.type));

    if (cur_token_.type != TokenType::invalid) {
        if (cur_token_.type == TokenType::if_s) {
            logger_->log("Found if statement");
            return ParseIfStatement();
        } else if (cur_token_.type == TokenType::ret) {
            logger_->log("Found return statement");
            return ParseReturnStatement();
        } else if (cur_token_.type == TokenType::while_s) {
            logger_->log("Found while statement");
            return ParseWhileStatement();
        } else if (cur_token_.type == TokenType::break_s) {
            logger_->log("Found break statement");
            AdvanceTokens();
            return make_ptr<BreakStatement>();
        } else if (cur_token_.type == TokenType::continue_s) {
            logger_->log("Found continue statement");
            AdvanceTokens();
            return make_ptr<ContinueStatement>();
        } else {
            return ParseAssignStatement();
        }
    }
    
    logger_->log("Invalid statement encountered");
}

Statement Parser::ParseAssignStatement() {
    logger_->log("Attempting to parse assignment");

    if (cur_token_.type != TokenType::ident) {
        logger_->log("Not an identifier, parsing as expression statement");
        return ParseExprStatement();
    }
    
    if (peek_token_.type != TokenType::assign && peek_token_.type != TokenType::lsquare && peek_token_.type != TokenType::funny_assign) {
        logger_->log("Not an assignment or array access, parsing as expression statement");
        return ParseExprStatement();
    }

    std::string_view ident = cur_token_.value.value();
    logger_->log("Processing identifier: ", ident);
    AdvanceTokens();

    if (cur_token_.type == TokenType::lsquare) {
        logger_->log("Parsing array assignment");
        ptr<ArrayAssignStatement> ans = make_ptr<ArrayAssignStatement>();
        ans->array = ident;
        while (cur_token_.type == TokenType::lsquare) {
            AdvanceTokens();
            ans->indices.push_back(ParseExpression());
            AdvanceTokens();
        }
        if (cur_token_.type == TokenType::funny_assign) {
            ans->funny_assign = lexer_->GetFunnyAssign(cur_token_.value.value());
            logger_->log("Funny assign: ", static_cast<int>(ans->funny_assign.value()));
        }
        AdvanceTokens();
        
        ans->value = ParseExpression();
        return ans;
    }

    logger_->log("Parsing simple or compound assignment");
    ptr<AssignStatement> ans = make_ptr<AssignStatement>();
    ans->ident = ident;
    
    if (cur_token_.type == TokenType::funny_assign) {
        ans->funny_assign = lexer_->GetFunnyAssign(cur_token_.value.value());
        logger_->log("Funny assign: ", static_cast<int>(ans->funny_assign.value()));
        AdvanceTokens();
        
        // Parse the right-hand side expression
        Expression rhs = ParseExpression();
        
        // Create a new identifier expression for the left side
        auto id_expr = make_ptr<ExpressionImpl<0>>();
        id_expr->value = make_ptr<IdentifierExpression>();
        std::get<ptr<IdentifierExpression>>(id_expr->value)->name = ident;
        
        // Create the binary operation at level 3
        
        ans->expr = rhs;
    } else {
        AdvanceTokens();
        ans->expr = ParseExpression();
    }
    
    return ans;
}

ptr<Program> Parser::ParseProgram() {
    logger_->log("Starting program parsing");
    ptr<Program> ans = make_ptr<Program>();
    
    while (cur_token_.type != TokenType::invalid) {
        logger_->log("Parsing next statement");
        ans->statements.emplace_back(ParseStatement());
        logger_->log("Statement parsed, current token: ", static_cast<int>(cur_token_.type));
    }
    
    logger_->log("Program parsing completed");
    return ans;
}

Expression Parser::ParseExpression() {
    logger_->log("Parsing expression");

    if (cur_token_.type == TokenType::func) {
        logger_->log("Parsing function definition");
        AdvanceTokens(); // skip func

        ptr<FunctionalExpression> ans = make_ptr<FunctionalExpression>();
        logger_->log("Parsing function arguments");

        AdvanceTokens(); // skip (
        while (cur_token_.type != TokenType::rparen) {
            logger_->log("Parsing argument: ", cur_token_.value.value());
            ans->arguments.push_back(cur_token_.value.value());
            AdvanceTokens(); // skip cur_token
            if (cur_token_.type == TokenType::comma) {
                AdvanceTokens(); // skip ,
            }
        }

        if (cur_token_.type == TokenType::rparen) {
            AdvanceTokens(); // skip )
        }

        logger_->log("Parsing function body");

        while (cur_token_.type != TokenType::endfunc) {
            ans->body.push_back(ParseStatement());
        }

        AdvanceTokens();
        logger_->log("Function parsing completed");
        return ans;
    } else {
        Expression expr = ParseLeveledExpression<operators_levels>();
        
        return expr;
    }
}

ptr<ExprStatement> Parser::ParseExprStatement() {
    logger_->log("Parsing expression statement");
    ptr<ExprStatement> ans = make_ptr<ExprStatement>();
    ans->expr = ParseExpression();
    return ans;
}

ptr<WhileStatement> Parser::ParseWhileStatement() {
    logger_->log("Parsing while statement");
    ptr<WhileStatement> ans = make_ptr<WhileStatement>();
    AdvanceTokens();
    
    logger_->log("Parsing while condition");
    ans->condition = ParseExpression();
    
    logger_->log("Parsing while body");
    while (cur_token_.type != TokenType::endwhile) {
        ans->body.push_back(ParseStatement());
    }
    
    AdvanceTokens();
    logger_->log("While statement parsing completed");
    return ans;
}

ptr<ReturnStatement> Parser::ParseReturnStatement() {
    logger_->log("Parsing return statement");
    ptr<ReturnStatement> ans = make_ptr<ReturnStatement>();
    AdvanceTokens();
    ans->value = ParseExpression();
    return ans;
}

ptr<IfStatement> Parser::ParseIfStatement() {
    logger_->log("Parsing if statement");
    ptr<IfStatement> if_statement = make_ptr<IfStatement>();
    
    AdvanceTokens();
    logger_->log("Parsing if condition");
    if_statement->condition = ParseExpression();
    
    if (cur_token_.type != TokenType::then) {
        logger_->log("Error: missing 'then' keyword");
    } else {
        AdvanceTokens();
    }
    
    logger_->log("Parsing if body");
    if_statement->to_do = ParseStatement();
    
    logger_->log("Parsing else-if clauses");
    while(true) {
        auto else_if = ParseElseIfs();
        if (else_if.has_value()) {
            if_statement->else_ifs.push_back(else_if.value());
        } else {
            break;
        }
    }
    
    logger_->log("Parsing else clause");
    if_statement->else_s = ParseElseStatement();
    AdvanceTokens();
    
    logger_->log("If statement parsing completed");
    return if_statement;
}

std::optional<ptr<ElseIfStatement>> Parser::ParseElseIfs() {
    if (cur_token_.type == TokenType::elif_s) {
        logger_->log("Parsing else-if clause");
        AdvanceTokens();
        ptr<ElseIfStatement> ans = make_ptr<ElseIfStatement>();
        ans->condition = ParseExpression();
        ans->underlying = ParseStatement();
        return ans;
    }
    return std::nullopt;
}

std::optional<ptr<ElseStatement>> Parser::ParseElseStatement() {
    if (cur_token_.type != TokenType::else_s) {
        return std::nullopt;
    }

    ptr<ElseStatement> ans = make_ptr<ElseStatement>();
    ans->token = cur_token_;
    AdvanceTokens();
    ans->underlying = ParseStatement();
    return ans;
}

std::vector<Expression> Parser::ParseArguments() {
    logger_->log("Parsing function arguments");
    std::vector<Expression> arguments;
    AdvanceTokens(); // skip (
    
    while (cur_token_.type != TokenType::rparen) {
        logger_->log("Parsing next argument");
        arguments.push_back(ParseExpression());
        if (cur_token_.type == TokenType::comma) {
            AdvanceTokens();
        }
    }
    
    AdvanceTokens(); // skip )
    logger_->log("Arguments parsing completed");
    return arguments;
}

ptr<ExpressionImpl<0>> Parser::ParseArrayAccess(ptr<ExpressionImpl<0>> array) {
    logger_->log("Parsing array access or slice");
    AdvanceTokens(); // skip [

    if (cur_token_.type == TokenType::colon) {
        // Handle [:end] case
        return ParseArraySlice(array, std::nullopt);
    }

    if (peek_token_.type == TokenType::colon) {
        // Handle [start:...] case
        Expression start = ParseExpression();
        return ParseArraySlice(array, start);
    }

    // Regular array access
    ptr<ArrayAccessExpression> array_access = make_ptr<ArrayAccessExpression>();
    array_access->array = array;
    array_access->indices.push_back(ParseExpression());
    AdvanceTokens(); // skip ]

    // Create level 0 expression
    ptr<ExpressionImpl<0>> expr = make_ptr<ExpressionImpl<0>>();
    expr->value = array_access;
    return expr;
}

ptr<ExpressionImpl<0>> Parser::ParseArraySlice(ptr<ExpressionImpl<0>> array, std::optional<Expression> start) {
    logger_->log("Parsing array slice");
    ptr<ArraySliceExpression> slice = make_ptr<ArraySliceExpression>();
    slice->array = array;
    slice->start = start;

    if (cur_token_.type == TokenType::colon) {
        AdvanceTokens(); // skip first :
    }

    // Parse end if present
    if (cur_token_.type != TokenType::colon && cur_token_.type != TokenType::rsquare) {
        slice->end = ParseExpression();
    }

    // Parse step if present
    if (cur_token_.type == TokenType::colon) {
        AdvanceTokens(); // skip second :
        if (cur_token_.type != TokenType::rsquare) {
            slice->step = ParseExpression();
        }
    }

    if (cur_token_.type == TokenType::rsquare) {
        AdvanceTokens(); // skip ]
    }

    // Create level 0 expression
    ptr<ExpressionImpl<0>> expr = make_ptr<ExpressionImpl<0>>();
    expr->value = slice;
    return expr;
}
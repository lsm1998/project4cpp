//
// Created by 刘时明 on 2021/1/20.
//

#ifndef PROJECT4CPP_TOKENS_H
#define PROJECT4CPP_TOKENS_H

#include <map>

typedef enum Token
{
    // (
    TOKEN_LEFT_PAREN = 1,
    // )
    TOKEN_RIGHT_PAREN = 2,
    // [
    TOKEN_LEFT_BRACK = 3,
    // ]
    TOKEN_RIGHT_BRACK = 4,
} Token;

std::map<std::string, Token> *token_map = {};

#endif //PROJECT4CPP_TOKENS_H

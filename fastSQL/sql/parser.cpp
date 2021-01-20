//
// Created by 刘时明 on 2021/1/20.
//
#include <sql/parser.h>
#include <lex.h>

Prepared *SqlParser::parse(const std::string &sql)
{
    this->tokens = sql_tokenizer(sql);
    this->next();
    if (this->check_equal("SELECT"))
    {
        return this->parse_select();
    } else if (this->check_equal("INSERT"))
    {
        return this->parse_insert();
    } else if (this->check_equal("UPDATE"))
    {
        return this->parse_update();
    }
    return nullptr;
}

std::string SqlParser::next()
{
    if (!this->tokens->empty())
    {
        this->currentToken = this->tokens->front();
        this->tokens->pop_front();
        return this->currentToken;
    }
    return nullptr;
}

bool SqlParser::check_equal(const std::string &str)
{
    return this->currentToken == str;
}

Prepared *SqlParser::parse_select()
{

    return nullptr;
}

Prepared *SqlParser::parse_insert()
{
    return nullptr;
}

Prepared *SqlParser::parse_update()
{
    return nullptr;
}

//
// Created by 刘时明 on 2021/1/20.
//
#include <sql/parser.h>
#include <lex.h>

Prepared *SqlParser::parse(const std::string &sql)
{
    this->tokens = sqlTokenizer(sql);
    this->next();
    if (this->checkEqual("SELECT"))
    {
        return this->parseSelect();
    } else if (this->checkEqual("INSERT"))
    {
        return this->parseInsert();
    } else if (this->checkEqual("UPDATE"))
    {
        return this->parseUpdate();
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

bool SqlParser::checkEqual(const std::string &str)
{
    return this->currentToken == str;
}

Prepared *SqlParser::parseSelect()
{

    return nullptr;
}

Prepared *SqlParser::parseInsert()
{
    return nullptr;
}

Prepared *SqlParser::parseUpdate()
{
    return nullptr;
}

//
// Created by 刘时明 on 2020/12/22.
//

#ifndef PROJECT4CPP_TOKEN_H
#define PROJECT4CPP_TOKEN_H

#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#define _long long long

_long debug=0;    // print the executed instructions
_long assembly=0; // print out the assembly and source
_long token=0; // current token

// instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
    OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
    OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };

// tokens and classes (operators last and in precedence order)
// copied from c4
enum {
    Num = 128, Fun, Sys, Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};


// types of variable/function
enum { CHAR, INT, PTR };

// type of declaration.
enum {Global, Local};

_long *text= nullptr, // text segment
*stack= nullptr;// stack
_long * old_text; // for dump text segment
char *data; // data segment
_long *idmain;

char *src, *old_src;  // pointer to source code string;

_long poolsize; // default size of text/data/stack
_long *pc, *bp, *sp, ax, cycle; // virtual machine registers

_long *current_id, // current parsed ID
*symbols,    // symbol table
line,        // line number of source code
token_val;   // value of current token (mainly for number)

_long basetype;    // the type of a declaration, make it global for convenience
_long expr_type;   // the type of an expression

_long index_of_bp; // index of bp pointer on stack

void next();

void match(_long tk);

void expression(_long level);

void statement();

void enum_declaration();

void function_parameter();

void function_body();

void function_declaration();

void global_declaration();

void program();

_long eval();

#endif //PROJECT4CPP_TOKEN_H

/*
InfixExpr.h

Declares class InfixExpr

Class InfixExpr represents an infix arithmetic expression
*/

/*
DSA Assignment 2

Salman Abdullah
221478 BSCS IIIC

11 October 2023
*/

#ifndef INFIXEXPR_H
#define INFIXEXPR_H

#include <string>
#include <utility>

#include "Queue.h"

class InfixExpr
{
    public:

        InfixExpr();
        explicit InfixExpr(const std::string& expr);
        explicit InfixExpr(std::string&& expr);

        void assign(const std::string& expr);
        void assign(std::string&& expr);

        std::string postfix() const { return postfix_expr_str; }
        std::string infix() const { return postfix_expr_str; }
        double evaluate() const { return result; }

    private:

        std::string infix_expr_str;
        std::string postfix_expr_str;
        Queue<std::string> infix_token_queue;
        Queue<std::string> postfix_token_queue;
        double result;

        bool valid() const;
        void to_infix_queue();
        void to_postfix_queue();
        void to_postfix_str();
        void eval();
        void remove_whitespace();
};

#endif
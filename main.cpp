/*
main.cpp

Basic command-line calculator driver program
*/

#include "InfixExpr.h"

#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

int main()
{
    cout << "Enter \"exit\" to quit\n";
    InfixExpr expr;
    string expr_str;
    string choice;
    while (true)
    {
        cout << ">> ";
        getline(cin, expr_str);
        if (expr_str.empty())
        {
            cout << "Expression is empty\n";
            continue;
        }
        if (expr_str == "exit") break;
        try
        {
            expr.assign(expr_str);
        }
        catch(std::invalid_argument)
        {
            cout << "Syntax error\n";
            continue;
        }

        cout << expr.evaluate() << '\n';
    }
    return 0;
}
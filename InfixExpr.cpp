/*
InfixExpr.cpp

Definition of class InfixExpr delcared in InfixExpr.h
*/

#include <stdexcept>
#include <algorithm>
#include <cmath>

#include "InfixExpr.h"
#include "Stack.h"

using std::string;
using std::invalid_argument;
using std::size_t;
using std::pow;
using std::move;
using std::remove_if;

InfixExpr::InfixExpr()
{}

InfixExpr::InfixExpr(const string& expr) : infix_expr_str{ expr }
{
    remove_whitespace();

    if (!valid()) throw invalid_argument(
        "InfixExpr constructor: invalid infix expression");

    to_infix_queue();  // Store all tokens in infix expression as a queue
    to_postfix_queue();  // Store all tokens in postfix expresstion as a queue
    to_postfix_str();  // Store postfix string
    eval();  // Evaluate postfix expression and store the result
}

InfixExpr::InfixExpr(string&& expr) : infix_expr_str{ move(expr) }
{
    remove_whitespace();

    if (!valid()) throw invalid_argument(
        "InfixExpr constructor: invalid infix expression");

    to_infix_queue();  // Store all tokens in infix expression as a queue
    to_postfix_queue();  // Store all tokens in postfix expresstion as a queue
    to_postfix_str();  // Store postfix string
    eval();  // Evaluate postfix expression and store the result
}

void InfixExpr::assign(const string& expr)
{
    // Clear everything
    infix_expr_str.clear();
    postfix_expr_str.clear();
    infix_token_queue.clear();
    postfix_token_queue.clear();

    infix_expr_str = expr;
    remove_whitespace();

    if (!valid()) throw invalid_argument(
        "InfixExpr constructor: invalid infix expression");

    to_infix_queue();  // Store all tokens in infix expression as a queue
    to_postfix_queue();  // Store all tokens in postfix expresstion as a queue
    to_postfix_str();  // Store postfix string
    eval();  // Evaluate postfix expression and store the result
}

void InfixExpr::assign(string&& expr)
{
    infix_expr_str = move(expr);
    remove_whitespace();

    if (!valid()) throw invalid_argument(
        "InfixExpr constructor: invalid infix expression");

    to_infix_queue();  // Store all tokens in infix expression as a queue
    to_postfix_queue();  // Store all tokens in postfix expresstion as a queue
    to_postfix_str();  // Store postfix string
    eval();  // Evaluate postfix expression and store the result
}

bool InfixExpr::valid() const
{
    Stack<char> paren_stack;
    char c;
    size_t str_sz = infix_expr_str.size();
    for (size_t i = 0; i < str_sz; ++i)
    {
        c = infix_expr_str[i];

        if (!isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/'
                && c != '^' && c != '(' && c != ')' && c != '.')
            return false;

        if (c == '(')
            paren_stack.push(c);
        if (c == ')')
        {
            if (paren_stack.empty()) return false;
            if (paren_stack.top() != '(') return false;
            paren_stack.pop();
        }

        if (c == '*' || c == '/' || c == '^')
        {
            if (i == 0 || i == str_sz - 1)
                return false;
            if (infix_expr_str[i - 1] == '*'
                    || infix_expr_str[i - 1] == '/'
                    || infix_expr_str[i - 1] == '^'
                    || infix_expr_str[i - 1] == '+'
                    || infix_expr_str[i - 1] == '-'
                    || infix_expr_str[i + 1] == '*'
                    || infix_expr_str[i + 1] == '/'
                    || infix_expr_str[i + 1] == '^')
                return false;
        }

        if (c == '+' || c == '-')
        {
            if (i == str_sz - 1)
                return false;
            if (i == 0 && (!isdigit(infix_expr_str[i + 1])
                    && infix_expr_str[i + 1] != '+'
                    && infix_expr_str[i + 1] != '-'))
                return false;
            if (!isdigit(infix_expr_str[i - 1])
                    && infix_expr_str[i - 1] != '('
                    && infix_expr_str[i - 1] != ')'
                    && (!isdigit(infix_expr_str[i + 1]))
                    && infix_expr_str[i + 1] != '+'
                    && infix_expr_str[i + 1] != '-')
                return false;
        }

        if (c == '.')
        {
            if (i == 0 || i == str_sz - 1)
                return false;
            if (!isdigit(infix_expr_str[i - 1])
                    || !isdigit(infix_expr_str[i + 1]))
                return false;
        }
    }

    if (!paren_stack.empty())
        return false;

    return true;
}

void InfixExpr::to_infix_queue()
{
    size_t str_sz = infix_expr_str.size();
    string token;
    int token_sign = 1;
    char c;
    for (size_t i = 0; i < str_sz; ++i)
    {
        c = infix_expr_str[i];
        
        if (c == '(' || c == ')' || c == '*' || c == '/' || c == '^')
        {
            token = c;
            infix_token_queue.enqueue(token);
        }
        else if (c == '+' || c == '-')
        {
            if (i == 0 || (!isdigit(infix_expr_str[i - 1])
                    && infix_expr_str[i - 1] != ')'))
            {
                // + or - is unary

                // Prepare for new token
                token_sign = 1;

                while (!isdigit(c))
                {
                    c == '+' ? token_sign *= 1 : token_sign *= -1;
                    c = infix_expr_str[++i];
                }

                // Prepare for new token
                token = "";

                while (isdigit(c) || c == '.')
                {
                    token += c;
                    c = infix_expr_str[++i];
                }
                if (token_sign == -1) token = '-' + token;
                infix_token_queue.enqueue(token);
                --i;  // For loop will increment again
            }
            else
            {
                // + or - is binary
                token = c;
                infix_token_queue.enqueue(token);
            }
        }
        else
        {
            // Token is a number

            token = "";  // Prepare for new token
            while (isdigit(c) || c == '.')
            {
                token += c;
                c = infix_expr_str[++i];
            }
            infix_token_queue.enqueue(token);
            --i;  // For loop will increment again
        }
    }
}

void InfixExpr::to_postfix_queue()
{
    Stack<string> op_stack;
    string top_op;
    Queue<std::string> infix = infix_token_queue;
    string token;
    while (!infix.empty())
    {
        token = infix.front();

        if (token.size() > 1 || isdigit(token[0]))
        {
            // Token is a number
            postfix_token_queue.enqueue(token);
        }
        else
        {
            if (token == "+" || token == "-")
            {
                if (op_stack.empty())
                    op_stack.push(token);
                else
                {
                    top_op = op_stack.top();

                    if (top_op == "(")
                        op_stack.push(token);
                    else
                    {
                        while (top_op != "(" && op_stack.size() > 0)
                        {
                            postfix_token_queue.enqueue(top_op);
                            op_stack.pop();
                            try
                            {
                                top_op = op_stack.top();   
                            }
                            catch(std::out_of_range)
                            {
                                break;
                            }
                        }
                        op_stack.push(token);
                    }
                }
            }
            else if (token == "*" || token == "/")
            {
                if (op_stack.empty())
                    op_stack.push(token);
                else
                {
                    top_op = op_stack.top();

                    if (top_op == "(")
                        op_stack.push(token);
                    else
                    {
                        if (top_op == "+" || top_op == "-")
                            op_stack.push(token);
                        else if (top_op == "*" || top_op == "/"
                                || top_op == "^")
                        {
                            while (top_op != "(" && top_op != "+"
                                    && top_op != "-" && op_stack.size() > 0)
                            {
                                postfix_token_queue.enqueue(top_op);
                                op_stack.pop();
                                try
                                {
                                    top_op = op_stack.top();   
                                }
                                catch(std::out_of_range)
                                {
                                    break;
                                }
                            }
                            op_stack.push(token);
                        }
                    }
                }
            }
            else if (token == "^")
                op_stack.push(token);
            else if (token == "(")
                op_stack.push(token);
            else if (token == ")")
            {
                top_op = op_stack.top();

                while (top_op != "(")
                {
                    postfix_token_queue.enqueue(top_op);
                    op_stack.pop();
                    top_op = op_stack.top();
                }

                // Discard left parenthesis
                op_stack.pop();
            }
        }

        // Move to the next token
        infix.dequeue();
    }

    // Push the remaining operators on the output queue
    while (!op_stack.empty())
    {
        postfix_token_queue.enqueue(op_stack.top());
        op_stack.pop();
    }
}

void InfixExpr::to_postfix_str()
{
    // (Note: use a linked list instead of a queue; no copying will
    // be required)
    Queue<string> postfix_q = postfix_token_queue;
    while (!postfix_q.empty())
    {
        postfix_expr_str += postfix_q.front();
        postfix_q.dequeue();
    }
}

void InfixExpr::eval()
{
    // Use linked lists for traversal without copying
    Queue<string> postfix_q = postfix_token_queue;
    Stack<double> result_stack;
    string token;
    char op;
    double temp;
    while (!postfix_q.empty())
    {
        token = postfix_q.front();
        if (token.size() > 1 || isdigit(token[0]))
        {
            // Token is a number
            result_stack.push(std::stod(token));
        }
        else
        {
            op = token[0];

            switch(op)
            {
                case '+':
                    temp = result_stack.top();
                    result_stack.pop();
                    result_stack.top() += temp;
                    break;
                case '-':
                    temp = result_stack.top();
                    result_stack.pop();
                    result_stack.top() -= temp;
                    break;
                case '*':
                    temp = result_stack.top();
                    result_stack.pop();
                    result_stack.top() *= temp;
                    break;
                case '/':
                    temp = result_stack.top();
                    result_stack.pop();
                    result_stack.top() /= temp;
                    break;
                case '^':
                    temp = result_stack.top();
                    result_stack.pop();
                    result_stack.top() = pow(result_stack.top(), temp);
                    break;
            }
        }
        postfix_q.dequeue();
    }
    result = result_stack.top();
}

void InfixExpr::remove_whitespace()
{
    infix_expr_str.erase(remove_if(infix_expr_str.begin(),
        infix_expr_str.end(), isspace), infix_expr_str.end());
}
#include <cctype>
#include <ctime>
#include <iostream>
#include <string>
#include "parse.h"

op_t Parser::is_op(std::string &op)
{
    for(int i = 1; i < ops_.size(); i++)
    {
        if(ops_[i].first == op)
        {
            return ops_[i].second;
        }
    }

    return op_t::NONE;
}

Node *Parser::getID()
{
    Node new_node = {type_t::NONE, 0, NULL, NULL};

    int old_p = pos_;
    std::string tmp_var = {};

    while(std::isalpha(str_[pos_]))
    {
        tmp_var.push_back(str_[pos_]);
        ++pos_;
    }
    tmp_var.push_back('\0');

    if(old_p == pos_)
    {
        std::cout << "syntax error in " << pos_ << " position of the \"" << str_ << "\"\nexpected ['a'-'z''A'-'Z']" << std::endl;
        return NULL;
    }
    op_t operation = is_op(tmp_var);

    if(operation == op_t::NONE)
    {
        new_node.var = tmp_var;
        new_node.type = type_t::VAR;
    }
    else
    {
        new_node.op = operation;
        new_node.type = type_t::OP;
    }
    return new_node;
}

// Node *Parser::getE()
// {
//     Node *val = getT();
//
//     if(!val)
//     {
//         return NULL;
//     }
//
//     while(str_[pos_] == '+' || str_[pos_] == '-')
//     {
//         int old_pos_ = pos_;
//         char opos_ = str_->str[str_->pos_];
//         (str_->pos_)++;
//         Node *val2 = getT(str_);
//
//         if(!val2)
//         {
//             Del_tree(val);
//             return NULL;
//         }
//
//         switch (opos_)
//         {
//             case '+':
//             {
//                 Node *tmpos__val = Node({.typos_e = OP, .opos_ = ADD}, val, val2);
//                 if(!tmpos__val)
//                 {
//                     Del_tree(val);
//                     Del_tree(val2);
//                     VERROR_MEM;
//                     return NULL;
//                 }
//                 val = tmpos__val;
//                 break;
//             }
//             case '-':
//             {
//                 Node *tmpos__val = Node({.typos_e = OP, .opos_ = SUB}, val, val2);
//                 if(!tmpos__val)
//                 {
//                     Del_tree(val);
//                     Del_tree(val2);
//                     VERROR_MEM;
//                     return NULL;
//                 }
//                 val = tmpos__val;
//                 break;
//             }
//             default:
//                 Del_tree(val);
//                 Del_tree(val2);
//                 VERROR("syntax error in %d pos_osition of the \"%s\"\nexpos_ected \"+\" or \"-\"", old_pos_, line->str);
//                 break;
//         }
//     }
//     return val;
// }
//
//
// Node *getPow(struct parse_line *line)
// {
//     Node *val = getP(line);
//
//     if(!val)
//     {
//         return NULL;
//     }
//
//     while(line->str[line->p] == '^')
//     {
//         (line->p)++;
//         Node *val2 = getP(line);
//
//         if(!val2)
//         {
//             Del_tree(val);
//             return NULL;
//         }
//
//         Node *tmp_val = Node({.type = OP, .op = POW}, val, val2);
//         if(!tmp_val)
//         {
//             Del_tree(val);
//             Del_tree(val2);
//             VERROR_MEM;
//             return NULL;
//         }
//         val = tmp_val;
//     }
//     return val;
// }
//
// Node *Parser::getT()
// {
//     Node *val = getPow(line);
//
//     if(!val)
//     {
//         return NULL;
//     }
//
//     while(line->str[line->p] == '*' || line->str[line->p] == '/')
//     {
//         int old_p = line->p;
//         char op = line->str[line->p];
//         (line->p)++;
//         Node *val2 = getPow(line);
//
//         if(!val2)
//         {
//             Del_tree(val);
//             return NULL;
//         }
//
//         switch (op)
//         {
//             case '*':
//             {
//                 Node *tmp_val = Node({.type = OP, .op = MUL}, val, val2);
//                 if(!tmp_val)
//                 {
//                     VERROR_MEM;
//                     Del_tree(val);
//                     Del_tree(val2);
//                     return NULL;
//                 }
//                 val = tmp_val;
//                 break;
//             }
//             case '/':
//             {
//                 Node *tmp_val = Node({.type = OP, .op = DIV}, val, val2);
//                 if(!tmp_val)
//                 {
//                     VERROR_MEM;
//                     Del_tree(val);
//                     Del_tree(val2);
//                     return NULL;
//                 }
//                 val = tmp_val;
//                 break;
//             }
//             default:
//             {
//                 VERROR("syntax error in %d position of the \"%s\"\nexpected \"*\" or \"/\"", old_p, line->str);
//                 Del_tree(val);
//                 Del_tree(val2);
//                 return NULL;
//             }
//         }
//     }
//     return val;
// }
//
// Node *Parser::getP()
// {
//     if(str_[pos_] == '(')
//     {
//         (pos_)++;
//         Node *val = getE(line);
//
//         if(!val)
//         {
//             return NULL;
//         }
//
//         if(str_[pos_] != ')')
//         {
//             VERROR("syntax error in %d position of the \"%s\"\nexpected \")\"", pos_, str_);
//             return NULL;
//         }
//         (pos_)++;
//         return val;
//     }
//     else if(isalpha(str_[pos_]))
//     // else if('a' <= str_[pos_] && str_[pos_] <= 'z')
//     {
//         Node *val = getID(line);
//
//         if(!val)
//         {
//             return NULL;
//         }
//
//         if(str_[pos_] == '(')
//         {
//             (pos_)++;
//             Node *val2 = getE(line);
//
//             if(!val2)
//             {
//                 return NULL;
//             }
//
//             if(str_[pos_] != ')')
//             {
//                 VERROR("syntax error in %d position of the \"%s\"\nexpected \")\"", pos_, str_);
//                 return NULL;
//             }
//             (pos_)++;
//             val->right = val2;
//             return val;
//         }
//         return val;
//     }
//
//     return getN(line);
// }
//
//
//
// static int is_unar_minus(struct parse_line *line)
// {
//     if(line->p == 0)
//     {
//         (line->p)++;
//         return 1;
//     }
//     char prev = line->str[line->p - 1];
//     char next = line->str[line->p + 1];
//
//     if((prev== '+' || prev == '-' || prev == '*' || prev == '/' || prev == '(') && '0' <= next && next <= '9')
//     {
//         (line->p)++;
//         return 1;
//     }
//
//     return 0;
// }
//
// Node *getN(struct parse_line *line)
// {
//     Node *val = Node({.type = DIGIT, .val = 0}, NULL, NULL);
//
//     if(!val)
//     {
//         VERROR_MEM;
//         return NULL;
//     }
//
//     int old_p = line->p;
//     int dot = 0;
//     double mult = 1;
//
//     int minus = 1;
//     if(line->str[line->p] == '-')
//     {
//         if(is_unar_minus(line))
//         {
//             minus = -1;
//         }
//     }
//
//     while(('0' <= line->str[line->p] && line->str[line->p] <= '9') || line->str[line->p] == '.')
//     {
//
//         if(line->str[line->p] == '.')
//         {
//             if(dot)
//             {
//                 VERROR("syntax error in %d position of the \"%s\"\nexpected a digit", line->p, line->str);
//                 Del_tree(val);
//                 return NULL;
//             }
//             dot = 1;
//             (line->p)++;
//             continue;
//         }
//         if(dot)
//         {
//             mult *= 0.1;
//             val->val.val = val->val.val + mult * (line->str[line->p] - '0');
//         }
//         else
//         {
//             val->val.val = val->val.val * 10 + line->str[line->p] - '0';
//         }
//         (line->p)++;
//     }
//
//     val->val.val *= minus;
//
//     if(old_p == line->p)
//     {
//         VERROR("syntax error in %d position of the \"%s\"\nexpected digit", line->p, line->str);
//         Del_tree(val);
//         return NULL;
//     }
//     return val;
// }
//
// Node *Parser::getG(char *str)
// {
//     Node *val = getE();
//
//     if(str_[pos_] != '\0')
//     {
//         VERROR("syntax error in %d position of the \"%s\"\nexpected '\\0'", line.p, line.str);
//         Del_tree(val);
//         return NULL;
//     }
//
//     return val;
// }

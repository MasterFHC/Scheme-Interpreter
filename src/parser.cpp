#ifndef PARSER 
#define PARSER

// parser of myscheme 

// #define DEBUG

#include "RE.hpp"
#include "Def.hpp"
#include "syntax.hpp"
#include "expr.hpp"
#include "value.hpp"
#include <map>
#include <cstring>
#include <iostream>
#define mp make_pair
using std :: string;
using std :: vector;
using std :: pair;

extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

Expr Syntax :: parse(Assoc &env) {}

Expr Number :: parse(Assoc &env) {
    return Expr(new Fixnum(n));
}

Expr Identifier :: parse(Assoc &env) {
    return Expr(new Var(s));
}

Expr TrueSyntax :: parse(Assoc &env) {
    return Expr(new True());
}

Expr FalseSyntax :: parse(Assoc &env) {
    return Expr(new False());
}

Expr List :: parse(Assoc &env) {
    // std::cout<<"before"<<std::endl;
    // if(typeid(stxs[0]).name()!="Identifier") throw(RuntimeError("Not an operator"));
    std::string op;
    // std::cout<<typeid(stxs[0]).name()<<std::endl;
    // if(IsProcedure(stxs[0]->parse(env))){
    //     std::cout<<"this is an function too!!!"<<std::endl;

    //     vector<Expr> to_expr;

    //     for(int i=1;i<stxs.size();i++) to_expr.push_back(stxs[i]->parse(env));
    //     return Expr(new Apply(stxs[0]->parse(env), to_expr));
    // }
    if(dynamic_cast<Identifier*>(stxs[0].get())==nullptr){
        // std::cout<<"not an identifier"<<std::endl;
        Expr opexpr = stxs[0]->parse(env);
        vector<Expr> to_expr;
        for(int i=1;i<stxs.size();i++) to_expr.push_back(stxs[i]->parse(env));
        return Expr(new Apply(opexpr, to_expr));
        // Value opvalue = opexpr->eval(env);
        // if(opvalue->v_type!=V_SYM) throw(RuntimeError("operator not a symbol"));
        // op=dynamic_cast<Symbol*>(opvalue.get())->s;
    }
    else op=dynamic_cast<Identifier*>(stxs[0].get())->s;
    #ifdef DEBUG
    std::cout<<"dealing with operator:"<<op<<std::endl;
    #endif
    if(find(op,env).get()!=nullptr){
        #ifdef DEBUG
        std::cout<<"this is an function!"<<std::endl;
        #endif
        // Closure* closure_ptr = dynamic_cast<Closure*>(find(op,env).get());
        // std::cout<<"sucess"<<std::endl;

        // Expr exe_expr = closure_ptr->e;
        // std::cout<<"sucess"<<std::endl;

        // if(stxs.size()!= closure_ptr->parameters.size()+1) throw(RuntimeError("wrong parameter number"));
        // else{
        vector<Expr> to_expr;

        for(int i=1;i<stxs.size();i++) to_expr.push_back(stxs[i]->parse(env));
        return Expr(new Apply(stxs[0]->parse(env), to_expr));
        // }
    }
    else if(primitives.count(op)!=0){
        #ifdef DEBUG
        std::cout<<"found in primitives"<<std::endl;
        #endif
        switch (primitives[op])
        {
            case E_MUL:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Mult(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_MINUS:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Minus(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_PLUS:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Plus(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_LT:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Less(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_LE:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new LessEq(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_EQ:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Equal(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_GE:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new GreaterEq(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_GT:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Greater(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_VOID:
                if(stxs.size()!=1) throw(RuntimeError("wrong parameter number"));
                else return Expr(new MakeVoid());
                break;
            case E_EQQ:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsEq(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_BOOLQ:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsBoolean(stxs[1]->parse(env)));
                break;
            case E_INTQ:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsFixnum(stxs[1]->parse(env)));
                break;
            case E_NULLQ://need preperation
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsNull(stxs[1]->parse(env)));
                break;
            case E_PAIRQ://need preperation
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsPair(stxs[1]->parse(env)));
                break;
            case E_PROCQ://need preperation
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsProcedure(stxs[1]->parse(env)));
                break;
            case E_SYMBOLQ:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new IsSymbol(stxs[1]->parse(env)));
                break;
            case E_CONS:
                if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Cons(stxs[1]->parse(env),stxs[2]->parse(env)));
                break;
            case E_NOT:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Not(stxs[1]->parse(env)));
                break;
            case E_CAR:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Car(stxs[1]->parse(env)));
                break;
            case E_CDR:
                if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Cdr(stxs[1]->parse(env)));
                break;
            case E_EXIT:
                if(stxs.size()!=1) throw(RuntimeError("wrong parameter number"));
                else return Expr(new Exit());
                break;
            default:
                throw(RuntimeError("Keyword not found"));
                break;
        }
    }
    else if(reserved_words.count(op)!=0){
        #ifdef DEBUG
        std::cout<<"found in reserved words"<<std::endl;
        #endif
        switch (reserved_words[op])
        {
        case E_LET:
            // std::cout<<"letting!"<<std::endl;
            if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
            else{
                // std::cout<<"enter:"<<std::endl;
                // this->show(std::cout);
                vector<std::pair< std::string, Expr> > binded_vector;
                List* binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
                for(auto &stx_tobind_raw : binder_list_ptr->stxs){
                    List* stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
                    // stx_tobind->show(std::cout);
                    // std::cout<<std::endl;
                    // Syntax
                    if(stx_tobind->stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                    Identifier* temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
                    std::string var_name = temp_id->s;
                    env = extend(var_name, NullV(), env);
                    //temp:
                    Expr temp_store = (stx_tobind->stxs[1])->parse(env);
                    // std::cout<<"showing off: "<<temp_store->e_type<<std::endl;
                    
                    binded_vector.push_back(std::make_pair
                        (var_name, temp_store));
                }
                // std::cout<<"binded vector size: "<<binded_vector.size()<<std::endl;
                return Expr(new Let(binded_vector, stxs[2]->parse(env)));
            }
            break;
        case E_IF:
            if(stxs.size()!=4) throw(RuntimeError("wrong parameter number"));
            else
                return Expr(new If(stxs[1]->parse(env), stxs[2]->parse(env), stxs[3]->parse(env)));
            break;
        case E_BEGIN:
        {
            vector<Expr> passed_exprs;
            for(int i=1;i<stxs.size();i++)
                passed_exprs.push_back(stxs[i]->parse(env));
            return Expr(new Begin(passed_exprs));
        }
            break;
        case E_QUOTE:
            if(stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
            else
                return Expr(new Quote(stxs[1]));
            break;
        case E_LAMBDA:
            if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
            else{
                // std::cout<<"parsing a lambda!"<<std::endl;
                vector<std::string> paras;
                List* paras_ptr = dynamic_cast<List*>(stxs[1].get());
                for(auto &one_para : paras_ptr->stxs){
                    paras.push_back(dynamic_cast<Identifier*>(one_para.get())->s);
                }
                // std::cout<<"before pass"<<std::endl;
                Expr pass_expr=stxs[2]->parse(env);
                // std::cout<<"after pass"<<std::endl;
                return Expr(new Lambda(paras, pass_expr));
            }
            break;
        case E_LETREC:
            if(stxs.size()!=3) throw(RuntimeError("wrong parameter number"));
            else{
                vector<std::pair< std::string, Expr> > binded_vector;
                List* binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
                for(auto &stx_tobind_raw : binder_list_ptr->stxs){
                    List* stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());

                    if(stx_tobind->stxs.size()!=2) throw(RuntimeError("wrong parameter number"));
                    Identifier* temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
                    std::string var_name = temp_id->s;
                    env = extend(var_name, NullV(), env);   
                    //temp:
                    Expr temp_store = (stx_tobind->stxs[1])->parse(env);
                    binded_vector.push_back(std::make_pair
                        (var_name, temp_store));
                }
                return Expr(new Letrec(binded_vector, stxs[2]->parse(env)));
            }
            break;
        default:
            throw(RuntimeError("Keyword not found"));
            break;
        }
    }
    else{
        // throw(RuntimeError("operator not found"));
        #ifdef DEBUG
            std::cout<<"the default apply!"<<std::endl;
        #endif
        Expr opexpr = stxs[0]->parse(env);
        vector<Expr> to_expr;
        for(int i=1;i<stxs.size();i++) to_expr.push_back(stxs[i]->parse(env));
        return Expr(new Apply(opexpr, to_expr));
    }
}

#endif
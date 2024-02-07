#include "Def.hpp"
#include "value.hpp"
#include "expr.hpp"
#include "RE.hpp"
#include "syntax.hpp"
#include <cstring>
#include <vector>
#include <map>

extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

Value Let::eval(Assoc &env) { // let expression
    Assoc cur_env = env;
    std::vector< std::pair<std::string, Value> > tobind;
    // std::cout<<"We have "<<bind.size()<<" pairs to bind"<<std::endl;
    for(auto binded_pair : bind){
        tobind.push_back({binded_pair.first, binded_pair.second->eval(env)});
    }
    for(auto binded_pair: tobind){
        // std::cout<<"binding:"<<binded_pair.first<<" to ";
        // binded_pair.second->show(std::cout);
        // std::cout<<std::endl;
        cur_env = extend(binded_pair.first, binded_pair.second, cur_env);
    }

    // std::cout<<"binding is done!!!!!!!"<<std::endl;
    return body->eval(cur_env);
}

Value Lambda::eval(Assoc &env) { // lambda expression
    Assoc new_env=env;
    return ClosureV(x,e,new_env);
}

Value Apply::eval(Assoc &e) { // for function calling
    // Assoc clos_env = 
    Value mid_fun = rator->eval(e);
    Closure* clos_ptr = dynamic_cast<Closure*>(mid_fun.get());
    // Closure* clos_ptr = dynamic_cast<Closure*>(rator->eval(e).get());
    Assoc cur_env = clos_ptr->env;
    std::vector< std::pair<std::string, Value> > tobind;
    for(int i=0;i<clos_ptr->parameters.size();i++){
        tobind.push_back({clos_ptr->parameters[i], rand[i]->eval(e)});
    }
    for(auto binded_pair: tobind){
        // std::cout<<"!!binding "<<binded_pair.first<<" to ";
        // binded_pair.second->show(std::cout);
        // std::cout<<std::endl;
        cur_env = extend(binded_pair.first, binded_pair.second, cur_env);
    }
    return clos_ptr->e->eval(cur_env);
}

Value Letrec::eval(Assoc &env) { // letrec expression //modified version
    std::vector <Value> values;
    Assoc new_env=env;
    for(int i=0;i<bind.size();i++)
        new_env = extend(bind[i].first,Value(NULL),new_env);
    for(int i=0;i<bind.size();i++)
        values.push_back(bind[i].second->eval(new_env));
    for(int i=0;i<bind.size();i++)
        modify(bind[i].first,values[i],new_env);
    return body->eval(new_env);
}

Value Var::eval(Assoc &e) { // evaluation of variable
    Value matched_value = find(x, e);
    if(matched_value.get()==nullptr){
        // std::cout<<"failure when looking for var:"<<x<<std::endl;
        throw(RuntimeError("undefined variable"));
    }
    return matched_value;
}

Value Fixnum::eval(Assoc &e) { // evaluation of a fixnum
    return IntegerV(n);
}

Value If::eval(Assoc &e) { // if expression
    Value valueof_condition = cond->eval(e);
    if(valueof_condition->v_type == V_BOOL and dynamic_cast<Boolean*>(valueof_condition.get())->b == false)
        return alter->eval(e);
    else return conseq->eval(e);
}

Value True::eval(Assoc &e) { // evaluation of #t
    return BooleanV(true);
}

Value False::eval(Assoc &e) { // evaluation of #f
    return BooleanV(false);
}

Value Begin::eval(Assoc &e) { // begin expression
    if(es.size()==0) return NullV();
    else{
        for(int i=0;i<es.size()-1;i++)
            es[i]->eval(e);
        return es[es.size()-1]->eval(e);
    }
}

Value Quote::eval(Assoc &e) { // quote expression
    // Expr peek_value = 
    // std::cout<<"the datum is:"<<std::endl;
    // std::cout<<typeid(*(s.get())).name()<<std::endl;
    // s->show(std::cout);
    // std::cout<<std::endl;
    std::string typeof_stx = typeid(*(s.get())).name();
    if(typeof_stx == "10TrueSyntax") return BooleanV(true);
    else if(typeof_stx == "11FalseSyntax") return BooleanV(false);
    else if(typeof_stx == "4List"){
        std::vector<Syntax> stxs_got = dynamic_cast<List*>(s.get())->stxs;
        if(stxs_got.size()==0) return NullV();
        Value now_pair = Value(new Pair(stxs_got[stxs_got.size()-1]->parse(e)->eval(e), NullV()));
        for(int i=stxs_got.size()-2; i>=0;i--){
            now_pair = Value(new Pair(stxs_got[i]->parse(e)->eval(e), now_pair));
        }
        return now_pair;
    }
    else if(typeof_stx == "6Number") return IntegerV(dynamic_cast<Number*>(s.get())->n);
    else if(typeof_stx == "10Identifier") return SymbolV(dynamic_cast<Identifier*>(s.get())->s);
    else throw(RuntimeError("Unknown quoted typename"));
}

Value MakeVoid::eval(Assoc &e) { // (void)
    return VoidV();
}

Value Exit::eval(Assoc &e) { // (exit)
    return TerminateV();
}

Value Binary::eval(Assoc &e) { // evaluation of two-operators primitive
    return evalRator(rand1->eval(e),rand2->eval(e));
}

Value Unary::eval(Assoc &e) { // evaluation of single-operator primitive
    return evalRator(rand->eval(e));
}

Value Mult::evalRator(const Value &rand1, const Value &rand2) { // *
    // std::cout<<rand1->v_type<<' '<<rand2->v_type<<std::endl;
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) * (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Plus::evalRator(const Value &rand1, const Value &rand2) { // +
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) + (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Minus::evalRator(const Value &rand1, const Value &rand2) { // -
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) - (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Less::evalRator(const Value &rand1, const Value &rand2) { // <
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) < (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value LessEq::evalRator(const Value &rand1, const Value &rand2) { // <=
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) <= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Equal::evalRator(const Value &rand1, const Value &rand2) { // =
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) == (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value GreaterEq::evalRator(const Value &rand1, const Value &rand2) { // >=
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) >= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Greater::evalRator(const Value &rand1, const Value &rand2) { // >
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) > (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value IsEq::evalRator(const Value &rand1, const Value &rand2) { // eq?
    if(rand1->v_type==V_INT and rand2->v_type==V_INT)
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) == (dynamic_cast<Integer*>(rand2.get())->n));
    else if(rand1->v_type==V_BOOL and rand2->v_type==V_BOOL)
        return BooleanV((dynamic_cast<Boolean*>(rand1.get())->b) == (dynamic_cast<Boolean*>(rand2.get())->b));
    else if(rand1->v_type==V_SYM and rand2->v_type==V_SYM)
        return BooleanV((dynamic_cast<Symbol*>(rand1.get())->s) == (dynamic_cast<Symbol*>(rand2.get())->s));
    else{
        return BooleanV(rand1.get() == rand2.get());
    }
}

Value Cons::evalRator(const Value &rand1, const Value &rand2) { // cons
    return PairV(rand1,rand2);
}

Value IsBoolean::evalRator(const Value &rand) { // boolean?
    return BooleanV(rand->v_type == V_BOOL);
}

Value IsFixnum::evalRator(const Value &rand) { // fixnum?
    return BooleanV(rand->v_type == V_INT);
}

Value IsSymbol::evalRator(const Value &rand) { // symbol?
    return BooleanV(rand->v_type == V_SYM);
}

Value IsNull::evalRator(const Value &rand) { // null?
    return BooleanV(rand->v_type == V_NULL);
}

Value IsPair::evalRator(const Value &rand) { // pair?
    return BooleanV(rand->v_type == V_PAIR);
}

Value IsProcedure::evalRator(const Value &rand) { // procedure?
    return BooleanV(rand->v_type == V_PROC);
}

Value Not::evalRator(const Value &rand) { // not
    if(rand->v_type == V_BOOL and (dynamic_cast<Boolean*>(rand.get())->b == false))
        return BooleanV(true);
    else return BooleanV(false);
}

Value Car::evalRator(const Value &rand) { // car
    if(rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->car;
    else throw(RuntimeError("Wrong typename"));
}

Value Cdr::evalRator(const Value &rand) { // cdr
    if(rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->cdr;
    else throw(RuntimeError("Wrong typename"));
}

#include "Def.hpp"

std :: map<std :: string, ExprType> primitives;
std :: map<std :: string, ExprType> reserved_words;

void initPrimitives()
{
    // primitives stores all procedures in library, mapping them to ExprTypes
    primitives["*"] = E_MUL;//done
    primitives["-"] = E_MINUS;//done
    primitives["+"] = E_PLUS;//done
    primitives["<"] = E_LT;//done
    primitives["<="] = E_LE;//done
    primitives["="] = E_EQ;//done
    primitives[">="] = E_GE;//done
    primitives[">"] = E_GT;//done
    primitives["void"] = E_VOID;//done
    primitives["eq?"] = E_EQQ;//done
    primitives["boolean?"] = E_BOOLQ;//done
    primitives["fixnum?"] = E_INTQ;//done
    primitives["null?"] = E_NULLQ;//half done
    primitives["pair?"] = E_PAIRQ;//half done
    primitives["procedure?"] = E_PROCQ;//half done
    primitives["symbol?"] = E_SYMBOLQ;//done
    primitives["cons"] = E_CONS;//done
    primitives["not"] = E_NOT;//done
    primitives["car"] = E_CAR;//done
    primitives["cdr"] = E_CDR;//done
    primitives["exit"] = E_EXIT;//done? not sure
}

void initReservedWords()
{
    // reserved_words stores all reserved words, mapping them to bools
    reserved_words["let"] = E_LET;//done
    reserved_words["lambda"] = E_LAMBDA;
    reserved_words["letrec"] = E_LETREC;
    reserved_words["if"] = E_IF;//done
    reserved_words["begin"] = E_BEGIN;//done
    reserved_words["quote"] = E_QUOTE;//done?
}

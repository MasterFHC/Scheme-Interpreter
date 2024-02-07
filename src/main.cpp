#include "Def.hpp"
#include "syntax.hpp"
#include "expr.hpp"
#include "value.hpp"
#include "RE.hpp"
#include <sstream>
#include <iostream>
#include <map>


extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

void REPL()
{
    // read - evaluation - print loop
    Assoc global_env = empty();
    while (1)
    {
        printf("scm> ");
        Syntax stx = readSyntax(std :: cin); // read
        // stx -> show(std::cout);/
        // std::cout<<std::endl;
        // std::cin.get();
        try
        {
            // stx -> show(std :: cout); // syntax print
            Expr expr = stx -> parse(global_env); // parse

            // std::cout<<"end of parse!!!"<<std::endl;
            Value val = expr -> eval(global_env);
            if (val -> v_type == V_TERMINATE)
                break;
            val -> show(std :: cout); // value print
        }
        catch (const RuntimeError &RE)
        {
            // std :: cout << RE.message();
            std :: cout << "RuntimeError";
        }
        puts("");
    }
}


int main(int argc, char *argv[]) {
    initPrimitives();
    initReservedWords();
    REPL();
    return 0;
}

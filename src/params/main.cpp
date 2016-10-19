#include <iostream>
#include <param.tab.hpp>
#include <params/paramConstructor.hpp>

ParamConstructor pc;
int main()
{
    yy::parser* p=new yy::parser();
    p->parse();
    std::cout << pc.get()->getString();
    return 0;
}   

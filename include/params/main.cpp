#include <iostream>
#include "param.tab.hpp"


int main()
{
    yy::parser* p=new yy::parser();
    p->parse();
    return 0;
}   

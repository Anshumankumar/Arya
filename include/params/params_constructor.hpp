#ifndef PARAM_CONSTRUCTOR_HPP
#define PARAM_CONSTRUCTOR_HPP

#include <stack>
#include "params.hpp"
ParamConstructor
{
    std::stack<Params*> paramsStack;
    Params* params;
    public:
    ParamConstructor()
    {
        params = new Params       
    }
    Param get()
    {
    }
    int add<T>(std::string key, T value,  int tabCount)  
    {
        if (tabCount+1 > stack.size())
        {
            return -1;
        }

        for (int i=0; i<stack.size()-(tabCount+1),i++)
        {
            paramsStack.pop();
        }

        paramsStack.top()->setParam<T>(key,value);   
        if (typeId(T).name == typeId(Params).name)
        {
            paramStack.push(&paramsStack.top()->get<T>(value));
        }
        return 1;


    }
}



#endif //PARAM_CONSTRUCTOR

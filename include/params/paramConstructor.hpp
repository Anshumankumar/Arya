#ifndef PARAM_CONSTRUCTOR_HPP
#define PARAM_CONSTRUCTOR_HPP

#include <stack>
#include <params/params.hpp>
class ParamConstructor
{
    std::stack<Params*> paramStack;
    Params* params;
    public:
    ParamConstructor()
    {
        params = new Params;  
        paramStack.push(params);
    }
    void reset()
    {
        delete params;
        params = new Params;
        while(!paramStack.empty()) paramStack.pop();
        paramStack.push(params);
        
    } 
    ~ParamConstructor()
    {
        delete params;
    }
    Params* get()
    {
        return params;
    }
    template <typename T>
    int add(std::string key, T value,  int tabCount)  
    {
        if (tabCount+1 > paramStack.size()) return -1;
        for (int i=0; i<paramStack.size()-(tabCount+1); i++) paramStack.pop();
        paramStack.top()->setParam<T>(key,value);
        postrun<T>(key);  
        return 1;
    }
    template <typename T> void postrun(std::string key){}
    
};

template<> void ParamConstructor::postrun<Params>(std::string key);


#endif //PARAM_CONSTRUCTOR

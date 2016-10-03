#ifndef ARYA_PARAMS_HPP
#define ARYA_PARAMS_HPP

#include <unordered_map>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>

class ParamBase
{
    friend class Params;   
    protected:
    std::string t_info;
    virtual void* get()=0;
    virtual std::shared_ptr<ParamBase> copy()=0;
};


typedef std::unordered_map<std::string,std::shared_ptr<ParamBase>> ParamMap;

template <class T=double>
class Param: public ParamBase
{
    friend class Params;   
    private:
    std::shared_ptr<T> value;

    Param(T value)
    {
        this->value = std::shared_ptr<T>(new  T(value));
        t_info = typeid(T).name();
    }

    std::shared_ptr<ParamBase> copy()
    {
        return std::shared_ptr<ParamBase>(new Param<T>(*value));
    }
    void* get()
    {
        return (void*) &(*value);
    }
};


class Params
{

    ParamMap params;

    public:
    Params(){}
    Params(const Params &p)
    {
        for (auto& x: p.params)
        {
            this->params[x.first] = x.second->copy();
        }
    }
    private:
    template <class T> void check(std::string key)
    {
        if (params.find(key) == params.end())
        {
            std::cerr << "Not got param with key: " << key << std::endl;
            exit(2);
        }
        if (typeid(T).name() != params[key]->t_info)
        {
            std::cout << "Availbale: "<< typeid(T).name() << " \n";
            std::cout << "Type Mismatch: Requested: "  << params[key]->t_info << "\n" ;
            exit(3);
        }

    }

    public:
    template <class T=Params> T* getPtr(std::string key)
    {
        check<T>(key);
        void *value = params[key]->get();
        return (T*)value;

    }
    template <class T=Params> T get(std::string key)
    {
        check<T>(key);
        void *value = params[key]->get();
        return *(T*)value;
    }

    template <class  T> void setParam(std::string key, T value)
    {
        std::shared_ptr<ParamBase> param(new Param<T>(value));
        if (params.find(key) != params.end())
        {
            //  delete params[key];
        }
        params[key]= param; 
    }
};

#endif //ARYA_PARAM_HPP
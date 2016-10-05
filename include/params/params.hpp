#ifndef ARYA_PARAMS_HPP
#define ARYA_PARAMS_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>

class ParamBase
{
    friend class Params;   
    friend class ParamVector;   
    protected:
    std::string t_info;
    virtual void* get()=0;
    virtual std::shared_ptr<ParamBase> copy()=0;
};


typedef std::unordered_map<std::string,std::shared_ptr<ParamBase>> ParamMap;

template <typename T=double>
class Param: public ParamBase
{
    friend class Params;   
    friend class ParamVector;   
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
    template <typename T> void check(std::string key)
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
    template <typename T=Params> T* getPtr(std::string key)
    {
        check<T>(key);
        void *value = params[key]->get();
        return (T*)value;

    }
    template <typename T=Params> T get(std::string key)
    {
        check<T>(key);
        void *value = params[key]->get();
        return *(T*)value;
    }

    template <typename  T> void setParam(std::string key, T value)
    {
        std::shared_ptr<ParamBase> param(new Param<T>(value));
        if (params.find(key) != params.end())
        {
            //  delete params[key];
        }
        params[key]= param; 
    }
};


class ParamVector
{
    public:
    std::vector<std::shared_ptr<ParamBase>> pList;
    ParamVector(){}
    ParamVector(const ParamVector &p)
    {
        for (auto& x: pList)
        {
            pList.push_back(x->copy());    
        } 
    }
    
    template <typename T=Params>
    void add(T value)
    {
        std::shared_ptr<ParamBase> param(new Param<T>(value));
        pList.push_back(param);
    }

    template <typename T=Params>
    void get(int i)
    {
        void *value = pList[i]->get();
        return *(T*)value;
    }
};

#endif //ARYA_PARAM_HPP

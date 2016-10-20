#ifndef ARYA_PARAMS_HPP
#define ARYA_PARAMS_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <sstream>

class ProcessBase;

class ParamBase
{
    friend class Params;   
    friend class ParamVector;   
    friend class ProcessBase;   
    protected:
    std::string t_info;
    virtual std::shared_ptr<void> get()=0;
    virtual std::string getString(int tcount=0)=0;
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
    std::shared_ptr<void>  get()
    {
        return std::static_pointer_cast<void>(value);
    }
    std::string getString(int tcount=0);
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
    Params(Params *p)
    {
        for (auto& x: p->params)
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
            std::cerr << "Availbale: "<< typeid(T).name() << " \n";
            std::cerr << "Type Mismatch: Requested: "  << params[key]->t_info << "\n" ;
            exit(3);
        }

    }

    public:
    std::shared_ptr<ParamBase> getParam(std::string key)
    {
        if (params.find(key) == params.end()) return nullptr; 
        else return params[key];
    }
    template <typename T=Params> T& get(std::string key)
    {
        check<T>(key);
        auto value = params[key]->get();
        return *std::static_pointer_cast<T>(value);
    }

    template <typename T=Params> std::shared_ptr<T> getPtr(std::string key)
    {
        check<T>(key);
        return std::static_pointer_cast<T>(params[key]->get());
    }

    template <typename  T> void setParam(std::string key, T value)
    {
        std::shared_ptr<ParamBase> param(new Param<T>(value));
        params[key]= param; 
    }

    std::string getString(int tabCount=0)
    {
        std::stringstream ss;
        for(int i=0; i< tabCount; i++)
        {
            ss << "    ";
        }
        std::stringstream ss2;
        for (auto& it:params)
        {
            ss2 << ss.str() << it.first << ":" << it.second->getString(tabCount);
        }
        return ss2.str(); 
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

        template <typename T=Params> void add(T value)
        {
            std::shared_ptr<ParamBase> param(new Param<T>(value));
            pList.push_back(param);
        }

        template <typename T=Params> void get(int i)
        {
            void *value = pList[i]->get();
            return *(T*)value;
        }
};

template <>  std::string Param<std::string>::getString(int tcount);
template <>  std::string Param<const char *>::getString(int tcount);
template <> std::string Param<Params>::getString(int tcount);

template <typename T> std::string Param<T>::getString(int tcount)
{
    std::stringstream ss;
    ss <<std::showpoint << *value << "\n";
    return ss.str();
}

extern Params parseParam(std::string file);
#endif //ARYA_PARAMS_HPP

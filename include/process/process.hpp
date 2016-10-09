#ifndef ARYA_PROCESS_HPP
#define ARYA_PROCESS_HPP
#include <params/params.hpp>
#include <vector>
#include <string>

class ProcessBase;
using ProcessPtr = std::shared_ptr<ProcessBase>;

class ProcessBase
{
    protected:
    std::vector<ProcessPtr> subPsList;
    std::unordered_map<std::string,ProcessPtr> subPsMap;
    std::weak_ptr<ProcessBase>  parent;
    public:
    virtual std::shared_ptr<const void> run(std::shared_ptr<const void> in)=0;
    std::string name;
    Params* params;
    ProcessBase(std::string name,Params *params=NULL)
    {
        this->name = name;
        if(params==NULL)
        {
            this->params = new Params();
        }
        else
        {
            this->params=params;
        }
        this->params->setParam("debug",false);
    }
    ~ProcessBase()
    {
        delete this->params;
    }
    void setParam(Params* params)
    {
        this->params = params;
    }

    void addSubProcess(ProcessBase* ps)
    {
        /* use only with new class Don't repeat the class*/
        addSubProcess(std::shared_ptr<ProcessBase>(ps));
    } 
    void addSubProcess(ProcessPtr ps)
    {
        std::string subPsName = ps->name;
        subPsList.push_back(ps);
       // ps->parent = std::weak_ptr<ProcessBase>(this);
        subPsMap[subPsName] = ps;
    }
};

template <typename A, typename B=A>
class Process: public ProcessBase
{
    public:
    using ProcessBase::ProcessBase;
    using Input = A;
    using Output = B;

    virtual Output run(const Input &in) = 0;
    virtual void runDebug(const Input &in, const Output &out){}
    std::shared_ptr<const void> run(std::shared_ptr<const void> in)
    {
        return std::shared_ptr<const void>(new Output(
                this->run(*std::static_pointer_cast<const Input>(in))));
     /*   if (params->get<bool>("debug")) runDebug(*(Input*)in, *out);
        return (void*) out;*/
    }
};

template <typename A, typename B=A>
class SerialProcess: public Process<A,B>
{
    public: 
    using Process<A,B>::Process;
    using Output = typename Process<A, B>::Output;
    using  Input = typename Process<A, B>::Input;
    Output run(const Input &in)
    {
        auto inptr = std::shared_ptr<const void>(new Input(in));
        for(auto ps:this->subPsList)
        {

            auto outptr = ps->run(inptr);      
            inptr = outptr;
        }
        return *std::static_pointer_cast<const Output>(inptr);
    }
};
/*
template <typename A, typename B=A, typename C=std::vector<B>>
class ParallelProcess:public Process<A,C>
{
    public:
    using Process<A,B>::Process;
    using Output = typename Process<A, B>::Output;
    using Input = typename Process<A, B>::Input;
 
    Output run(const Input &in)
    { 
        std::vector<B> vec;
        const void *inptr = &in;
        for(auto ps:this->subPsList)
        {
            vec.push_back(*(Output*)ps->run(inptr));
        }
        return vec;
    } 
    virtual C merge(std::vector<B> in)
    {
        return in;
    }
};

template <typename A, typename B=A>
class SelectorProcess:public Process<A,B>
{
    public:
    using Process<A,B>::Process;
    using Output = typename Process<A, B>::Output;
    using Input = typename Process<A, B>::Input;
 
    Output run(const Input &in)
    {
        std::string sProcess = this->params->template get<bool>("process");
        return *(Output*)this->subPsMap[sProcess]->run(&in);
    } 
};
*/

#endif // ARYA_PROCESS_HPP

#ifndef ARYA_PROCESS_HPP
#define ARYA_PROCESS_HPP
#include <params/params.hpp>
#include <vector>

class ProcessBase;
using ProcessPtr = std::shared_ptr<ProcessBase>;

class ProcessBase
{
    protected:
    std::vector<ProcessPtr> subPsList;
    std::unordered_map<std::string,ProcessPtr> subPsMap;
    ProcessPtr parent;
    virtual void* run(void *in)=0;
    public:
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
    void setParam(Params* params)
    {
        this->params = params;
    }
    void addSubProcess(ProcessBase* ps)
    {
        addSubProcess(ProcessPtr(ps));
    } 
    
    void addSubProcess(ProcessPtr ps)
    {
        std::string subPsName = ps->name;
        subPsList.push_back(ps);
        ps->parent = ProcessPtr(this);
        subPsMap[subPsName] = ps;
    }
};

template <class A, class B=A>
class Process: public ProcessBase
{
    public:
    using ProcessBase::ProcessBase;
    using Input=A;
    using Output=B;
    virtual Output run(const Input &in) = 0;
    virtual void runDebug(const Input &in, const Output &out){}
    void* run(void *in)
    {
        Output *out = new Output(this->run(*(Input*)in));
        if (params->get<bool>("debug")) runDebug(*(Input*)in, *out);
        return (void*) out;
    }
};


#endif // ARYA_PROCESS_HPP

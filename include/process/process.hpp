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
    public:
    std::string name;
    Params* params;
    ProcessBase(Params *params=NULL)
    {
        if(params==NULL)
        {
            this->params = new Params();
        }
        else
        {
            this->params=params;
        }
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
        std::string subProcName = ps->name;
        subPsList.push_back(ps);
        ps->parent = ProcessPtr(this);
        subPsMap[subProcName] = ps;
    }
};

template <class A, class B=A>
class Process: public ProcessBase
{
    using Input=A;
    using Output=B;
    virtual Output run(Input in) = 0;
};
#endif // ARYA_PROCESS_HPP

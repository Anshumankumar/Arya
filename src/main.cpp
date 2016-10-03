#include <params/params.hpp>
#include <process/process.hpp>
#include <iostream>
#include <vector>
#include <AryaConfig.h>

class P1:public Process<int,int>
{
    public:
    using Process::Process;
    Output run(const Input &in)
    {
        return 2*in;
    }
};

int main()
{
    std::cout << "Welcome to Arya\n";
    std::cout << "Current Version is: " <<  Arya_VERSION_MAJOR << "." << Arya_VERSION_MINOR << "\n";
    Params p;
    p.setParam("a1",1);
    p.setParam("a2",2);
    P1 pb1 = P1("pb1");
    ProcessBase *pb2 = new P1("pb2",&p);
    std::cout << pb1.run(4) <<"\n";
    pb2->params->setParam("a2",3);
    std::cout << p.get<int>("a2");
    return 0;
}

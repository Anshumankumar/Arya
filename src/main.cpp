#include <params/params.hpp>
#include <process/process.hpp>
#include <params/paramConstructor.hpp>
#include <iostream>
#include <vector>
#include <AryaConfig.h>

class P1:public Process<double,double>
{
    public:
    using Process::Process;
    Output run(const Input &in)
    {
        std::cout << in << "\n";
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
    p.setParam("a3","Anshuman");
    p.setParam("a4",2.0);
    p.setParam("a5",true);
    p.setParam("a6",p);
    std::cout << p.getString();
/*    std::cout << p.get<int>("a1") << "\n";
    SerialProcess<double> sp("s1");
    sp.addSubProcess(new P1("pb1"));
    sp.addSubProcess(new P1("pb2"));
    std::cout << sp.run(2) <<"\n"; */
    ParamConstructor pc;
    pc.add("an",123,0);
    pc.add("an2",123.0,0);
    pc.add("p6",*pc.get(),0);
    pc.add("p2",123.0,1);
    std::cout << pc.get()->getString();
    return 0;
}

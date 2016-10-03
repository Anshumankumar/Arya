#include <params/params.hpp>
#include <process/process.hpp>
#include <iostream>
#include <vector>
#include <AryaConfig.h>
int main()
{
    std::cout << "Welcome to Arya\n";
    std::cout << "Current Version is: " <<  Arya_VERSION_MAJOR << "." << Arya_VERSION_MINOR << "\n";
    Params p;
    p.setParam("a1",1);
    p.setParam("a2",2);
    ProcessBase *pb1 = new ProcessBase();
    ProcessBase *pb2 = new ProcessBase(&p);
    pb2->params->setParam("a2",3);
    std::cout << p.get<int>("a2");
    return 0;
}

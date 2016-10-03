#include <params/params.hpp>
#include <iostream>
#include <vector>
#include <AryaConfig.h>
int main()
{
    std::cout << "Welcome to Arya\n";
    std::cout << "Current Version is: " <<  Arya_VERSION_MAJOR << "." << Arya_VERSION_MINOR << "\n";
    Params p;
    p.setParam("ansuhman",1);
    p.setParam("anshman",2);
    Params p2;
    p2.setParam("p",p);
    p2.setParam("p2",p);
    std::cout << p.get<int>("ansuhman") << "\n";
    p.setParam("anshman",3);
    std::cout << p.get<int>("anshman") << "\n";
    std::cout << p2.get("p").get<int>("anshman") << "\n";
    p2.getPtr("p2")->setParam<std::string>("anshman","Anshuman");
    std::cout << p2.get("p").get<int>("anshman") << "\n";
    std::cout << p2.get("p2").get<std::string>("anshman") << "\n";
    Params p3 = Params(p2);
    p2.getPtr("p2")->setParam<std::string>("anshman","Anshuman123");
    std::cout << p2.get("p2").get<std::string>("anshman") << "\n";
    std::cout << p3.get("p2").get<std::string>("anshman") << "\n";
    std::ve
    return 0;
}

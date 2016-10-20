#ifndef ML_MAIN_HPP
#define ML_MAIN_HPP

#include <process/process.hpp>

class CsvParser:public Process<std::string,std::string>
{
    public:
    using Process<Input,Output>::Process;
    Output run(const Input & in)
    {
        return in;
    }
};

class TrainingSelector:public Process<std::string,std::string>
{
    public:
    using Process<Input,Output>::Process;
    Output run(const Input & in)
    {
        return in;
    }
};



class MlMain:public SerialProcess<std::string,std::string>
{
    public:
    using SerialProcess<Input,Output>::SerialProcess;
    MlMain(std::string name):SerialProcess(name)
    {
        this->addSubProcess(new CsvParser("csvParser"));
        this->addSubProcess(new TrainingSelector("selector"));
    } 
};


int main()
{
    MlMain mlMain = MlMain("mlMain");
    Params p = new Params(parseParam("/home/anshuman/Personal/arya/config/ml.param"));
    mlMain.setParam(std::make_shared<Params>(p));
    std::cout << mlMain.run("Ansuhman") << "\n";
}
#endif 

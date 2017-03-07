#include <ml/optimiser.hpp>
#include <ml/model.hpp>
#include <ml/baseMl.hpp>
#include <ml/dataHandler.hpp>

int main()
{
    Params p = new Params(parseParam("/home/anshuman/Personal/arya/config/base.param")); 
    std::string modelPath = p.get<std::string>("modelPath");
    Model model(modelPath);
    LogisticRegression lr("lr");
    lr.setParam(std::make_shared<Params>(p));
    lr.run(&model);
    model.saveModel(modelPath);
    return 0;
}

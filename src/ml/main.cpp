#include <ml/optimiser.hpp>
#include <ml/model.hpp>

class NN
{
    public: 
        void updategradient(Model * model, Model *gmodel)
        {
            std::cout << "updating gradient\n"; 
        }
        double getCost(Model *model)
        {
            return 0;
        }
};
int main()
{
    Params param;
    param.setParam("noOfLayer",2);
    param.setParam("s1_0",10);
    param.setParam("s2_0",10);
    param.setParam("s1_1",10);
    param.setParam("s2_1",10);
    Model model(param);
    model.randomIntialize();
    model.set(1,5,5,10.0);
    std::cout << model.get(1,5,5);
    model.saveModel("/home/anshuman/Personal/arya/config/model");
    NN nn;
    using namespace std::placeholders;
    Params p2; p2.setParam("alpha",1.0);
    Optimiser * optim = new GradientDescent(std::bind(&NN::updategradient,&nn,_1,_2), std::bind(&NN::getCost,&nn,_1));
    optim->setParam(std::make_shared<Params>(p2));
    optim->run(&model);
    return 0;    
}

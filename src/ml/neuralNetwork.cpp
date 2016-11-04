#include <ml/neuralNetwork.hpp>
#include <numeric>
NeuralLayer::Output NeuralLayer::run(const Input &in)
{
    int outputLen = PGET<int> ("outputLen");
    auto weightMat = PGET<Matrix*>("weight");
    
    if (in->type==Vector::SPARSE_VECTOR)
    {
        result = new Vector(Vector::DENSE_VECTOR);
        result->value.resize(weightMat->size());
        int i =0;
        for (auto &weights: (*weightMat))
        {
            result->value[i] =0;
            for (auto &v:in->index)
            {
                result->value[i] += weights[v];
            }
            i++;
        }
    }
    else
    {
        result = new Vector(Vector::DENSE_VECTOR);
        result->value.resize(weightMat->size());
        int i =0;
        for (auto &weights: (*weightMat))
        {
            result->value[i] = std::inner_product(weights.begin(),weights.end(),in->value.begin(),0.0);
            i++;
        }

    }
    return result;
}


void NeuralNetwork::preRun()
{
    noOfLayer = PGET<int>("noOfLayer");
    auto forwardPropagation = new SerialProcess<Vector*,Vector*>("fp"); 
    auto backwardPropagation = new SerialProcess<Vector*,Vector*>("bp"); 
    std::cout << noOfLayer  << "\n";
    for (int i=0;i<noOfLayer-1;i++)
    {
        std::shared_ptr<NeuralLayer> nl= std::shared_ptr<NeuralLayer>(new NeuralLayer("layer" + std::to_string(i)));
        forwardPropagation->addSubProcess(nl);
    }
    for (int i= noOfLayer-2; i>=0;i--)
    {
        backwardPropagation->addSubProcess(forwardPropagation->subPsList[i]);
    }
    this->addSubProcess(forwardPropagation);
    this->addSubProcess(backwardPropagation);
}

NeuralNetwork::Output NeuralNetwork::run(const Input &in)
{
    if (in->isEmpty()) in->createModel(noOfLayer,PGET<Params>("fp"));
    in->resize(2);
    Vector* vec = new Vector(Vector::SPARSE_VECTOR);
    vec->index.push_back(0);
    vec->index.push_back(0);
    auto sp = std::static_pointer_cast<SerialProcess<Vector*,Vector*>> (this->subPsMap["fp"]);
    int i=0;
    for(auto subPs:this->subPsMap["fp"]->subPsList)
        subPs->params->setParam<Matrix*>("weight",in->getWeight(i++));
    sp->run(vec);
    return in;
}

class SP:public SerialProcess<Model*,Model*>
{
    public:
        using SerialProcess::SerialProcess;
    void preRun()
    {
        int mtoken =  PGET<int>("maxToken");
        setCommonProcess(ProcessPtr (new Indexer("indexer",mtoken)));
    }

};


int main()
{
    Model model;
    SP sp("sp"); 
    sp.addSubProcess(new NeuralNetwork("neuralNetwork"));
    Params p = new Params(parseParam("/home/anshuman/Personal/arya/config/nn.param"));
    sp.setParam(std::make_shared<Params>(p));
    sp.run(&model);
    return 0;
}

#include <ml/neuralNetwork.hpp>
#include <numeric>
#include <algorithm>
#include <cmath>

inline double sigmoid(double value)
{
    return 1/(1+exp(-value));
}

inline double diffsigmoid(double value)
{
        return sigmoid(value)*(1-sigmoid(value));
}
NeuralLayer::Output NeuralLayer::run(const Input &in)
{
    if (PGET<bool>("back")) return bp(in);
    else return fp(in); 
}
NeuralLayer::Output NeuralLayer::bp(const Input &in)
{
   // std::cout << "BP\n";
    auto weightMat = PGET<Matrix*>("weight"); 
    auto gWeight = PGET<Matrix*>("gweight"); 
    Vector v(Vector::DENSE_VECTOR);
    Vector v2(Vector::DENSE_VECTOR);
    v.value.resize((*input)[0].value.size());
    v2.value.resize((*weightMat)[0].size());
    for (auto &weights:(*gWeight))
    {
        for (auto &weight:weights) weight =0;
    }
    if ((*input)[0].type == Vector:: SPARSE_VECTOR)
    {
        for (int i=0; i<input->size(); i++)
        {
            for (auto& j:(*input)[i].index)
            {
                for (int k=0;k<(*weightMat).size();k++)
                {
                    (*gWeight)[k][j] +=(*in)[i].value[0];
                }
            }

        }

    }
    else
    {
        bresult.resize(result.size());
        for (int i=0; i<input->size(); i++)
        {
            std::transform((*input)[i].value.begin(),(*input)[i].value.end(),
                    (*in)[i].value.begin(),v.value.begin(),[&](auto a,auto b)
                    {
                    return a*(1-a)*b;
                    });
            for (int j=0; j<(*weightMat)[0].size();j++)
            {
                double delta = 0;
                for (int k=0;k<(*weightMat).size();k++)
                {
                    delta += (*weightMat)[k][j]*v.value[k];
                    (*gWeight)[k][j] +=(*in)[i].value[k]*(*input)[i].value[j];
                }
                v2.value[j] = delta;
            }
            bresult[i] = v2;

        }
    }
    for (auto &weights:(*gWeight))
    {
        for (auto &weight:weights) 
        {   
            weight = weight/input->size();
        //    std::cout << weight << "\n";
        }
    }

    return &bresult;
}
NeuralLayer::Output NeuralLayer::fp(const Input &in)
{
    input = in;
    int outputLen = PGET<int> ("outputLen");
    auto weightMat = PGET<Matrix*>("weight"); 
    result.resize(in->size());
    if ((*in)[0].type==Vector::SPARSE_VECTOR)
    {   
        std::transform(in->begin(),in->end(),result.begin(),[&] (auto a)
            {
                Vector vec(Vector::DENSE_VECTOR);
                vec.value.resize(weightMat->size());
                int i =0;
                for (auto &weights: (*weightMat))
                {
                    vec.value[i] =0;
                    for (auto &v:a.index)
                    {
                        vec.value[i] += weights[v];
                    }
                    vec.value[i] = sigmoid(vec.value[i]);
                //    std::cout << vec.value[i] << "\n";
                    i++;
                }
                return vec;
            });
    }
    else
    {
        std::transform(in->begin(),in->end(),result.begin(),[&] (auto a)
                {
                Vector vec(Vector::DENSE_VECTOR);
                vec.value.resize(weightMat->size());
                int i =0;
                for (auto &weights: (*weightMat))
                {
                vec.value[i] = std::inner_product(weights.begin(),weights.end(),a.value.begin(),0.0);
                vec.value[i] = sigmoid(vec.value[i]);
                //    std::cout << vec.value[i] << "\n";
                i++;
                }
                return vec;
                });

    }
    return &result;
}


using PropProcess = SerialProcess<std::vector<Vector>*, std::vector<Vector>*>;
void NeuralNetwork::preRun()
{
    noOfLayer = PGET<int>("noOfLayer");
    auto forwardPropagation = new PropProcess("fp"); 
    auto backwardPropagation = new PropProcess("bp"); 
    std::cout << noOfLayer  << "\n";
    for (int i=0;i<noOfLayer-1;i++)
    {
        auto nl= ProcessPtr(new NeuralLayer("layer" + std::to_string(i)));
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
    in->loadFile(PGET<std::string>("weightFolder"));
    Model gModel;
    if (in->isEmpty()) in->createModel(noOfLayer,PGET<Params>("fp"));
    auto indexer = std::static_pointer_cast<Indexer>(PRGET("indexer"));
    auto parser = std::static_pointer_cast<CsvParser<Row>>(PRGET("csvParser"));
    parser->run( PGET<std::string>("csvFile"));
    indexer->setWeight(in->getWeight(0));
    std::cout <<" size: "<<in->getWeight(0)->size() << "\n";
   // indexer->run("");
    int n = PGET<int>("maxDataLen"); 
    auto fp = std::static_pointer_cast<PropProcess>(this->subPsMap["fp"]);
    auto bp = std::static_pointer_cast<PropProcess>(this->subPsMap["bp"]);
    std::cout << "Reading File Started\n";
    auto t = parser->getNext(n); 
    std::cout << "Reading File Completed\n";

    gModel.createModel(in);
    int i=0;
    for(auto subPs:fp->subPsList)
    {
        subPs->params->setParam<Matrix*>("weight",in->getWeight(i));
        subPs->params->setParam<Matrix*>("gweight",gModel.getWeight(i++));

    }

    std::vector<double>value(n);
    std::vector<Vector> del(n,Vector(Vector::DENSE_VECTOR));
    for (int i=0;i<n;i++) del[i].value.resize(1);
    for (int j=0;j<PGET<int>("iterCount");j++)
    { 

        for(auto subPs:fp->subPsList)
        {
            subPs->params->setParam<bool>("back",false);

        }

        auto r= fp->run(&t.second);
        if (j == PGET<int>("iterCount") -1 )
        {
            int ones=0; int zeros=0;
            double pos=0; double neg=0;
            for (int k=0;k<n;k++)
            {
                if (t.first[k] == 1)
                {
                    pos += (*r)[k].value[0];
                    ones++;
                }
                else
                {
                    neg +=  (*r)[k].value[0];
                    zeros++;
                }
            }
            std::cout << "AVG" << pos/ones << " " << neg/zeros <<"\n";
        }
        double cost = 0;
        for (int i=0;i<n;i++)
        {
            cost += -t.first[i]*log((*r)[i].value[0])-(1-t.first[i])*log(1-(*r)[i].value[0]);
            del[i].value[0] = (*r)[i].value[0]-t.first[i];
        }
        cost = cost/n;
        std::cout << cost << "\n";
        for(auto subPs:fp->subPsList)
        {
            subPs->params->setParam<bool>("back",true);
        }

        auto result= bp->run(&del);
        in->updateModel(&gModel, PGET<double>("alpha"));
        //std::cout << (*result)[0].value[0] << "\n";
    }
    in->saveWeight(PGET<std::string>("weightFolder"));
    return in;
}

class SP:public SerialProcess<Model*,Model*>
{
    public:
        using SerialProcess::SerialProcess;
        void preRun()
        {
            int mtoken =  PGET<int>("maxToken");
            ProcessPtr indexer = ProcessPtr (new Indexer("indexer",mtoken));
            ProcessPtr parser = ProcessPtr(new CsvParser<Row>("csvParser"));
            this->setCommonProcess(indexer);
            this->setCommonProcess(parser);
            parser->setCommonProcess(indexer);
            parser->params->setParam<int>("maxToken",mtoken);
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

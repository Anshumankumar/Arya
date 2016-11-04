#ifndef ARYA_NEURAL_NETWORK_HPP
#define ARYA_NEURAL_NETWORK_HPP

#include <random>
#include <process/process.hpp>

using Matrix = std::vector<std::vector<double>>;
class Model
{
    std::vector<Matrix> weightMats;
    public:
    bool isEmpty()
    {
        return (weightMats.size() == 0);
    }
    void createModel(int n, Params p)
    {
       weightMats.resize(n);
        int prevLen = -1;
        for (int i=0; i< n-1;i++)
        {
            auto oLen = p.get<Params>("layer"+ std::to_string(i)).get<int>("outputLen");
            weightMats[i].resize(oLen);
            if (prevLen != -1)
            {
                 fillWeight(i,prevLen);
            }
            prevLen = oLen;
        }
        
    }
    void fillWeight(int i, int len)
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(-1.0,1.0); 
        for (auto &weights:weightMats[i])
        {   weights.resize(len);
            for (auto &weight:weights) weight= distribution(generator);
        }
    }
    void resize(int n)
    {
        for (auto &weight:weightMats[0])
            weight = std::vector<double>(n,1);
    }
    Matrix* getWeight(int i)
    {
        return &weightMats[i]; 
    }

};



class Vector
{
    public:
    int type;
    enum
    {
        DENSE_VECTOR=1,
        SPARSE_VECTOR=2
    };
    Vector(int type)
    {
        this->type = type;
    }
    
    std::vector<int> index;
    std::vector<double> value;
};


class NeuralLayer:public Process<Vector*, Vector*>
{
    Vector* result;
    public:
        using Process::Process;
        Output run(const Input &in);
};

class NeuralNetwork:public Process<Model*,Model*>
{
    int noOfLayer;
    public:
    using Process::Process;
    Output run(const Input &in);    
    void preRun();    
};


#endif //ARYA_NEURAL_NETWORK_HPP

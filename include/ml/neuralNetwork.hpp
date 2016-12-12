#ifndef ARYA_NEURAL_NETWORK_HPP
#define ARYA_NEURAL_NETWORK_HPP

#include <random>
#include <algorithm>
#include <process/process.hpp>
#include <ml/indexer.hpp>
#include <ml/csvParser.hpp>
#include <fstream>
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
       weightMats.resize(n-1);
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
    void createModel(Model* model)
    {
       weightMats.resize(model->weightMats.size());
        for (int i=0; i< model->weightMats.size();i++)
        {
            weightMats[i].resize(model->weightMats[i].size());
            for (int j=0; j< model->weightMats[i].size();j++)
            {
                weightMats[i][j].resize(model->weightMats[i][j].size());
            }
        }
        
    }
    void updateModel(Model* model, double alpha)
    {
        for (int i=0; i< model->weightMats.size();i++)
        {
            for (int j=0; j< model->weightMats[i].size();j++)
            {
                std::transform(weightMats[i][j].begin(),
                              weightMats[i][j].end(),
                              model->weightMats[i][j].begin(),
                              weightMats[i][j].begin(), [&](auto a, auto b)
                              {return a-alpha*b;}
 
                                );
            }
        }
        
    }

    void fillWeight(int i, int len)
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(-0.1,0.1); 
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
    void saveWeight(std::ofstream &file, Matrix &weightMat)
    {
        for (auto  &weights:weightMat)
        {
            for (auto &weight:weights) file << weight << " ";
            file << "\n";
        }
    }
    void saveWeight(std::string folder) 
    {
        std::ofstream file;
        file.open(folder+"/config");
        file << weightMats.size() << "\n";
        std::ofstream file2;
        int i=0;
        for (auto &weightMat:weightMats)
        {
            file << weightMat.size() << " " << weightMat[0].size() << "\n";
            file2.open(folder +"/" + std::to_string(i++) +".weight");
            saveWeight(file2,weightMat);
            file2.close();
        }
        file.close();
    }
    void loadFile(std::string folder)
    {
        std::ifstream file;
        std::ifstream file2;
        file.open(folder+"/config");
        if (!file.good())
        {
            return;
        }
        int n;
        file >> n;
        weightMats.resize(n);
        int row , column;
        double value;
        int k=0;
        for (auto &weightMat:weightMats)
        {
            file2.open(folder +"/" + std::to_string(k++) +".weight");
            file >> row >> column;
            weightMat.resize(row);
            for (int i=0;i<row;i++)
            {
                for (int j=0;j< column;j++) 
                {   
                    file2 >> value;
                    weightMat[i].push_back(value);
                }
            }
            file2.close();
        }    
    }

};

    


class NeuralLayer:public Process<std::vector<Vector>*, std::vector<Vector>*>
{
    std::vector<Vector> result;
    std::vector<Vector> bresult;
    std::vector<Vector>* input;
    public:
    using Process::Process;
    Output run(const Input &in);
    Output bp(const Input &in);    
    Output fp(const Input &in);    
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

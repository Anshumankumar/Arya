#ifndef ARYA_ML_INDEXER_HPP
#define ARYA_ML_INDEXER_HPP
#include <process/process.hpp>
#include <random>
class Indexer:public Process<std::string,bool>
{
    private:
    int currentIndex;
    std::vector<std::vector<double>> *weightMat;
    std::vector<std::vector<double>> sumWeight;
    std::vector<std::unordered_map<std::string, int>> items;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;

    public:
    void preRun();
    int maxIndex;
    Indexer(std::string name,int t);
    Indexer(int t);
    bool run(const std::string &filename);
    int getIndex(int i, std::string str);
    std::vector <double> getWeight();
    void setWeight(std::vector<std::vector<double>>* v);
    void saveIndex(std::string folder);
};
#endif

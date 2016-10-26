#ifndef ARYA_ML_INDEXER_HPP
#define ARYA_ML_INDEXER_HPP
#include <process/process.hpp>

class Indexer:public Process<std::string,bool>
{
    private:
    int currentIndex;
    std::vector<double> weight;
    std::vector<std::unordered_map<std::string, int>> items;
    public:
    Indexer(std::string name,int t);
    Indexer(int t);
    bool run(const std::string &filename);
    int getIndex(int i, std::string str);
    std::vector <double> getWeight();
    void setWeight(std::vector<double> v);
};
#endif

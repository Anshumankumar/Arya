#include <fstream>
#include <ml/indexer.hpp>

Indexer::Indexer(std::string name, int t):items(t),Process(name),distribution(-0.5,0.5)
{
    currentIndex = 1;
}

Indexer::Indexer(int t):Indexer("indexer",t){}

void Indexer::preRun()
{
    maxIndex = PGET<int>("maxIndex");
}
bool Indexer::run(const std::string &folder)
{
    std::ifstream file;
    for (auto &weights:(*weightMat))
    {
        weights.resize(0);
        weights.push_back(0);
    }
}

int Indexer::getIndex(int i, std::string str)
{

    if (items[i].find(str) == items[i].end())
    {
        while (str.find(" ") != str.npos)
        {
            str = str.replace(str.find(" "),1,"_");
        }

        if (items[i].find(str) != items[i].end())
        {
            return items[i][str];
        }
        items[i][str] = currentIndex;
        currentIndex++;
    }
    return items[i][str]%maxIndex;
}


void Indexer::saveIndex(std::string folder)
{
/*    std::ofstream file;
    for (int i=0; i<items.size();i++)
    {
        file.open(folder +"/"+ std::to_string(i) +  ".weightMat");
        for(auto & item :items[i])
        { 
            file << item.first << " "<< item.second << "\n";
        } 
        file.close();
    }
    file.open(folder +"/weightMat");
    for(auto & c:weightMat)
    { 
        file << c<< "\n";
    } 
    file.close(); */

}

void Indexer::setWeight(std::vector<std::vector<double>>* v)
{
    weightMat = v;
}

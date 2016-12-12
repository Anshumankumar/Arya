#include <fstream>
#include <ml/indexer.hpp>

Indexer::Indexer(std::string name, int t):items(t),Process(name),distribution(-0.5,0.5)
{
    currentIndex = 1;
}

Indexer::Indexer(int t):Indexer("indexer",t){}

bool Indexer::run(const std::string &folder)
{
    std::ifstream file;
    for (auto &weights:(*weightMat))
    {
        weights.resize(0);
        weights.push_back(0);
    }
   /* file.open(folder +"/weightMat");
    while (file.good())
    {
        double a; 
        file >> a;
        weightMat.push_back(a);
    } 
    if (weightMat.size()==0) weight.push_back(0);
    currentIndex = weightMat.size();
    file.close();
    for (int i=0; i<items.size();i++)
    {
        sumWeight[i] = 0;
        file.open(folder +"/"+ std::to_string(i) +  ".weightMat");
        std::string value;
        int index; 
        while (file.good())
        { 
            file >> value >> index;
            items[i][value] = index;
            sumWeight[i] += weightMat[index];
        } 
        file.close();
    }*/
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
     
        if (currentIndex+1 > (*weightMat)[0].size())
        {
            for (auto &weights:(*weightMat))
            {
                weights.push_back(distribution(generator));
            }
        }
 
     //   weightMat.push_back(sumWeight[i]/items[i].size());
     //   sumWeight[i] += weightMat[currentIndex];
        currentIndex++;
    }
    return items[i][str];
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

#include <fstream>
#include <ml/indexer.hpp>

Indexer::Indexer(std::string name, int t):items(t),sumWeight(t,0),Process(name),currentIndex(1)
{
    weight.push_back(0);
}

Indexer::Indexer(int t):Indexer("indexer",t){}

bool Indexer::run(const std::string &folder)
{
    std::ifstream file;
    weight.resize(0);
    file.open(folder +"/weight");
    while (file.good())
    {
        double a; 
        file >> a;
        weight.push_back(a);
    } 
    if (weight.size()==0) weight.push_back(0);
    currentIndex = weight.size();
    file.close();
    for (int i=0; i<items.size();i++)
    {
        sumWeight[i] = 0;
        file.open(folder +"/"+ std::to_string(i) +  ".weight");
        std::string value;
        int index; 
        while (file.good())
        { 
            file >> value >> index;
            items[i][value] = index;
            sumWeight[i] += weight[index];
        } 
        file.close();
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
        weight.push_back(sumWeight[i]/items[i].size());
        sumWeight[i] += weight[currentIndex];
        currentIndex++;
    }
    return items[i][str];
}
std::vector <double> Indexer::getWeight()
{
    return weight;
}


void Indexer::saveIndex(std::string folder)
{
    std::ofstream file;
    for (int i=0; i<items.size();i++)
    {
        file.open(folder +"/"+ std::to_string(i) +  ".weight");
        for(auto & item :items[i])
        { 
            file << item.first << " "<< item.second << "\n";
        } 
        file.close();
    }
    file.open(folder +"/weight");
    for(auto & c:weight)
    { 
        file << c<< "\n";
    } 
    file.close();

}

void Indexer::setWeight(std::vector<double> v)
{
    weight = v;
}

#include <ml/indexer.hpp>

Indexer::Indexer(std::string name, int t):items(t),Process(name),currentIndex(0)
{}

Indexer::Indexer(int t):Indexer("indexer",t){}

bool Indexer::run(const std::string &filename)
{
    //Will load the indexer from file
}
int Indexer::getIndex(int i, std::string str)
{
    if (items[i].find(str) == items[i].end())
    {
        items[i][str] = currentIndex;
        weight.push_back(0);
        currentIndex++;
    }
    return items[i][str];
}
std::vector <double> Indexer::getWeight()
{
    return weight;

}
void Indexer::setWeight(std::vector<double> v)
{
    weight = v;
}

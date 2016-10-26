#ifndef ARYA_LOGISTIC_REGRESSION_HPP
#define ARYA_LOGISTIC_REGRESSION_HPP

#include <ml/csvParser.hpp>
#include <cmath>

#define PGET this->params->template get

using Row = std::pair< int ,std::vector <int>>;
class LogisticRegression:public Process<CsvParser<Row>*,bool>
{
    off_t maxRow;
    double alpha;
    public:
    using Process::Process;
    double sigmoid(double t)
    {
        return 1/(1+exp(-t));
    }
    
    double calculateWeight(std::vector<double> &weight ,std::vector<int> &vec, int &label)
    {
       double sum=0;
       for (auto &value:vec) sum += weight[value];
       double sig = sigmoid(sum);
       return -label*log(sig)-(1-label)*log(1-sig);
    }
    
    double calculateError(std::vector<double> &weight ,std::vector<int> &vec, int &label)
    {
       double sum=0;
       for (auto &value:vec) sum += weight[value];
       return sigmoid(sum) -label;

    }
    void printWeight(std::vector<double> &weight ,Row *t)
    {
        double J =0;
        for (int i=0;i<maxRow;i++)
        {
            J += calculateWeight(weight,t[i].second, t[i].first);
        }
        std::cout << J/maxRow << "\n";
    }
    
    void optimise(std::vector<double> &weight, Row *t, double *error)
    {
        for (int i=0; i< maxRow; i++)
            error[i] = calculateError(weight,t[i].second, t[i].first); 
        std::vector<double> weight2(weight.size(),0.0);
        for (int i=0; i< maxRow; i++)
            for (auto &value:t[i].second) weight2[value] += error[i]; 
        for (int i=0;i<weight.size();i++)
            weight[i] -= alpha*weight2[i]/maxRow;
    }
    Output run(const Input &in)
    {
        maxRow = (off_t)(PGET<int>("maxRow"));
        alpha = PGET<double>("alpha");
        int maxIter = PGET<int>("maxIter");
        
        Row* t = in->getNext(maxRow);
        static std::shared_ptr<Indexer> indexer = std::static_pointer_cast<Indexer>(in->subPsMap["indexer"]);
        auto weight = indexer->getWeight();
        double * error = new double[maxRow];
        printWeight(weight,t); 
        for (int i=0; i<maxIter; i++)
        {
            optimise(weight,t,error); 
        }
        delete [] error;
        printWeight(weight,t); 
        return true;
    }

};

int main()
{
    SerialProcess<std::string,bool> sp("sp"); 
    sp.addSubProcess(new CsvParser<Row>("csvParser"));
    sp.addSubProcess(new LogisticRegression("lr"));
    Params p = new Params(parseParam("/home/anshuman/Personal/arya/config/ml.param"));
    sp.setParam(std::make_shared<Params>(p));
    sp.run("");
    return 0;
}
#endif

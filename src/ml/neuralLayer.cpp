#ifndef ARYA_LOGISTIC_REGRESSION_HPP
#define ARYA_LOGISTIC_REGRESSION_HPP

#include <ml/csvParser.hpp>
#include <cmath>


using Row = std::pair< int ,std::vector <int>>;
class NeuralNetwork:public Process<CsvParser<Row>*,CsvParser<Row>*>
{
    int maxRow;
    double alpha;
    double prevCost;
    public:
    using Process::Process;
    double sigmoid(double t)
    {
        return 1/(1+exp(-t));
    }
    
}
//endif

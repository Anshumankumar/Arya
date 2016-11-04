#ifndef ARYA_MODEL_HPP
#define ARYA_MODEL_HPP

/*
 * Author: Anshuman kumar
 * Description: Implementing model for Neural Network, Logistic Regression 
 * and Linear Regression
 */

#include <cmath>
#include <process/process.hpp>

//TODO Think about this in detail
//Handle Categorical Variable


double sigmoid(double t)
{
    return 1/(1 + exp(-t));
}

using Matrix = std::vector<std::vector<double>>;

class Model
{
    std::vector<Matix> weight; 
    bool sparsity;
    template <typename T> double predict(std::vector <T> &t)
    {
       if (sparsity)
        {

        }
        else
        {
        }
    }
    
    void computeParameter()
    {
    }
    void saveROC(std::string filename)
    {
    }
     
    void run(T*)
    {
    }
}
#endif //ARYA_MODEL_HPP

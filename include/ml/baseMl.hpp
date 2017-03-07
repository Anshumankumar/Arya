#ifndef ARYA_BASE_ML_HPP
#define ARYA_BASE_ML_HPP

#include <ml/model.hpp>
#include <ml/optimiser.hpp>
#include <ml/dataHandler.hpp>
#include <cmath>

using namespace std::placeholders;

class BaseMl:public Process<Model*, Model*>
{
    public:
    Row data;
    std::vector<double> error;
    DataHandler<Row> * dataHandler;
    virtual void updateGradient(Model * model, Model * gmodel) =0;
    virtual double computeCost(Model * model) = 0;
    Optimiser *optimiser;
    using Process::Process;    
    void preRun()
    {
        optimiser = new GradientDescent("gd");
        dataHandler = new CsvParser("csvParser");
        optimiser->bindGFunc(std::bind(&BaseMl::updateGradient, this,_1,_2));
        optimiser->bindCFunc(std::bind(&BaseMl::computeCost, this,_1));
        this->addSubProcess(optimiser);
        this->addSubProcess(dataHandler);
    }
    Output run(const Input &in)
    {
        int dataLen = PGET<int>("dataNo");
        data = dataHandler->run(dataLen);
        int iterNo = PGET<int>("iterNo");
        for (int i=0; i< iterNo; i++)
        {
            std::cout << "Iter no: "  << i <<"\n";
            optimiser->run(in);
        }       
    }
};

class LogisticRegression:public BaseMl
{
    using BaseMl::BaseMl;
    inline double sigmoid(double t) {return 1/(1+exp(-t));}
    virtual void updateGradient(Model * model, Model *gModel)
    {
        error.resize(data.second.size());
        gModel->reset();
        gsl_matrix * mat = model->getMatrix(0);
        gsl_matrix * gmat = gModel->getMatrix(0);
 
       for (int i=0; i< data.second.size();i++)
        {
            double cost =0;


            if (data.second[i].type == Vector::SPARSE_VECTOR)
            {
                for (int j=0; j < data.second[i].index.size() ;j++)
                {
                    cost += gsl_matrix_get(mat,0,data.second[i].index[j])*
                        data.second[i].value[j];
                }
            }       
            else
            {
                for (int j=0; j < data.second[i].value.size() ;j++)
                {
                    cost += gsl_matrix_get(mat,0,j)* data.second[i].value[j];
                }

            }
            cost = sigmoid(cost);
            int lb = data.first[i];
            error[i]  = cost -lb;

            if (data.second[i].type == Vector::SPARSE_VECTOR)
            {
                for (int j=0; j < data.second[i].index.size() ;j++)
                {
                    double value = gsl_matrix_get(gmat,0,data.second[i].index[j]);
                    value += error[i]/data.second.size();
                    gsl_matrix_set(gmat,0,data.second[i].index[j], value);
                }
            }
            else
            {
                for (int j=0; j < data.second[i].value.size() ;j++)
                {
                    double value = gsl_matrix_get(gmat,0,j);
                    value += error[i]*data.second[i].value[j]/data.second.size();
                    gsl_matrix_set(gmat,0,j, value);
                }
            }
        }
    }

    virtual double computeCost(Model *model)
    {
        double totalCost =0;
        gsl_matrix * mat = model->getMatrix(0);
        for (int i=0; i< data.second.size();i++)
        {

            double cost =0;
            if (data.second[i].type == Vector::SPARSE_VECTOR)
            {
                for (int j=0; j < data.second[i].index.size() ;j++)
                {
                    cost += gsl_matrix_get(mat,0,data.second[i].index[j])*
                        data.second[i].value[j];
                }
            }       
            else
            {
                for (int j=0; j < data.second[i].value.size() ;j++)
                {
                    cost += gsl_matrix_get(mat,0,j)* data.second[i].value[j];
                }

            }
            cost = sigmoid(cost);
            int lb = data.first[i];
            totalCost += -lb*log(cost) - (1-lb)*log(1-cost);

        }
        totalCost = totalCost/data.second.size();
        std::cout << "Cost: " << totalCost <<"\n";
        return totalCost;

    }
};
/*
   class MlLayer
   {
   std::vector <Vector> result;
   MlLayer(int m, int n)
   {
   result.resize(n);
   for (auto &vec:result)
   {
   vec.type = VECTOR::DENSE_VECTOR;
   vec.value.resize(m);
   }
   }

   ~MlLayer()
   {
   delete result;
   }
   Vector *forwardPropagate(gsl_matrix *matrix, Vector *vec )
   {
   intialize();
   }
   }

   class NeuralNetwork:public BaseMl
   {
   using BaseMl::BaseMl;
   NeuralNetwork()
   {

   }
   bool isIntialized;
   void intialize()
   {
   if (isIntialized) 
   return;
   isIntialized = true;
   }
   virtual void updateGradient(Model *model, Model *gmodel)
   {

   }

   virtual double  computeCost(Model *model)
   {
//        model->noOfLayer
}
};*/
#endif //ARYA_BASE_ML_HPP

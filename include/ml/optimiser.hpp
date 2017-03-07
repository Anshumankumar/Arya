#ifndef ARYA_OPTIMISER_HPP
#define ARYA_OPTIMISER_HPP


#include <ml/model.hpp>
#include <functional>
#include <cmath>

class Optimiser: public Process<Model*, Model*>
{
    public:

    // G = Gradient 
    // C = Cost
    // H = Hessian
    using GFunc  = std::function<void(Model *, Model *)>;
    using CFunc  = std::function<double(Model *)>;
    using GCFunc = std::function<double(Model *, Model *)>;
    using HFunc  = std::function<void(Model *, Model *, double *hessian)>;
    using GCHFunc  = std::function<double(Model *, Model *, double *hessian)>;
    Model * gradient;
    Model * tmodel;
    double pCost;
    double alpha;
    GFunc updateGradient;
    CFunc getCost;
    GCFunc costAndGradient;
    HFunc updateHessian ;
    GCHFunc allupdate;
    using Process::Process;
    void bindGFunc(GFunc gFunc)
    {
        updateGradient = gFunc;
    }
    void bindCFunc(CFunc cFunc)
    {
        getCost = cFunc;
    }
};


class GradientDescent: public Optimiser
{
    double alpha;
    public:
    GradientDescent(GFunc gFunc, CFunc cFunc):Optimiser("GradientDescent")
    {
        gradient = nullptr;
        tmodel = nullptr;
        updateGradient = gFunc;
        getCost = cFunc;
        alpha = -1;
        pCost = -1;
    } 
    GradientDescent(std::string name ):Optimiser(name)
    {
        gradient = nullptr;
        tmodel = nullptr;
        alpha = -1;
        pCost = -1;

    }
    Output run(const Input &model)
    {
        if (gradient == nullptr)
            gradient = model->copy();
        if (tmodel == nullptr)
            tmodel = model->copy();
        model->copyTo(tmodel);
        if (alpha == -1)
            alpha = PGET<double>("alpha");
        pCost = getCost(model);
        updateGradient(model, gradient);
        alpha = alpha*1.2;
        tmodel->updateModel(alpha, gradient);
        double cost = getCost(tmodel);
        
        while(std::isnan(cost) || (pCost -cost) < 0 )
        {    
            model->copyTo(tmodel);
            alpha = alpha/1.2;
            tmodel->updateModel(alpha, gradient);
            cost = getCost(tmodel);
        }
        tmodel->copyTo(model);
        return model;
    } 
};
/*
   class Bfgs:public Optimiser
   {
   public:
   using Process::Process;
   static Bfgs *currentModel;
   static void fun(Model *model)
   {
   currentModel->updateGradient(model);
   }


   Output run(Input &in)
   {
   currentModel = this;
   if (gradient == nullptr)
   {
   gradient = new Model(*model);
   }
   updateGradient(gradient);
   void (*gup)(Model*);
   fun(gradient);
   gup =fun;
   gup(gradient);
//       model->update(gmodel, alpha);
return model;

}
};
Bfgs* Bfgs::currentModel = nullptr; */
#endif

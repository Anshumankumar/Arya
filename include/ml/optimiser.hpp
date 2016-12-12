#ifndef ARYA_OPTIMISER_HPP
#define ARYA_OPTIMISER_HPP


#include <ml/model.hpp>
#include <functional>

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
    GFunc updateGradient;
    CFunc getCost;
    GCFunc costAndGradient;
    HFunc updateHessian ;
    GCHFunc allupdate;
    using Process::Process;
};


class GradientDescent: public Optimiser
{
    double alpha;
    public:
    GradientDescent(GFunc gFunc, CFunc cFunc):Optimiser("GradientDescent")
    {
        updateGradient = gFunc;
        getCost = cFunc;  
    }
    using Optimiser::Optimiser;
    Output run(const Input &model)
    {
        alpha = PGET<double>("alpha");
        updateGradient(model, gradient);
    //    model->update(gmodel, alpha);
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

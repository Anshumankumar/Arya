#ifndef ARYA_MODEL_HPP
#define ARYA_MODEL_HPP

/*
 * Author: Anshuman kumar
 * Description: Implementing model for Neural Network, Logistic Regression 
 * and Linear Regression
 */

#include <cmath>
#include <cstring>
#include <gsl/gsl_block.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <process/process.hpp>
#include <algorithm>
#include <params/params.hpp>
#include <random>
#include <fstream>

class Model
{
    Params  params;
    bool owner; 
    public:
    unsigned int noOfLayer;
    gsl_block *block;
    std::vector<gsl_matrix> matrices;
    Model(Params param)
    {
        this->params = param;
        owner = true;
        noOfLayer = param.get<int>("noOfLayer");    
        matrices.resize(noOfLayer);
        std::for_each(matrices.begin(), matrices.end(), [&](gsl_matrix &a){
            a.owner = false;
                
        });
        unsigned int size = 0;
        for (int i=0; i<noOfLayer; i++)
        {
            matrices[i].size1 = param.get<int>("s1_" + std::to_string(i)); 
            matrices[i].size2 = param.get<int>("s2_" + std::to_string(i)); 
            matrices[i].tda = param.get<int>("s2_" + std::to_string(i)); 
            size += matrices[i].size1 *matrices[i].size2;
        }
        block = gsl_block_alloc(size);
        size=0;
        for (int i=0; i<noOfLayer; i++)
        { 
            matrices[i].block = this->block;
            matrices[i].data = &(block->data[size]); 
            size += matrices[i].size1 *matrices[i].size2;
        }
    }

    Model  (const Model &model)
    {
        owner = false;
        params = model.params;
        this->block = model.block;
        matrices = model.matrices;
        noOfLayer = model.noOfLayer;
    }

    Model* copy ()
    {
        Model *model = new Model(this->params);
    
        memcpy(model->block->data, this->block->data, this->block->size*sizeof(double));
    
        return model;
    }

    void copyTo (Model *tmodel)
    {
        memcpy(tmodel->block->data, this->block->data, this->block->size*sizeof(double));
    
    }

    Model (std::string dir):Model(parseParam(dir+"/model.param"))
    {
        FILE* stream =fopen((dir+"/data").c_str(), "r");
        if (stream)
        {
            gsl_block_fread(stream,this->block);
            fclose(stream); 
        }
        else
        {   
            randomIntialize();
        }
    }

    void saveModel(std::string dir)
    {
        std::ofstream ostream;
        ostream.open(dir+"/model.param");
        ostream << params.getString();
        ostream.close();
        FILE* stream =fopen((dir+"/data").c_str(), "w");
        gsl_block_fwrite(stream,this->block);      
        fclose(stream);
    }
    ~Model()
    {
        if (owner == true) gsl_block_free(block);
    }

    void reset()
    {
        for (int i=0; i< block->size;i++)
        {
            block->data[i] = 0;        
        }
    }
    void randomIntialize()
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(-0.1,0.1);
        for (int i=0; i< block->size;i++)
        {
            block->data[i] = distribution(generator);            
        }
    }
    inline double get(int lno, int i, int j)
    {
        return gsl_matrix_get(&matrices[lno],i,j);
    }
    inline void  set(int lno, int i, int j,  double x)
    {
        gsl_matrix_set(&matrices[lno],i,j,x);
    }
    
    void updateModel(double alpha, Model *model)
    {
        gsl_vector vec1; 
        gsl_vector vec2;
        vec1.block = block;
        vec1.data = block->data;
        vec1.size = block->size; 
        vec1.stride  = 1;
        vec2 = vec1;
        vec2.block = model->block;
        vec2.data = model->block->data; 
        gsl_blas_daxpy(-alpha,&vec2, &vec1); 
    }

    gsl_matrix* getMatrix(int lno)
    {
       return &matrices[lno];
    }    
};

#endif //ARYA_MODEL_HPP

#ifndef ARYA_LOGISTIC_REGRESSION_HPP
#define ARYA_LOGISTIC_REGRESSION_HPP

#include <ml/csvParser.hpp>
#include <ml/model.hpp>
#include <cmath>


class LogisticRegression:public Process<Model*, Model *>
{
    calculateCost();
     
}


using Row = std::pair< int ,std::vector <int>>;
class LogisticRegression:public Process<CsvParser<Row>*,CsvParser<Row>*>
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
    
    double calculateCost(std::vector<double> &weight ,std::vector<int> &vec, int &label)
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

    
    double calculateProb(std::vector<double> &weight ,std::vector<int> &vec)
    {
       double sum=0;
       for (auto &value:vec) sum += weight[value];
       return sigmoid(sum);
    }

    double printCost(std::vector<double> &weight ,Row *t)
    {
        double J =0;
        for (int i=0;i<maxRow;i++)
        {
            J += calculateCost(weight,t[i].second, t[i].first);
        }
        std::cerr << J/maxRow << "\n";
        return J/maxRow;
    }
    
    void optimise(std::vector<double> &weight, Row *t, double *error,int i)
    {
        #pragma omp parallel for
        for (int i=0; i< maxRow; i++)
            error[i] = calculateError(weight,t[i].second, t[i].first); 
         
        std::vector<std::vector<double>> weights(4,std::vector<double>(weight.size(),0.0));

        #pragma omp parallel for
        for (int j=0; j<4; j++)
        {
            for (int i=j*maxRow/4; i< (j+1)*maxRow/4; i++)
            {
                
                for (auto &value:t[i].second)
                {
                     weights[j][value] += error[i];
                
                }
            }
        }
        if (i%10 == 1)
            alpha =  alpha*2;
        std::vector<double>weight3 = weight;
        for (int i=0;i<weight.size();i++)
        {
            weight3[i] -= alpha*(weights[0][i] + weights[1][i] + weights[2][i] + weights[3][i])/maxRow;
        }
        double currentCost = printCost(weight3,t);

        while ((prevCost - currentCost ) < 0)
        {
            weight3 = weight;
            alpha = alpha/1.35;
            for (int i=0;i<weight.size();i++)
            {
                weight3[i] -= alpha*(weights[0][i] + weights[1][i] + weights[2][i] + weights[3][i])/maxRow;
            }
            currentCost = printCost(weight3,t);
        }
        prevCost =  currentCost; 
        weight = weight3;
    }


    std::vector<double> train(Row *t, std::vector<double> &weight)
    {
        int maxIter = PGET<int>("maxIter");
        double * error = new double[maxRow];
        prevCost = printCost(weight,t); 
        for (int i=0; i<maxIter; i++)
        {
            std::cerr << "Iteration no: " << i  << "\n";
            optimise(weight,t,error,i); 
        }
        delete [] error;
        return weight;
    }

    void printROC(Row *t, double *prob, double k)
    {
        int tp = 0;
        int fp = 0;
        int fn = 0;
        int tn = 0;
        for(int i=0; i < maxRow; i++)  
        {
            if (prob[i] > k && t[i].first == 1 ) tp++; 
            if (prob[i] > k && t[i].first == 0 ) fp++; 
            if (prob[i] < k && t[i].first == 1 ) fn++; 
            if (prob[i] < k && t[i].first == 0 ) tn++; 
        }
        if (tp ==0) return; 
        double prec = ((double)tp)/(tp+fp);
        double rec = ((double)tp)/(tp+fn);
        double fpr = ((double)fp)/(fp+tn);
        double F1 = 2*prec*rec/(prec+rec);
        std::cout << prec << "," << fpr << "," <<rec <<"," << F1 << "\n";

    } 
    double* test(Row *t, std::vector<double> &weight)
    {

        printCost(weight,t);
        double * prob = new double[maxRow]; 

        for(int i=0; i < maxRow; i++)  
        {
            prob[i] =  calculateProb(weight,t[i].second);
        }
        for (int i=0; i< 1000;i++) printROC(t,prob,i*0.0001);
        return prob;
    }

    Output run(const Input &in)
    {
        Output out;
        int ti = PGET<int>("ti");
        for (int i=0;i<ti;i++)
        {
            std::cerr << i <<"\n";
            maxRow = PGET<int>("maxRow");
            alpha = PGET<double>("alpha");
            std::string classType = PGET<std::string>("classType");
            std::string filename = PGET<std::string>("filename");
            std::cerr << "FILENAME: " <<  filename << "\n";
            out = in->run(filename);
            Row* t = out->getNext(maxRow);
            std::cerr << maxRow << "\n";
            auto indexer = std::static_pointer_cast<Indexer>(PRGET("indexer"));
            auto weight = indexer->getWeight();
            std::cerr << "Weight size " <<  weight.size() << "\n";
            if (classType == "trainer")
            {
                weight = train(t,weight);
                indexer->setWeight(weight);
                std::string ifile = PGET<std::string>("ifile");
                indexer->saveIndex(ifile);
            }
            else if (classType == "tester")
            {
                double *prob = test(t,weight);
                delete prob;
            }
        }
        return out;
    }

};
class SP:public SerialProcess<std::string,bool>
{
    public:
    using SerialProcess::SerialProcess;
    void preRun()
    {
        int mtoken =  PGET<int>("maxToken");
        setCommonProcess(ProcessPtr (new Indexer("indexer",mtoken)));
    }
};

int main()
{
    SP sp("sp"); 
    sp.addSubProcess(new CsvParser<Row>("csvParser"));
    sp.addSubProcess(new LogisticRegression("lr"));
    sp.addSubProcess(new LogisticRegression("lrt"));
    Params p = new Params(parseParam("/home/anshuman/Personal/arya/config/ml.param"));
    sp.setParam(std::make_shared<Params>(p));
    sp.run("");
    return 0;
}
#endif

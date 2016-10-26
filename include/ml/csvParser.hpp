#ifndef ARYA_ML_CSVPARSER_HPP
#define ARYA_ML_CSVPARSER_HPP

#include <process/process.hpp>
#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <ml/csvParser.hpp>
#include <ml/indexer.hpp>

template  <typename T>
class CsvParser:public Process<std::string, CsvParser<T>*>
{
    FILE* fp;
    off_t fileSize;
    char *data;
    off_t ptr;
    int maxToken;
    T* rows;
    public:
    using Process<std::string,CsvParser*>::Process;
    virtual void preRun(){}
    
    CsvParser* run(const std::string & in)
    {
        rows = NULL;
        ptr =0;
        std::string filename = this->params->template get<std::string>("filename");
        maxToken = this->params->template get<int>("maxToken");
        fp = fopen(filename.c_str(), "rm");
        if (fp == NULL)
        {
            perror(NULL);
            exit(2);
        }
        struct stat st;
        fstat(fileno(fp), &st);
        fileSize = st.st_size;
        data = (char *)mmap(NULL, fileSize, PROT_READ,MAP_SHARED,fileno(fp), 0);
        return this;
    }
    T * getNext(int n=1);

};

using Row = std::pair< int ,std::vector <int>>;
template <> Row* CsvParser<Row>::getNext(int n);

#endif

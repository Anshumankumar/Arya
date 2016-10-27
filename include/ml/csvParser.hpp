#ifndef ARYA_ML_CSVPARSER_HPP
#define ARYA_ML_CSVPARSER_HPP

#include <process/process.hpp>
#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <ml/csvParser.hpp>
#include <ml/indexer.hpp>

template  <typename T>
class CsvParser:public Process<std::string, CsvParser<T>*>
{
    int fp;
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
        fp = open(filename.c_str(), O_RDONLY);
        if (fp == -1)
        {
            perror(NULL);
            exit(2);
        }
        struct stat st;
        fstat(fp, &st);
        fileSize = st.st_size;
        data = (char *)mmap(NULL, fileSize, PROT_READ,MAP_SHARED,fp, 0);
        posix_madvise((void *)data,fileSize, POSIX_FADV_SEQUENTIAL);
        return this;
    }
    T * getNext(int &n);

};

using Row = std::pair< int ,std::vector <int>>;
template <> Row* CsvParser<Row>::getNext(int &n);

#endif

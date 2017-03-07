#ifndef ARYA_DATA_HANDLER_HPP
#define ARYA_DATA_HANDLER_HPP

#include <process/process.hpp>
#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <utility>
#include <fcntl.h>
#include <unistd.h>
#include <ml/indexer.hpp>
#include <ml/utils.hpp>
#include <process/process.hpp>

template <typename T>
class DataHandler:public Process<int, T>
{
    using Process<int,T>::Process;
};


using Row = std::pair< std::vector<int>,std::vector<Vector>>;
class CsvParser:public DataHandler<Row>
{
    int catIndex;
    int normalIndex;
    int labelIndex;
    public:
    using DataHandler<Row>::DataHandler;
    off_t fileSize;
    int fp;
    std::string file;
    char *data;
    off_t ptr;
    off_t clineNo;
    Row rows;
    Indexer *indexer;
    void preRun()
    {
        indexer = new Indexer(19);
        std::string filename = PGET<std::string> ("filename");            
        if (filename == file) return;
        else file = filename;
        munmap(data,fileSize);
        close(fp);
        fp = open(file.c_str(), O_RDONLY);
        if (fp < 0)
        {
            perror(NULL);
            exit(2);
        }
        struct stat st;
        fstat(fp, &st);
        fileSize = st.st_size;
        data = (char *)mmap(NULL, fileSize, PROT_READ,MAP_SHARED,fp, 0);
        catIndex=PGET<int>("cindex");
        normalIndex=PGET<int>("nindex");
        this->addSubProcess(indexer);
    }

    Row run(const Input &in)
    {
        rows.first.resize(in);
        rows.second.resize(in);
        if (catIndex == 0) 
        {   
            for(auto &vec: rows.second) vec.type = Vector::DENSE_VECTOR;
        }
        off_t cptr = ptr;
        int token =0;
        for (auto &vec:rows.second) 
        {
            vec.index.resize(0);
            vec.value.resize(0);
        }
        clineNo = 0;
        while ( ptr < fileSize )
        {
            if (data[ptr] == ',')
            {
                if (token <catIndex)
                {
                    rows.second[clineNo].index.push_back(indexer->getIndex(
                                token,std::string(&data[cptr],ptr-cptr)));
                    rows.second[clineNo].value.push_back(1);
                }   
                if (token >= catIndex && token < normalIndex)
                {
                    rows.second[clineNo].value.push_back(std::stod(std::string(&data[cptr],ptr-cptr)));
                    if (rows.second[clineNo].type != Vector::DENSE_VECTOR)
                        rows.second[clineNo].index.push_back(indexer->maxIndex + token-catIndex);
                }
                token++;
                cptr = ptr+1;

            }
            else if (data[ptr] == '\n')
            {
                token =0;
                rows.second[clineNo].index.push_back(0);
                rows.second[clineNo].value.push_back(1);
                rows.first[clineNo++] = std::stoi(std::string(&data[cptr],ptr-cptr));
                cptr = ptr+1;
                if (clineNo >= in) {ptr++;break;}
            }

            ptr++;
        }
        if (ptr >= fileSize) ptr = 0;
        rows.first.resize(clineNo);
        rows.second.resize(clineNo); 
        for (int i=0; i<clineNo; i++)
            std::cout << rows.second[i].value[0] << " " << rows.second[i].value[1] << "->" << rows.first[i] << "\n";
        return rows;
    }
};
#endif

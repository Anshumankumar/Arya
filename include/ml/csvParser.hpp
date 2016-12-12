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
#include <ml/utils.hpp>
template  <typename T>
class CsvParser:public Process<std::string, bool>
{
    int fp;
    std::string pfile;
    off_t fileSize;
    char *data;
    off_t ptr;
    int maxToken;
    T rows;
    public:
    using Process<std::string,bool>::Process;
    virtual void preRun(){}
    
    bool run(const std::string  &in)
    {
        std::string filename = in; 
        if (pfile == filename)
            return true;
        pfile = filename;
        munmap(data,fileSize);
        close(fp);
        ptr =0;
        maxToken = this->params->template get<int>("maxToken");
        fp = open(filename.c_str(), O_RDONLY);
        if (fp < 0)
        {
            perror(NULL);
            exit(2);
        }
        struct stat st;
        fstat(fp, &st);
        fileSize = st.st_size;
        data = (char *)mmap(NULL, fileSize, PROT_READ,MAP_SHARED,fp, 0);
        posix_madvise((void *)data,fileSize, POSIX_FADV_SEQUENTIAL);
        static std::shared_ptr<Indexer> indexer = std::static_pointer_cast<Indexer>(PRGET("indexer"));
        return true;
    }
    T  getNext(int &n);

};

using Row = std::pair< std::vector<int>,std::vector<Vector>>;
template <> Row CsvParser<Row>::getNext(int &n);

#endif

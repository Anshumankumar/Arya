#include <ml/csvParser.hpp>

using Row = std::pair< int ,std::vector <int>>; 
template <> Row* CsvParser<Row>::getNext(int n)
{
    static std::shared_ptr<Indexer> indexer = std::static_pointer_cast<Indexer>(this->subPsMap["indexer"]);
    if (rows!=NULL) delete [] rows;
    rows = new Row [n];
    off_t clineNo =0;
    off_t cptr = ptr;
    int token =0;
    while ( ptr < fileSize )
    {
        if (data[ptr] == ',')
        { 
            if (token <maxToken)
                rows[clineNo].second.push_back(indexer->getIndex(token,std::string(&data[cptr],ptr-cptr)));
            token++;
            cptr = ptr+1;

        }
        else if (data[ptr] == '\n') 
        {
            token =0;
            rows[clineNo++].first = std::stoi(std::string(&data[cptr],ptr-cptr));
            cptr = ptr+1;
            if (clineNo >= n) break;
        }

        ptr++;
    }
    if (ptr >= fileSize) ptr = 0;
    return rows;

}


template <> void CsvParser<Row>::preRun()
{
    int mToken =  this->params->template get<int>("maxToken");
    addSubProcess(new Indexer("indexer", mToken));
}

#include <ml/csvParser.hpp>

template <> Row CsvParser<Row>::getNext(int &n)
{
    auto indexer = std::static_pointer_cast<Indexer>(PRGET("indexer"));
    rows.first.resize(n);
    rows.second.resize(n);
    off_t clineNo =0;
    off_t cptr = ptr;
    int token =0;
    for (auto &vec:rows.second) vec.index.resize(0);
    while ( ptr < fileSize )
    {
        if (data[ptr] == ',')
        { 
            if (token <maxToken)
                rows.second[clineNo].index.push_back(indexer->getIndex(
                        token,std::string(&data[cptr],ptr-cptr)));
            token++;
            cptr = ptr+1;

        }
        else if (data[ptr] == '\n') 
        {
            token =0;
            rows.second[clineNo].index.push_back(0);
            rows.first[clineNo++] = std::stoi(std::string(&data[cptr],ptr-cptr));
            cptr = ptr+1;
            if (clineNo >= n) {ptr++;break;}
        }

        ptr++;
    }
    if (ptr >= fileSize) ptr = 0;
    n = clineNo;
    return rows;

}

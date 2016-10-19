#include "params.hpp"

template <> 
std::string Param<std::string>::getString(int tcount)
{
    std::stringstream ss;
    ss << "\""<< *value << "\"\n";
    return ss.str();
}
template <> 
std::string Param<const char *>::getString(int tcount)
{
    std::stringstream ss;
    ss << "\""<< *value << "\"\n";
    return ss.str();
}


template <> 
std::string Param<Params>::getString(int tcount)
{
    std::stringstream ss;
    ss << "\n"<< value->getString(tcount+1);
    return ss.str();
}

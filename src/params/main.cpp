#include <iostream>
#include <fstream>
#include <params/params.hpp>

extern Params parseParam(std::string file);
int main(int argc, char **argv )
{
    parseParam(argv[1]);
    return 0;
}   

#ifndef ARYA_UTILS_HPP
#define ARYA_UTILS_HPP

#include <vector>

class Vector
{
    public:
    int type;
    enum
    {
        DENSE_VECTOR=1,
        SPARSE_VECTOR=2
    };
    Vector()
    {
        this->type = SPARSE_VECTOR;
    }
    Vector(int type)
    {
        this->type = type;
    }
    
    std::vector<int> index;
    std::vector<double> value;
};
#endif

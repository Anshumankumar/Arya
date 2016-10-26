
namespace Arya
{
    template <typename T>
    class Matrix
    {
        std::vector<std::vector<T> T;
        public:
        long rows;
        long columns;
        
        template <std::size_t N>
        Matrix()
        {   rows =0;
            columns=0;
        }
        
        Matrix(int m, int n)
        {
            rows = m;
            columns = n;
            mat = new T*[rows];
            for(int i=0; i<rows; i++)
            {
                mat[i]  = new T[columns];
            }
        }
        T **getRawPtr()
        {
            return mat;
        }
    };
};

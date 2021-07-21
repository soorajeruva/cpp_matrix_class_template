#include "../include/matrix.h"

int main()
{
    //[WARNING]: CLASS MATRIX ONLY SUPPORT INT,FLOAT,DOUBLE AND BOOL.

    /*
        Matrix<float>   MatrixF
        Matrix<int>     MatrixInt
        Matrix<double>  MatrixDbl
        Matrix<bool>    MatrixBool
    */

    //=========================== CONSTRUCTORS =================================
    //"Matrix<datatype> name" can be used to override default type.

    using matrix::MatrixF;

    Matrix m1={{5,6,7,8},{6,7,8,9},{10,11,12,13},{15,16,17,18}};    
    //initializer list (constructed matrix<int> - values in list are int)

    Matrix m2(3,3); //matrix declared and memory allocated (default:-float)
    Matrix m3;      //declared a float matrix. ( use allocate() to set values )

    m3.allocate(3,3);   //initializing values (only) for default constructor.

    //======================== ACCESSING VALUES ================================

    float val=10; 

    std::cout<<m1.loc(0,0)<<"\n";   //access value and modify value at(0,0)
    m1.set_loc(0,0,val);            //used to modify values at (0,0)
    std::cout<<m1(0,0)<<"\n";       //access value and modify value at(0,0)

    std::cout<<m1(0);               //access row(0)
    std::cout<<m1.get_row(0);       //access row(0)
    std::cout<<m1.get_col(0);       //access col(0)

    //Can also use iterartors [ Matrix::Iterators or MatrixIterator }

    //=========================== MATRIX PROPERTIES =============================

    std::cout<<m1.size()<<"\n";     //returns size of matrix.(owned memory size.)
    std::cout<<m1.row()<<"\n";      //return rows of matrix.
    std::cout<<m1.col()<<"\n";      //return col of matrix.
    auto[row,col]=m1.shape();       //return tuple of row and col of matrix.
    m1.matrix_details();            //prints all the details about the matrix.

    //=========================== ASSIGNING VALUES =============================

    /* using iterator */
    for(MatrixF::Iterator iterator=m2.begin();iterator!=m2.end();iterator++)
    {
        iterator=val;
    }


    /* by accessing location */
    for(int i=0;i<m3.row();i++)
    {
        for(int j=0;j<m3.col();j++)
        {
            m3.loc(i,j)=val;
        }
    }

    /* using operator '<<( string with comma seperation)'. */
    Matrix m6(3,3);
    m6<<"1,2,3,4,5,6,7,8,9";

    /* using vector (namespace matrix)*/
    std::vector< std::vector<float> > vec{{1,2,3},{1,2,3},{1,2,3}};
    Matrix m4=matrix::vector2matrix<MatrixF>(vec);

    /* from file (namespace matrix)*/
    Matrix m5=matrix::file2matrix<MatrixF>("../dataset/adult_data.csv",",",false);
    /* deep copy*/
    Matrix m1_copy=m1.copy();             //returns a deep copy.

    //=========================== BOOL FUNCTIONS ===============================

    MatrixF::is_SameShape(m2,m3);
    m2.is_SameShape(m3);            //return true if row and col are same.

    m2.equal(m3);                   //return true if matrices are equal.(used along with is_Same_Shape())
    m2.not_equal(m3);               //return true if matrices are not equal.(used along with is_Same_Shape())

    return 0;
}
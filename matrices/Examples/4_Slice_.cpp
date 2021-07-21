#include "../include/matrix.h"

int main()
{
    //[WARNING]: CLASS MATRIX ONLY SUPPORT INT,FLOAT,DOUBLE AND BOOL.

    //In this example we'll discuss various slicing operation.

    /*
        #   Slicing let you read and modify existing matrix data.
        #   Slice behaves same as a matrix but has no memory ownership.
        #   It's deletion won't affect the existance of data.(if parent matrix is still in existance)  
        #   See the example for more Info.
    */

    /*  
        #   Here we load the data from file to matrix.
        #   Select slices of the data as per the need.
        #   Class Matrix provide various inbuilt function for slicing.
    */
    
    using matrix::MatrixF;
    
    MatrixF data=matrix::file2matrix<MatrixF>("../dataset/adult_data.csv",",",false);

    int N=5;

    Matrix top_five         =   data.topRows(N);            //returns top N rows.

    Matrix bottom_five      =   data.bottomRows(N);         //returns bottom N rows.

    Matrix left_five        =   top_five.leftCols(N);       //returns N number of left-most coloumns.

    Matrix right_five       =   bottom_five.rightCols(N);   //returns N number of right-most coloumns.

    /*
        #   The most powerful tool amoung them is the slice function.
        #   slice (row_index1,row_index2,col_index_1,col_index2).
    */

    Matrix slice            =   data.slice(0,N,0,N);    //returns slice (size: N x N) formed by(0,0)and(N,N).

    Matrix diagonal_elements =   slice.diagonal();       //returns diagonal elements of a square matrix.

    

    std::cout<<top_five<<"\n";
    std::cout<<bottom_five<<"\n";
    std::cout<<left_five<<"\n";
    std::cout<<right_five<<"\n";
    std::cout<<slice<<"\n";
    std::cout<<diagonal_elements<<"\n";



    return 0;
}
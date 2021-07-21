#include "../include/matrix.h"

int main()
{
    //[WARNING]: CLASS MATRIX ONLY SUPPORT INT,FLOAT,DOUBLE AND BOOL.
    
    /*
        #   In section 1, we use row and coloumn manipulation like :-
        #   row switch, row scale , suming rows after scaling (same for coloumn)
        #   In section 2, we use basic operations like sum,product,mean of elements etc.
        #   In section 3, we use array-wise, row-wise and coloumn-wise manipulations.
        #   Uncomment them section-wise for better visualization.
    */
   using matrix::MatrixF;
    // SECTION 1 : ROW AND COLOUMN MANIPULATION.

    MatrixF m1={{5,6,7,8},{6,7,8,9},{10,11,12,13},{15,16,17,18}};  
    MatrixF m2=m1.copy();
    
    std::cout<<"Orginal: "<<m1;

    m1.row_switch(1,2);
    std::cout<<"Switched row one and two: "<<m1;

    m1.scale_row(3,2);
    std::cout<<"Scaled row three: "<<m1;

    m1.sum_rows(2,3,2);
    std::cout<<"Added row-two to 2 x third-row: "<<m1;

    
    std::cout<<"Deep copy of Orginal: "<<m2;

    m2.col_switch(1,2);
    std::cout<<"Switched coloumn one and two: "<<m2;

    m2.scale_col(3,2);
    std::cout<<"Scaled col three: "<<m2;

    m2.sum_cols(2,3,2);
    std::cout<<"Added col-two to 2 x third-col: "<<m2;


/*    
    //  SECTION 2 : SOME BASIC OPERATIONS

    int N=2;

    MatrixF m3={{5.2589,6.2687},{6.1547,7.965}};

    std::cout<<"Sum: "<<m3.sum()<<"\n";               //return sum of elements in matrix.

    std::cout<<"Max: "<<m3.maxCoeff()<<"\n";          //return maximum coefficent of elements in matrix.

    std::cout<<"Min: "<<m3.minCoeff()<<"\n";          //return minimum coefficent of elements in matrix.

    std::cout<<"Mean: "<<m3.mean()<<"\n";             //return mean of elements in matrix.

    std::cout<<"Sum of diagonal: "<<m3.trace()<<"\n"; //return sum of diagonal elements in the matrix.

    std::cout<<"Product: "<<m3.prod()<<"\n";          //return product of elements in matrix.

    std::cout<<m3.round(N);                           //sets the precision to N decimals.
*/

/*
    // SECTION 3 : Arraywise, Rowwise and Colwise.

    MatrixF m4={{5,6,7,8},{6,7,8,9},{10,11,12,13},{15,16,17,18}};  
    MatrixF m5=m4.copy();

    std::cout<<m4.arraywise_divide(m5);     //divide each element of m4 with that of m5.

    std::cout<<m4.arraywise_multiply(m5);   //multipy each element of m4 with that of m5.

    arraywise_divide(m4,m5);                //divide each element of m4 with that of m5 and store result in m4.
    std::cout<<m4;                          //same as doing "m4/=m5"

    arraywise_multiply(m4,m5);              //multipy each element of m4 with that of m5 and store result in m4.
    std::cout<<m4;                          //same as doing "m4*=m5"

    std::cout<<row_wise(m4,matrix::sum);        //calculates row wise sum and outputs matrix
    std::cout<<col_wise(m4,matrix::maxCoeff);   //finds col wise maximum coefficent and outputs matrix

    std::cout<<m4.row_wise(matrix::sum);        //calculates row wise sum and out puts matrix
    std::cout<<m4.col_wise(matrix::maxCoeff);   //finds col wise maximum coefficent and out puts matrix
*/

    /*
        #   More operations are supported by row_wise and col_wise(see namespace matrix).
        #   you can make your own functions which takes in a const matrix reference as argument 
        #   and plug it to row_wise and col_wise.
        #   But make sure that the output of the function is having same 'type' as Matrix<'type'>.
    */
    return 0;
}
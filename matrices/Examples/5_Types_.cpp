#include "../include/matrix.h"

int main()
{
    using matrix::MatrixF;

    int N=5;
    int R=3;
    int C=3;

    Matrix _matrix=MatrixF::constant(R,C,N);                     //return a matrix with all the elements equal to the given value.
    std::cout<<"Scalar : "<<_matrix;
    std::cout<<"Is square ? "<<_matrix.is_square()<<"\n";
    std::cout<<"Is symmetric ? "<<_matrix.is_symmetric()<<"\n";

    Matrix _matrix2=MatrixF::diagonal(R,C,N);                    //returns a matrix with all the non-diagonal elements zero and the diagonal with the given value.
    std::cout<<"Diagonal matrix : "<<_matrix2;
    std::cout<<"Is diagonal ? "<<_matrix2.is_diagonal()<<"\n";

    Matrix _matrix3=MatrixF::ones(R,C);                          //returns a matrix with all the elements equal one.
    std::cout<<"Matrix of ones : "<<_matrix3;

    Matrix _matrix4=MatrixF::identity(R,C);                     //returns a matrix with all the non-diagonal elements zero and the diagonal with value ones.
    std::cout<<"Identity matrix : "<<_matrix4;
    std::cout<<"Is identity matrix? "<<_matrix4.is_identity()<<"\n";

    Matrix _matrix5=MatrixF::zeroes(R,C);                        //returns a matrix with all the elements equal zero.
    std::cout<<"Zero matrix : "<<_matrix5; 

    Matrix _matrix6=MatrixF::random(R,C,0,1);                    //returns a matrix with random elements between(0,1).
    std::cout<<"Random matrix : "<<_matrix6;

    _matrix.lower_triangular();                                  //set all the elements below diagonal as zeros.
    std::cout<<"Lower triangular matrix : "<<_matrix;

    _matrix3.upper_triangular();                                 //set all the elements above diagonal as zeros.
    std::cout<<"Upper triangular matrix : "<<_matrix3;

    return 0;
}
#include "../include/matrix.h"

int main()
{
  //[WARNING]: CLASS MATRIX ONLY SUPPORT INT,FLOAT,DOUBLE AND BOOL.

  /*
    # Using this library, you can do several matrix operations.
    # Here you can see some implementation of this library.
    # As an example, when you call determinant method it returns determinant of the correspoding matrix.
  */
  using matrix::MatrixF;
  MatrixF _matrix= {{1,5,8},{5,6,8},{9,5,7}};

  //Use the following matrices to test out rank of the matrix.

  //MatrixF _matrix={{1,3,2},{2,6,4},{1,3,2}};
  //MatrixF _matrix={{1,3,2,5},{2,6,4,10},{1,3,2,5},{1,3,2,5}};
  //MatrixF _matrix={{1,0,0,0,1},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{1,0,0,0,1}};
  //MatrixF _matrix={  {5,6,7,8},{6,7,8,9},{10,11,12,13},{15,16,17,18}   };


  std::cout<<"\nMatrix: "<<_matrix;

  std::cout<<"\nSubmatrix of(0,0):"<<_matrix.submatrix(0,0);

  std::cout<<"\nDeterminant: "<<_matrix.determinant()<<"\n";

  std::cout<<"\nminor of(0,0): "<<_matrix.minor(0,1)<<"\n";

  std::cout<<"\nCofactor of (0,0)"<<_matrix.cofactor(0,1)<<"\n";

  std::cout<<"\nAdjugate: "<<_matrix.adjugate();

  std::cout<<"\nInverse: "<<_matrix.inverse();

  std::cout<<"\nTest if inverse : "<<_matrix.is_inverse(_matrix.inverse())<<"\n";

  std::cout<<"\nmatrix rank is: "<<_matrix.rank()<<"\n";

  _matrix.transpose();

  std::cout<<"\nmatrix transpose: "<<_matrix;

  return 0;
}
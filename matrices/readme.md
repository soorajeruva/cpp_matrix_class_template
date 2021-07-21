    /*  
        IMPORTANT NOTE

        #   This is my first cpp project and is far from even good.
        #   Open to healthly criticism.(  help me to know more )

    */

    /*  
        ABOUT

        #   This is a templatized library enabling matrix manipulations.

        #   The library include 3 template header.
        #   The "matrix_iterator" header file help to traverse the matrix.
        #   The "matrix" header is the heart and "matrix_pooling" is just an extension.
        #   The Matrix class is just a wrapper around a normal heap allocated array.
        #   Stack allocation of array is not supported and is going to my priority in version2.

    */

    /*
        INTRODUCTION

        #   The matrix header include a class Matrix and namespace matrix.
        #   Through the examples given, you would be able to familiarise the functions in the library.
        #   MEMBER FUNCTIONS :-

            #   copy           -    creates deep copy of a matrix.
            #   allocate       -    allocate memory for a default constructed matrix.

            #   row            -    returns rows of matrix.
            #   col            -    returs cols of matrix.
            #   size           -    returns size owned by matrix.
            #   shape          -    returns pair(row,col).
            #   set_loc        -    set the argument value at the given location.
            #   loc            -    access location.
            #   matrix_details -    full state of the matrix.
            #   mean           -    return mean of all elements in matrix.
            #   minCoeff       -    return minimum coefficent in matrix.
            #   maxCoeff       -    return max coefficent in matrix.
            #   sum            -    return sum of all elements in matrix.
            #   prod           -    return product of all elements in matrix.
            #   trace          -    return sum of diagonal elements in matrix.
            #   arraywise_multiply- multiplies element arraywise.
            #   arraywise_divide  - divide elements arraywise.
            #   rowwise        -    does an operation row wise(limitaion are there)
            #   colwise        -    does an operation col wise(limitaion are there)
            #   round          -    round elements of the matrix to given precision.
            #   row_switch     -    switch position of two rows.
            #   scale_row      -    scale a row by given factor.
            #   sum_rows       -    takes sum of two rows and set it in the first row.
            #   col_switch     -    switch position of two cols.
            #   scale_col      -    scale a col by given factor.
            #   sum_cols       -    takes sum of two cols and set it in the first col.
            #   slice          -    return the slice of a matrix as another matrix.
            #   topRows        -    return slice of matrix upto the given row_index from top.
            #   bottomRows     -    return slice of matrix upto the given row_index from bottom.
            #   leftCols       -    return slice of matrix upto the given col_index from left.
            #   rightCols      -    return slice of cols upto the given col_index from right.
            #   diagonal       -    return matrix of diagonal elements of a matrix.
            #   get_row        -    return row of specified index.
            #   get_col        -    return col of specified index.
            #   transpose      -    transpose a matrix.
            #   submatrix      -    returns submatrix of the given coefficent from a matrix.
            #   minor          -    returns minor of the given coefficent from a matrix.
            #   cofactor       -    returns cofactor of the given coefficent from a matrix.
            #   adjugate       -    returns adjoint matrix.
            #   inverse        -    returns inverse of a matrix.
            #   determinant    -    returns determinant of a matrix.
            #   rank           -    returns rank of a matrix.
            #   upper_triangular-   convert a matrix into upper triangular matrix.
            #   lower_triangular-   convert a matrix into lower triangular matrix.
            #   begin          -    returns an iterator object.
            #   end            -    returns an iterator object.

            #   equal          -    check whether two matrix are equal.
            #   not_equal      -    check whether two matrix are unequal.
            #   any            -    returns true if any element evaluate to true.
            #   all            -    returns true if all element evaluate to true.
            #   is_sameshape   -    check whether two matrix is of same shape.
            #   is_square      -    check whether matrix is a square matrix.
            #   is_symmeric    -    check whether matrix is a symmeric matrix.
            #   is_digonal     -    check whether matrix is a diagonal matrix.
            #   is_identity    -    check whether matrix is an identity matrix.
            #   is_inverse     -    check whether a matrix is inverse of another.
        
        #   STATIC FUNCTIONS    :-

            #   constant       -    returns a matrix of given constant.
            #   zeroes         -    returns a matrix of zeroes.
            #   ones           -    returns a matrix of ones.
            #   identity       -    returns a identity matrix
            #   diagonal       -    returns a digonal matrix.
            #   random         -    returns a matrix with random elements.

        #   FRIEND FUNCTIONS    :-
            
            #   pow            -    take power of every element in the matrix.
            #   round          -    take rounded values of every elements in the matrix
            #   sqrt           -    takes square root of elements.
            #   matrix_stride  -    is just a function created out of curiosity.
                                    It moves another matrix over the matrix doing specificed operations.
                                    (usally these operations are defined in the namespace matrix but will
                                    also support custom operations* )

        #   OPERATOR OVERLOADS  :-

            #   ()operator     -    returns location baes on the indices(or index)
            #   <<operator     -    takes a string of elements as argument and set the 
                                    elements to the location in the matrix.
            #   + operator     -    returns sum of matrices.
            #   - operator     -    returns differents of matrices.
            #   * operator     -    returns a matrix after matrix multiplication.
            #   += operator    -    sums and assign value.
            #   -= operator    -    substract and assign value.
            #   *= operator    -    arraywise multiplication
            #   /= operator    -    arraywise division
            #   = operator     -    assigns value.
            #   == operator    -    returns a bool matrices after checking the condition.
            #   same is done for every other comparison operator.
            
            









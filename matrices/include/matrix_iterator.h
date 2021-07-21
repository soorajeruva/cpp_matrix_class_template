#pragma once
#ifndef MATRIX_ITERATOR_H
#define MATRIX_ITERATOR_H

//===================================================+++ CLASS MATRIX ITERATOR +++===================================================
template<class Matrix>
class MatrixIterator                                       //for iterating through the matrix class.
{
public:
    using ValueType =typename Matrix::ValueType;           //the Valuetype is the type of matrix (matrix<valuetype>).
    using PointerType= ValueType*;
    using ReferenceType=ValueType&;
private:
    PointerType m_ptr;
    int m_col,                                             //number of elements of the coloumn.
        m_stride,                                          //number of instances to skip(applicable in case of slices).
        c_col=0;                                           //keep trace of the position of the pointer.

public:
    MatrixIterator(PointerType ptr,int col,int stride)
    :m_ptr(ptr),    m_col(col-1),   m_stride(stride+1){}   //the m_col is zero based unlike that of matrix class.

    MatrixIterator& operator++()
    {
        if (c_col!=m_col)   {c_col++;  m_ptr++;}
        else    {c_col=0;    m_ptr+=m_stride;}
        return *this;
    }
    MatrixIterator operator++(int)                         //calls the ++operator above;
    {
        MatrixIterator iterator=*this;
        ++(*this);
        return iterator;
    }
    MatrixIterator& operator--()
    {
        if (c_col!=m_col)   {c_col--;   m_ptr--;}
        else    {c_col=0;   m_ptr-=m_stride;}
        return *this;
    }
    MatrixIterator operator--(int)                         //calls the --operator above;
    {

        MatrixIterator iterator=*this;
        --(*this);

        return iterator;
    }
    PointerType operator->()
    {
        return m_ptr;
    }
    ReferenceType operator*()
    {
        return *m_ptr;
    }
    bool operator==( const MatrixIterator &other)const
    {
        return m_ptr==other.m_ptr;
    }
    bool operator!=(const MatrixIterator &other)const
    {
        return !(*this==other);
    }
    void operator=(ValueType t)
    {
        *m_ptr=t;
    }
    friend std::ostream & operator<<(std::ostream& stream, MatrixIterator& mat)
    { 
        return stream << *mat;
    }
};
#endif
#pragma once
#ifndef POOLING_LIB_H
#define POOLING_LIB_H

#include "matrix.h"

#define ITERATE_OVER_LOCATION(X) for(int i=0;i<row;i++){for (int j=0;j<col;j++){X}}

template<class Matrix>
class Pool: public Matrix
{
private:
    static std::random_device rd;
    static std::mt19937 mt(rd());

    using ValueType=typename Matrix::ValueType;
    
public:
    Pool(){}
    static ValueType conv(const Matrix& mat1,const Matrix& mat2)
    {
        auto[row,col]=mat1.shape();
        ValueType sum=0;
        ITERATE_OVER_LOCATION(sum+=mat1.loc(i,j)*mat2.loc(i,j);)
        return sum;
    }
    static Matrix& pad_matrix(const Matrix& feature_map,int pad_size=1,int pad_val=0)
    {
        auto[map_row,map_col]=feature_map.shape();
        int pad_row=map_row+2*pad_size;
        int pad_col=map_col+2*pad_size;
        int row,col;
        Matrix* padded_map=new Matrix(pad_row,pad_col,true);

        for(int i=0;i<pad_row;i++)
        {   
            row=i-pad_size;
            for(int j=0;j<pad_col;j++)
            {
                col=j-pad_size;
                if (map_row>row&&row>=0 && map_col>col&&col>=0)padded_map->loc(i,j)=feature_map.loc(row,col);
                else padded_map->loc(i,j)=pad_val;
            }
        }        
        return *padded_map;
    }
    static Matrix& FilterGenerator(int row,int col,int lower_limit=0,int upper_limit=1)
    {

        Matrix* filter=new Matrix(row,col,true);
        std::uniform_real_distribution<double> dist(lower_limit, upper_limit);

        ITERATE_OVER_LOCATION(filter->loc(i,j)=dist(mt);)
        return *filter;
    }
    static Matrix& MaxPool(const Matrix& feature_map,int filter_row,int filter_col,int stride, int padding=1)
    {
        if (padding!=0)
        {
            Matrix padded_input=pad_matrix(feature_map,padding);
            return matrix_stride(padded_input,filter_row,filter_col,stride,stride,matrix::maxCoeff);
        }
        else return matrix_stride(feature_map,filter_row,filter_col,stride,stride,matrix::maxCoeff);
    }
    static Matrix& MinPool(const Matrix& feature_map,int filter_row,int filter_col,int stride, int padding=0)
    {
        if (padding!=0)
        {
            Matrix padded_input=pad_matrix(feature_map,padding);
            return matrix_stride(padded_input,filter_row,filter_col,stride,stride,matrix::minCoeff);
        }
        return matrix_stride(feature_map,filter_row,filter_col,stride,stride,matrix::minCoeff);
    }
    static Matrix& MeanPool(const Matrix& feature_map,int filter_row,int filter_col,int stride, int padding=0)
    {   
        if (padding!=0)
        {
            Matrix padded_input=pad_matrix(feature_map,padding);
            return matrix_stride(padded_input,filter_row,filter_col,stride,stride,matrix::mean);
        }
        return matrix_stride(feature_map,filter_row,filter_col,stride,stride,matrix::mean);
    }
    static Matrix& Conv_2D(const Matrix& feature_map,const Matrix& filter,int stride)
    {
        return matrix_stride(feature_map,filter,stride,stride,conv);
    }
};
#endif
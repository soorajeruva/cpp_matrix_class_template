

#pragma once
#ifndef MATRIX_LIB_H
#define MATRIX_LIB_H

#include <iostream>
#include <typeinfo>
#include <math.h>
#include <tuple>
#include <vector>
#include <initializer_list>
#include <fstream>
#include <string>
#include <random>
#include "boost/algorithm/string.hpp"

#include "matrix_iterator.h"

#define DEBUG_MODE 0

//=========================================================+++ MACROS USED +++======================================================

#define ASSERT(X,Y) if(X){std::cout<<"\n"<<"  [Error] : "<<Y<<"\n";abort();}
#define SLICE m_size==0&&_row!=0
#define LOOP_OVER_ELEMENTS(X) for (int i=0;i<_row;i++){for (int j=0;j<_col;j++){ X }}

//=========================================================+++ NAMESPACE matrix +++======================================================

// -> Allows to call an already existing function in a different form.[eg:- sum(Matrix)]
// -> Some of these are used by  friend functions and functions in "matrix_pooling.h".

namespace matrix
{
    template<class Matrix, typename ValueType>
    Matrix& vector2matrix(std::vector< std::vector<ValueType> >& data)
    {
        Matrix* new_matrix= new Matrix(data.size(),data[0].size(),true);

        for(int i=0;i<data.size();i++)
        {
            for(int j=0;j<data[0].size();j++)
            {
                new_matrix->loc(i,j)=data[i][j];
            }
        }
        return *new_matrix;
    }
    template<class Matrix>
    Matrix& file2matrix(std::string file_name,std::string delimeter,bool head=true)
    {
        using ValueType=typename Matrix::ValueType;

        std::ifstream file;
        file.open(file_name);
        std::string raw_data;
        std::vector<std::string> line_data;

        std::vector <ValueType>coloumn_data;
        std::vector<std::vector<ValueType>> full_data;

        if(file.is_open())
        {
            if(file>>raw_data)
            {
                if(!head)
                {
                    boost::split(line_data,raw_data,boost::is_any_of(delimeter));
                    for(int i=0;i<line_data.size();i++)
                    {
                        coloumn_data.push_back(std::stof(line_data[i]));    // the function depends on the ValueType;
                    }
                    full_data.push_back(coloumn_data);
                    coloumn_data.clear();
                }

                while(file>>raw_data)
                {
                    boost::split(line_data,raw_data,boost::is_any_of(delimeter));

                    for(int i=0;i<line_data.size();i++)
                    {
                        coloumn_data.push_back(std::stof(line_data[i]));    // the function depends on the ValueType;
                    }

                    full_data.push_back(coloumn_data);
                    coloumn_data.clear();
                }
            }
            else std::cout<<"\nfile is empty";
        }
        else std::cout<<"\nunable to read file";
        return vector2matrix<Matrix>(full_data);
    }
    
    template<class Matrix, typename ValueType>
    ValueType sum(const Matrix& data)
    {
        ValueType sum_=0;auto _row=data.row();auto _col=data.col();
        LOOP_OVER_ELEMENTS(sum_+=data.loc(i,j);)
        return sum_;
    }
    template<class Matrix, typename ValueType>
    ValueType mean(const Matrix& data)
    {
        ValueType average=0;auto _row=data.row();auto _col=data.col();
        LOOP_OVER_ELEMENTS(average+=data.loc(i,j);)
        average=average/(_row*_col);
        return average;
    }
    template<class Matrix, typename ValueType>
    ValueType minCoeff(const Matrix& data)
    {
        auto _row=data.row();auto _col=data.col();
        ValueType min=data.loc(0,0);
        ValueType now;
        LOOP_OVER_ELEMENTS(now=data.loc(i,j);if(now<min)min=now;)
        return min;
    }
    template<class Matrix, typename ValueType>
    ValueType maxCoeff(const Matrix& data)
    {
        auto _row=data.row();auto _col=data.col();
        ValueType max=0;
        ValueType now;
        LOOP_OVER_ELEMENTS(now=data.loc(i,j);if(now>max){max=now;})
        return max;
    }
    template<class Matrix, typename ValueType>
    ValueType prod(const Matrix& data)
    {
        auto _row=data.row();auto _col=data.col();
        ValueType result=1;
        LOOP_OVER_ELEMENTS(result*=data.loc(i,j);)
        return result;
    }
}

//=======================================================+++ CLASS MATRIX +++========================================================

template<typename T=float>
class Matrix                                    
{
public:
using Matrix_f=Matrix<float>;
private:
    typedef Matrix* matrixPtr;
    typedef Matrix& matrixRef;
    typedef const Matrix& const_matrixRef;
public:
    using ValueType =T;
    using Iterator=MatrixIterator<Matrix<T>>;  //iterator.

private:
    ValueType* m_data=nullptr;                 //pointer to the memory.
    size_t m_size=0;                           //keep track of the size of owned memory buffer.
    bool temp=false;                           //whether a temporary variable or not.
    mutable int* ref_count=nullptr;            //keep track of number of references.

    int _row=0,_col=0;                         //store shape data.
    int stride=0;                              //store the instances to skip to reach next row(applicable in case of slices)

//-------------------------------------------------- MATRIX MEMORY MANIPULATION ---------------------------------------------------

    ValueType* ALLOCATOR (int size)const        //allocate memory for data.
    {   
        ValueType* data= new ValueType[size];

        #if (DEBUG_MODE==1)
        std::cout<<"\t\t[memory allocated : "<<size*sizeof(ValueType)<<" bytes ]\n";
        #endif

        return data;
    }
    void DEALLOCATOR(ValueType* data)           //deallocate memory in which data is stored.
    {
        delete[] data;
        data=nullptr;

        #if (DEBUG_MODE==1)
        std::cout<<"\t\t\t[memory Deallocated : "<<m_size*sizeof(ValueType)<<" bytes ]\n";
        #endif
    }

//--------------------------------------------- MATRIX CONSTRUCTORS AND DESTRUCTOR ------------------------------------------------
    Matrix(int row,int col,int stride,int* ref_cnt, ValueType* data,bool tmp=false)    //creates a slice matrix.
    :_row(abs(row)),_col(abs(col)),stride(abs(stride)),ref_count(ref_cnt),
     m_data(data),temp(tmp)
    {
        this->m_size=0;
        #if (DEBUG_MODE==1)
        std::cout<<"\n  slice_constructor:\t";
        #endif
    }
public:
    Matrix(int row,int col,bool temp):_row(abs(row)),_col(abs(col)),temp(temp) //construct matrix with a new memory block [temporary]
    {   
        ASSERT(
            !(typeid(ValueType).name()==typeid(int).name()  ||
            typeid(ValueType).name()==typeid(float).name()  ||
            typeid(ValueType).name()==typeid(double).name() ||
            typeid(ValueType).name()==typeid(bool).name())
            ,"THE MATRIX CLASS ONLY SUPPORT INT , FLOAT AND DOUBLE")
        #if (DEBUG_MODE==1)
        std::cout<<"\n  temporary variable_constructor:\t";
        #endif

        stride=0;
        this->ref_count=new int;
        *this->ref_count=0;
        m_size=_row*_col;
        m_data=ALLOCATOR(m_size);
    }

    Matrix()                                                                   //construct matrix without any initialisation.
    {   
        ASSERT(
            !(typeid(ValueType).name()==typeid(int).name()  ||
            typeid(ValueType).name()==typeid(float).name()  ||
            typeid(ValueType).name()==typeid(double).name() ||
            typeid(ValueType).name()==typeid(bool).name())
            ,"THE MATRIX CLASS ONLY SUPPORT INT , FLOAT AND DOUBLE")
        _row=0;_col=0;stride=0;temp=false;
        m_size=0;m_data=nullptr;
        ref_count=new int;
        *ref_count=0;
        #if (DEBUG_MODE==1)
        std::cout<<"\n  default_constructor: (no memory allocation done ).\n";
        #endif
    }
    Matrix(std::initializer_list< std::initializer_list<ValueType> > data)     //construct a matrix from initializer list
    {
        ASSERT(
            !(typeid(ValueType).name()==typeid(int).name()  ||
            typeid(ValueType).name()==typeid(float).name()  ||
            typeid(ValueType).name()==typeid(double).name() ||
            typeid(ValueType).name()==typeid(bool).name())
            ,"THE MATRIX CLASS ONLY SUPPORT INT , FLOAT AND DOUBLE")

        #if (DEBUG_MODE==1)
        std::cout<<"\n  list_constructor:\t";
        #endif

        _row=data.size();
        _col=data.begin()->size();
        stride=0;
        temp=false;
        ASSERT(_row<0||_col<0,"initializer list error")

        m_size=_row*_col;
        m_data=ALLOCATOR(m_size);
        ref_count=new int;
        *ref_count=0;

        int i=0;int j=0;size_t size=data.begin()->size();
        for(auto a1=data.begin();a1!=data.end();a1++){
            ASSERT(a1->size()!=size,"size irregularity in initializer_list.")
            for(auto a2=a1->begin();a2!=a1->end();a2++){
                loc(i,j)=*a2;j++;
            }
            j=0;i++;
        }
    }
    Matrix(int row,int col):_row(abs(row)),_col(abs(col))                      //construct matrix with a new memory block from rows and coloumns
    {
        ASSERT(
            !(typeid(ValueType).name()==typeid(int).name()  ||
            typeid(ValueType).name()==typeid(float).name()  ||
            typeid(ValueType).name()==typeid(double).name() ||
            typeid(ValueType).name()==typeid(bool).name())
            ,"THE MATRIX CLASS ONLY SUPPORT INT , FLOAT AND DOUBLE")

        #if (DEBUG_MODE==1)
        std::cout<<"\n  template_constructor:\t";
        #endif
        stride=0;temp=false;
        m_size=_row*_col;
        m_data=ALLOCATOR(m_size);
        ref_count=new int;
        *ref_count=0;
    }
    Matrix(matrixRef other)                                                    //construct a matrix with content copied[shallow]
    {
        #if (DEBUG_MODE==1)
        std::cout<<"\n  copy_constructor :\t";
        #endif

        std::tie(_row,_col)=other.shape();

        if (other.temp)
        {
            if(other.m_size==0&&other._col!=0)
            {*other.ref_count+=1;}
            stride=other.stride;
            m_size=other.m_size;
            m_data=other.m_data;
            ref_count=other.ref_count;
            temp=false;

            #if (DEBUG_MODE==1)
            std::cout<<"instance created and temporary variable deleted";
            #endif
            other.m_data=nullptr;
            other.m_size=0;
            other.~Matrix();
        }
        else
        {
            stride=other.stride;
            m_size=0;
            m_data=other.m_data;
            ref_count=other.ref_count;
            *ref_count+=1;
            temp=false;
            
            #if (DEBUG_MODE==1)
            std::cout<<"pointer instance has been created to the matrix.";
            #endif
        }
        return;
    }
    matrixRef copy()const                                                      //a method returning a deep copied matrix reference.
    {
        matrixPtr copy_= new Matrix(_row,_col);
        copy_->temp=true;

        LOOP_OVER_ELEMENTS(copy_->set_loc(i,j,loc(i,j));)

        #if (DEBUG_MODE==1)
        std::cout<<" [ deep copy done ].\n";
        #endif

        return *copy_;
    }
    void allocate(int row,int col)                                             //allocator for default constructed matrix.
    {
        ASSERT(_row!=0&&_col!=0,"allocation can only be done to default matrices.")
        _row=row;_col=col;temp=false;stride=0;
        m_size=_row*_col;
        m_data=ALLOCATOR(m_size);
    }
    void operator=(matrixRef other)                                              //create a matrix with pointer instance to the m_data
    {
        if(_row==0&&_col==0&&m_size==0)
        {
            _row=other.row();_col=other.col();
        }
        ASSERT(!(is_SameShape(other)),"Assigning failed! shape of matrices are not same!")

        #if (DEBUG_MODE==1)
            std::cout<<"\n  Assigned :";
        #endif

        if(other.temp)
        {
            if(SLICE)
            {  

                LOOP_OVER_ELEMENTS(set_loc(i,j,other.loc(i,j));)
                #if (DEBUG_MODE==1)
                std::cout<<"\t\t[ deep copying done from temperory variable to slice].";
                #endif
            }
            else
            {
                T* data=m_data;
                m_data=other.m_data;
                ref_count=other.ref_count;
                other.m_data=data;
                #if (DEBUG_MODE==1)
                std::cout<<"\t\t[ shallow copying done from temperory variable ].";
                #endif
            }
            if(other.m_size==0&&other._col!=0)*ref_count+=1;
            other.~Matrix();
        }
        else
        {
            if (_row!=0&&m_size!=0)
            {
                #if (DEBUG_MODE==1)
                std::cout<<"\t\t memory stored (old data) has been deleted; instance degraded.\n";
                #endif

                DEALLOCATOR(m_data);
            }
            stride=other.stride;
            m_size=0;
            *ref_count+=1;
            m_data=other.m_data;

            #if (DEBUG_MODE==1)
            std::cout<<"\t\t[ shallow copying done pointer instance created. ]\n";
            #endif
        }
        return;
    }
    void operator=(const ValueType other)
    {
        LOOP_OVER_ELEMENTS(loc(i,j)=other;)
        #if (DEBUG_MODE==1)
            std::cout<<"[Assigning a scalar value ]\n";
        #endif
    }
    ~Matrix()                                                                  //Destroys the matrix object after checking if it's a slice.
    {
        #if (DEBUG_MODE==1)
            std::cout<<"\n  distructor :\t";
        #endif
        if (*ref_count<=0)                                        //slice have no memory ownership.so no memory freed.
        {
            DEALLOCATOR(m_data);
            m_size=0;
            ref_count=nullptr;
        }
        else
        {
            #if (DEBUG_MODE==1)
                std::cout<<"A pointer instance has been deleted.\n";
            #endif
            m_data=nullptr;
            if(!temp)*ref_count-=1;
        } 
    }
//----------------------------------------------- FUNCTIONS HANDLING MATRIX DATA ---------------------------------------------------

    bool is_SameShape(const_matrixRef other)const                  //check if matrix have same shape.
    {
        return other._row==_row&&other._col==_col;
    }
    static bool is_SameShape(const_matrixRef m1,const_matrixRef m2)                 //check if matrix have same shape.
    {
        return m1._row==m2._row&&m1._col==m2._col;
    }
    std::pair<int,int> shape() const{
        return std::make_pair(_row,_col);
    }
    size_t size() const {return m_size;}
    int row() const {return _row;}
    int col() const {return _col;}
    void set_loc(int r,int c,const T& other_loc)                   //Set a location on matrix with another value.
    {
            ASSERT(r>=_row &&c>=_col,"location out of index!")
            m_data[c+(stride+_col)*r]=other_loc;

            return;
    }
    ValueType& loc(int r,int c)const                               //returns a Readonly reference of specific location.
    {
            ASSERT(r>=_row || c>=_col,"location out of index!")
            return m_data[c+(stride+_col)*r];
    }
    ValueType& loc(int r,int c)                                    //(can be used instead of setloc())
    {                                                      
            ASSERT(r>=_row || c>=_col,"location out of index!")
            return m_data[c+(stride+_col)*r];
    }
    void matrix_details()                                          //details of the matrix
    {
        std::string status;
        std::cout<<"\n---------------------------------------------------------------------------";
        if (m_size==0&&_row!=0)status="reference";else if(temp)status="temperory";else status="permanant";
        std::cout<<"\n MEMORY SIZE [ OWNED ]         : "<<m_size*sizeof(T)<<"\t";
        std::cout<<"|  SHAPE OF MATRIX: [ROW x COL]  : "<<_row<<" x "<<_col;
        std::cout<<"\n STATUS                        : "<<status<<"\t";
        std::cout<<"|  REFERENCE COUNT               : "<<*ref_count;
        std::cout<<*this;
        std::cout<<"----------------------------------------------------------------------------\n";
    }

//----------------------------------------------- OPERATOR OVERLOADS FOR MATRIX ---------------------------------------------------
    ValueType& operator()(int row,int col)
    {
        return loc(row,col);
    }
    ValueType& operator()(int row,int col)const
    {
        return loc(row,col);
    }
    matrixRef& operator()(int row_index)
    {
        return get_row(row_index);
    }
    void operator<<(std::string values)
    {
        std::vector<std::string>line_data;int count=0;
        boost::split(line_data,values,boost::is_any_of(","));
        ASSERT(line_data.size()!=(_row*_col)," comma seperated string; [number of elements]")
        LOOP_OVER_ELEMENTS(set_loc(i,j,std::stof(line_data[count]));count++;)
    }

    matrixRef operator+(const ValueType& scalar_val)const
    {
        #if DEBUG_MODE
            std::cout<<"  calculated sum of matrix and scalar.\n";
        #endif
        matrixPtr result=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(result->set_loc(i,j,loc(i,j)+scalar_val);)
        return *result;
    }
    matrixRef operator-(const ValueType& scalar_val)const
    {
        #if DEBUG_MODE
            std::cout<<"  calculated difference of matrix and scalar.\n";
        #endif
        matrixPtr result=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(result->set_loc(i,j,loc(i,j)-scalar_val);)
        return *result;
    }
    matrixRef operator*(const ValueType& scalar_val)const
    {
        #if DEBUG_MODE
            std::cout<<"  multiplied matrix and scalar.\n";
        #endif
        matrixPtr result=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(result->set_loc(i,j,loc(i,j)*scalar_val);)

        return *result;
    }
    matrixRef operator/(const ValueType& scalar_val)const
    {
        #if DEBUG_MODE
            std::cout<<"  divided matrix with scalar.\n";
        #endif
        matrixPtr div=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(div->set_loc(i,j,loc(i,j)/scalar_val);)
        return *div;
    }
    void operator+=(const ValueType&scalar_val)
    {
        #if DEBUG_MODE
            std::cout<<"  calculated sum of matrix and scalar and assign it to self.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)+scalar_val);)
        return;

    }
    void operator-=(const ValueType&scalar_val)
    {
        #if DEBUG_MODE
            std::cout<<"  calculated difference of matrix and scalar and assign it to self.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)-scalar_val);)
        return ;

    }
    void operator*=(const ValueType&scalar_val)
    {
        #if DEBUG_MODE
            std::cout<<"  multiply matrix and scalar and assign it to self.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)*scalar_val);)
        return;

    }
    void operator/=(const ValueType&scalar_val)
    {
        #if DEBUG_MODE
            std::cout<<"  divide matrix and scalar and assign it to self.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)/scalar_val);)
        return;

    }

    friend matrixRef operator+(const ValueType& scalar_val,const_matrixRef matrix)
    {
        return matrix.operator+(scalar_val);
    }
    friend matrixRef operator*(const ValueType& scalar_val,const_matrixRef matrix)
    {
        return matrix.operator*(scalar_val);
    }
    friend matrixRef operator-(const ValueType& scalar_val,const_matrixRef matrix)
    {
        #if DEBUG_MODE
            std::cout<<"  calculated difference of scalar and matrix.\n";
        #endif
        Matrix* result=new Matrix(matrix._row,matrix._col,true);
        for (int i=0;i<matrix._row;i++)
        {
            for (int j=0;j<matrix._col;j++)
            {
                result->set_loc(i,j,scalar_val-matrix.loc(i,j));
            }
        }
        return *result;
    }
    friend matrixRef operator/(const ValueType& scalar_val,const_matrixRef matrix)
    {
        #if DEBUG_MODE
            std::cout<<"  divided scalar with matrix.\n";
        #endif
        Matrix* result=new Matrix(matrix._row,matrix._col,true);
        for (int i=0;i<matrix._row;i++)
        {
            for (int j=0;j<matrix._col;j++)
            {
                result->set_loc(i,j,scalar_val/matrix.loc(i,j));
            }
        }
        return *result;
    }

    matrixRef operator+(const_matrixRef m1)const
    {
        ASSERT(!(is_SameShape(m1)),"unable to sum two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"  calculated sum of two matrix.\n";
        #endif
        matrixPtr result=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(result->set_loc(i,j,loc(i,j)+m1.loc(i,j));)
        return *result;
    }
    matrixRef operator-(const_matrixRef m1)const
    {
        ASSERT(!(is_SameShape(m1)),"unable to substract two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"  calculated difference of two matrix.\n";
        #endif
        matrixPtr result=new Matrix(_row,_col,true);

        LOOP_OVER_ELEMENTS(result->set_loc(i,j,loc(i,j)-m1.loc(i,j));)
        return *result;
    }
    matrixRef operator*(const_matrixRef m1)const              //does matrix multiplication.(Matrix of size: [mxn]*[nxp] =[mxp])
    {
        auto [row,col]=m1.shape();
        ASSERT(_col!=row,"Unable multiply matrix with sizes: [ "<<_row<<" X "<<_col<<" ] with ["<<row<<" X "<<col<<" ]")
        matrixPtr result=new Matrix(_row,col,true);

        T sum=0;
        for (int i=0;i<_row;i++)
        {
            for(int j2=0;j2<col;j2++)
            {
                for (int a=0;a<_col;a++)
                {
                    sum+=loc(i,a)*m1.loc(a,j2);
                }
                result->loc(i,j2)=sum;
                sum=0;
            }
        }
        #if DEBUG_MODE
            std::cout<<"  multiplied two matrices.\n";
        #endif
        return *result;
    }
    void operator+=(const_matrixRef m1)
    {
        ASSERT(!(is_SameShape(m1)),"unable to sum two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"  calculated sum of two matrix.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)+m1.loc(i,j));)
        return;

    }
    void operator-=(const_matrixRef m1)
    {
        ASSERT(!(is_SameShape(m1)),"unable to difference two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"  calculated difference of two matrix.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)-m1.loc(i,j));)
        return;
    }
    void operator*=(const_matrixRef m1)// arraywise multiplication.
    {
        ASSERT(!(is_SameShape(m1)),"unable to arraywise multiply two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise multipication; two matrix.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)*m1.loc(i,j));)
        return;
    }
    void operator/=(const_matrixRef m1)// arraywise division.
    {
        ASSERT(!(is_SameShape(m1)),"unable to arraywise divide two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise division; two matrix.\n";
        #endif
        LOOP_OVER_ELEMENTS(set_loc(i,j,loc(i,j)/m1.loc(i,j));)
        return;
    }

    Matrix<bool>& operator>(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)>scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator<(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)<scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator>=(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)>=scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator<=(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)<=scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator==(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)==scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator!=(const ValueType& scalar_val)const
    {   
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)!=scalar_val)vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    friend Matrix<bool>& operator>(const ValueType& scalar_val,const_matrixRef m1)
    {   
        return m1.operator<(scalar_val);
    }
    friend Matrix<bool>& operator<(const ValueType& scalar_val,const_matrixRef m1)
    {   

        return m1.operator>(scalar_val);
    }
    friend Matrix<bool>& operator>=(const ValueType& scalar_val,const_matrixRef m1)
    {   
        return m1.operator<=(scalar_val);
    }
    friend Matrix<bool>& operator<=(const ValueType& scalar_val,const_matrixRef m1)
    {   
        return m1.operator>=(scalar_val);
    }
    friend Matrix<bool>& operator==(const ValueType& scalar_val,const_matrixRef m1)
    {   
        return m1.operator==(scalar_val);
    }
    friend Matrix<bool>& operator!=(const ValueType& scalar_val,const_matrixRef m1)
    {   
        return m1.operator!=(scalar_val);
    }
    Matrix<bool>& operator>(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)>matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator<(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)<matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator>=(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)>=matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator<=(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)<=matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator==(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)==matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }
    Matrix<bool>& operator!=(const_matrixRef matrix)const
    {   
        ASSERT(!(is_SameShape(matrix)),"unable to evaluvate two matrix with different shape.")
        std::vector<std::vector<bool>> vec;
        std::vector<bool>vec2;
        for (int i=0;i<_row;i++){
            for (int j=0;j<_col;j++){
                if(loc(i,j)!=matrix.loc(i,j))vec2.push_back(true);
                else vec2.push_back(false);
            }
            vec.push_back(vec2);vec2.clear();
        }
        return matrix::vector2matrix<Matrix<bool>>(vec);
    }

//-------------------------------------------- OTHER BASIC OPERATIONS WITH MATRIX -------------------------------------------------
    bool equal(const_matrixRef other)const
    {
        ASSERT(!(is_SameShape(other)),"cannot equate; shape are not same.")
        LOOP_OVER_ELEMENTS(if(loc(i,j)!=other.loc(i,j))return false;)
        return true;
    }
    bool not_equal(const_matrixRef other)const
    {
        ASSERT(!(is_SameShape(other)),"cannot equate; shape are not same.")
        LOOP_OVER_ELEMENTS(if(loc(i,j)!=other.loc(i,j))return true;)
        return false;
    }
    bool any()
    {
        LOOP_OVER_ELEMENTS(if(loc(i,j))return true;)
        return false;
    }
    bool all()
    {
        LOOP_OVER_ELEMENTS(if(!(loc(i,j)))return false;)
        return true;
    }
    ValueType mean()const                                                   //returns average value of elements in the matrix.
    {
        ValueType average=0;
        LOOP_OVER_ELEMENTS(average+=loc(i,j);)
        average=average/(_row*_col);
        return average;
    }
    ValueType minCoeff()const
    {
        ValueType min=loc(0,0);
        ValueType now;
        LOOP_OVER_ELEMENTS(now=loc(i,j);if(now<min)min=now;)
        return min;
    }
    ValueType maxCoeff()const
    {
        ValueType max=0;
        ValueType now;
        LOOP_OVER_ELEMENTS(now=loc(i,j);if(now>max){max=now;})
        return max;
    }
    ValueType sum()const
    {
        ValueType sum_=0;
        LOOP_OVER_ELEMENTS(sum_+=loc(i,j);)
        return sum_;
    }
    ValueType prod()const
    {
        ValueType result=1;
        LOOP_OVER_ELEMENTS(result*=loc(i,j);)
        return result;
    }
    ValueType trace()const
    {
        ASSERT(_row!=_col,"  not a square Matrix.")
        ValueType result=0;
        LOOP_OVER_ELEMENTS(if (i==j)result+=loc(i,j);)
        return result;
    }
    static void arraywise_multiply(matrixRef result,const_matrixRef m1) //function supporting multiplication(arraywise) without matrix creation
    {
        ASSERT(!(is_SameShape(m1,result)),"unable to multiply two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise multipication; two matrix.\n";
        #endif
        int _row=m1.row(),_col=m1.col();
        LOOP_OVER_ELEMENTS(result.set_loc(i,j,result.loc(i,j)*m1.loc(i,j));)
        return;
    }
    static void arraywise_divide(matrixRef result,const_matrixRef m1)
    {
        ASSERT(!(is_SameShape(m1,result)),"unable to divide two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise division; two matrix.\n";
        #endif
        int _row=m1.row(),_col=m1.col();
        LOOP_OVER_ELEMENTS(result.set_loc(i,j,result.loc(i,j)/m1.loc(i,j));)
        return;
    }
    matrixRef arraywise_multiply(const_matrixRef m1)const
    {
        ASSERT(!(is_SameShape(m1)),"unable to multiply two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise multipication; two matrix.\n";
        #endif
        matrixPtr mul=new Matrix(_row,_col,true);
        LOOP_OVER_ELEMENTS(mul->set_loc(i,j,loc(i,j)*m1.loc(i,j));)
        return *mul;
    }
    matrixRef arraywise_divide(const_matrixRef m1)const               //function supporting division(arraywise) without matrix creation.
    {
        ASSERT(!(is_SameShape(m1)),"unable to divide two matrix with different shape.")
        #if DEBUG_MODE
            std::cout<<"array wise division; two matrix.\n";
        #endif
        matrixPtr div=new Matrix(_row,_col,true);
        LOOP_OVER_ELEMENTS(div->set_loc(i,j,loc(i,j)/m1.loc(i,j));)
        return *div;
    }
    matrixRef row_wise(ValueType(*func)(const_matrixRef _matrix))
    {
            Matrix* row_result=new Matrix(row(),1);
            for(int i=0;i<row();i++) 
            row_result->loc(i,0)=func(get_row(i));
            return *row_result;
    }
    matrixRef col_wise(ValueType(*func)(const_matrixRef _matrix))
    {
            Matrix* col_result=new Matrix(1,col());

            for(int i=0;i<col();i++) 
            col_result->loc(0,i)=func(get_col(i));
            return *col_result;
    }
    static matrixRef row_wise(matrixRef _matrix,ValueType(*func)(const_matrixRef _matrix))
    {
            Matrix* row_result=new Matrix(_matrix.row(),1);
            for(int i=0;i<_matrix.row();i++) 
            row_result->loc(i,0)=func(_matrix.get_row(i));
            return *row_result;
    }
    static matrixRef col_wise(matrixRef _matrix,ValueType(*func)(const_matrixRef _matrix))
    {
            Matrix* col_result=new Matrix(1,_matrix.col());

            for(int i=0;i<_matrix.col();i++) 
            col_result->loc(0,i)=func(_matrix.get_col(i));
            return *col_result;
    }
    matrixRef round(int precision=0)
    {   
        ValueType ten_pow=std::pow(10,precision);

        LOOP_OVER_ELEMENTS
        (
            ValueType temp=std::round( loc(i,j)*ten_pow )/ten_pow;
            if(temp==0)temp=abs(temp);
            loc(i,j)=temp;
        )
        return *this;
    }

//------------------------------------------- ROW AND COLOUMN OPERATIONS ON MATRIX ------------------------------------------------

//  ALL THE OPERATIONS ARE DONE IN PLACE WITHOUT ANY MEMORY ALLOCATION.

    void row_switch(int row1,int row2)
    {
        for(int j=0;j<_col;j++)
        {
            ValueType temp=loc(row1,j);
            set_loc(row1,j,loc(row2,j));
            set_loc(row2,j,temp);
        }
        return;
    }
    void scale_row(int row1,ValueType scale)
    {
        for(int j=0;j<_col;j++)set_loc(row1,j,loc(row1,j)*scale);
        return;
    }
    void sum_rows(int row1,int row2, ValueType scale_row2=1)
    {
        for(int j=0;j<_col;j++)set_loc(row1,j,(loc(row1,j)+loc(row2,j)*scale_row2));
        return;
    }
    void col_switch(int col1,int col2)
    {
        for(int i=0;i<_row;i++)
        {
            ValueType temp=loc(i,col1);
            set_loc(i,col1,loc(i,col2));
            set_loc(i,col2,temp);
        }
        return;
    }
    void scale_col(int col1,ValueType scale)
    {
        for(int i=0;i<_row;i++)set_loc(i,col1,loc(i,col1)*scale);
        return;
    }
    void sum_cols(int col1,int col2, ValueType scale_col2=1)
    {
        for(int i=0;i<_row;i++)set_loc(i,col1,(loc(i,col1)+loc(i,col2)*scale_col2));
        return;
    }

//-------------------------------------------- ADVANCED OPERATIONS USING MATRIX ---------------------------------------------------

    matrixRef slice(int r1,int r2, int c1, int c2)const               //return a matrix reference without own memory buffer but is a pointer to another.
    {                                                      // have predefined set of rules for traversing the memory.
        if (r2==-1)r2=_row;   if (c2==-1)c2=_col;
        
        ASSERT(r1>_row || c1>_col || r2>_row || c2>_col,
        "Unable to slice; location out of index!")

        auto s_row=abs(r1-r2);
        auto s_col=abs(c1-c2);
        auto s_stride=(stride+_col-s_col);              //updating the stride; thereby changing how matrix traversing is done.

        ASSERT(s_col==0&&s_row==0,
        "Unable to slice; slice has no data!")

        matrixPtr sliced=new Matrix(s_row,s_col,s_stride,ref_count,&loc(r1,c1),true);

        #if (DEBUG_MODE==1)
        std::cout<<"  slice pointer created.\n";
        #endif

        return *sliced;
    }
    matrixRef topRows(int rows)
    {
        return slice(0,rows,0,-1);
    }
    matrixRef bottomRows(int rows)
    {
        rows=row()-rows;
        return slice(rows,-1,0,-1);
    }
    matrixRef leftCols(int cols)
    {
        return slice(0,-1,0,cols);
    }
    matrixRef rightCols(int cols)
    {
        cols=col()-cols;
        return slice(0,-1,cols,-1);
    }
    matrixRef diagonal()
    {
        ASSERT(_row!=_col,"  not a square Matrix.")
        matrixPtr result=new Matrix(_row,1,(_col+stride),ref_count,&loc(0,0),true);
        return *result;
    }
    matrixRef get_row(int row_index)
    {
        return slice(row_index,row_index+1,0,-1);
    }
    matrixRef get_col(int col_index)
    {
        return slice(0,-1,col_index,col_index+1);
    }

    void transpose()                                                  //transpose of a matrix or a slice.
    {
        if(!(SLICE))                           
        {
            ValueType* transpose=ALLOCATOR(m_size);                //allocating a new_block of memory for copy the data to.
            int index=0;

            for (int i=0;i<_col;i++)
            {
                for (int j=0;j<_row;j++)
                {
                    transpose[index]=loc(j,i);
                    index++;
                }
            }
            int temp=_row;
            _row=_col;
            _col=temp;
            DEALLOCATOR(m_data);                            //deallocating the old data buffer
            m_data=transpose;                               //setting new data buffer.
            transpose=nullptr;

            #if DEBUG_MODE
            std::cout<<"transpose done with memory allocation.\n";
            #endif
            
        }
        else
        {
            // For a slice inorder to perform a transpose the parent has to be modified.
            //So, it's only done if the slice is a square matrix.

            ASSERT(_row!=_col,"Unable to transpose. [not a square slice]")
            int j=0;int skip=0;int i=0;T temp;

            while(i<_row)
            {
                i=j=skip;
                
                while(j<_col)
                {
                    temp=loc(i,j);
                    set_loc(i,j,loc(j,i));                  //interchange the elements other than the diagonals.(2,1)to(1,2).
                    set_loc(j,i,temp);
                    j++;
                }
                skip++;
            }

            #if DEBUG_MODE
            std::cout<<"Square matrix; Transpose done without memory allocation.\n";
            #endif
        }
        return;
    }
    matrixRef submatrix(int row_index,int col_index)const
    {
        int row=_row-1;int col=_col-1;
        matrixPtr submatrix_=new Matrix (row,col);
        submatrix_->temp=true;

        int a=0,b=0;
        for (int i=0;i<_row-1;i++)
        {
            if(i==row_index)a=1;
            for (int j=0;j<_col-1;j++)
            {
                if(j==col_index)b=1;
                submatrix_->loc(i,j)=loc(i+a,j+b);
            }
            b=0;
        }
        return *submatrix_;
    }
    void submatrix(int row_index,int col_index,matrixRef result)const  //submatrix with no memory allocation.
    {
        auto[row,col]=result.shape();
        ASSERT(row!=_row-1||col!=_col-1,"submatrix operation failed! result shape should be [row-1,col-1]")
        int a=0,b=0;
        for (int i=0;i<_row-1;i++)
        {
            if(i==row_index)a=1;
            for (int j=0;j<_col-1;j++)
            {
                if(j==col_index)b=1;
                result.loc(i,j)=loc(i+a,j+b);
            }
            b=0;
        }
        return;
    }
    ValueType determinant()const
    {
        ASSERT(_row!=_col,"Unable to find determinant [ not a square matrix ]")
        
        ValueType determinant_;
        determinant_=Determinant(*this);
        return determinant_;
    }
    ValueType minor(int row_index,int col_index) const
    {
        ASSERT(_row!=_col,"Unable to find determinant [ not a square matrix ]")
        Matrix matrix=submatrix(row_index,col_index);
        ValueType minor=Determinant(matrix);
        return minor;
    }
    ValueType cofactor(int row_index,int col_index) const
    {
        return (minor(row_index,col_index)*pow(-1,(row_index+col_index)));
    }
    matrixRef adjugate() const
    {
        matrixPtr adjugate_=new Matrix(_row,_col,true);
        LOOP_OVER_ELEMENTS(adjugate_->loc(j,i)=cofactor(i,j);)
        return *adjugate_;
    }
    matrixRef inverse() const
    {
        matrixPtr adjugate_= new Matrix(adjugate());
        ValueType det=0;
        for (int j=0;j<_col;j++)
        {
            det+=(adjugate_->loc(j,0)*loc(0,j));
        }
        ASSERT(det==0,"cannot inverse [ determinant=0 ]")

        *adjugate_=(*adjugate_)/det;

        return*(adjugate_);
    }
    int rank()const
    {
        ValueType det=determinant();
        if(det!=0) return _row;

        int size=_row;
        while (true){   
            int r1=0,c1=0,r2=0,c2=0;
            size--;
            if(size==1)return 1;

            while(r2<_col){
                r2=r1+size;
                while(c2<_col){
                    c2=c1+size;
                    det=Determinant(slice(r1,r2,c1,c2));
                    if(det!=0) return size;
                    c1++;
                }
                r1++;c1=0;c2=0;
            }
        }
    }
//-------------------------------------------------- TYPE OF MATRICES ------------------------------------------------------------

    static matrixRef constant(int row,int col, ValueType val)                                      //returns a matrix with the given value as elements
    {
        matrixPtr constant_=new Matrix(row,col,true);
        for (int i=0;i<constant_->size();i++){constant_->m_data[i]=val;}
        return *constant_;
    }
    static matrixRef zeroes(int row,int col)                                                    //returns a matrix with the zero value as elements
    {
        matrixPtr zero_=new Matrix(row,col,true);
        for (int i=0;i<zero_->size();i++){zero_->m_data[i]=0;}
        return *zero_;
    }
    static matrixRef ones(int row,int col)                                                      //returns a unit matrix
    {
        matrixPtr ones_=new Matrix(row,col,true);
        for (int i=0;i<ones_->size();i++){ones_->m_data[i]=1;}
        return *ones_;
    }
    static matrixRef identity(int row,int col)                                                  //returns a identity matrix.
    {   
        matrixPtr identity_=new Matrix(row,col,true);
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                if (i==j)identity_->set_loc(i,j,1);
                else identity_->set_loc(i,j,0);
            }
        }   
        return *identity_;
    }
    static matrixRef diagonal(int row,int col,ValueType val)                                    //return diagonal matrix with given value.
    {
        matrixPtr diagonal_=new Matrix(row,col,true);
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                if (i==j)diagonal_->set_loc(i,j,val);
                else diagonal_->set_loc(i,j,0);
            }
        }   
        return *diagonal_;
    }
    static matrixRef random(int row,int col,ValueType lower_limit=-1, ValueType upper_limit=1)
    {
        static std::random_device rd;
        static std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(lower_limit, upper_limit);

        matrixPtr random_=new Matrix(row,col,true);
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                random_->loc(i,j)=dist(mt);
            }
        }
        return *random_;
    }
    
    void upper_triangular()
    {
        ASSERT(_row!=_col,"identity matrix must be a square matrix")

        for(int i=0;i<_row;i++)
        {  
            for(int j=0;j<_col;j++)
            { 
                if(i==j)break;
                loc(i,j)=0;
            }   
       }
       return;
    }
    void lower_triangular()
    {
        ASSERT(_row!=_col,"identity matrix must be a square matrix")
        int j=_col-1;
        for(int i=_row-1;i>=0;i--)
        {
            while(j>=0)
            {
                if(i==j)break;
                loc(i,j)=0;
                j--;
            }
            j=_col-1;
        }
        return;
    }

/*  #   Here we are checking the type of the matrix #   */

    bool is_square()const
    {
        return _row==_col;
    }
    bool is_symmetric()const
    {
        if(_row!=_col) return false;

        int j=0;int skip=0;int i=0;
        while(i<_row)
        {
            i=j=skip;
            
            while(j<_col)
            {
                if(!(loc(i,j)==loc(j,i)))return false;
                j++;
            }
            skip++;
        }

        return true;
    }
    bool is_diagonal()const
    {
        if(_row!=_col) return false;
        ValueType data=loc(0,0);
        LOOP_OVER_ELEMENTS
        (
            if (i==j)
            {
                if(!(loc(i,j)==data)) return false;
            }
            else{if(!(loc(i,j)==0))return false;}
        )
        return true;
    }
    bool is_identity()const
    {
        if(_row!=_col) return false;
        LOOP_OVER_ELEMENTS
        (
            if (i==j)
            {
                if(!(loc(i,j)==1)) return false;
            }
            else{if(!(loc(i,j)==0))return false;}
        )
        return true;
    }
    bool is_inverse(const_matrixRef matrix_)const
    {
        return (*this*matrix_).round().is_identity();
    }

//----------------------------------------------- OTHER ADDITIONAL FUNCTIONS--------------------------------------------------

    Iterator begin()                                       //returns begining of memory buffer.
    {
        return Iterator(m_data,_col,stride);
    }
    Iterator end()                                         //returns pointer to memory space after the memory buffer. 
    {
        return Iterator(m_data+((stride+_col)*_row),_col,stride);
    }

//--------------------------------------------------- FRIEND FUNCTIONS ------------------------------------------------------------

    friend std::ostream & operator<<(std::ostream& stream,const Matrix<ValueType>& mat)
    {
        stream<<"\n";
        for (int i=0;i<mat._row;i++)
        {   

            stream<<"   [ ";
            for (int j=0;j<mat._col;j++)
            {
                stream<<mat.loc(i,j);
                stream<<" ,";
            }
            stream<<"\b ]\n";
        }
        return stream;
    }
    friend matrixRef round(const_matrixRef matrix, int precision=0)
    {   
        ValueType ten_pow=std::pow(10,precision);
        int _row=matrix.row();int _col=matrix.col();
        
        Matrix* result=new Matrix(_row,_col);

        LOOP_OVER_ELEMENTS  
        (
            ValueType temp=std::round( matrix.loc(i,j)*ten_pow )/ten_pow;
            if(temp==0)temp=abs(temp);
            result->loc(i,j)=temp;
        )
        return *result;
    }
    friend matrixRef pow(const_matrixRef matrix, int power=1)
    {   
        int _row=matrix.row();int _col=matrix.col();
        
        Matrix* result=new Matrix(_row,_col);

        LOOP_OVER_ELEMENTS  
        (
            result->loc(i,j)=std::pow(matrix.loc(i,j),power);
        )
        return *result;
    }
    friend matrixRef sqrt(const_matrixRef matrix)
    {   
        int _row=matrix.row();int _col=matrix.col();
        
        Matrix* result=new Matrix(_row,_col);

        LOOP_OVER_ELEMENTS  
        (
            result->loc(i,j)=std::sqrt(matrix.loc(i,j));
        )
        return *result;
    }

    friend  matrixRef matrix_stride(const_matrixRef input_map, const_matrixRef filter
            ,int stride_row=1,int stride_col=1,ValueType(*func)(const_matrixRef,const_matrixRef) //Function specifically made to support convolution operation.
            =[](const_matrixRef b,const_matrixRef c){ValueType data=0;return data;})
    {
        auto[filter_row,filter_col]=filter.shape();
        auto[map_row,map_col]=input_map.shape();
        ASSERT(map_row<filter_row||map_col<filter_col,"feature row or col less than that of filter")
        float new_row=(float(map_row-filter_row)/stride_row)+1;
        float new_col=(float(map_col-filter_col)/stride_col)+1;

        ASSERT((new_row!=std::round(new_row)),"Unable to make row_stride. [not compactable]")
        ASSERT((new_col!=std::round(new_col)),"Unable to make col_stride. [not compactable]")
        
        new_col=int(new_col);
        new_row=int(new_row);
        #if DEBUG_MODE
            std::cout<<"\nmatrix_stride result[double matrix input function]: \n";
        #endif
        matrixPtr new_matrix =new Matrix(new_row,new_col,true);
        
        int i=0,j=0,i_end=0,j_end=0,index=0;

        while(i_end<map_row)
        {
            i_end=i+filter_row;
            while(j_end<map_col)
            {
                j_end=j+filter_col;
                
                T val=func(input_map.slice(i,i_end,j,j_end),filter);                //function that does the required operation.

                new_matrix->m_data[index]=val;
                
                j+=stride_col;
                index++;
            }
            i+=stride_row;
            j=0;j_end=0;
        }
        return *new_matrix;
    }
    friend  matrixRef matrix_stride(const_matrixRef input_map,int filter_row,int filter_col
            ,int stride_row=1,int stride_col=1,ValueType(*func)(const_matrixRef)              //Function specifically made to support pooling operations.
            =[](const_matrixRef b){ValueType data=0;return data;})
    {
        auto[map_row,map_col]=input_map.shape();
        ASSERT(map_row<filter_row||map_col<filter_col,"feature row or col less than that of filter")
        float new_row=(float(map_row-filter_row)/stride_row)+1;
        float new_col=(float(map_col-filter_col)/stride_col)+1;

        ASSERT((new_row!=std::round(new_row)),"Unable to make row_stride. [not compactable]")
        ASSERT((new_col!=std::round(new_col)),"Unable to make col_stride. [not compactable]")
        
        new_col=int(new_col);
        new_row=int(new_row);
        #if DEBUG_MODE
            std::cout<<"\nmatrix_stride result[single matrix input function]: \n";
        #endif
        matrixPtr new_matrix =new Matrix(new_row,new_col,true);
        
        int i=0,j=0,i_end=0,j_end=0,index=0;

        while(i_end<map_row)
        {
            i_end=i+filter_row;
            while(j_end<map_col)
            {
                j_end=j+filter_col;
                
                T val=func(input_map.slice(i,i_end,j,j_end));                       //function that does the required operation.

                new_matrix->m_data[index]=val;
                
                j+=stride_col;
                index++;
            }
            i+=stride_row;
            j=0;j_end=0;
        }
        return *new_matrix;
    }
private:
    static ValueType Determinant(const_matrixRef matrix)
    {   
        int size=matrix.row();
        ValueType det=0;int sign=1;
        if(size==1)
        {
            return matrix.loc(0,0);
        }
        if(size==2)
        {
            return (matrix.loc(0,0)*matrix.loc(1,1) - matrix.loc(0,1)*matrix.loc(1,0));
        }
        Matrix submatrix_(size-1,size-1);                           // the allocation of memory by the submatrix needs to be changed to stack somehow !!!!!!!!!!!!!!!!!!!!!.
        for(int i=0;i<size;i++)
        {
            matrix.submatrix(0,i,submatrix_);
            det+= sign*matrix.loc(0,i)*Determinant(submatrix_);
            sign=-sign;
        }
        return det;
    }
};

namespace matrix
{
    typedef Matrix<float> MatrixF;
    typedef Matrix<int> MatrixInt;
    typedef Matrix<double> MatrixDbl;
    typedef Matrix<bool> MatrixBool;
};
#endif
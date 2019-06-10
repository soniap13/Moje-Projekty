#include "klasy.hpp"
using namespace std;

string matrix_exception::what(){
    return description;
}

size_exception::size_exception(){
    description="Matrix has wrong size for that operation!!!";
}

nonexistent_element::nonexistent_element(){
    description="You are trying to use nonexistent element, row or column!!!";
}

zero_det_exception::zero_det_exception(){
    description="Matrix has not opposite";
}

Matrix::Matrix(int n){
    rows=n;
    columns=n;
    for(int i=0; i<n; i++){
        T[i]=new double [n];
        for(int j=0;j<n;j++)
            T[i][j]=0;
        T[i][i]=1;
    }
}

Matrix::Matrix(int m,int n){
    rows=m;
    columns=n;
    for(int i=0;i<m;i++){
        T[i]=new double[n];
        for(int j=0;j<n;j++)
            T[i][j]=0;
    }
}

Matrix::Matrix(const Matrix &m){
    rows=m.rows;
    columns=m.columns;
    for(int i=0; i<rows; i++){
        T[i]=new double [columns];
        for(int j=0; j<columns; j++)
            T[i][j]=m.T[i][j];
    }
}

Matrix::Matrix(Matrix &&m){
    rows=m.rows;
    columns=m.columns;
    for(int i=0; i<rows; i++){
        T[i]=new double [columns];
        T[i]=m.T[i];
        m.T[i]=nullptr;
    }
    m.rows=0;
    m.columns=0;
}

Matrix& Matrix::operator = (const Matrix &m){
    rows=m.rows;
    columns=m.columns;
    for(int i=0; i<rows; i++){
        T[i]= new double[columns];
        for(int j=0; j<columns; j++)
            T[i][j]=m.T[i][j];
    }

return *this;
}

Matrix& Matrix::operator = (Matrix &&m){
    rows=m.rows;
    columns=m.columns;
    for(int i=0; i<rows; i++){
        T[i]= new double[columns];
        for(int j=0; j<columns; j++)
            T[i][j]=m.T[i][j];
    }
    for(int i=0; i<rows; i++)
        delete [] m.T[i];
    m.rows=0;
    m.columns=0;
return *this;
}

Matrix::~Matrix(){
    for(int i=0; i<rows; i++)
        delete [] T[i];
}

Matrix operator * (double a, const Matrix &m){
    Matrix w(m.rows, m.columns);
    for(int i=0; i<m.rows; i++){
        for(int j=0; j<m.columns; j++)
            w.T[i][j]=a*m.T[i][j];
    }
    return w;
}

Matrix operator + (const Matrix &m1, const Matrix &m2){

    if(m1.rows!=m2.rows || m1.columns!=m2.columns)
        throw size_exception();
    Matrix w(m1.rows, m1.columns);
    for(int i=0; i<m1.rows; i++){
        for(int j=0; j<m1.columns; j++)
            w.T[i][j]=m1.T[i][j] + m2.T[i][j];
    }
    return w;
}

Matrix operator - (const Matrix &m1, const Matrix &m2){
    if(m1.rows!=m2.rows || m1.columns!=m2.columns)
        throw size_exception();
    Matrix w(m1.rows, m1.columns);
    for(int i=0; i<m1.rows; i++){
        for(int j=0; j<m1.columns; j++)
            w.T[i][j]=m1.T[i][j] - m2.T[i][j];
    }
    return w;
}

Matrix operator * (const Matrix &m1, const Matrix &m2){
    if(m1.columns!=m2.rows)
        throw size_exception();
    Matrix w(m2.rows, m2.rows);
    for(int i=0; i<m2.rows; i++)
        for(int j=0; j<m2.rows; j++)
            for(int k=0; k<m2.rows; k++)
                w.T[i][j]=w.T[i][j] + (m1.T[i][k] * m2.T[k][j]);


    return w;
}

void Matrix::trans (){
    if(rows!=columns)
        throw size_exception();
    for(int i=0; i<rows; i++)
        for(int j=i+1; j<columns; j++)
            swap(T[i][j], T[j][i]);
}

istream & operator >> (istream &in, Matrix &m){
    for(int j=0; j<m.columns; j++)
        for(int i=0; i<m.rows; i++)
           in>>m.T[i][j];
return in;
}

ostream & operator << (ostream &out, const Matrix &m){
    for(int i=0; i<m.rows; i++){
        for(int j=0; j<m.columns; j++)
           out<<m.T[i][j]<<"   ";
        out<<endl;
    }
    out<<endl;
return out;
}

void Matrix::swap_rows(int a, int b){
    if(a>=rows || b>=rows)
        throw nonexistent_element();
    for(int j=0; j<columns; j++){
        swap(T[a][j],T[b][j]);
    }
}

void Matrix::swap_columns(int a, int b){
    if(a>=columns || b>=columns)
        throw nonexistent_element();
    for(int i=0; i<rows; i++){
        swap(T[i][a],T[i][b]);
    }
}

void Matrix::mult_row(int k, double a){
    if(k>=rows)
        throw nonexistent_element();
    for(int j=0; j<columns; j++)
        T[k][j]=a*T[k][j];
}

void Matrix::mult_column(int l, double a){
    if(l>=columns)
        throw nonexistent_element();
    for(int i=0; i<rows; i++)
        T[i][l]=a*T[i][l];
}

void Matrix::add_factor_row(int k1, int k2, double a){
    if(k1>=rows || k2 >=rows)
        throw nonexistent_element();
    for(int j=0; j<columns; j++)
        T[k1][j]+=a*T[k2][j];
}

void Matrix::add_factor_column(int l1, int l2, double a){
    if(l1>=columns || l2 >=columns)
        throw nonexistent_element();
    for(int i=0; i<rows; i++)
        T[i][l1]+=a*T[i][l2];
}

void Matrix::delete_row_and_column(int k, int l){
    if(k>=rows || l>=columns)
        throw nonexistent_element();
    for(int i=0; i<rows; i++)
        for(int j=l; j<columns-1; j++)
            T[i][j]=T[i][j+1];
    for(int j=0; j<columns-1; j++)
        for(int i=k; i<rows-1; i++)
            T[i][j]=T[i+1][j];
    rows--;
    columns--;
}

double Matrix::det(){
    if(rows!=columns)
        throw size_exception();
    if(rows==1)
        return T[0][0];
    int suma=0;
    for(int i=0; i<rows; i++){
        Matrix help(*this);
        help.delete_row_and_column(i,0);
        if(i%2==0)
            suma+=T[i][0]*help.det();
        else
            suma-=T[i][0]*help.det();
    }
    return suma;
}

void opposite_help(Matrix &view, Matrix &op){
    int k,j;
    int rows=view.rows;
    for(int l=0; l<rows; l++)
        for(int i=l+1; i<rows; i++)
            while(view.T[i][l]!=0){
                op.add_factor_row(i,l,-view.T[i][l]/view.T[l][l]);
                view.add_factor_row(i,l,-view.T[i][l]/view.T[l][l]);
                k=0;
                while(k<rows){
                    j=k+1;
                    while(view.T[k][k]==0){
                        view.swap_rows(k,j);
                        op.swap_rows(k,j);
                        j++;
                    }
                    k++;
                }
            }
}

Matrix Matrix::opposite(){
    if (det()==0)
        throw zero_det_exception();
    Matrix op(rows);
    Matrix view(*this);
    opposite_help(view,op);
    for(int i=0; i<rows; i++){
        op.mult_row(i,1/view.T[i][i]);
        view.mult_row(i,1/view.T[i][i]);
    }
    op.trans();
    view.trans();
    opposite_help(view,op);
    op.trans();
    view.trans();
return op;
}

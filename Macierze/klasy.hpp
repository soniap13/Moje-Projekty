#include<iostream>
#include<stdexcept>

class matrix_exception : public std::exception {
    protected: std::string description;
    public: std::string what();
};

class size_exception : public matrix_exception {
    public: size_exception();
};

class nonexistent_element : public matrix_exception{
    public: nonexistent_element();
};

class zero_det_exception : public matrix_exception{
    public: zero_det_exception();
};

class Matrix{

    private:
        double **T=new double*[100];
        int rows=0;
        int columns=0;

    public:
        Matrix(int n);
        Matrix(int m,int n);
        Matrix(const Matrix &m);
        Matrix(Matrix &&m);
        ~Matrix();

        Matrix& operator = (const Matrix &m);
        Matrix& operator = (Matrix &&m);
        Matrix friend operator * (double a, const Matrix &m);
        Matrix friend operator + (const Matrix &m1, const Matrix &m2);
        Matrix friend operator - (const Matrix &m1, const Matrix &m2);
        Matrix friend operator * (const Matrix &m1, const Matrix &m2);
        void trans();
        friend std::istream & operator >> (std::istream &in, Matrix &m);
        friend std::ostream & operator << (std::ostream &out, const Matrix &m);
        friend std::ostream & operator << (std::ostream &out, const Matrix &m);
        void swap_rows(int a, int b);
        void swap_columns(int a, int b);
        void mult_row(int k, double a);
        void mult_column(int l, double a);
        void add_factor_row(int k1, int k2, double a);
        void add_factor_column(int l1, int l2, double a);
        void delete_row_and_column(int k, int l);
        double det();
        Matrix opposite();
        friend void opposite_help(Matrix &view, Matrix &op);

};




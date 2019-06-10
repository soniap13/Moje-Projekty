#include "funkcje.cpp"

int main()
{
    try{

        Matrix m1(2);
        Matrix m3(3);
        Matrix m2(4);
        cout<<m1;
        cout<<m2;
        m1=m2;
        m1.mult_row(0,7);
        cout<<m1;
        cout<<m2;
        m2=move(m3);
        cout<<m2;
        cout<<"m3:";
        cout<<m3;
        /*
        m1.mult_row(12,4);
        cout<<m1;

        cout<<m1.det()<<endl<<endl;
        m1.delete_row_and_column(1,2);
        cout<<m1;
        cout<<m1.det()<<endl<<endl;

        Matrix m3(m2);
        cout<<m3;
        Matrix m4(move(m1));
        cout<<m4;
        Matrix m5(5);
        Matrix m6=m5+m4;
        cout<<m6;
        Matrix m7=m5-m4;
        cout<<m7;
        Matrix m8=m5*m4;
        cout<<m8;
        Matrix m9=4*m5;
        cout<<m9;
        m9.swap_rows(1,2);
        cout<<m9;
        m9.swap_columns(1,2);
        cout<<m9;
        m9.mult_row(1,2);
        cout<<m9;
        m9.mult_column(1,2);
        cout<<m9;
        m9.add_factor_row(1,2,7);
        cout<<m9;
        m9.add_factor_column(1,2,7);
        cout<<m9;
        m9.delete_row_and_column(1,2);
        cout<<m9;

        Matrix m10(3);
        cin>>m10;
        cout<<m10;
        Matrix m13(3);
        cin>>m13;
        cout<<m13;
        cout<<m10*m13;

        Matrix m11(m10);
        m11.trans();
        cout<<m10;
        cout<<m11;
        cout<<"wyznacznik m11: "<<m11.det()<<endl;
        Matrix m12=m11.opposite();
        cout<<m12;
        cout<<m11*m12;
        cout<<m12*m11;
        */


    }
    catch(matrix_exception e){
        cout<<e.what();
    }

return 0;
}

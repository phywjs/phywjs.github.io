#include <iostream.h>
#include <assert.h>

template <class T>
class Matrix {
public:
  Matrix(int, int);
  Matrix(int, int, T a[]);
  ~Matrix();

  Matrix<T> &operator+=(Matrix<T> &);
  Matrix<T> &operator*=(Matrix<T> &);

  friend ostream &operator<<(ostream &, Matrix<T> &);

private:
  int m, n;     // dimension of matrix
  T * el;       // elments (size m*n)
}; 


template <class T>
Matrix<T>::Matrix(int a, int b)  // initialize a row matrix
{
   assert(a >= 1 && b >= 1);
   m = a;
   n = b;
   el = new T[m*n];
}

template <class T>
Matrix<T>::Matrix(int a, int b, T arr[]) 
{
   assert(a >= 1 && b >= 1);
   m = a;
   n = b;
   el = new T[m*n];
   for(int i = 0; i < m*n; ++i) {
      el[i] = arr[i];
   }
}
 
template <class T>
Matrix<T>::~Matrix()  
{
   delete [] el;
}

template <class T>
Matrix<T>& Matrix<T>::operator+=(Matrix<T> &mleft)
{
   assert(mleft.m == m && mleft.n == n);
   for(int i = 0; i < m*n; ++i) {
      el[i] += mleft.el[i];
   }
   return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator*=(Matrix<T> &mleft)
{
   int i, j, k;
   T dot;
   
   assert(n == mleft.m);
   Matrix c(m, mleft.n);

   for(i = 0; i < m; ++i) {
      for(j = 0; j < mleft.n; ++j) {
         dot = 0.0;
         for(k = 0; k < n; ++k) {
            dot += el[i*n+k] * mleft.el[k*mleft.n+j];
         }
         c.el[i*mleft.n+j] = dot; 
      }
   }

   for(i = 0; i < m*mleft.n; ++i) {      // copy back to current object
      el[i] = c.el[i];
   }
   return *this;
}

template <class T>
ostream& operator<<(ostream & out, Matrix<T> & a)
{
   for(int i = 0; i < a.m*a.n; ++i) {
      out << a.el[i] << ' ';
      if( (i%a.n) == a.n-1 ) out << endl;
   }
   return out;
}

main()
{
   double arr1[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
   double arr2[12] = {1,0,3,0,5,0,7,0,9,0,11,0};
   double arr3[16] = {1,0,0,0, 0,2,0,0, 1,2,3,4, 1,0,3,1};
   Matrix<double> a(3,4,arr1), b(3,4,arr2), c(4,4,arr3);

   cout << "a = " << a << endl;
   cout << "b = " << b << endl;
   cout << "c = " << c << endl;
   cout << "a + b = " << (a += b) << endl;
   cout << "b * c = " << (b *= c) << endl;
}

#include <iostream.h>

// Set class
template<class T>
class Set { 
   public:
      Set(int = 0);
      Set(int, T *list);
      ~Set() { delete [] els; }
      friend ostream &operator<<(ostream &, const Set<T> &);
      int belong(const T&);
      Set<T> setunion(const Set<T> &S);
   private:
      int cardinality;          // size of the set
      T *els;                   // array of elements
};

// Constructor (default to empty set)
template<class T>
Set<T>::Set(int s)
{
   cardinality = (s>0)? s : 0; 
   if (cardinality > 0) {
      els = new T[cardinality];
   } 
   for(int i = 0; i < cardinality; ++i) {
      els[i] = i;
   }
}


template<class T>
Set<T>::Set(int s, T *list)
{
   if(s > 0) {
      cardinality = s;
      els = new T[cardinality];
      for(int i = 0; i < s; ++i) {
         els[i] = list[i];
      }
   } else {
      cardinality = 0;      // empty set
   } 
}

// overload << for output
template<class T>
ostream &operator<<(ostream & inp, const Set<T> &S)
{
   inp << "Set(";
   for(int i = 0; i < S.cardinality; ++i) {
      inp << S.els[i] <<", ";
   }
   inp << ")\n";

   return inp;
}

template<class T>
int Set<T>::belong(const T &e)
{
   int eq;
   eq = 0;
   for(int i = 0; !eq && i < cardinality; ++i) {
      eq = (els[i] == e);       
   }
   return eq;
}

template<class T>
Set<T> Set<T>::setunion(const Set<T> &S)
{
   T *tmp_el;
   int j;
   
   tmp_el = new T[cardinality + S.cardinality];
   
   for(int i = 0; i < cardinality; ++i) {
      tmp_el[i] = els[i];
   }
   j = cardinality;
   for(int i = 0; i < S.cardinality; ++i) { 
      if (!belong(S.els[i])) {
         tmp_el[j++] = S.els[i]; 
      }
   }
   // create new Set (union of current with S)
   Set<T> C(j, tmp_el);

   return C;
}

// main program for testing
int main()
{
   Set<int> A, B(10);
 
   int lis[5] = {-2,-1,1,2,3};
   Set<int> C(5, lis);

   cout << A;
   cout << B;
   cout << C;

   int k = 7;
   cout << "is " << k << " in " << endl;
   cout << C;
   cout << C.belong(k) << endl;

   cout << "union of ";
   cout << A << "and " << B << " is ";
   cout << A.setunion(B);

   cout << "union of ";
   cout << B << "and " << C << " is ";
   cout << B.setunion(C);

   return 0;
}

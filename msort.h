#include<vector>
using namespace std;
template <class T, class Comparator>
void mergeSort(vector<T>& myArray, Comparator comp)
{
  int l = 0; 
  int r = myArray.size()-1;
  mergesort(myArray, l, r, comp);
}

template <class T, class Comparator>
void mergesort(vector<T>& myArray, int l, int r, Comparator comp)
{
  if (l<r)
  {
  	int m = (l+r)/2;
  	mergesort(myArray, l, m, comp);
  	mergesort(myArray, m+1, r, comp);
  	merge(myArray, l, r, m, comp);
  }
}

template <class T, class Comparator>
void merge(vector<T>& myArray, int l, int r, int m, Comparator comp)
{
	//cout<<"test"<<endl;
	//cout<<l<<" "<<r<<" "<<m<<endl;
	vector<T> temp(r+1-l); 
	int i = l, j =m+1, k=0;
	while (i <=m || j <=r){
	    if (i <=m && (j > r || comp(myArray[i],myArray[j]))){
	   /* smallest is on left */
	     temp[k] = myArray[i];
	     i++;
	     k++;
	    } 
	   else {
	   /*smallest is in the right half*/
	     temp[k] = myArray[j];
	     j++;
	     k++;
	   }
	}
	for (k=0; k < r+1-l; k++) myArray[k+l] = temp[k]; 
}

       

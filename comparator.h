#include "product.h"
struct Rcomp{
   bool operator()(Product* a, Product* b)
   {
   	 //cout<<"test"<<endl;
   	 //find rating for a 
   	 double aRate = 0;
   	 double sum = 0;
   	 vector<Review> aComments = a->getReviews();
     for(unsigned int i = 0; i<aComments.size(); i++)
     {
        sum += (double)aComments[i].getRating();
     }
     aRate = sum/aComments.size();
     //find rating for b
     double bRate = 0;
     sum = 0;
     vector<Review> bComments = b->getReviews();
     for(unsigned int i = 0; i<bComments.size(); i++)
     {
        sum += (double)bComments[i].getRating();
     }
     bRate = sum/bComments.size();
     
     //cout<<"test"<<endl;
     // cout<<aRate<<" "<<bRate<<endl;
     // //cout<<bRate>=aRate<<endl;
     // cout<<"------"<<endl;
     return bRate<=aRate;
   }
};

struct Ncomp{
	bool operator()(Product* a, Product* b)
    {
    	return a->getName()<= b->getName();
    }
};

struct Dcomp{
    bool operator()(Review a, Review b)
    {
    	return b.getDate()<=a.getDate();
    }
};

struct Pcomp{
    bool operator()(pair<string, double> a, pair<string, double> b)
    {
       if (a.second != b.second)
       {
           return a.second > b.second;
       }
       else
       {
           return a.first < b.first;
       }
    }
};
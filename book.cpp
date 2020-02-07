#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>
using namespace std;
Book::Book(const std::string category, const std::string name, double price, int qty, string ISBN, string author):
      Product(category, name, price, qty),
      ISBN_(ISBN),
      author_(author)
{

}
Book::~Book()
{
}

set<string> Book::keywords() const
{
	set<string> keywords = parseStringToWords(this->getName());
    keywords.insert(ISBN_);
    set<string> name = parseStringToWords(author_);
    set<string>::iterator it;
    for(it = name.begin(); it!= name.end(); it++)
    {
    	keywords.insert(*it);
    }
    return keywords;
}

bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
	return false;
}

string Book::displayString() const
{
   string first = this->getName();
   string second = "Author: " + this->getAuthor() + " " + "ISBN: " + this->getISBN();
   stringstream s1;
   s1<<this->getPrice();
   stringstream s2;
   s2<<this->getQty();
   string third = s1.str() + " " + s2.str() + " " + "left.";
   string fourth; 
   double sum = 0;
   for(unsigned int i = 0; i<reviews_.size(); i++)
   {
     sum += (double)reviews_[i].getRating();
   }
   double average = sum/reviews_.size();
   stringstream s3;
   s3<<"Rating: "<<fixed<<setprecision(2)<<average;
   fourth = s3.str();
   string total = first + "\n" + second + "\n" + third + "\n" + fourth;
   return total;
}

void Book::dump(std::ostream& os) const
{
   os<<"book"<<"\n"<<this->getName()<<"\n"<<this->getPrice()<<"\n"<<this->getQty()<<"\n"<<this->getISBN()<<"\n"<<this->getAuthor()<<endl;
}

string Book::getISBN() const
{
	return ISBN_;
}

string Book::getAuthor() const
{
	return author_;
}


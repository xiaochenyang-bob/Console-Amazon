#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>
using namespace std;
Clothing::Clothing(const std::string category, const std::string name, double price, int qty, string size, string brand):
      Product(category, name, price, qty),
      size_(size),
      brand_(brand)
{

}
Clothing::~Clothing()
{
}

set<string> Clothing::keywords() const
{
	set<string> keywords = parseStringToWords(this->getName());
    set<string> name = parseStringToWords(brand_);
    set<string>::iterator it;
    for(it = name.begin(); it!= name.end(); it++)
    {
    	keywords.insert(*it);
    }
    return keywords;
}

bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
	return false;
}

string Clothing::displayString() const
{
   string first = this->getName();
   string second = "brand: " + this->getBrand() + " " + "size: " + this->getSize();
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

void Clothing::dump(std::ostream& os) const
{
   os<<"clothing"<<"\n"<<this->getName()<<"\n"<<this->getPrice()<<"\n"<<this->getQty()<<"\n"<<this->getSize()<<"\n"<<this->getBrand()<<endl;
}

string Clothing::getSize() const
{
	return size_;
}

string Clothing::getBrand() const
{
	return brand_;
}


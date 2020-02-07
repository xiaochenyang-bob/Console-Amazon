#include "movie.h"
#include "util.h"
#include <sstream>
#include <iomanip>
using namespace std;
Movie::Movie(const std::string category, const std::string name, double price, int qty, string genre, string rating):
      Product(category, name, price, qty),
      genre_(genre),
      rating_(rating)
{

}
Movie::~Movie()
{
}

set<string> Movie::keywords() const
{
	set<string> keywords = parseStringToWords(this->getName());
    keywords.insert(genre_);
    return keywords;
}

bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
	return false;
}

string Movie::displayString() const
{
   string first = this->getName();
   string second = "rating: " + this->getRating() + " " + "genre: " + this->getGenre();
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

void Movie::dump(std::ostream& os) const
{
   os<<"movie"<<"\n"<<this->getName()<<"\n"<<this->getPrice()<<"\n"<<this->getQty()<<"\n"<<this->getGenre()<<"\n"<<this->getRating()<<endl;
}

string Movie::getGenre() const
{
	return genre_;
}

string Movie::getRating() const
{
	return rating_;
}


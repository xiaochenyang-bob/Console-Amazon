#include "review.h"
using namespace std; 

Review::Review() :  rating(0), username(), date(), reviewText()
    { }

Review::~Review()
{}

Review::Review(int rate,
           const std::string& u,
           const std::string& d,
           const std::string& review_text) :

        rating(rate),
        username(u),
        date(d),
        reviewText(review_text)
    { }

int Review::getRating() const
{
	return rating;
}

string Review::getUsername() const
{
	return username;
}

string Review::getDate() const
{
   return date;
}

string Review::getText() const
{
	return reviewText;
}

void Review::dump(std::ostream& os) const
{
	os<<rating<<" "<<username<<" "<<date<<" "<<reviewText<<endl;
}

void Review::viewReviews(std::ostream& os) const
{
  os<<date<<" "<<rating<<" "<<username<<" "<<reviewText<<endl;
}
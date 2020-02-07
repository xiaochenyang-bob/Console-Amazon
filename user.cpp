#include "user.h"
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1),hashcode_(0)
{

}
User::User(std::string name, double balance, int type, unsigned long long hashcode) :
    name_(name), balance_(balance), type_(type), hashcode_(hashcode)
{

}

User::~User()
{

}


std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

unsigned long long User::getHashCode() const
{
	return hashcode_;
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << balance_ << " " << type_ << endl;
}


#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <map>
#include <vector>
#include "datastore.h"
#include "heap.h"
#include "util.h"
#include "comparator.h"

class MyDataStore: public DataStore
{
public:
	MyDataStore();
	~MyDataStore();
	void addProduct(Product* p);
	void addUser(User* u);
	void addReview(const std::string& prodName,
			   int rating,
			   const std::string& username,
			   const std::string& date,
			   const std::string& review_text);
	std::vector<Product*> search(std::vector<std::string>& terms, int type);
	void dump(std::ostream& ofile);
	//dummies
	void viewCart();
	void buyCart();
	void quit();
	double findBasicSimilarity(User* a, User* b);
	std::set<User*> getUsers();
	std::map<User*, std::vector<std::pair<double, User*> > > produceMap();
	std::map<User*, double> getDist(User* a);
	std::set<Product*> notReviewed(User* a);
	double getRefinedRate(Product* p, User* a);
	std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
private: 
	std::set<Product*> products_;
	std::set<User*> users_;
};
#endif
#include "mydatastore.h"
#include <cmath>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
MyDataStore::MyDataStore()
{}
MyDataStore::~MyDataStore()
{
	set<Product*>::iterator it;
	for(it = products_.begin(); it != products_.end(); it++)
	{
		delete *it;
	}
	set<User*>::iterator location;
	for(location = users_.begin(); location != users_.end(); location++)
	{
		delete *location;
	}
}

void MyDataStore::addProduct(Product* p)
{
	products_.insert(p);
}

void MyDataStore::addUser(User* u)
{
	users_.insert(u);
}

void MyDataStore::addReview(const std::string& prodName,
			   int rating,
			   const std::string& username,
			   const std::string& date,
			   const std::string& review_text)
{
	Review comment = Review(rating,username, date, review_text);
	for (set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it)
	{
		if ((*it)->getName() == prodName)
		{
			(*it)->addComment(comment);
		}
	}
	return;
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type)
{
	for (unsigned int i = 0; i<terms.size(); i++)
	{
		terms[i] = convToLower(terms[i]);
	}
	set<Product*> results;
	//cout<<terms.size()<<endl;
	vector<set<Product*> > all;
	for (unsigned int i = 0; i<terms.size(); i++)
	{
		set<Product*> add;
		//set<Product*>::iterator it;
		for (set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it)
		{
			set<string> keywords = (*it)->keywords();
			// for (set<string>::iterator it = keywords.begin(); it!=keywords.end(); it++)
			// {
			// 	cout<<*it<<endl;
			// }
			//set<string>::iterator location;
			for (set<string>::iterator location = keywords.begin(); location != keywords.end(); ++location)
			{
				if (*location == terms[i])
				{
					add.insert(*it);
					//cout<<terms[i]<<endl;
				}
			}
 		}
 		all.push_back(add);
	}
	if (type == 0)
	{
		results = all[0];
		for (unsigned int i = 0; i<all.size(); i++)
		{
			results = setIntersection(results, all[i]);
		}
	}
	else if (type == 1)
	{
		for (unsigned int i = 0; i<all.size(); i++)
		{
			results = setUnion(results, all[i]);
		}
	}
	//set<string>::iterator it;
	vector<Product*> list;
	for(set<Product*>::iterator it = results.begin(); it != results.end(); ++it)
	{
       list.push_back(*it);
	}
	return list;
}

void MyDataStore::dump(std::ostream& ofile)
{
	ofile<<"<products>"<<endl;
	for(set<Product*>::iterator it = products_.begin(); it != products_.end(); it++)
	{
		(*it)->dump(ofile);
	}
	ofile<<"</products>"<<endl;
	ofile<<"<users>"<<endl;
	for(set<User*>::iterator it = users_.begin(); it!= users_.end(); it++)
	{
		(*it)->dump(ofile);
	}
	ofile<<"</users>"<<endl;
	ofile<<"<reviews>"<<endl;
    for(set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it)
	{
		vector<Review> commentSection = (*it)->getReviews();
		if (!commentSection.empty())
		{
			for (unsigned int i = 0; i<commentSection.size(); i++)
			{
               ofile<<(*it)->getName()<<endl;
               commentSection[i].dump(ofile);
			}
		}
	}
	ofile<<"</reviews>"<<endl;
	return;
}

void MyDataStore::viewCart()
{
	return;
}

void MyDataStore::buyCart()
{
	return;
}

void MyDataStore::quit()
{
	return;
}

set<User*> MyDataStore::getUsers()
{
	return users_;
}

double MyDataStore::findBasicSimilarity(User* a, User* b)
{
	set<Product*> productsA;
    set<Product*> productsB;
	map<Product*, int> reviewA;
	map<Product*, int> reviewB;
	for(set<Product*>::iterator it = products_.begin(); it!= products_.end(); ++it)
	{
		vector<Review> reviews = (*it)->getReviews();
		for (vector<Review>::iterator current = reviews.begin(); current != reviews.end(); ++current)
		{
			int rate = current->getRating();
			if (current->getUsername() == a->getName())
			{
				productsA.insert((*it));
				reviewA.insert(pair<Product*, int>((*it), rate));
			}
			if (current->getUsername() == b->getName())
			{
				productsB.insert((*it));
				reviewB.insert(pair<Product*, int>((*it), rate));
			}
		}
	}
	set<Product*> intersect = setIntersection(productsA, productsB);
	if (intersect.empty()) return 1.0;
	double total = 0;
	for (set<Product*>::iterator here = intersect.begin(); here!= intersect.end(); ++here)
	{
		int a = reviewA[(*here)];
		int b = reviewB[(*here)];
		total += (double)abs(a - b);
	}
	double rate = total/(double)(4*intersect.size());
	return rate;
}

map<User*, vector<pair<double, User*> > > MyDataStore::produceMap()
{
	map<User*, vector<pair<double, User*> > > graph;
	for (set<User*>::iterator it = users_.begin(); it != users_.end(); ++it)
	{
		vector<pair<double, User*> > connections;
		for (set<User*>::iterator current = users_.begin(); current != users_.end(); ++current)
		{
			double rate = findBasicSimilarity((*it), (*current));
			connections.push_back(make_pair(rate, (*current)));
		}
        graph.insert(pair<User*, vector<pair<double, User*> > >((*it), connections));
	}
	return graph;
}

map<User*, double> MyDataStore::getDist(User* a)
{
	map<User*, double> list;
	Heap<User*> tree(2);
	map<User*, vector<pair<double, User*> > > graph = produceMap();
    tree.push(0, a);
    for (set<User*>::iterator it = users_.begin(); it != users_.end(); ++it)
    {
    	if ((*it) != a)
    	{
    		tree.push(1, *it);
    	}
    }
    list.insert(pair<User*, double>(a, 0));
    for (set<User*>::iterator find = users_.begin(); find != users_.end(); ++find)
    {
    	if ((*find) != a)
    	{
    		list.insert(pair<User*, double>(*find, 1));
    	}
    }
    while (!tree.empty())
    {
    	User* v = tree.top();
    	tree.pop();
    	vector<pair<double, User*> > nodes = graph[v];
    	for (unsigned int i = 0; i<nodes.size(); ++i)
    	{
    		if (nodes[i].first != 1.0)
    		{
    			double weight = nodes[i].first;
    			User* u = nodes[i].second;
    			if (list[v] + weight < list[u])
    			{
    				list[u] = list[v] + weight;
    				tree.decreaseKey(list[u], u);
    			}
    		}
    	}
    }
    return list;
}

set<Product*> MyDataStore::notReviewed(User* a)
{
	set<Product*> recommend;
	for(set<Product*>::iterator it = products_.begin(); it != products_.end(); ++it)
	{
	   vector<Review> reviews = (*it)->getReviews();
	   bool commented = false;
	   if (reviews.empty()) continue;
	   for(unsigned int i = 0; i<reviews.size(); ++i)
	   {
	   	  if (reviews[i].getUsername() == a->getName())  commented = true;
	   }
	   if(!commented) recommend.insert((*it));
	}
	return recommend;
}

double MyDataStore::getRefinedRate(Product* p, User* a)
{
	vector<Review> reviews = p->getReviews();
	map<User*, double> rate;
	for (unsigned int i = 0; i < reviews.size(); ++i)
	{
		for(set<User*>::iterator it = users_.begin(); it != users_.end(); ++it)
		{
			if ((*it)->getName() == reviews[i].getUsername())
			{
				rate.insert(make_pair((*it), reviews[i].getRating()));
			}
		}
	}
	map<User*, double> similarity = getDist(a);
    double r = 0.0;
    double w = 0.0;
    for (map<User*, double>::iterator it = rate.begin(); it != rate.end(); ++it)
    {
        double rating = it->second;
        double closeness = similarity[(it->first)];
        r += (1-closeness)*rating;
        w += (1-closeness);
    }
    if (w == 0) return 0.0;
    else return r/w;
}

vector<pair<string, double> > MyDataStore::makeSuggestion(string currentUser)
{
	User* current;
	for (set<User*>::iterator it = users_.begin(); it != users_.end(); ++it)
	{
		if ((*it)->getName() == currentUser)
		{
			current = (*it);
			break;
		}
	}
	set<Product*> recommend = notReviewed(current);
	vector<pair<string, double> > result; 
	for (set<Product*>::iterator it = recommend.begin(); it != recommend.end(); ++it)
	{
        double rate = getRefinedRate((*it), current);
        string name = (*it)->getName();
        result.push_back(make_pair(name, rate));
	}
    Pcomp comp;
    stable_sort(result.begin(), result.end(), comp);
    return result;
}
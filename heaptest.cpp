#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "heap.h"
#include "user.cpp"
using namespace std;
int main()
{
	User* one = new User("John", 0.0, 0, 0);
	User* two = new User("Bob", 0.0, 0, 0);
	User* three = new User("Tom", 0.0, 0, 0);
	User* four = new User("Bruce", 0.0, 0, 0);
	User* five = new User("Jim", 0.0, 0, 0);
	User* six = new User("Lucas", 0.0, 0, 0);
	User* seven = new User("David", 0.0, 0, 0);
	set<User*> together;
	together.insert(one);
	together.insert(two);
	together.insert(three);
	together.insert(four);
	together.insert(five);
	together.insert(six);
	together.insert(seven);
	Heap<User*> tree(2);
	tree.push(0, two);
	for (set<User*>::iterator it = together.begin(); it != together.end(); ++it)
    {
    	if ((*it) != two)
    	{
    		//cout<<(*it)->getName()<<endl;
    		tree.push(1, *it);
    	}
    }
	User* top = tree.top();
	cout<<top->getName()<<endl;
	tree.decreaseKey(0, five);
	top = tree.top();
	cout<<top->getName()<<endl;
	tree.pop();
	top = tree.top();
	cout<<top->getName()<<endl;
	// delete one;
	// delete two;
	// delete three;
	// delete four;
	// delete five;
	// delete six;
	// delete seven;
	return 0;
}
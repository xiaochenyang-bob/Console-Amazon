#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <deque>
#include <iomanip>
#include <algorithm>
#include "db_parser.h"
#include "mydatastore.h"
#include "msort.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);
unsigned long long hashIt(string code);
unsigned long long cast(string code, int length);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;
    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);
    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  LOGIN username password           " << endl;
    cout << "  LOGOUT                    " << endl;
    cout << "  AND r/n term term ...                  " << endl;
    cout << "  OR r/n term term ...                   " << endl;
    cout << "  ADD search_hit_number     " << endl;
    cout << "  VIEWCART                  " << endl;
    cout << "  BUYCART                   " << endl;
    cout << "  ADDREV search_hit_number rating date review_text " << endl;
    cout << "  VIEWREV search_hit_number          " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "  REC                       " <<endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    set<User*> people = ds.getUsers();
    vector<string> names;
    vector<deque<Product*> > carts;
    for (set<User*>::iterator it = people.begin(); it!=people.end();it++)
    {
        names.push_back((*it)->getName());
        deque<Product*> cart;
        carts.push_back(cart);
    }
    bool done = false;
    string currentUser;
    bool loggedIn;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if (cmd == "LOGIN")
            {
               string name;
               string hashcode;
               ss>>name;
               if (ss.fail())
               {
                 cout<<"Invalid user"<<endl;
                 continue;
               }
               ss>>hashcode;
               if (ss.fail())
               {
                  cout<<"Invalid password"<<endl;
                  continue;
               }
               vector<string>::iterator it = find(names.begin(), names.end(), name);
               if (it == names.end())
               {
                  cout<<"Invalid login credentials"<<endl;
                  continue;
               }
               User* chosen;
               for (set<User*>::iterator it = people.begin(); it!=people.end();it++)
               {
                  if ((*it)->getName() == name)
                  {
                     chosen = (*it);
                  }
               }
               unsigned long long decode = hashIt(hashcode);
               if (chosen->getHashCode() == decode)
               {
                  currentUser = name;
                  loggedIn = true;
                  cout<<"User logged in"<<endl;
               }
               else 
               {
                  cout<<"Invalid login credentials"<<endl;
               }
            }
            else if (cmd == "LOGOUT")
            {
                loggedIn = false;
            }
            else if( cmd == "AND") {
                char sort;
                ss>>sort;
                if (ss.fail())
                {
                    continue;
                }
                if (sort != 'r' && sort != 'n')
                {
                    continue;
                }
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                if (sort == 'r')
                {
                    Rcomp comp;
                    mergeSort(hits, comp);
                }
                else
                {
                    Ncomp comp;
                    mergeSort(hits, comp);
                }
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                char sort;
                ss>>sort;
                if (ss.fail())
                {
                    continue;
                }
                if (sort != 'r' && sort != 'n')
                {
                    continue;
                }
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                if (sort == 'r')
                {
                    Rcomp comp;
                    mergeSort(hits, comp);
                }
                else
                {
                    Ncomp comp;
                    mergeSort(hits, comp);
                }
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            else if (cmd == "ADD"){
                int hit_number;
                if (!loggedIn)
                {
                    cout<<"No current user"<<endl;
                    continue;
                }
                ss>>hit_number;
                if(ss.fail())
                {
                    cout<<"Invalid request"<<endl;
                    continue;
                }
                if ((unsigned int)hit_number<1 || (unsigned int)hit_number>hits.size())
                {
                    cout<<"Invalid number"<<endl;
                    continue;
                }
                int index;
                for (unsigned int i = 0; i<names.size(); i++)
                {
                    if (names[i] == currentUser)
                    {
                        index = i;
                    }
                }
                carts[index].push_back(hits[hit_number-1]);
            }
            else if (cmd == "VIEWCART"){
                if (!loggedIn)
                {
                    cout<<"No current user"<<endl;
                    continue;
                }
                int index;
                for (unsigned int i = 0; i<names.size(); i++)
                {
                    if (names[i] == currentUser)
                    {
                        index = i;
                    }
                }
                deque<Product*> print = carts[index];
                int count = 1;
                if (!print.empty())
                {
                   for(deque<Product*>::iterator it = print.begin(); it!=print.end(); it++)
                   {
                       cout<<"Item"<<" "<<count<<endl;
                       string out = (*it)->displayString();
                       cout<<out<<endl;
                       count++;
                   }
                }
            }
            else if (cmd == "BUYCART")
            {
                if (!loggedIn)
                {
                    cout<<"No current user"<<endl;
                    continue;
                }
                int index;
                for (unsigned int i = 0; i<names.size(); i++)
                {
                    if (names[i] == currentUser)
                    {
                        index = i;
                    }
                }
                deque<Product*> cart = carts[index];
                User* person;
                for(set<User*>::iterator it = people.begin(); it != people.end(); it++)
                {
                    if ((*it)->getName()==currentUser)
                    {
                        person = *it;
                    }
                }
                deque<Product*>::iterator it;
                for (it = cart.begin(); it!=cart.end();)
                {
                    Product* element = *it;
                    if (element->getPrice() <= person->getBalance() && element->getQty()>0)
                    {
                        person->deductAmount(element->getPrice());
                        element->subtractQty(1);
                        cart.erase(it);
                    }
                    else
                    {
                        ++it;
                    }     
                }
            }
            else if (cmd == "ADDREV")
            {
               int hit_number;
               double rating;
               string date;
               string text;
               if (!loggedIn)
                {
                    cout<<"No current user"<<endl;
                    continue;
                }
               ss>>hit_number;
               if(ss.fail())
               {
                 cout<<"Invalid request"<<endl;
                 continue;
               }
               if ((unsigned int)hit_number<1 || (unsigned int)hit_number>hits.size())
               {
                  cout<<"Invalid number"<<endl;
                  continue;
               }
               ss>>rating;
               if (rating<1 || rating>5)
               {
                  cout<<"Invalid rating"<<endl;
                  continue;
               }
               ss>>date;
               if (date[4]!= '-' || date[7]!='-' || date.length()!= 10)
               {
                  cout<<"Invalid date format"<<endl;
                  continue;
               }
               string month = date.substr(5,2);
               stringstream buffer;
               buffer<<month;
               int mon;
               buffer>>mon;
               if (buffer.fail())
               {
                 cout<<"Invalid month"<<endl;
                 continue;
               }
               else if (mon<1 || mon >12)
               {
                 cout<<"Invalid month"<<endl;
                 continue;
               }
               string day = date.substr(8,2);
               int da;
               stringstream temp;
               temp<<day;
               temp>>da;
               if (buffer.fail())
               {
                 cout<<"test"<<endl;
                 cout<<"Invalid day"<<endl;
                 continue;
               }
               if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)
               {
                 if (da<1 || da>31)
                 {
                    cout<<"Invalid day"<<endl;
                    continue;
                 }
               }
               else if (mon == 2)
               {
                 if (da<1 || da>28)
                 {
                    //cout<<da<<endl;
                    cout<<"Invalid day"<<endl;
                    continue;
                 }
               }
               else 
               {
                 if (da<1 || da>30)
                 {
                    cout<<"Invalid day"<<endl;
                    continue;
                 }
               }
               getline(ss, text);
               text = trim(text);
               Review newComment = Review(rating, currentUser, date, text);
               hits[hit_number-1] -> addComment(newComment);
            }
            else if (cmd == "VIEWREV")
            {
               int hit_number;
               if (!loggedIn)
                {
                    cout<<"No current user"<<endl;
                    continue;
                }
               ss>>hit_number;
               if(ss.fail())
               {
                 cout<<"Invalid request"<<endl;
                 continue;
               }
               if ((unsigned int)hit_number<1 || (unsigned int)hit_number>hits.size())
               {
                  cout<<"Invalid number"<<endl;
                  continue;
               }
               vector<Review> view = hits[hit_number-1]->getReviews();
               Dcomp comp;
               mergeSort(view, comp);
               for (unsigned int i = 0; i<view.size(); i++)
               {
                  view[i].viewReviews(cout);
               }
            }
            else if (cmd == "REC")
            {
               if (!loggedIn)
               {
                  cout<<"No current user"<<endl;
                  continue;
               }
               ofstream ofile("rec.txt");
               vector<pair<string, double> > recommends = ds.makeSuggestion(currentUser);
               cout<<currentUser<<endl;
               ofile<<currentUser<<endl;
               if (recommends.size() == 0)
               {
                  cout<<"No recommendations available"<<endl;
               }
               for (unsigned int i = 0; i<recommends.size(); ++i)
               {  
                  cout.setf(ios::fixed,ios::floatfield);
                  cout.precision(2);
                  cout<<recommends[i].second<<" "<<recommends[i].first<<endl;
                  ofile<<recommends[i].second<<" "<<recommends[i].first<<endl;
               }
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    //std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

unsigned long long cast(string code, int length)
{
  if (length == 1)
  {
    return (unsigned long long)code[0];
  }
  else 
  {
    unsigned long long start = (unsigned long long)code[length-1];
    string substring = code.substr(0, length-1);
    return start + 128*cast(substring, --length);
  }
}

unsigned long long hashIt(string code)
{
  int length = code.length();
  unsigned long long castCode = cast(code, length);
  unsigned int w1;
  unsigned int w2;
  unsigned int w3;
  unsigned int w4;
  w4 = castCode%65521;
  castCode /= 65521;
  w3 = castCode%65521;
  castCode /= 65521;
  w2 = castCode%65521;
  castCode /= 65521;
  w1 = castCode%65521;
  unsigned long long result;
  result = (45912*w1 + 35511*w2 + 65169*w3 + 4625*w4)%65521;
  return result;
}
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <deque>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

unsigned long long hashIt(string code);
unsigned long long cast(string code, int length);


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

int main()
{
	string password = "graphics";
	unsigned long long print = hashIt(password);
	cout<<print<<endl;
	return 0;
}
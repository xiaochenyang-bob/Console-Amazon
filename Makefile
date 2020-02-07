CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJS=amazon.o user.o db_parser.o product.o product_parser.o util.o mydatastore.o review.o book.o clothing.o movie.o 

all: amazon

amazon: $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $(OBJS)

amazon.o: amazon.cpp db_parser.h datastore.h msort.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c amazon.cpp
user.o: user.cpp user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c user.cpp
db_parser.o: db_parser.cpp db_parser.h product.h product_parser.h user.h datastore.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c db_parser.cpp
product.o: product.cpp product.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product.cpp
product_parser.o: product_parser.cpp product_parser.h product.h review.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product_parser.cpp
util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c util.cpp
mydatastore.o: mydatastore.cpp mydatastore.h util.h heap.h comparator.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c mydatastore.cpp
review.o: review.cpp review.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c review.cpp
book.o: book.cpp book.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c book.cpp
clothing.o: clothing.cpp clothing.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c clothing.cpp
movie.o: movie.cpp movie.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c movie.cpp

clean:
	rm -f *.o amazon

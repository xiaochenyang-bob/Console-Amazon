#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
using namespace std;
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);
    double getPriority(const T& item);
private:
    /// Add whatever helper functions you need below
    void trickleUp(size_t loc);
    void swap(std::pair<double, T>& a, std::pair<double, T>& b);
    void heapify(size_t idx);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(size_t loc)
{
    if (loc <= 0) return;
    size_t parent = (loc - 1)/(size_t)m_;
    while (c_(store_[loc].first, store_[parent].first))
    {
        swap(store_[loc], store_[parent]);
        loc = parent;
        if (loc == 0) break;
        else parent = (loc - 1)/(size_t)m_;
    }
    return;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::swap(std::pair<double, T>& a, std::pair<double, T>& b)
{
   std::pair<double, T> temp = a;
   size_t indexA = keyToLocation_[a.second];
   size_t indexB = keyToLocation_[b.second];
   keyToLocation_[a.second] = indexB;
   keyToLocation_[b.second] = indexA;
   a = b;
   b = temp;
   return;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    pair<double, T> test = pair<double, T>(priority, item);
    store_.push_back(test);
    keyToLocation_.insert(pair<T, double>(item, store_.size()-1));
    trickleUp(store_.size() -1);  
    return;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    // std::unordered_map<T, size_t>::iterator it = keyToLocation_.find(item);
    // if (it == keyToLocation_.end()) return;
    size_t loc = keyToLocation_[item];
    store_[loc].first = priority;
    trickleUp(loc);
    return;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    // You complete
    return store_[0].second;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(size_t idx)
{
   if ((size_t)m_*idx + 1 > (size_t)store_.size())
   {
      return;
   }
   size_t selectChild = idx*m_ + 1;
   size_t count = selectChild;
   for (size_t i = count; i<count+m_; i++)
   {
      if (i>(size_t)store_.size()-1)
      {
          break;
      }
      if (c_(store_[i].first,store_[selectChild].first))
      {
         selectChild = i;
      }
   }
   if (c_(store_[selectChild].first,store_[idx].first))
   {
      swap(store_[idx], store_[selectChild]);
      heapify(selectChild);
   }
}


/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    store_[0] = store_.back();
    store_.pop_back();
    heapify(0);
    return;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
double Heap<T,TComparator,PComparator,Hasher>::getPriority(const T& item)
{
   size_t loc = keyToLocation_[item];
   return store_[loc].first;
}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif


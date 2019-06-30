#ifndef CPPND_GARBAGE_COLLECTOR_GC_ITERATOR_H_
#define CPPND_GARBAGE_COLLECTOR_GC_ITERATOR_H_
// Exception thrown when an attempt is made to
// us an Iter that exeeds the range of the
// underlying object.
//

class OutOfRangeExc {
  // Add functionality if needed by your application.
};

// An iterator-like class for cycling through arrays
// that are pointed to by GCPtrs. Iter pointers
// ** do not ** participate in or affect garbage
// collection. Thus, an Iter pointing to
// some object does not prevent that object
// from being recycled.
//
template<typename T>
class Iter {
 public:
  // Types
  
  // Constants

  // Constructors
  Iter() : ptr_(nullptr), begin_(nullptr), end_(nullptr), length_(0) {}
  Iter(T *p, T *first, T *last) 
      : ptr_(p), begin_(first), end_(last), length_(last - first) {}

  // Assignment operators

  // Destructor

  // All other methods

  // Return length of sequence to which this Iter points.
  unsigned size() { return length_; }

  // Return value pointed to by ptr_.
  // Do not allow out-of-bounds access.
  T &operator*() {
    if ((ptr_ >= end_) || (ptr_ < begin_))
      throw OutOfRangeExc();
    return *ptr_;
  };

  // Return address contained in ptr_.
  // Do not allow out-of-bounds access.
  T *operator->() {
    if ((ptr_ >= end_) || (ptr_ < begin_))
      throw OutOfRangeExc();
    return ptr_;
  }

  // Prefix ++.
  Iter operator++() {
    ++ptr_;
    return *this;
  }

  // Postfix ++.
  Iter operator++(int) {
    T *tmp = ptr_;
    ++ptr_;
    return Iter<T>(tmp, begin_, end_);
  }

  // Prefix --.
  Iter operator--() {
    --ptr_;
    return *this;
  }

  // Postfix --.
  Iter operator--(int) {
    T *tmp = ptr_;
    --ptr_;
    return Iter<T>(tmp, begin_, end_);
  }

  // Return a reference to the object at the
  // specified index. Do not allow out-of-bounds
  // access.
  T &operator[](int i) {
    if ((i < 0) || (i >= length_))
      throw OutOfRangeExc();
    return ptr_[i];
  }

  // Define the relational operators.
  bool operator==(const Iter &op2) { return ptr_ == op2.ptr_; }
  bool operator!=(const Iter &op2) { return ptr_ != op2.ptr_; }
  bool operator<(const Iter &op2) { return ptr_ < op2.ptr_; }
  bool operator<=(const Iter &op2) { return ptr_ <= op2.ptr_; }
  bool operator>(const Iter &op2) { return ptr_ > op2.ptr_; }
  bool operator>=(const Iter &op2) { return ptr_ >= op2.ptr_; }

  // Subtract an integer from an Iter.
  Iter operator-(int n) {
    ptr_ -= n;
    return *this;
  }

  // Add an integer to an Iter.
  Iter operator+(int n) {
    ptr_ += n;
    return *this;
  }

  // Return number of elements between two Iters.
  int operator-(Iter<T> &itr2) {
    return ptr_ - itr2.ptr_;
  }

 private:
  T *ptr_;  // current pointer value
  T *begin_;        // points to start of allocated array
  T *end_;  // points to element one past end
  unsigned length_;  // length of sequence
};

#endif  // CPPND_GARBAGE_COLLECTOR_GC_ITERATOR_H_

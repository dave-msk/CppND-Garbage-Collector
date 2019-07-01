#ifndef CPPND_GARBAGE_COLLECTOR_GC_POINTER_H_
#define CPPND_GARBAGE_COLLECTOR_GC_POINTER_H_

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <typeinfo>
#include <utility>

#include "gc_details.h"
#include "gc_iterator.h"

// Pointer implements a pointer type that uses
// garbage colection to release unused memory.
// A Pointer must only be used to point to memory
// that was dynamically allocated using new.
// When used to refer to an allocated array,
// specify the array size.
template<typename T, int size = 0>
class Pointer {
 public:
  // Collect garbage. Returns true if at least
  // one object was freed.
  static bool Collect();
  // A utility function that displays ref_container_.
  static void ShowList();
  // Clear ref_container_ when program exits.
  static void Shutdown();
  // Return the size of ref_container_ for this type of Pointer.
  static int RefContainerSize() { return ref_container_.size(); }

  template<typename U, int size_u>
  friend void swap(Pointer<U, size_u> &, Pointer<U, size_u> &);

  // Types
  // Define an iterator type for Pointer<T>.
  using GCIterator = Iter<T>;

  // Constants

  // Factory functions

  // Constructors
  Pointer() : Pointer(nullptr) {}
  Pointer(T*);

  // Copy constructor.
  Pointer(const Pointer &ob) : Pointer(ob.addr_) {}

  // Assignment operators
  // Overload assignment of pointer to Pointer.
  T *operator=(T *t);

  // Overload assignment of Pointer to Pointer.
  Pointer &operator=(Pointer ptr);

  // Destructors
  // Destructor for Pointer.
  ~Pointer();

  // All other methods
  // Return a reference to the object pointed
  // to by this Pointer.
  T &operator*() { return *addr_; }

  // Return the address being pointed to.
  T *operator->() { return addr_; }

  // Return a reference to the object at the
  // index specified by i.
  T &operator[](int i) { return addr_[i]; }

  // Conversion function to T *.
  operator T*() { return addr_; }

  // Return an Iter to the start of the allocated memory.
  Iter<T> begin() {
    int _size = (is_array_) ? array_size_ : 1;
    return Iter<T>(addr_, addr_, addr_ + _size);
  }

  // Return an Iter to one past the end of an allocated array.
  Iter<T> end() {
    int _size = (is_array_) ? array_size_ : 1;
    return Iter<T>(addr_ + _size, addr_, addr_ + _size);
  }

  // Data members

 private:
  // Return an iterator to pointer details in ref_container_.
  typename std::list<PtrDetails<T>>::iterator FindPtrInfo(T *ptr);

  // ref_container_ maintains the garbage collection list.
  static std::list<PtrDetails<T>> ref_container_;
  static bool first;  // true when first Pointer is created
  
  // addr_ points to the allocated memory to which
  // this Pointer pointer current points.
  T *addr_;

  // is_array_ is true if this Pointer points
  // to an allocated array. It is false otherwise.
  bool is_array_;  // true if pointing to array

  // If this Pointer is pointing to an allocated
  // array, then array_size_ contains its size.
  unsigned array_size_;  // size of the array
};

// STATIC INITIALIZATION
// Creates storage for the static variables
template<typename T, int size>
std::list<PtrDetails<T>> Pointer<T, size>::ref_container_;

template<typename T, int size>
bool Pointer<T, size>::first = true;

// Collect garbage. Returns true if at least
// one object was freed.
template<typename T, int size>
bool Pointer<T, size>::Collect() {
  // TODO: Implement Collect function
  bool freed = false;

  typename std::list<PtrDetails<T>>::iterator p;

  do {
    for (p = ref_container_.begin(); p != ref_container_.end(); ++p) {
      if (p->refcount > 0) continue;
      ref_container_.erase(p);
      if (p->is_array) delete[] p->mem_ptr;
      else delete p->mem_ptr;
      break;
    }
  } while (p != ref_container_.end());
  return freed;
}

// A utility function that displays ref_container_.
template<typename T, int size>
void Pointer<T, size>::ShowList() {
  typename std::list<PtrDetails<T>>::iterator p;
  std::cout << "ref_container_<" << typeid(T).name() << ", " << size << ">:\n";
  std::cout << "mem_ptr refcount value\n ";
  if (ref_container_.begin() == ref_container_.end())
    std::cout << " Container is empty!\n\n ";

  for (p = ref_container_.begin(); p != ref_container_.end(); ++p) {
    std::cout << "[" << (void *)p->mem_ptr << "]" << " " << p->refcount << " ";
    if (p -> mem_ptr)
      std::cout << " " << *p->mem_ptr;
    else
      std::cout << "---";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

// Clear ref_container_ when program exits.
template<typename T, int size>
void Pointer<T, size>::Shutdown() {
  if (RefContainerSize() == 0)
    return;  // list is empty
  typename std::list<PtrDetails<T>>::iterator p;
  for (p = ref_container_.begin(); p != ref_container_.end(); ++p)
    p->refcount = 0;  // Set all reference counts to zero
  Collect();
}

// Find a pointer in ref_container_.
template<typename T, int size>
typename std::list<PtrDetails<T>>::iterator
Pointer<T, size>::FindPtrInfo(T *ptr) {
  typename std::list<PtrDetails<T>>::iterator p;
  // Find ptr in ref_container_.
  for (p = ref_container_.begin(); p != ref_container_.end(); ++p)
    if (p->mem_ptr == ptr)
      return p;
  return p;
}

// Swap method
template<typename T, int size>
void swap(Pointer<T, size> &first, Pointer<T, size> &second) {
  // enable ADL
  using std::swap;

  std::swap(first.addr_, second.addr_);
  std::swap(first.is_array_, second.is_array_);
  std::swap(first.array_size_, second.array_size_);
}

// Constructor for both initialized and uninitialized objects.
// See class interface.
template<typename T, int size>
Pointer<T, size>::Pointer(T *t) {
  // Register Shutdown() as an exit function.
  if (first)
    atexit(Shutdown);
  first = false;

  addr_ = t;
  is_array_ = (size > 0);
  array_size_ = size;

  auto ptr_info_it = FindPtrInfo(t);
  if (ptr_info_it != ref_container_.end()) {
    ++ptr_info_it->refcount;
  } else {
    PtrDetails<T> ptr_details;
    ptr_details.refcount = 1;
    ptr_details.mem_ptr = t;
    ptr_details.is_array = is_array_;
    ptr_details.array_size = array_size_;
    ref_container_.push_back(ptr_details);
  }
}


// Overload assignment of pointer to Pointer.
template<typename T, int size>
T *Pointer<T, size>::operator=(T *t) {
  Pointer<T, size> temp(t);
  swap(*this, temp);
  return t;
}

// Overload assignment of Pointer to Pointer.
template<typename T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer ptr) {
  swap(*this, ptr);
  return *this;
}

// Destructor
template<typename T, int size>
Pointer<T, size>::~Pointer() {
  auto ptr_info_it = FindPtrInfo(addr_);
  if (ptr_info_it != ref_container_.end()) {
    if (ptr_info_it->refcount > 0) --ptr_info_it->refcount;
    if (ptr_info_it->refcount == 0) Collect();
  }
}

#endif  // CPPND_GARBAGE_COLLECTOR_GC_POINTER_H_
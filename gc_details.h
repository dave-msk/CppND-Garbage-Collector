#ifndef CPPND_GARBAGE_COLLECTOR_GC_DETAILS_H_
#define CPPND_GARBAGE_COLLECTOR_GC_DETAILS_H_

// This class defines an element that is stored
// in the garbage collection information list.
//
template<typename T>
struct PtrDetails {
 public:
  unsigned refcount;   // current reference count
  T *mem_ptr;  // pointer to allocated memory

  // is_array is true is mem_ptr points
  // to an allocated array. It is false otherwise.
  bool is_array;  // true if pointing to array

  // If mem_ptr is pointing to an allocated
  // array, then array_size contains its size.
  unsigned array_size;  // size of array
  // Here, mem_ptr points to the allocated memory.
  // If this is an array, then size specifies
  // the size of the array.

  PtrDetails() 
      : refcount(0), mem_ptr(nullptr), is_array(false), array_size(0) {}
};

// Overloading operator== allows two class objects to be compared.
// This is needed by the STL list class.
template<typename T>
bool operator==(const PtrDetails<T> &ob1,
                const PtrDetails<T> &ob2) {
  return ((ob1.mem_ptr == ob2.mem_ptr) && 
          (ob1.refcount == ob2.refcount) &&
          (ob1.is_array == ob2.is_array) &&
          (ob1.array_size == ob2.array_size));
}

#endif  // CPPND_GARBAGE_COLLECTOR_GC_DETAILS_H_

#ifndef PERSISTENTARRAY_H
  #define PERSISTENTARRAY_H
/*
 * NEVER put using namespace std in a header file. Use the namespace (std::)
 * in the header. Users of your header must be able to decide whether to use
 * the namespace (or elements from it); there is no way to "un-use" a namespace.
 */
#include <istream>
#include <fstream>
#include <string>

/**
 * PersistentArray is an "array" of C-style strings. The array
 * elements live in indexes from 0-(n-1) for a length n. get and put
 * require valid indices; the add method adds a new string to the end
 * of the array.
 */
class PersistentArray {
public:
  // constructor
  PersistentArray(std::string fileName);
  // destructor
  ~PersistentArray();

  // get the length (in records) of the array
  size_t length();

  // get a copy of the kth element in the array
  char * get(size_t k);

  // set the value of the kth element to the given string
  void set(size_t k, const char * str);

  // add a new element at the end
  void add(const char * str);

private:
  // the internal routines that do the real work.
  void read_k(size_t k, char buffer[40]);
  void write_k(size_t k, char buffer[40]);
  // the stream
  std::fstream stream;
  size_t cached_length;

}; // PersistentArray
// that little semicolon must come after a class (or struct) definition.
// leaving it out creates difficult to comprehend error messages.

  #endif /* PERSISTENTARRAY_H */

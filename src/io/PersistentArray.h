//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu
// <note i did not know wether i should put ID blocks on these since I edited them
// feedback would be appreciated on the proper way to push ID blocks on used code>

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
  PersistentArray(std::string fileName, bool initialize);
  // destructor
  ~PersistentArray();

  // get the length (in records) of the array
  size_t length(size_t blockSize);

  // writes an element to the array
  void write_k(size_t k, char * str, size_t blockSize);

  //reads the element in the array
  char * read_k(size_t k, char * oldbuffer, size_t blockSize);

  //flushes the stream out to the file
  void flush_stream();
private:

  // the stream
  std::fstream stream;
  size_t cached_length;

}; // PersistentArray
// that little semicolon must come after a class (or struct) definition.
// leaving it out creates difficult to comprehend error messages.

  #endif /* PERSISTENTARRAY_H */

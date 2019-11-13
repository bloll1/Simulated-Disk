#include <cstring>
#include "PersistentArray.h"
#include <iostream>
using namespace std;

/*
 * Note on the full names of the functions: the function length is *declared*
 * in =PersistentArray.h= between the curly braces of the class of the same
 * name. How do we tell the compiler that we are writing the body of the length
 * member of that class and not some other function named length? We use the
 * name scope resolution operator, ::
 *
 * The name of the function length inside the class PersistentArray is
 * PersistentArray::length. The name of the length function inside the string
 * class is std::string::length (in addition to classes, there are namespaces
 * which behave as name scopes; that is where the std:: (or using namespace)
 * comes from).
 *
 * A typical error for people unfamiliar with C++ is to leave the class scope
 * out of the signature in the .cpp file. The problem is that the compiler
 * cannot find any instance variables (fields) without you putting the
 * *definition* of the function /back/ into the scope of the class it is
 * a member of.
 *
 * You do not need scope resolution on the call because you use a dot and
 * the type of the object to the left of the dot determines the class in
 * which the name is sought.
 */

/**
 * Constructor: name matches the name of the class. Parameters are used
 * to initialize fields of the class. A PersistentArray is a wrapper around a
 * file so stream is opened here. A file MUST be closed before a program
 * terminates; the destructor takes care of that.
 *
 * @param fileName
 */
PersistentArray::PersistentArray(string fileName, bool initialize):
    stream{fileName, ios::in | ios::out | ios::binary} {
  if (!stream.is_open() || initialize) { // it did not already exist...
    stream.open(fileName, ios::in | ios::out | ios::binary | ios::trunc);
    // reopen with ios::trunc; truncate or create as necessary. Already tried
    // to open w/o truncation so we are not clobbering anything
  }
}

/**
 * Destructor is named with a ~ (not) in front of the name of the class.
 * It cleans up any resources used by the object. This is the last
 * method called on the object. It can have no parameters and, like the
 * constructor, has no parameters.
 *
 * Close the stream.
 */
PersistentArray::~PersistentArray() {
    stream.close();
}

/**
 * Returns the length, in records, of the persistent array.
 *
 * Since we do not keep a length field, we measure the length by
 * moving the read cursor to the end of the file and asking for
 * the position (in bytes) from the beginning. Divide that by
 * the size of a "record" and we have the length.
 *
 * @return the length, in records, of the persistent array.
 */
size_t PersistentArray::length(size_t blockSize) {
  stream.seekg(0, ios::end);
  return stream.tellg() / blockSize;
}

void PersistentArray::flush_stream() {
  stream.flush();
}

void PersistentArray::write_k(size_t k, char * str, size_t blockSize) {
  char buffer[blockSize];
  buffer[blockSize - 1] = '\0';
  strncpy(buffer, str, blockSize - 1);
  stream.seekp(k*blockSize, ios::beg);
  stream.write(buffer, blockSize);
  stream.flush();
}

/**
 * Read the k-th record of size 40 from the stream. No error checking
 * is done on k here. It has to be valid before the call.
 *
 * @param k number of the record to fetch; [0, length] (note double closed)
 * @param char
 */
char * PersistentArray::read_k(size_t k, char * oldbuffer, size_t blockSize) {
  char str[blockSize];
  strncpy(str, oldbuffer, blockSize);
  stream.seekg(k*blockSize, ios::beg);
  stream.read(str, blockSize);
  char * buffer = new char[blockSize + 1];
  strncpy(buffer, str, blockSize);
  return buffer;
  //std::cout << "ReadBuffer: " << buffer << '\n';
}

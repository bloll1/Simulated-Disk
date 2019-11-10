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
PersistentArray::PersistentArray(string fileName):
    stream{fileName, ios::in | ios::out | ios::binary} {
  if (!stream.is_open()) { // it did not already exist...
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
size_t PersistentArray::length() {
  stream.seekg(0, ios::end);
  return stream.tellg() / 40;
}

/**
 * Get the record at position k (on the range [0,n) for length n array). Value
 * returned is dynamically allocated C-string.
 *
 * @param k index on [0, length()) of record (string) to read.
 *
 * @return a pointer at a dynamic copy of the record at position k if
 *         k was a valid index into the persistent array; nullptr otherwise
 *
 * @note caller is responsible for freeing the memory returned from this
 *       function
 */
char * PersistentArray::get(size_t k) {
  cached_length = length();
  if (k < cached_length) {
    char buffer[40];
    read_k(k, buffer);
    size_t len = strlen(buffer);
    char * retval = new char[len + 1];
    strncpy(retval, buffer, len);
    retval[len] = '\0';
    return retval;
  }
  return nullptr;
}

/**
 * Set the record at position k (on the range [0,n) for length n array). Value
 * is truncated to 39 characters (with room for the required null char).
 *
 * @param k index on [0, length()) of record (string) to set.
 * @param str pointer at a constant C-string copied and written.
 *
 * @note caller is responsible for freeing the memory returned from this
 *       function
 */
void PersistentArray::set(size_t k, const char * str) {
  cached_length = length();
  if (k < cached_length) {
    char buffer[40];
    buffer[39] = '\0';
    strncpy(buffer, str, 39);
    write_k(k, buffer);
  }
}

// add a new element at the end
void PersistentArray::add(const char * str) {
  cached_length = length();
  char buffer[40];
  buffer[39] = '\0';
  strncpy(buffer, str, 39);
  write_k(length(), buffer);
}

void PersistentArray::write_k(size_t k, char buffer[40]) {
  stream.seekp(k*40, ios::beg);
  stream.write(buffer, 40);
}

/**
 * Read the k-th record of size 40 from the stream. No error checking
 * is done on k here. It has to be valid before the call.
 *
 * @param k number of the record to fetch; [0, length] (note double closed)
 * @param char
 */
void PersistentArray::read_k(size_t k, char buffer[40]) {
  stream.seekg(k*40, ios::beg);
  stream.read(buffer, 40);
}

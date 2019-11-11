#include "BitMap.h"
/**
 * Construct a BitMap for holding the given number of entries.
 * Initially, all entries are to be unset.
 *
 * @param number_of_entries the number of entries that bitmap should track
 */
BitMap::BitMap(unsigned int number_of_entries) {
  // Do nothing...
}

/**
 * Clean up after the BitMap as necessary.
 */
BitMap::~BitMap() {
  // Do nothing ...
}

/**
 * Query the number of entries that this BitMap contains.
 *
 * @return the number of entries the BitMap was initialized with
 */
unsigned int BitMap::entry_count() {
  return 0; // broken for any non-zero size
}

/**
 * Set ALL bits in the BitMap.
 */
void BitMap::set() {
  // Do nothing ...
}

/**
 * Unset ALL bits in the BitMap.
 */
void BitMap::unset() {
  // Do nothing ...
}

/**
 * Get the lowest index of a bit that is not set.
 *
 * @return the smallest index [0,entry_count) of a bit that is unset if
 *   such a bit exists; return nbit otherwise
 */
unsigned int BitMap::get_unset_index() {
  return nbit;
}

/**
 * Get the bit at the given index as a Boolean: true if the bit is
 * set to 1; false if the bit is unset to 0.
 *
 * @param index the index [0,entry_count) of the bit to get the value of
 * @return true if index is valid and the given bit is set; false otherwise
 *   (if index is invalid OR the validly indexed bit is unset).
 */
bool BitMap::get(unsigned int index) {
  return false;
}

/**
 * Set the bit at the given index to 1 if it is a valid index; do
 * nothing if the index is out of range.
 *
 * @param index the index [0,entry_count) of the bit to set
 */
void BitMap::set(unsigned int index) {
  // Do nothing ...
}

/**
 * Set the bit at the given index to 0 if it is a valid index; do
 * nothing if the index is out of range.
 *
 * @param index the index [0,entry_count) of the bit to get the value of
 */
void BitMap::unset(unsigned int index) {
  // Do nothing ...
}

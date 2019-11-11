//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#ifndef WORKINGBITMAP_H
#define WORKINGBITMAP_H

#include "BitMap.h"

class WorkingBitMap : public BitMap {

private:

  // char * for storing the bit_map array
  unsigned char * bit_map;

  // the number of entries submitted in the constructor
  unsigned int noe;

 public:

  // return value if no unset bits are found in the get_unset_index function
  constexpr static unsigned int nbit = 0xFFFFFFFF;

  // constructor; initializes the bit_map array; unsets all the bits in the
  // array; stores the number_of_entries in noe
  WorkingBitMap(unsigned int number_of_entries);

  // deconstructor; deletes the bit_map array from memory
   ~WorkingBitMap();

  // return the number of entries in the bit map
   unsigned int entry_count();

  // unset all entries in the bitmap
   void unset();

  // set all entries in the bitmap
   void set();

  // return the lowest unset index
   unsigned int get_unset_index();

  // return whether entry at the given index is set; out of range always unset
   bool get(unsigned int index);

  // set entry at given index; ignore out-of-range indices
   void set(unsigned int index);

  // set entry at given index; ignore out-of-range indices
  void unset(unsigned int index);
};

#endif /* WORKINGBITMAP_H */

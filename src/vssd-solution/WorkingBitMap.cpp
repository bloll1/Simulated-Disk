//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#include "WorkingBitMap.h"
#include <math.h> //Included for calling the ceil function to round numbers up

/* This is the constructor for the working_bit_map. The constructor takes a number
 of entries (unsigned int) thats desired by the user and creates a new array of chars
 (each char consisting of a single byte) rounding up the number of entries to fit all
 the bits in the array. It also stores the number of entries for referencing in other
 functions. Lastly because when you initialize all of the bytes they have undefined
 values, so as to start with a fully unset array of bytes we call the unset function. */

WorkingBitMap::WorkingBitMap(unsigned int number_of_entries)  :  BitMap(number_of_entries) {

   int bytes = ceil((double) number_of_entries / 8.0);
   bit_map  = new unsigned char[bytes];
   noe = number_of_entries;
   unset();
}



/* This destructor function deletes the bit_map from memory as to not leave
allocated memory behind */

WorkingBitMap::~WorkingBitMap() {
  delete[] bit_map;

}



/* For the entry_count function we return the stored value of the originally
submitted number of entries stored in the constructor */

unsigned int WorkingBitMap::entry_count() {

  return noe;
  }




/* For the set function we find the number of bytes that was in the bit_map
array by using the ceil function to round the number up. Then for each byte
in the array it sets that byte equal to 255 (11111111 in binary) essentially
turning all of the bits in the bit_map array on*/

void WorkingBitMap::set() {

  for (unsigned int i = 0; i < ceil( (double) noe / 8.0); i++) {

    bit_map[i] = 255;
  }
}




/* The unset function does the opposite of the set function. It takes the
number of entries originally submitted by the user, divides it by 8 and rounds
that number up with the ceil function. This data is the number of bytes stored
in the array upon initialization and we use it for our check in the for loop.
Each iteration of the for loop sets each char in each array index to 0 essentially
unsetting all of the bits in the bit_map array */

void WorkingBitMap::unset() {

  for (unsigned int i = 0; i < ceil( (double) noe / 8.0); i++)
     bit_map[i] = 0;
}





/* The get_unset_index function finds the first iteration of an unset bit
at the beginning of the bit_map array. It does this by using the get function
to check each bit in the array counting up till it reaches the submitted
number_of_entries. If the get function returns false then a unset bit was found
and the function returns the location of the bit in the bit_map array. If no
unset bits are found it returns the nbit (0xFFFFFFFF) to indicate no unset bits
were found.*/

unsigned int WorkingBitMap::get_unset_index() {
  unsigned int array_index = 0;

  while ( array_index != noe) {
    if (get(array_index)== false) {
        return array_index;
      }
    array_index++;
  }
    return nbit;
}






/* The bread and butter of the functions. The get function first takes the bit
index submitted to it and then divides that number by 8 to find the bit_map
array index. Once stored it then stores the index % 8 (which finds the remainder
of the index / 8) then shifts the decimal 128 (10000000 in binary) over that
many times in order to be able to manipulate that bit stored in the bit_map
array.  Once that integer is stored the function returns the byte at the bit_map
&'d with the shift_position byte. If that bit is set the return value will be
greater than 0 and therfore true. Likewise if the bit is unset the return value
will be 0 returning a false.*/

bool WorkingBitMap::get(unsigned int index) {
  int array_index = index / 8;
  int shift_position = 128 >> (index % 8);
  return shift_position & bit_map[array_index];

}





/* The set function will set a bit to 1 at a given index it the bit_map array.
 The function first checks if the index is out of bounds of the originally
 submitted number_of_entries. If it isnt then it will find the bit_map array
 index by dividing the given index by 8. With the index of the bit_map found we
 we then find the position of the bit. We do this by shifting 128 (10000000) to
 the right with the remainder of the given index when divided by 8. Now with the
 given bit isolated we OR the isolated bit with the byte stored in the bit_map
 array, essentially setting the bit to on within the byte.*/

void WorkingBitMap::set(unsigned int index) {
  if (index <= noe) {
  int array_index = index / 8;
  int shift_position = 128 >> (index % 8);
  bit_map[array_index] = shift_position | bit_map[array_index];
  }
}





/* The unset function is essentially the opposite of the set function. It takes
the given index submitted in the fucntion call and first checks if it is greater
than the number of entires the bit_map array was first allocated with. If it
passes the test it finds the index within the bit_map array the desired byte is
stored in. Then we isolate the desired bit by shifting 128 (10000000 in binary)
with the remainder of the index / 8. With both of those values stored we take
the byte at the given bit_map array index and '&' it with the negation of the
of the stored byte. This unsets the desired bit and leaves all other bits within
the byte a 1 if their stored as a 1 or a 0.*/

void WorkingBitMap::unset(unsigned int index) {
  if (index <= noe) {
  int array_index = index / 8;
  int shift_position = 128 >> (index % 8);
  bit_map[array_index] = bit_map[array_index] & ~shift_position;
  }
}

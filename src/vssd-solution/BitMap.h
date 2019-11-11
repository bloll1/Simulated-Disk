#ifndef BITMAP_H
#define BITMAP_H
class BitMap {
 public:
  // the value returned by get_unset_entry to indicate no entry available
  constexpr static unsigned int nbit = 0xFFFFFFFF;

  BitMap(unsigned int number_of_entries);
  virtual ~BitMap();

  // return the number of entries in the bit map
  virtual unsigned int entry_count();

  // unset all entries in the bitmap
  virtual void unset();

  // set all entries in the bitmap
  virtual void set();

  // return the lowest unset index
  virtual unsigned int get_unset_index();

  // return whether entry at the given index is set; out of range always unset
  virtual bool get(unsigned int index);

  // set entry at given index; ignore out-of-range indices
  virtual void set(unsigned int index);

  // set entry at given index; ignore out-of-range indices
  virtual void unset(unsigned int index);
};

#endif /* BITMAP_H */

//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#include "RAMVSSD.h"

/*RAMVSSD Constructor:: initializes a new vector and sets the global vector to
*  it. Then for metadata the first two blocks of the ram vector contain the
*  size of each block and the count of all total blocks. These variables are then
*  stored globally in the header (block_s/block_c). Finally we set the DiskStatus
*  to OK signifiying that the disk is ready.
*  @param - block_size: the size of each block of data
*  @param - block_count: the total number of blocks in the disk
*  @param - filename: (unused)
*  @param - initialize: (unused) (the disk is always new)
*/


RAMVSSD::RAMVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {

  std::vector<std::string> tempram(block_count , "");
  ram = tempram;

  ram[0] = std::to_string(block_size);
  ram[1] = std::to_string(block_count);

  block_s = block_size;
  block_c = block_count;

  ds = DiskStatus::OK;
}




/*RAMVSSD Deconstructor:: doesn't do anything since theres no pointers
*  to free
*/

RAMVSSD::~RAMVSSD() {
}



/*blockSize:: Returns the size of each block(block_s in the header) unless
*  the disk is currently set to NOT_READY
*/

std::size_t RAMVSSD::blockSize() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_s;
  }
  return 0;
}


/*blockCount:: Returns the total # of blocks(block_c in the header) unless
*  the disk is currently set to NOT_READY
*/

std::size_t RAMVSSD::blockCount() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_c;
  }
  return 0;
}



/*status:: Returns the ds variable in the header(AKA the current DiskStatus)
*/

DiskStatus RAMVSSD::status() const {
  return ds;
}




/*read:: reads the desired sector from the ram vector if the sector is below
*  or out of bounds of the vector the DiskStatus is set to BLOCK_OUT_OF_RANGE
*  and a error is printed to the user. If the buffer passed in is a nullptr the
*  DiskStatus is set to error and the corresponding message is printed.
*  @param - sector: the desired sector to access
*  @param - buffer: the pointer the data will be set to from the ram sector
*/

DiskStatus RAMVSSD::read(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    free(buffer);
    char * entry = new char[stoi(ram.at(0))];
    strncpy(entry, (ram.at(sector + 2)).c_str(), stoi(ram.at(0)));
    buffer = entry;
    if (buffer == nullptr) {
      std::cout << "Error: Block #" << sector << " is a null pointer" << '\n';
      ds = DiskStatus::ERROR;
    } else
      ds = DiskStatus::OK;
  } else {
    std::cout << "Error: Block #" << sector << " is out of range" << '\n';
    ds = DiskStatus::BLOCK_OUT_OF_RANGE;
  }
  return ds;
}






/*write:: writes the desired sector from the ram vector if the sector is below
*  or out of bounds of the vector the DiskStatus is set to BLOCK_OUT_OF_RANGE
*  and a error is printed to the user. If the buffer set is a nullptr the
*  DiskStatus is set to error and the corresponding message is printed.
*  @param - sector: the desired sector to access
*  @param - buffer: the pointer the data will be read from and set in the disk
*/

DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    if (buffer != nullptr) {
    std::string entry((char *) buffer);
    ram.at(sector + 2) = entry;
    free(buffer);
    ds = DiskStatus::OK;
    } else {
      std::cout << "Error: Block #" << sector << " is a null pointer" << '\n';
      ds = DiskStatus::ERROR;
    }
  } else {
    std::cout << "Error: Block #" << sector << " is out of range" << '\n';
    ds = DiskStatus::BLOCK_OUT_OF_RANGE;
  }
  return ds;
}






/*sync:: currently returns DiskStatus::OK
*/

DiskStatus RAMVSSD::sync() {
  return DiskStatus::OK;
}

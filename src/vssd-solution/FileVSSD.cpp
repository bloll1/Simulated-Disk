//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#include "FileVSSD.h"
#include <stdio.h>
#include <cstring>
#include <iostream>

/*FileVSSD Constructor:: initializes a new file from the PersistentArray class stored
*  is the FileVSSD header. We also store the block_size and block_count as global variables.
*  The blocksize and the blockcount are stored onto the array in the first and second block
*  as metadata for reaccsessing. Lastly the DiskStatus is set to OK to signal that the disk
*  is ready.
*  @param - block_size: the size of each block of data
*  @param - block_count: the total number of blocks in the disk
*  @param - filename: the disried filename used to initialize a new PersistentArray
*  @param - initialize: if the file is new the file will be forced to truncate
*             regardless of any existence of old files.
*/

FileVSSD::FileVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {

    file = new PersistentArray(filename, initialize);
    block_s = block_size;
    block_c = block_size;

    char k[block_size];
    memcpy(k, (char*)&block_size, sizeof(block_size));
    file->write_k(0, k, block_size);

    memcpy(k, (char*)&block_count, sizeof(block_count));
    file->write_k(1, k, block_size);

    ds = DiskStatus::OK;
}


/*FileVSSD Deconstructor:: calls the PersistentArray Deconstructor to close
*  the stream to the open file
*/

FileVSSD::~FileVSSD() {
  file->~PersistentArray();
}

/*blockSize:: Returns the size of each block(block_s in the header) unless
*  the disk is currently set to NOT_READY
*/

std::size_t FileVSSD::blockSize() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_s;
  }
  return 0;
}

/*blockCount:: Returns the total # of blocks(block_c in the header) unless
*  the disk is currently set to NOT_READY
*/

std::size_t FileVSSD::blockCount() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_c;
  }
  return 0;
}


/*status:: Returns the ds variable in the header(AKA the current DiskStatus)
*/

DiskStatus FileVSSD::status() const {
  return ds;
}



/*read:: calls the read_k to read the desired sector from the PersistentArray if the sector is below
*  or out of bounds of the vector the DiskStatus is set to BLOCK_OUT_OF_RANGE
*  and a error is printed to the user. If the buffer passed in is a nullptr the
*  DiskStatus is set to error and the corresponding message is printed.
*  @param - sector: the desired sector to access
*  @param - buffer: the pointer the data will be set to from the PersistentArray sector
*/

DiskStatus FileVSSD::read(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    free(buffer);
    buffer = file->read_k(sector + 2, (char *)buffer, block_s);
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



/*write:: calls the write_k to write to the desired sector from the PersistentArray if the sector is below
*  or out of bounds of the vector the DiskStatus is set to BLOCK_OUT_OF_RANGE
*  and a error is printed to the user. If the buffer set is a nullptr the
*  DiskStatus is set to error and the corresponding message is printed.
*  @param - sector: the desired sector to access
*  @param - buffer: the pointer the data will be read from and set in the disk
*/

DiskStatus FileVSSD::write(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    if (buffer != nullptr) {
      file->write_k(sector + 2, (char *)buffer, block_s);
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



/*sync:: flushes the file stream and returns DiskStatus::OK
*/

DiskStatus FileVSSD::sync() {
  file->flush_stream();
  return DiskStatus::OK;
}

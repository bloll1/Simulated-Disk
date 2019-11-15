//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#ifndef rammVSSD
#define rammVSSD

#include "VVSSD.h"
#include "PersistentArray.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <iostream>

/*
*Virtualized Ram disk implementation through the RAMVSSD class,
*  the class stores the data as strings and each element acts as the disk
*  sector. RAMVSSD extends the VVSSD class inheriting calls requested of
*  the current status from the ds varaiable, the blocksize(block_s), or
*  the blockcount(block_c)
*/


class RAMVSSD
: public VVSSD {
  public:

    //Vector for holding block data
    std::vector<std::string> ram;
    unsigned int block_s, block_c;


    //Constructor for initializion of the virtualized disk
    RAMVSSD(unsigned int block_size, unsigned int block_count,
               const std::string & filename, bool initialize);

    //Destructor for memory
    virtual ~RAMVSSD();

  public:

    //Returns the size of each block given on initializion
    //  if disk is NOT_READY it returns 0
    virtual std::size_t blockSize() const;

    //Returns the total block count on the disk given on initializion
    //  if disk is NOT_READY it returns 0
    virtual std::size_t blockCount() const;

    //Returns the ds(current DiskStatus) variable
    virtual DiskStatus status() const;

    //Given the sector it reads the data at the given block and sets the
    // buffer pointer to it.
    virtual DiskStatus read(blocknumber_t sector, void * buffer);

    //Given the sector it writes the data from the buffer into the block
    //  as a string on the vector
    virtual DiskStatus write(blocknumber_t sector, void * buffer);


    //Does nothing exepct return DiskStatus::OK
    virtual DiskStatus sync();


    //A DiskStatus variable that is set anytime a RAMVSSD function initializes,
    // writes or reads
    DiskStatus ds = DiskStatus::NOT_READY;

};

#endif

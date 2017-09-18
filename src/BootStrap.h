/**
 * BootStrapSector is the first 512 bytes of the FAT.
 * Two byte fields are little-endian
 *
 * The format of this sector is:
 * byte(s) contents
 * ------- -------------------------------------------------------
 *  0-2     first instruction of bootstrap routine
 *  3-10    OEM name
 *  11-12   number of bytes per sector
 *  13      number of sectors per cluster
 *  14-15   number of reserved sectors
 *  16      number of copies of the file allocation table
 *  17-18   number of entries in root directory
 *  19-20   total number of sectors
 *  21      media descriptor byte
 *  22-23   number of sectors in each copy of file allocation table
 *  24-25   number of sectors per track
 *  26-27   number of sides
 *  28-29   number of hidden sectors
 *  30-509  bootstrap routine and partition information
 *  510     hexadecimal 55
 *  511     hexadecimal AA
 */

#ifndef _BOOTSTRAP_H_
#define _BOOTSTRAP_H_

#include <stdio.h>
#include "ByteType.h"

struct BootStrap {
    byte jumpInstruction[3];        /* 00-02 */
    byte OEMName[8];                /* 03-10 */
    byte numBytesPerSector[2];      /* 11-12 */
    byte numSectorsPerCluster[1];   /*  13   */
    byte numReservedSectors[2];     /* 14-15 */
    byte numCopiesFAT[1];           /*  16   */
    byte numEntriesInRootdir[2];    /* 17-18 */
    byte numSectors[2];             /* 19-20 */
    byte mediaDescriptor[1];        /*  21   */
    byte numSectorsInFAT[2];        /* 22-23 */
    byte numSectorsPerTrack[2];     /* 24-25 */
    byte numSides[2];               /* 26-27 */
    byte numHiddenSectors[2];       /* 28-29 */
};

typedef struct BootStrap BootStrap;

int loadBootStrap(BootStrap* bootstrap, FILE* file);
int getNumBytesInFAT(const BootStrap* bootstrap);
int getNumEntriesInRootDir(const BootStrap* bootstrap);
int getNumBytesInReservedSectors(const BootStrap* bootstrap);
int getNumCopiesFAT(const BootStrap* bootstrap);
int getNumBytesPerCluster(const BootStrap* bootstrap);

#endif  /* _BOOTSTRAP_H_ */

#include <unistd.h>
#include "BootStrap.h"

static int convertByteToInt(const byte* ptr, unsigned int len) {
    switch (len) {
        case 1:
        return ptr[0];
        case 2:
        return (ptr[1] << 8) + ptr[0];
        default:
        return 0;
    }
}

int loadBootStrap(BootStrap* bootstrap, FILE* file) {
    /* Seek to start address of dirEntry */
    int reposition = fseek(file, 0, SEEK_SET);
    if (reposition) {
        return -2;
    }

    /* load dirEntry */
    size_t read = fread(bootstrap, sizeof(byte), 30, file);
    if (read != 30) {
        return -1;
    }

    /* Success */
    return 0;
}

int getNumBytesInFAT(const BootStrap* bootstrap) {
    int numSectorInFAT = convertByteToInt(bootstrap->numSectorsInFAT, 2);
    int numBytesPerSector = convertByteToInt(bootstrap->numBytesPerSector, 2);
    return numSectorInFAT * numBytesPerSector;
}

int getNumEntriesInRootDir(const BootStrap* bootstrap) {
    return convertByteToInt(bootstrap->numEntriesInRootdir, 2);
}

int getNumBytesInReservedSectors(const BootStrap* bootstrap) {
    int numReservedSectors = convertByteToInt(bootstrap->numReservedSectors, 2);
    int numBytesPerSector = convertByteToInt(bootstrap->numBytesPerSector, 2);
    return numReservedSectors * numBytesPerSector;
}

int getNumCopiesFAT(const BootStrap* bootstrap) {
    return convertByteToInt(bootstrap->numCopiesFAT, 1);
}

int getNumBytesPerCluster(const BootStrap* bootstrap) {
    int numBytesPerSector = convertByteToInt(bootstrap->numBytesPerSector, 2);
    int numSectorsPerCluster = convertByteToInt(bootstrap->numSectorsPerCluster, 1);
    return numBytesPerSector * numSectorsPerCluster;
}

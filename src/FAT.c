#include "ByteType.h"
#include "FAT.h"

int loadFAT(FAT* fat, FILE* file, int address) {
    /* Check the validation of file and address */
    int reposition = fseek(file, address, SEEK_SET);
    if (reposition) {
        return -1;
    }

    /* Set data to struct */
    fat->file = file;
    fat->address = address;

    /* Success */
    return 0;
}

int getNextCluster(const FAT* fat, int cluster) {
    /* Save the current file position */
    long indicatorBackup = ftell(fat->file);

    /* Seek to start address of FAT */
    int set = fseek(fat->file, fat->address, SEEK_SET);
    if (set) {
        return -2;
    }

    /* Seek to the position of this cluster */
    long offset = cluster * 3 / 2;
    int cur = fseek(fat->file, offset, SEEK_CUR);

    /* Read data into entry */
    byte entry[2];
    size_t read = fread(entry, sizeof(byte), 2, fat->file);
    if (read != 2) {
        return -1;
    }

    /* Convert from entry to int value */
    int value;
    if (cluster % 2) {
        value = (entry[1] << 4) + ((entry[0] & 0xF0) >> 4);
    } else {
        value = ((entry[1] & 0x0F) << 8) + entry[0];
    }

    /* Recover the file position */
    int back = fseek(fat->file, indicatorBackup, SEEK_SET);
    if (back) {
        return -3;
    }

    /* Success */
    return value;
}

#include <string.h>
#include "DirEntry.h"

int loadDirEntry(DirEntry* dirEntry, FILE* file, int address) {
    /* Seek to start address of dirEntry */
    int reposition = fseek(file, address, SEEK_SET);
    if (reposition) {
        return -2;
    }

    /* load dirEntry */
    size_t read = fread(dirEntry, sizeof(byte), 32, file);
    if (read != 32) {
        return -1;
    }

    /* Success */
    return 0;
}

bool isEmpty(const DirEntry* dirEntry) {
    byte sign = dirEntry->name[0];
    if (sign == 0x00 || sign == 0x20) {
        return true;
    } else {
        return false;
    }
}

bool isDeleted(const DirEntry* dirEntry) {
    /* the DEL command will replace the first byte of the name
       by 0xe5 to signify "deleted" */
    byte sign = dirEntry->name[0];
    if (sign == 0xE5 || sign == 0x05) {
        return true;
    } else {
        return false;
    }
}

void getName(const DirEntry* dirEntry, char* name) {
    memcpy(name, dirEntry->name, 8);
    int i = 7;
    while(i >= 0 && name[i] == ' ') {
        i--;
    }
    name[i+1] = '\0';
}

void getExtension(const DirEntry* dirEntry, char* extension) {
    memcpy(extension, dirEntry->extension, 3);
    int i = 2;
    while(i >= 0 && extension[i] == ' ') {
        i--;
    }
    extension[i+1] = '\0';
}

bool isVolumeLabel(const DirEntry* dirEntry) {
    return (dirEntry->attribute[0] & 0x08) == 0x08;
}

int getHour(const DirEntry* dirEntry) {
    return dirEntry->time[1] >> 3;
}

int getMinute(const DirEntry* dirEntry) {
    int part1 = (0x07 & dirEntry->time[1]) << 3;
    int part2 = dirEntry->time[0] >> 5;
    return part1 + part2;
}

int getYear(const DirEntry* dirEntry) {
    int year = dirEntry->date[1] >> 1;
    return year + 1980;
}

int getMonth(const DirEntry* dirEntry) {
    int part1 = (0x01 & dirEntry->date[1]) << 3;
    int part2 = dirEntry->date[0] >> 5;
    return part1 + part2;
}

int getDay(const DirEntry* dirEntry) {
    return 0x1F & dirEntry->date[0];
}

int getStartCluster(const DirEntry* dirEntry) {
    return (dirEntry->startCluster[1] << 8) + dirEntry->startCluster[0];
}

int getFileSize(const DirEntry* dirEntry) {
    const byte* fileSize = dirEntry->fileSize;
    int size = 0;
    size += fileSize[0];
    size += fileSize[1] << 8;
    size += fileSize[2] << 16;
    size += fileSize[3] << 24;
    return size;
}

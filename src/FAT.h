#ifndef _FAT_H_
#define _FAT_H_

#include <stdio.h>

struct FAT {
    FILE* file;
    int address;
};

typedef struct FAT FAT;

int loadFAT(FAT* fat, FILE* file, int address);
int getNextCluster(const FAT* fat, int cluster);

#endif  /* _FAT_H_ */

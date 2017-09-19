/**
 * DirEntry is the abbreviation for directory entry.
 * It has 32 bytes.
 *
 * The format of this is:
 * byte(s) contents
 * ------- -------------------------------------------------------
 * 0-10    File name (8 bytes) with extension (3 bytes)
 * 11      Attribute - a bitvector. Bit 0: read only. Bit 1: hidden.
 *         Bit 2: system file. Bit 3: volume label. Bit 4: subdirEntry.
 *         Bit 5: archive. Bits 6-7: unused.
 * 12-21   Reserved (see below)
 * 22-23   Time (5/6/5 bits, for hour/minutes/doubleseconds)
 * 24-25   Date (7/4/5 bits, for year-since-1980/month/day)
 * 26-27   Starting cluster (0 for an empty file)
 * 28-31   Filesize in bytes
 */

#ifndef _DIRENTRY_H_
#define _DIRENTRY_H_

#include <stdio.h>
#include <stdbool.h>
#include "ByteType.h"

struct DirEntry {
    byte name[8];           /* 00-07 */
    byte extension[3];      /* 08-10 */
    byte attribute[1];      /*  11   */
    byte reserved[10];      /* 12-21 */
    byte time[2];           /* 22-23 */
    byte date[2];           /* 24-25 */
    byte startCluster[2];   /* 26-27 */
    byte fileSize[4];       /* 28-31 */
};

typedef struct DirEntry DirEntry;

int loadDirEntry(DirEntry* dirEntry, FILE* file, int address);
bool isEmpty(const DirEntry* dirEntry);
bool isDeleted(const DirEntry* dirEntry);
void getName(const DirEntry* dirEntry, char* name);
void getExtension(const DirEntry* dirEntry, char* extension);
bool isVolumeLabel(const DirEntry* dirEntry);
int getHour(const DirEntry* dirEntry);
int getMinute(const DirEntry* dirEntry);
int getYear(const DirEntry* dirEntry);
int getMonth(const DirEntry* dirEntry);
int getDay(const DirEntry* dirEntry);
int getStartCluster(const DirEntry* dirEntry);
int getFileSize(const DirEntry* dirEntry);

#endif  /* _DIRENTRY_H_ */

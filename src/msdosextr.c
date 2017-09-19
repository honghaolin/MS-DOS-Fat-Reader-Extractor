#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "BootStrap.h"
#include "DirEntry.h"
#include "FAT.h"

int main(int argc, char* argv[]) {
    /* Check arg numbers */
    if (argc < 2) {
        printf("[Error] Need a file path\n");
        printf("{Usage} %s <Path>\n", argv[0]);
        return -1;
    }

    /* Get file path */
    char* path = argv[argc-1];

    /* Open the file */
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("[Error] Failed to open %s\n", path);
        return -1;
    }

    /* Load bootstrap sector */
    BootStrap* bootstrap = (BootStrap*)malloc( sizeof(BootStrap) );
    if(!bootstrap) {
        printf("[Error] Failed to allocate memory for bootstrap\n");
        return -1;
    }

    int loadSign = loadBootStrap(bootstrap, file);
    if (loadSign != 0) {
        printf("[Error] Failed to load bootstrap\n");
        return -1;
    }

    /* Get information of root directory */
    int rootDirAddr = getNumBytesInReservedSectors(bootstrap)
                    + (getNumCopiesFAT(bootstrap) * getNumBytesInFAT(bootstrap));

    int maxEntries = getNumEntriesInRootDir(bootstrap);

    /* Allocate memory for root directory entries */
    DirEntry** entries = (DirEntry**)malloc(maxEntries * sizeof(DirEntry*));
    if (!entries) {
        printf("[Error] Failed to allocate memory for root directory entries\n");
        return -1;
    }

    /* Load data of root directory entries */
    for (int i = 0; i < maxEntries; i++) {
        entries[i] = (DirEntry*)malloc(sizeof(DirEntry));
        if (!entries[i]) {
            printf("[Error] Failed to allocate memory for directory entry %d\n", i);
            return -1;
        }

        int load = loadDirEntry(entries[i], file, rootDirAddr + (i*32));
        if (load != 0) {
            printf("[Error] Faild to load directory entry %d\n", i);
            continue;
        }
    }

    /* Count and print the total files to be extract */
    int fileCount = 0;
    for (int i = 0; i < maxEntries; i++) {
        if (   isEmpty(entries[i])
            || isDeleted(entries[i])
            || isVolumeLabel(entries[i])
        ) {
            continue;
        }

        fileCount++;
    }

    printf("Extracting: %d file(s)\n", fileCount);

    /* Load file allocation table */
    FAT* fat = (FAT*)malloc( sizeof(FAT) );
    if (!fat) {
        printf("[Error] Failed to allocate memory for FAT\n");
        return -1;
    }

    int address = getNumBytesInReservedSectors(bootstrap);
    int load_fat = loadFAT(fat, file, address);
    if (load_fat != 0) {
        printf("[Error] Failed to load FAT\n");
        return -1;
    }

    /* Extract files */
    char* name = (char*)malloc(9 * sizeof(char));
    if (!name) {
        printf("[Error] Failed to allocate memory for name\n");
        return -1;
    }

    char* extension = (char*)malloc(4 * sizeof(char));
    if (!extension) {
        printf("[Error] Failed to allocate memory for extension\n");
        return -1;
    }

    char* fullName = (char*)malloc(13 * sizeof(char));
    if (!fullName) {
        printf("[Error] Failed to allocate memory for full name\n");
        return -1;
    }

    int bytesPerCluster = getNumBytesPerCluster(bootstrap);
    char* buffer = (char*)malloc(bytesPerCluster * sizeof(char));
    if (!buffer) {
        printf("[Error] Failed to allocate memory for buffer\n");
        return -1;
    }

    int dataStartAddr = rootDirAddr + maxEntries * 32;

    for (int i = 0; i < maxEntries; i++) {
        /* Check for file */
        if (   isEmpty(entries[i])
            || isDeleted(entries[i])
            || isVolumeLabel(entries[i])
        ) {
            continue;
        }

        /* Get file name and extension */
        getName(entries[i], name);
        strcpy(fullName, name);

        getExtension(entries[i], extension);
        if (extension[0] != '\0') {
            strcat(fullName, ".");
            strcat(fullName, extension);
        }

        /* Print full name */
        printf("%s\n", fullName);

        /* Create file */
        FILE* extrFile = fopen(fullName, "w");
        if (!extrFile) {
            printf("[Error] Failed to create / open %s\n", path);
            return -1;
        }

        /* Traverse FAT and extract data from each cluster referenced */
        int cluster = getStartCluster(entries[i]);
        while (cluster < 0xFF8) {
            /* Seek to the loading position */
            long offset = dataStartAddr + (cluster - 2) * bytesPerCluster;
            int seek = fseek(file, offset, SEEK_SET);
            if (seek) {
                printf("[Error] Failed to seek address %ld in %s\n", offset, path);
                continue;
            }

            /* Update cluster */
            cluster = getNextCluster(fat, cluster);

            /* Get length of data in this cluster */
            int dataLength = bytesPerCluster;
            if (cluster >= 0xFF8) {
                dataLength = getFileSize(entries[i]) % bytesPerCluster;
            }

            /* Load cluster from image file */
            size_t read = fread(buffer, sizeof(char), dataLength, file);
            if (read != dataLength) {
                printf("[Error] Faild to read from address %ld in %s\n", offset, path);
                continue;
            }

            /* Write to file */
            size_t write = fwrite(buffer, sizeof(char), dataLength, extrFile);
            if (write != dataLength) {
                printf("[Error] Faild to write to %s\n", fullName);
                break;
            }
        }

        /* Close file */
        if (extrFile) {
            fclose(extrFile);
        }
    }

    /* Clean and Close the file */
    free(buffer);
    free(name);
    free(extension);
    free(fullName);
    free(fat);
    for (int i = 0; i < maxEntries; i++) {
        free(entries[i]);
    }
    free(entries);
    free(bootstrap);
    if (file) {
        fclose(file);
    }

    /* Success */
    return 0;
}

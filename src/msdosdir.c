#include <malloc.h>
#include <stdio.h>
#include "BootStrap.h"
#include "DirEntry.h"

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

    /* Allocate memory for name and extension */
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

    /* Find and print the volume name */
    for (int i = 0; i < maxEntries; i++) {
        if (isVolumeLabel(entries[i])) {
            getName(entries[i], name);
            getExtension(entries[i], extension);
            break;
        }
    }

    printf(" Volume name is %-8s%-3s\n\n", name, extension);

    /* Print the information for files */
    int fileCount = 0;
    int totalSize = 0;
    int fileSize = 0;

    for (int i = 0; i < maxEntries; i++) {
        if (   isEmpty(entries[i])
            || isDeleted(entries[i])
            || isVolumeLabel(entries[i])
        ) {
            continue;
        }

        getName(entries[i], name);
        getExtension(entries[i], extension);
        fileSize = getFileSize(entries[i]);

        printf("%-8s %-3s %12d %2d-%02d-%4d %02d:%02d\n",
            name,
            extension,
            fileSize,
            getMonth(entries[i]),
            getDay(entries[i]),
            getYear(entries[i]),
            getHour(entries[i]),
            getMinute(entries[i])
        );

        fileCount++;
        totalSize += fileSize;
    }

    printf("%9d file(s) %13d bytes\n\n", fileCount, totalSize);

    /* Clean and Close the file */
    free(name);
    free(extension);
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

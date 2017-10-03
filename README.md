MS DOS Fat Reader/Extractor
===========================

Features
--------

* Produce catalog listings of each floppy image
* Extract the files from the image

Example
-----

#### MS DOS Fat Reader
```
$ ./msdosdir samplefat.bin
 Volume name is DISK      2

ADDNAME  EX_        14032  8-31-1994 00:00
AVEXTRA  TXT          399  2-28-1994 10:19
DNR      EX_        17336 12-16-1994 18:47
EMSBFR   EX_         1407  8-31-1994 00:00
HOSTS                 715  8-31-1994 19:37
IPCONFIG EX_         8509  8-31-1994 00:00
LICENSE  TXT         2925  3-28-1995 17:23
LMHOSTS               817  8-31-1994 19:36
NEMM     DO_         1764  8-31-1994 00:00
NETBIND  COM         8513  8-31-1994 00:00
NETWORKS              395  8-31-1994 18:52
NMTSR    EX_        12434  8-31-1994 00:00
PING     EX_        47277  8-31-1994 00:00
PROTOCOL              795  8-31-1994 18:52
SERVICES             5973  5-08-1995 14:34
SOCKETS  EX_        27497  9-01-1994 13:22
TCPDRV   DO_         2810  8-31-1994 00:00
TCPTSR   EX_        48433  8-31-1994 00:00
TCPUTILS INI          233  8-31-1994 00:00
TINYRFC  EX_        23561 12-01-1994 19:39
UMB      CO_         2353  8-31-1994 00:00
VBAPI    386         9524  8-31-1994 00:00
VSOCKETS 386         9535  8-31-1994 00:00
WINSOCK  DL_        25236  1-23-1995 15:21
WIN_SOCK DL_        16122  8-31-1994 00:00
WSAHDAPP EX_         3271  8-31-1994 00:00
WSOCKETS DL_        15862  8-31-1994 00:00
       27 file(s)        307728 bytes

```

#### MS DOS Fat Extractor
```
$ ls
msdosdir  msdosextr  samplefat.bin
$ ./msdosextr samplefat.bin
Extracting: 27 file(s)
ADDNAME.EX_
AVEXTRA.TXT
DNR.EX_
EMSBFR.EX_
HOSTS
IPCONFIG.EX_
LICENSE.TXT
LMHOSTS
NEMM.DO_
NETBIND.COM
NETWORKS
NMTSR.EX_
PING.EX_
PROTOCOL
SERVICES
SOCKETS.EX_
TCPDRV.DO_
TCPTSR.EX_
TCPUTILS.INI
TINYRFC.EX_
UMB.CO_
VBAPI.386
VSOCKETS.386
WINSOCK.DL_
WIN_SOCK.DL_
WSAHDAPP.EX_
WSOCKETS.DL_
$ ls
ADDNAME.EX_   LICENSE.TXT  NETWORKS       SOCKETS.EX_   VBAPI.386
AVEXTRA.TXT   LMHOSTS      NMTSR.EX_      TCPDRV.DO_    VSOCKETS.386
DNR.EX_       msdosdir     PING.EX_       TCPTSR.EX_    WIN_SOCK.DL_
EMSBFR.EX_    msdosextr    PROTOCOL       TCPUTILS.INI  WINSOCK.DL_
HOSTS         NEMM.DO_     samplefat.bin  TINYRFC.EX_   WSAHDAPP.EX_
IPCONFIG.EX_  NETBIND.COM  SERVICES       UMB.CO_       WSOCKETS.DL_
```

Compilation
-----------

A [make file](./build/Makefile) has been provide to build the programs.

#### Usage
* Put the make file and the source file into same directory
* To build both Reade and Extractor run `make`
* To build MS DOS Fat Reade run `make msdosdir`
* To build MS DOS Fat Extractor run `make msdosextr`

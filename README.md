# **Project 5: A Minix Disk Program**

You are asked to develop a Minix Disk program that allows users to access a Minix disk locally mounted on a computer. You are expected to use C programming language. In your program, all Minix file I/O related operations must use the system calls discussed in our lectures, including `open()`, `close()`, `read()`, `lseek()`, and you should not use those library functions, like `fopen()`, `fclose()`, `fread()`, and etc, for doing file I/O. 

${\color{red}In \space this \space project, \space you \space may \space assume \space the \space block \space size \space if \space 1024 \space bytes.}$


---

## **Required Modules**

### **Minix Client Environment (Minix Console)**  
The console (interface) that allows users to operate on a minix disk by using the following commands:

---

### **help**  
**Points:** 5 
Prints the commands supported in this minix console.

---

### **minimount [image file]**  
**Points:** 5  
mount a local minix disk, name as a iminix image file, e.g. `imagefile.img`.

***Hint:** Mounting a disk image is equivalent to opening the image file. Use `lseek()` to move the file marker to the desired position to read the data.*

---

### **miniumount**  
**Points:** 5  
Unmounts the mounted Minix disk.

---

### **showsuper**  
**Points:** 20  
Displays information about the superblock.  

**Example:**  
```bash
minix: showsuper

number of inodes:       3424
number of zones:        10240
number of imap_blocks:  1
number of zmap_blocks:  2
first data zone:        112
log zone size:          0
max size:               268966912
magic:                  5007
state:                  0
zones:                  0
```

**Note:** The above values should be extracted directly from the Minix disk and **not hardcoded**. No credit will be given for hardcoded results.

---

### **traverse [-l]**  
**Points:** 40  
Lists the contents of the root directory (excluding `.` and `..`).

**Example:**  
```bash
minix: traverse

A.cla
Aloop1.cla
Aloop1.jav
Aloop2.cla
Aloop2.jav
Aloop3.cla
Aloop3.jav
B.cla
Condit.cla
Condit.jav
D.cla
Demo.cla
Demo.jav
dir1
PolyTest.cla
PolyTest.jav
some.cla
some.java
```

**Switch `-l` (long list):** Displays detailed information, including the file type, permission bits, user ID, file size (in bytes), last modified date (with year), and file/path name.

**Example:**  
```bash
minix: traverse -l (20 points)
-rw-rw-r-- 1000 670 Sep 20  2012 A.cla
-rw-r--r-- 1000 763 Dec 14  2010 Aloop1.cla
-rw-r--r-- 1000 333 Dec 14  2010 Aloop1.jav
-rw-r--r-- 1000 786 Dec 14  2010 Aloop2.cla
-rw-r--r-- 1000 404 Dec 14  2010 Aloop2.jav
-rw-r--r-- 1000 786 Dec 14  2010 Aloop3.cla
-rw-r--r-- 1000 408 Dec 14  2010 Aloop3.jav
-rw-rw-r-- 1000 358 Sep 20  2012 B.cla
-rw-r--r-- 1000 1067 Dec 14  2010 Condit.cla
-rw-r--r-- 1000 444 Dec 14  2010 Condit.jav
-rw-rw-r-- 1000 508 Sep 20  2012 D.cla
-rw-r--r-- 1000 967 Dec 14  2010 Demo.cla
-rw-r--r-- 1000 648 Dec 14  2010 Demo.jav
drwx------ 1000 128 Oct 27  2014 dir1
-rw-rw-r-- 1000 630 Sep 20  2012 PolyTest.cla
-rw-rw-r-- 1000 1063 Sep 20  2012 PolyTest.jav
-rw-r--r-- 1000 226 Dec 14  2010 some.cla
-rw-r--r-- 1000 278 Dec 14  2010 some.java
```

***Hint:** The `mode` field in the inode structure (a 2-byte unsigned integer) uses the following flags:* 
```c
S_IFMT     0170000   bit mask for the file type bit fields
S_IFSOCK   0140000   socket
S_IFLNK    0120000   symbolic link
S_IFREG    0100000   regular file
S_IFBLK    0060000   block device
S_IFDIR    0040000   directory
S_IFCHR    0020000   character device
S_IFIFO    0010000   FIFO
S_ISUID    0004000   set-user-ID bit
S_ISGID    0002000   set-group-ID bit (see below)
S_ISVTX    0001000   sticky bit (see below)
S_IRWXU    00700     mask for file owner permissions
S_IRUSR    00400     owner has read permission
S_IWUSR    00200     owner has write permission
S_IXUSR    00100     owner has execute permission
S_IRWXG    00070     mask for group permissions
S_IRGRP    00040     group has read permission
S_IWGRP    00020     group has write permission
S_IXGRP    00010     group has execute permission
S_IRWXO    00007     mask for permissions for others (not in group)
S_IROTH    00004     others have read permission
S_IWOTH    00002     others have write permission
S_IXOTH    00001     others have execute permission

```

---

### **showzone [zone number]**  
**Points:** 10  
Displays the ASCII content of the specified zone number (1024 bytes per zone). Non-ASCII values should be displayed as spaces. Use the `isprint()` library function to check if the given byte is an ASCII value.  

**Example:**  
```bash
minix: showzone 112

         0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
    0              .
   10
   20              .    .
   30
   40              A    .    c    l    a
   50
   60              A    l    o    o    p    1    .    c    l    a
   70
   80              A    l    o    o    p    1    .    j    a    v
   90
   a0              A    l    o    o    p    2    .    c    l    a
   b0
   c0              A    l    o    o    p    2    .    j    a    v
   d0
   e0              A    l    o    o    p    3    .    c    l    a
   f0
  100              A    l    o    o    p    3    .    j    a    v
  110
  120              B    .    c    l    a
  130
  140              C    o    n    d    i    t    .    c    l    a
  150
  160              C    o    n    d    i    t    .    j    a    v
  170
  180              D    .    c    l    a
  190
  1a0              D    e    m    o    .    c    l    a
  1b0
  1c0              D    e    m    o    .    j    a    v
  1d0
  1e0              d    i    r    1
  1f0
  200              P    o    l    y    T    e    s    t    .    c    l    a
  210
  220              P    o    l    y    T    e    s    t    .    j    a    v
  230
  240              s    o    m    e    .    c    l    a
  250
  260              s    o    m    e    .    j    a    v    a
  270
  280
  290
  2a0
  2b0
  2c0
  2d0
  2e0
  2f0
  300
  310
  320
  330
  340
  350
  360
  370
  380
  390
  3a0
  3b0
  3c0
  3d0
  3e0
  3f0
```

**Tip:** This command is useful for debugging.

---

### **[Bonus] showfile [filename]**  
**Points:** Up to 20 (Bonus)  
Displays the content of a specified file (in hex) located in the root directory. Assumes the file size is less than 7KB.  

**Behavior:**  
- If the file is found, display its contents in hexadecimal format.  
- If the file is not found, display an appropriate error message.  

**Example:**  
```bash
minix: showfile Aloop1.jav
   70   75   62   6c   69   63   20   63   6c   61   73   73   20   41   6c   6f
   6f   70   31   20   7b    a   20   20   20   20   70   75   62   6c   69   63
   20   73   74   61   74   69   63   20   76   6f   69   64   20   6d   61   69
   6e   28   53   74   72   69   6e   67   5b   5d   20   61   72   67   73   29
   20   7b    a    9    a    9   53   74   72   69   6e   67   5b   5d   20   72
   61   6e   6b   20   3d   20   7b   22   44   45   55   43   45   22   2c   20
   22   54   48   52   45   45   22   2c   20   22   46   4f   55   52   22   2c
   20   22   46   49   56   45   22   2c   20   22   53   49   58   22   2c   20
   22   53   45   56   45   4e   22   2c    a    9    9    9   20   22   45   49
   47   48   54   22   2c   20   22   4e   49   4e   45   22   2c   20   22   54
   45   4e   22   2c   20   22   4a   41   43   4b   22   2c   20   22   51   55
   45   45   4e   22   2c   20   22   4b   49   4e   47   22   2c   20   22   41
   43   45   22   7d   3b    a    9    a    9   69   6e   74   20   69   20   3d
   20   30   3b    a    9   69   6e   74   20   63   6f   75   6e   74   65   72
   20   3d   20   30   3b    a    9   77   68   69   6c   65   20   28   21   72
   61   6e   6b   5b   69   5d   2e   65   71   75   61   6c   73   28   22   53
   49   58   22   29   29   20   7b    a    9   20   20   20   20   69   2b   2b
   3b    a    9   20   20   20   20   63   6f   75   6e   74   65   72   2b   2b
   3b    a    9   7d    a    9   53   79   73   74   65   6d   2e   6f   75   74
   2e   70   72   69   6e   74   6c   6e   28   63   6f   75   6e   74   65   72
   29   3b    a    a   20   20   20   20   7d    a   7d    a    a    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
   0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0
```

---

### **quit**  
**Points:** 5  
Quits the minix console.

---

## **README/Makefile**  
**Points:** 10  
You are required to create a README file that includes:  
1. The contribution percentage of each group member.  
2. A brief and concise project description.  
3. Compilation instructions and execution command.  
4. A sample test run and its results.  
5. Explicitly list any non-working modules.  

---

## **Scoring Breakdown**
- **help:** 5 points  
- **minimount:** 5 points  
- **miniumount:** 5 points  
- **showsuper:** 20 points  
- **traverse:** 40 points  
- **showzone:** 10 points  
- **showfile (Bonus):** Up to 20 points  
- **quit:** 5 points  
- **README/Makefile:** 10 points  

**Total:** 100 points (+20 bonus)

---



# Project Notes
## General structure of `imagefile.img`:
```
+--------------------+
| Block 0            | -> Superblock
+--------------------+
| Block 1            | -> Group Descriptors
+--------------------+
| Block 2            | -> Block Bitmap
+--------------------+
| Block 3            | -> Inode Bitmap
+--------------------+
| Blocks 4-8         | -> Inode Table
+--------------------+
| Blocks 9-N         | -> Data Blocks
+--------------------+

Superblock:         Contains metadata about the file system.
Group Descriptors:  Contains pointers to the bitmaps, inode table, etc.
Block Bitmap:       Bitmap to track free/used blocks.
Inode Bitmap:       Bitmap to track free/used inodes.
Inode Table:        Array of inodes storing file metadata.
Data Blocks:        Blocks where file and directory content is stored.
```

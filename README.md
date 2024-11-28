# **Project 5: A Minix Disk Program**

This project involves developing a Minix Disk program that allows users to access a Minix disk locally mounted on a computer. You are required to use the **C programming language**. All Minix file I/O operations in your program must exclusively use system calls discussed in class, such as `open()`, `close()`, `read()`, and `lseek()`. **Do not use library functions like `fopen()`, `fclose()`, or `fread()`** for file I/O operations.

---

## **Required Modules**

### 1. **Minix Client Environment (Minix Console)**  
The console provides an interface for users to operate on a Minix disk using the following commands:

---

### **help**  
**Points:** 5  
Prints the commands supported in this Minix console.

---

### **minimount [image file]**  
**Points:** 5  
Mounts a local Minix disk, referred to as a Minix image file, e.g., `imagefile.img`.

**Hint:** Mounting a disk image is equivalent to opening the image file. Use `lseek()` to move the file marker to the desired position to read the data.

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
minix: traverse -l

-rw-rw-r-- 1000 670 Sep 20  2012 A.cla
-rw-r--r-- 1000 763 Dec 14  2010 Aloop1.cla
-rw-r--r-- 1000 333 Dec 14  2010 Aloop1.jav
-rw-r--r-- 1000 786 Dec 14  2010 Aloop2.cla
-rw-r--r-- 1000 404 Dec 14  2010 Aloop2.jav
-rw-r--r-- 1000 786 Dec 14  2010 Aloop3.cla
-rw-r--r-- 1000 408 Dec 14  2010 Aloop3.jav
drwx------ 1000 128 Oct 27  2014 dir1
-rw-rw-r-- 1000 630 Sep 20  2012 PolyTest.cla
-rw-r--r-- 1000 648 Dec 14  2010 Demo.jav
```

**Hint:** The `mode` field in the inode structure (a 2-byte unsigned integer) uses the following flags:  
```c
S_IFMT     0170000   // bit mask for the file type bit fields
S_IFSOCK   0140000   // socket
S_IFLNK    0120000   // symbolic link
S_IFREG    0100000   // regular file
S_IFDIR    0040000   // directory
S_IRUSR    00400     // owner has read permission
S_IWUSR    00200     // owner has write permission
S_IXUSR    00100     // owner has execute permission
...
```

---

### **showzone [zone number]**  
**Points:** 10  
Displays the ASCII content of the specified zone number (1024 bytes per zone). Non-ASCII values should be displayed as spaces. Use the `isprint()` library function to check ASCII values.  

**Example:**  
```bash
minix: showzone 112

 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
 0               .
 10
 20               .    .
 30
 40               A    .    c    l    a
 50
 ...
```

**Tip:** This command is useful for debugging.

---

### **[Bonus] showfile [filename]**  
**Points:** Up to 20 (Bonus)  
Displays the content of a specified file located in the root directory. Assumes the file size is less than 7KB.  

**Behavior:**  
- If the file is found, display its contents in hexadecimal format.  
- If the file is not found, display an appropriate error message.  

**Example:**  
```bash
minix: showfile Aloop1.jav

70 75 62 6c 69 63 20 63 6c 61 73 73 20 41 6c 6f
6f 70 31 20 7b
...
```

---

### **quit**  
**Points:** 5  
Exits the Minix console.

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

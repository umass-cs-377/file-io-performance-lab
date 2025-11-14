# Purpose

In this lab, you will explore how different file access patterns and block sizes affect performance. The goal is to understand how the operating system handles I/O under the hood, and how sequential versus random access, as well as block size, impact the time it takes to read data from a file. You will implement a simple benchmark in C, measure performance, and analyze the results.

# Setup

Once you have logged in to Edlab, you can clone this repo using

```bash
git clone https://github.com/umass-cs-377/file-io-performance-lab.git
```

Then you can use `cd` to open the directory you just cloned:

```bash
cd file-io-performance-lab
```

# Step 1: Create a (huge) file

You will begin by creating a large test file. On your terminal, use the following command to generate a 100 MB binary file filled with random data:

```c
dd if=/dev/urandom of=testfile.bin bs=1M count=100
```

This will generate a file named `testfile.bin` in your work directory.  Use `ls -lh` to make sure the file is created and the size is correct.  Don’t forget to delete it after you finish this lab.

# Step 2: Reads with Different Block Sizes

In this step, we will do a quick experiment and see how the **block size** you use to read a file changes performance.

But what is *block size*? The block size **here means **how many bytes your program reads from the file at once**. When you call `read(fd, buf, block_size)`, the operating system copies that many bytes from the disk into memory before returning.

Before you start, let’s make a prediction:

- Do you think reading with a small block size (like 1 KB) will be faster or slower than a large block size (like 16 KB)?  Why do you think so?

Let’s write it down in a table like this:

| Block Size (bytes) | Predicted Speed (Fast/Slow) | Actual Time (ms) | Observation |
| --- | --- | --- | --- |
| 1,024 |  |  |  |
| 4,096 |  |  |  |
| 8192 |  |  |  |
| 16,384 |  |  |  |

Now, test your prediction. Run your program in sequential mode (we will tell you later what this is) several times with different block sizes:

```c
./benchmark testfile.bin 1024 sequential
./benchmark testfile.bin 4096 sequential
./benchmark testfile.bin 8192 sequential
./benchmark testfile.bin 16384 sequential
```

Record the results in the table, then answer the following question:

- Big or small block size can improve the performance? Why is that?
- If the block sizes keep getting bigger and bigger, what do you think will happen to the performance?

# Step 3: Random vs Sequential Access

Next, you’ll explore the **access pattern,** whether you read the file sequentially or randomly.

Again, before you begin, let’s make some predictions:

- Which access pattern do you think will be faster, sequential or random?
- How big do you expect the difference to be?

For random access, please update the following part of the code.

```c
    size_t total_read = 0;
    while (total_read < filesize) {
        if (random) { // if random flag is True, we will randomly pick an offset
                  // TODO: pick a random block within block boundary
                  // for example, if the filesize is 9000 bytes and blocksize is 4000, then we have 3 blocks.
                  // TODO: move the file offset to block above using lseek
                  // for example, if the random block is 2 and blocksize is 4000, the offset should be block# * blocksize.
        }
        ssize_t bytes = read(fd, buf, block_size);
        if (bytes <= 0) break;
        total_read += bytes;
    }
```

Run both for comparison:

```c
./benchmark testfile.bin 4096 sequential
./benchmark testfile.bin 4096 random
```

Record your findings:

| Access Pattern | Block Size (bytes) | Time (ms) | Observation |
| --- | --- | --- | --- |
| Sequential | 4,096 |  |  |
| Random | 4,096 |  |  |

Answer the following questions:

- If 100,000,000 bytes file size is and `read()`’s block size is 4,096 bytes.
    - How many **full blocks** can be read from this file?
    - How many bytes will remain after reading all full blocks?
    - What is the byte offset of the start of the last full block?
    - If the random block number is 7, at what byte offset will the read operation start?
    - How many total reads required to read the entire file?

# Step 4: Buffered vs Synced Writes

So far, you’ve measured `read` performance. Now you’ll explore what happens when you `write` data to disk and learn that writing doesn’t necessarily mean “saved.”

When you call `write()`, Linux usually doesn’t send the data to the disk right away. It stores it in the **kernel’s page cache** and marks it as “dirty.” Later, a background process (the *flush daemon*) writes it to disk. This makes `write()` fast.

- You might want to think: Why buffering in kernel cache and write once in a while, make write() faster?

To ensure data actually reaches disk immediately, programs can call **`fsync(fd)`**, which forces the OS to flush the dirty pages to the physical device. Of course, that will be much slower.

Here is our simple program to do a fun experiment:

```c
    for (int i = 0; i < 1024; i++) {  // write 1024 blocks
        if (write(fd, buf, size) != size) {
            perror("write"); exit(1);
        }
        if (use_fsync) // flush if the use_fsync flag is True
            fsync(fd);
    }
```

Run this experiment to see:

```bash
./write_sync 4096 normal
./write_sync 4096 sync
```

Record your results:

| Block Size | Mode | Time (ms) | Observation |
| --- | --- | --- | --- |
| 4,096 | normal |  |  |
| 4,096 | sync |  |  |

Now answer the following question:

- How much slower is `write()` when followed by `fsync()`?
- Will there ever be a good time to use `fsync()`? Can you think of any use case or application?
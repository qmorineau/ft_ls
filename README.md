# ft_ls - Performance-Oriented System Listing

## Overview

**ft_ls** is a C implementation of the Unix `ls` command, focused on directory traversal, metadata retrieval, memory management, and buffered output.

The project explores modern Linux APIs and performance-oriented techniques, including `statx(2)`, custom memory pooling, and buffered I/O.

## Core Technical Features

* **Statx Masking:** Uses `statx(2)` with explicit request masks to retrieve only the metadata required by the current listing mode.
* **Custom Pool Allocator:** Uses a pool allocator for high-frequency file metadata allocations, reducing the overhead of repeated `malloc/free` calls during recursive traversal.
* **16 KB Output Buffer:** Batches formatted output into a fixed-size buffer, reducing the number of `write(2)` syscalls during large directory listings.
* **Environment-Driven Colors:** Features a dynamic color engine that parses `LS_COLORS` from the environment, supporting extension-specific and permission-based syntax highlighting.

## Supported Flags
`-l`, `-R`, `-a`, `-r`, `-t`, `-u`, `-f`, `-g`, `-d`

## Performance & Benchmarking

Benchmarks were conducted using `hyperfine` against the GNU `ls` utility.
For a consistent comparison, both implementations were tested with `LC_ALL=C` and ANSI color output enabled.

### Recursive Discovery Benchmark (`-R`)
*Target: /usr/lib (approx. 200,000+ entries)*

![Benchmark Screenshot](https://github.com/user-attachments/assets/60a91287-3ef1-40cc-ba9d-c009884cc0c7)

| Metric | GNU `ls -R` (`LC_ALL=C`) | **ft_ls -R** |
| :--- | :--- | :--- |
| **Real Time (Mean)** | 415.2 ms | **460.9 ms** |
| **User Time** | 93.3 ms | **108.6 ms** |
| **System Time** | 321.5 ms | **351.7 ms** |

---
> **Result:** `ft_ls` completes this benchmark in 460.9 ms, compared to 415.2 ms for GNU `ls`, corresponding to approximately 1.11× the runtime of the reference implementation.

## Architecture

### Kernel-Level Metadata Retrieval (`statx`)
`ft_ls` uses the Linux `statx(2)` system call with an explicit request mask to retrieve the metadata required by the current listing mode.
For example, when only file permissions and size are required, the implementation can request the corresponding `STATX_*` fields instead of requesting unrelated metadata.

### Sorting & Memory Pipeline
* **ASCII Sorting:** Uses raw byte-wise comparison for `LC_ALL=C` sorting, avoiding locale-dependent collation overhead.
* **Pool Allocation:** File metadata is allocated through a custom pool allocator, grouping allocations into contiguous memory blocks and reducing the overhead of many individual heap allocations.

## Quick Start

```bash
make
./ft_ls -R /usr/share
```

Developed by *Quentin Morineau*

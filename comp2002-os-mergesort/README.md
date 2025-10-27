README

## Project 3 / Concurrency Mergesort Alogrithm

* Authors: ZHAOXUAN GENG, JIAHAN YANG, YAO CHU
* Group name: Assignment 3 Groups 13

## Overview

This project implements the MergeSort algorithm in C, followed by a multithreaded version developed using the pthread library API. It allows users to freely input parameters to generate randomized lists of sufficient length and specify the number of threading levels, enabling performance comparisons to evaluate the acceleration effect of multithreading on the algorithm.

## Manifest

- Makefile: Makefile defines build rules and precompilation settings
- mergesort.c: The main alogrithm of the project
- mergesort.h: The starter code defines the following global variables in mergesort.h
- README.template: provide the template of README.md

## Building the project

1. Compilation:
    cd \os_assigment_3\comp2002-os-mergesort
    make
2. RUN
    ./test-mergesort 10000000 0 1234 (argument: the length of random array, thread level, random seed)

## Features and usage

    Main Features
    Implements the MergeSort algorithm in C.

    Provides a multithreaded version using the POSIX pthread library.

    Supports user-defined input parameters to:

        - Generate randomized integer arrays of custom length.
        - Specify the depth of multithreading for performance comparison.

    Measures and compares execution time between single-threaded and multithreaded sorting.

    Example:

    bash
    cd \os_assigment_3\comp2002-os-mergesort
    make
    ./test-mergesort 1000000 3 1234
    This command sorts 1,000,000 random integers using a multithreaded MergeSort with depth 3.

## Testing

How we test
Length of array: 100000000
Seed: 42

The result of testing: 
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 0 42
    Sorting 100000000 elements took 13.32 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 1 42
    Sorting 100000000 elements took 6.81 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 2 42
    Sorting 100000000 elements took 1.70 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 3 42
    Sorting 100000000 elements took 2.54 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 4 42
    Sorting 100000000 elements took 1.85 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 5 42
    Sorting 100000000 elements took 1.93 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 6 42
    Sorting 100000000 elements took -0.62 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 7 42
    Sorting 100000000 elements took 1.74 seconds.
    mark@DESKTOP-LRR356L:~/assigment/7064/7064-a3/os_assignment_3/comp2002-os-mergesort$ ./test-mergesort 100000000 8 42
    Sorting 100000000 elements took 1.93 seconds.

Performance (user-provided run, Linux bash, seed 42):

Sorting 100,000,000 elements with varying cutoff levels:

| cutoff | time (s) | speedup vs. 0 |
| 0 | 13.32 | 1.00× |
| 1 | 6.81  | 1.96× |
| 2 | 1.70  | 7.84× |
| 3 | 2.54  | 5.24× |
| 4 | 1.85  | 7.20× |
| 5 | 1.93  | 6.90× |
| 6 | -0.62 | n/a   |
| 7 | 1.74  | 7.66× |
| 8 | 1.93  | 6.90× |

- The best times here occur around cutoff 2 and 3; deeper levels don’t always help due to thread overhead and memory bandwidth contention.

## Known Bugs

There is a negative time in test when the cutoff is 6 we don't know the reason
And the cutoff at two level also don't fit our expect, it give 7 times speedup
`printA()`/`printB()` assume the arrays have at least 100 elements and will access out of bounds for smaller `n`.(Already fixed)

## Reflection and Self Assessment

- Concurrency strategy: We spawn two threads per recursion level until `level == cutoff`. This offers good parallelism but can create many short-lived threads; the cutoff must balance overhead vs. parallel work.
- Performance tuning: We observed diminishing returns beyond certain depths (over 3) due to overhead and memory bandwidth limits. Choosing a cutoff near log2(cores) often works well, but the optimal choice depends on CPU, cache, and input size.
- Learning: Practiced pthread creation/join patterns, avoiding data races by partitioning disjoint subarrays and merging after joins. Reviewed the cost of copying during merge and how it interacts with caches under parallelism.
- Diffcult: Understanding the recursive structure proved to be the most challenging aspect of the project(`void * parallel_mergesort `). The multithreaded implementation required each thread to independently execute its own instance of the sorting routine, which led to a non-intuitive execution flow—particularly in the sequence where my_mergesort is invoked before the final merging step. Designing and implementing the correct execution order within my_mergesort took considerable effort and careful reasoning.
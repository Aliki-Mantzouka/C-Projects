# **Project 1: Unix Shell Implementation (mysh)**

**Overview**

This project involves the design and implementation of a custom Unix shell, developed in four progressive stages. The goal was to understand process management, system calls, and inter-process communication (IPC) in a Linux environment.

**Technical Features**

- Process Management:

  Utilizes fork(), execvp(), and wait()/waitpid() for creating and managing child processes.

- **Modular Evolution:**

- mysh1: Implementation of basic command execution.

- mysh2: Addition of built-in commands like cd (directory navigation) and exit/terminate.

- mysh3: Standard Input/Output redirection using dup2() and file descriptors (<, >).

- mysh4: Support for background execution using the ampersand (&) operator.

**Pipeline Support:**

Implementation of command pipelines (|) using pipe() to connect the stdout of one process to the stdin of another.

**Robustness:**

Includes whitespace trimming (trim_whitespace) and signal handling for clean process termination.



# **Project 2: Multithreaded Merge Sort (ThreadSort)**

**Overview**

A high-performance implementation of the Merge Sort algorithm using the POSIX Threads (pthreads) library. The project demonstrates the "Divide and Conquer" strategy applied in a concurrent programming environment to optimize sorting large datasets.

**Technical Features**

- Concurrency Model: Uses a multi-threaded approach where the array is recursively split into sub-segments, each handled by a dedicated thread.

- **Synchronization:**

- Implementation of Mutexes (pthread_mutex_t) to protect shared resources and global counters.

- Usage of Condition Variables (pthread_cond_t) to coordinate thread execution and ensure proper merging sequence.

**Performance Analysis:**

- Includes benchmarking against sequential execution to measure speedup.

- Extensive testing using Sanitizers (Address, Thread, and Undefined Behavior Sanitizers) to ensure memory safety and the absence of data races.

**System Optimization:**

Evaluation of execution times under different system configurations and memory constraints.

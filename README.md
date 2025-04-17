Simple C program to simulate the behaviour of a dynamic memory allocator. Using I/O from the terminal, this program can create and allocate blocks of memory to a given process, deallocate them, compact holes, and print the current status of the memory region. Capable of using first, best, and worst fit allgorithms when choosing which hole to allocate.

Commands:
- RQ <Process> <Size> <Strategy>: Allocates a block to a process with identifier string <Process> of integer size <Size> with algorithm <Strategy>; F for first, B for best, W for worst.
- RL <Process>: Deallocates the first process found with identifier <Process>.
- C: Compacts all holes into a single trailing hole.
- STAT: Prints the current state of the memory region, including all allocated blocks and their associated processes.

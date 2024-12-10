### PADP Programs

- [ ] 1. a) Write an OpenMP program that computes the value of PI using Monto-Carlo Algorithm
         b) Write an MPI program that computes the value of PI using Monto-Carlo Algorithm
         ### 1st Program (Monte Carlo)
        ```
        1a) g++ -fopenmp monte_carlo_openmp.cpp
            ./a.out
            g++ for CPP files, gcc for C files
        1b) mpic++ monte_carlo_mpi.cpp
            mpirun -np 4 a.out
        ```
<!-- - [ ] 2. Write an OpenMP program that computes a simple matrix-matrix multiplication using dynamic memory allocation.
- [ ] 3. Write an OpenMP program for Cache unfriendly sieve of Erathothneses.
- [ ] 4. Write an OpenMP program to convert a color image to black and white image.
- [ ] 5. Write a MPI Program that has  a total of 4 processes. Process with rank 1, 2, and 3 should send the following messages respectives to teh process with rank 0: HELLO, CSE, RVCE.
- [ ] 6. Write an OpenMP program for Word search in a file and illustrate the perfomanace using different sizes of file. -->
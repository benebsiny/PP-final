CXX=g++
MPICXX=mpicxx
FLAGS=-O3 -Wall

all: gen_testcase quicksort quicksort_std_thread quicksort_omp quicksort_mpi quicksort_mpi2

gen_testcase: gen_testcase.cpp
	$(CXX) $(FLAGS) $< -o $@

quicksort: quicksort.cpp
	$(CXX) $(FLAGS) $< -o $@

quicksort_std_thread: quicksort_std_thread.cpp
	$(CXX) $(FLAGS) -pthread $< -o $@

quicksort_omp: quicksort_omp.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@

quicksort_mpi: quicksort_mpi.cc
	$(MPICXX) $(FLAGS) $< -o $@

quicksort_mpi_run:
	mpirun -np 4 --hostfile hosts quicksort_mpi random_numbers_100000000.bin

quicksort_mpi2: quicksort_mpi2.cc
	$(MPICXX) $(FLAGS) $< -o $@

quicksort_mpi2_run:
	mpirun -np 4 --hostfile hosts quicksort_mpi2 random_numbers_100000000.bin

clean:
	rm -f gen_testcase quicksort quicksort_std_thread quicksort_omp quicksort_mpi
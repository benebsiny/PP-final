CXX=g++
FLAGS=-O3 -Wall

all: gen_testcase quicksort quicksort_std_thread quicksort_omp quicksort_mpi

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
	mpirun -np 4 --hostfile hosts quicksort_mpi

clean:
	rm -f gen_testcase quicksort quicksort_std_thread quicksort_omp quicksort_mpi
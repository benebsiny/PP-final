CXX=g++
MPICXX=mpicxx
FLAGS=-O3 -Wall -Wextra -Werror -std=c++11

HELPER_FOLDER=./helpers

COMMONS_FOLDER=./commons
COMMONS=$(COMMONS_FOLDER)/helper.cpp

NVCC=nvcc
CUDA_FLAGS =-rdc=true -gencode=arch=compute_61,code=sm_61 -Xcompiler '-fPIC' -g -O3

all: gen_testcase quicksort quicksort_std_thread quicksort_std_thread_m quicksort_omp quicksort_omp_m quicksort_mpi quicksort_cuda

gen_testcase:
	cd $(HELPER_FOLDER) && make

quicksort: quicksort.cpp
	$(CXX) $(FLAGS) $< -o $@ $(COMMONS)

quicksort_std_thread: quicksort_std_thread.cpp
	$(CXX) $(FLAGS) -pthread $< -o $@ $(COMMONS)

quicksort_std_thread_m: quicksort_std_thread_m.cpp
	$(CXX) $(FLAGS) -pthread $< -o $@ $(COMMONS)

quicksort_omp: quicksort_omp.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@ $(COMMONS)

quicksort_omp_m: quicksort_omp_m.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@ $(COMMONS)

quicksort_mpi: quicksort_mpi.cc
	$(MPICXX) $(FLAGS) $< -o $@ $(COMMONS)

quicksort_mpi_run:
	mpirun -np 4 --hostfile hosts quicksort_mpi random_numbers_100000000.bin

quicksort_mpi_m: quicksort_mpi_m.cc
	$(MPICXX) $(FLAGS) $< -o $@ $(COMMONS)

quicksort_mpi_m_run:
	mpirun -np 4 --hostfile hosts quicksort_mpi_m random_numbers_100000000.bin

quicksort_cuda: quicksort_cuda.cu
	$(NVCC) $(CUDA_FLAGS) $< -o $@

clean:
	cd $(HELPER_FOLDER) && make clean
	rm -f gen_testcase quicksort quicksort_std_thread quicksort_std_thread_m quicksort_omp quicksort_omp_m quicksort_mpi quicksort_cuda

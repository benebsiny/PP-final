CXX=g++
FLAGS=-O3

all: gen_testcase quicksort quicksort_std_thread quicksort_omp

gen_testcase: gen_testcase.cpp
	$(CXX) $(FLAGS) $< -o $@

quicksort: quicksort.cpp
	$(CXX) $(FLAGS) $< -o $@

quicksort_std_thread: quicksort_std_thread.cpp
	$(CXX) $(FLAGS) -pthread $< -o $@

quicksort_omp: quicksort_omp.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@

clean:
	rm -f gen_testcase quicksort quicksort_std_thread
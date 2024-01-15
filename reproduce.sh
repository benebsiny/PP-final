CYAN='\033[1;36m'
NC='\033[0m'

make
./helpers/gen_testcase 10000000

# Serial
echo -e "${CYAN}Basic quicksort${NC}"
./quicksort random_numbers_10000000.bin
echo ----

# std::thread
echo -e "${CYAN}std::thread with 4 threads${NC}"
./quicksort_std_thread random_numbers_10000000.bin 4
echo ----
echo -e "${CYAN}std::thread with 4 threads and a partition threshold of 1/10${NC}"
./quicksort_std_thread random_numbers_10000000.bin 4 10
echo ----
echo -e "${CYAN}std::thread with initial data partitioning into 4 segments${NC}"
./quicksort_std_thread_m random_numbers_10000000.bin 4
echo ----

# OpenMP
echo -e "${CYAN}OpenMP with 4 threads${NC}"
./quicksort_omp random_numbers_10000000.bin 4
echo ----
echo -e "${CYAN}OpenMP with 4 threads and a partition threshold of 1/10${NC}"
./quicksort_omp random_numbers_10000000.bin 4 10
echo ----
echo -e "${CYAN}OpenMP with initial data partitioning into 4 segments${NC}"
./quicksort_omp_m random_numbers_10000000.bin 4

# MPI
echo -e "${CYAN}MPI Basic${NC}"
mpirun -np 4 --hostfile hosts quicksort_mpi random_numbers_10000000.bin
echo ----
echo -e "${CYAN}MPI with initial data partitioning into 4 segments${NC}"
mpirun -np 4 --hostfile hosts quicksort_mpi_m random_numbers_10000000.bin
echo ----

# CUDA
echo -e "${CYAN}CUDA${NC}"
./quicksort_cuda random_numbers_10000000.bin

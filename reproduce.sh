YELLOW='\033[1;33m'
CYAN='\033[1;36m'
NC='\033[0m'

make
./helpers/gen_testcase 10000000

# sequential
echo -e "${CYAN}Basic quicksort${NC}"
./quicksort random_numbers_10000000.bin
echo -e "${YELLOW}-------${NC}"

# std::thread
echo -e "${CYAN}std::thread with 2 threads${NC}"
./quicksort_std_thread random_numbers_10000000.bin 2
echo -e "${CYAN}std::thread with 4 threads${NC}"
./quicksort_std_thread random_numbers_10000000.bin 4
echo -e "${CYAN}std::thread with 8 threads${NC}"
./quicksort_std_thread random_numbers_10000000.bin 8
echo -e "${CYAN}std::thread with 16 threads${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16
echo -e "${YELLOW}-------${NC}"

echo -e "${CYAN}std::thread with 16 threads and a partition threshold of 1/5${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16 5
echo -e "${CYAN}std::thread with 16 threads and a partition threshold of 1/10${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16 10
echo -e "${CYAN}std::thread with 16 threads and a partition threshold of 1/20${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16 20
echo -e "${CYAN}std::thread with 16 threads and a partition threshold of 1/30${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16 30
echo -e "${CYAN}std::thread with 16 threads and a partition threshold of 1/50${NC}"
./quicksort_std_thread random_numbers_10000000.bin 16 50
echo -e "${YELLOW}-------${NC}"

echo -e "${CYAN}std::thread with initial data partitioning into 2 segments${NC}"
./quicksort_std_thread_m random_numbers_10000000.bin 2
echo -e "${CYAN}std::thread with initial data partitioning into 4 segments${NC}"
./quicksort_std_thread_m random_numbers_10000000.bin 4
echo -e "${CYAN}std::thread with initial data partitioning into 8 segments${NC}"
./quicksort_std_thread_m random_numbers_10000000.bin 8
echo -e "${CYAN}std::thread with initial data partitioning into 16 segments${NC}"
./quicksort_std_thread_m random_numbers_10000000.bin 16
echo -e "${YELLOW}-------${NC}"


# OpenMP
echo -e "${CYAN}OpenMP with dynamic threads${NC}"
./quicksort_omp random_numbers_10000000.bin d 0
echo -e "${CYAN}OpenMP with 2 threads${NC}"
./quicksort_omp random_numbers_10000000.bin 2
echo -e "${CYAN}OpenMP with 4 threads${NC}"
./quicksort_omp random_numbers_10000000.bin 4
echo -e "${CYAN}OpenMP with 8 threads${NC}"
./quicksort_omp random_numbers_10000000.bin 8
echo -e "${CYAN}OpenMP with 16 threads${NC}"
./quicksort_omp random_numbers_10000000.bin 16
echo -e "${YELLOW}-------${NC}"

echo -e "${CYAN}OpenMP with dynamic threads and a partition threshold of 1/5${NC}"
./quicksort_omp random_numbers_10000000.bin 0 5
echo -e "${CYAN}OpenMP with dynamic threads and a partition threshold of 1/10${NC}"
./quicksort_omp random_numbers_10000000.bin 0 10
echo -e "${CYAN}OpenMP with dynamic threads and a partition threshold of 1/20${NC}"
./quicksort_omp random_numbers_10000000.bin 0 20
echo -e "${CYAN}OpenMP with dynamic threads and a partition threshold of 1/30${NC}"
./quicksort_omp random_numbers_10000000.bin 0 30
echo -e "${CYAN}OpenMP with dynamic threads and a partition threshold of 1/50${NC}"
./quicksort_omp random_numbers_10000000.bin 0 50
echo -e "${YELLOW}-------${NC}"

echo -e "${CYAN}OpenMP with initial data partitioning into 2 segments${NC}"
./quicksort_omp_m random_numbers_10000000.bin 2
echo -e "${CYAN}OpenMP with initial data partitioning into 4 segments${NC}"
./quicksort_omp_m random_numbers_10000000.bin 4
echo -e "${CYAN}OpenMP with initial data partitioning into 8 segments${NC}"
./quicksort_omp_m random_numbers_10000000.bin 8
echo -e "${CYAN}OpenMP with initial data partitioning into 16 segments${NC}"
./quicksort_omp_m random_numbers_10000000.bin 16
echo -e "${YELLOW}-------${NC}"

# MPI
echo -e "${CYAN}MPI with 2 processes${NC}"
mpirun -np 2 --hostfile hosts quicksort_mpi random_numbers_10000000.bin
echo -e "${CYAN}MPI with 4 processes${NC}"
mpirun -np 4 --hostfile hosts quicksort_mpi random_numbers_10000000.bin
echo -e "${CYAN}MPI with 8 processes${NC}"
mpirun -np 8 --hostfile hosts quicksort_mpi random_numbers_10000000.bin
echo -e "${CYAN}MPI with 16 processes${NC}"
mpirun -np 16 --hostfile hosts quicksort_mpi random_numbers_10000000.bin
echo -e "${YELLOW}-------${NC}"

echo -e "${CYAN}MPI with initial data partitioning into 2 segments${NC}"
mpirun -np 2 --hostfile hosts quicksort_mpi_m random_numbers_10000000.bin
echo -e "${CYAN}MPI with initial data partitioning into 4 segments${NC}"
mpirun -np 4 --hostfile hosts quicksort_mpi_m random_numbers_10000000.bin
echo -e "${CYAN}MPI with initial data partitioning into 8 segments${NC}"
mpirun -np 8 --hostfile hosts quicksort_mpi_m random_numbers_10000000.bin
echo -e "${CYAN}MPI with initial data partitioning into 16 segments${NC}"
mpirun -np 16 --hostfile hosts quicksort_mpi_m random_numbers_10000000.bin
echo -e "${YELLOW}-------${NC}"

# CUDA
# echo -e "${CYAN}CUDA${NC}"
# ./quicksort_cuda random_numbers_10000000.bin

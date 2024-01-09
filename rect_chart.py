import subprocess
import matplotlib.pyplot as plt
import numpy as np

programs = [
    ["./quicksort", "random_numbers_10000000.bin"],
    ["./quicksort_omp", "random_numbers_10000000.bin"],
    ["./quicksort_omp", "random_numbers_10000000.bin", "2"],
    ["./quicksort_omp", "random_numbers_10000000.bin", "4"],
    ["./quicksort_omp", "random_numbers_10000000.bin", "8"],
    ["./quicksort_omp", "random_numbers_10000000.bin", "16"],
    ["./quicksort_std_thread", "random_numbers_10000000.bin", "2"],
    ["./quicksort_std_thread", "random_numbers_10000000.bin", "4"],
    ["./quicksort_std_thread", "random_numbers_10000000.bin", "8"],
    ["./quicksort_std_thread", "random_numbers_10000000.bin", "16"],
    ["mpirun", "-np", "2", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "4", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "8", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "16", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "2", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "4", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "8", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_10000000.bin"],
    ["mpirun", "-np", "16", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_10000000.bin"],
]

labels = [
    "SEQ",
    "OMPd",
    "OMP2",
    "OMP4",
    "OMP8",
    "OMP16",
    "THD2",
    "THD4",
    "THD8",
    "THD16",
    "MPI2",
    "MPI4",
    "MPI8",
    "MPI16",
    "MPI*2",
    "MPI*4",
    "MPI*8",
    "MPI*16",
]

assert(len(programs) == len(labels))

rounds = 5

times = {label: [] for label in labels}

for program, label in zip(programs, labels):
    print("[*] Running " + label)
    for _ in range(rounds):
        print("[*] Round " + str(_ + 1))
        result = subprocess.run(program, capture_output=True, text=True)
        time_line = result.stdout.splitlines()[-1]
        try:
            time = float(time_line.split()[-2])
        except:
            print("[!] Error: " + time_line)
            exit(1)
        times[label].append(time)

time_means = [np.mean(times[label]) for label in labels]
time_std = [np.std(times[label]) for label in labels]

x = np.arange(len(labels))
width = 0.6

fig, ax = plt.subplots()
rects1 = ax.bar(x, time_means, width, label="Mean", yerr=time_std)

ax.set_ylabel("Execution Time (ms)")
ax.set_title("Execution times by program and round")
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation='vertical')
ax.legend()

# Calculate speedup times
seq_time = time_means[0]
speedup = [seq_time / time for time in time_means]

# Plot speedup times
ax2 = ax.twinx()
ax2.set_ylabel('Speedup')
ax2.plot(x, speedup, color='red', marker='o')
ax2.tick_params(axis='y', labelcolor='red')

fig.tight_layout()

plt.savefig("rect_chart.png")

import subprocess
import matplotlib.pyplot as plt
import numpy as np

programs = [
    ["./quicksort", "random_numbers_{NUM}.bin"],
    ["./quicksort_omp", "random_numbers_{NUM}.bin"],
    ["./quicksort_omp", "random_numbers_{NUM}.bin", "2"],
    ["./quicksort_omp", "random_numbers_{NUM}.bin", "4"],
    ["./quicksort_omp", "random_numbers_{NUM}.bin", "8"],
    ["./quicksort_omp", "random_numbers_{NUM}.bin", "16"],
    ["./quicksort_std_thread", "random_numbers_{NUM}.bin", "2"],
    ["./quicksort_std_thread", "random_numbers_{NUM}.bin", "4"],
    ["./quicksort_std_thread", "random_numbers_{NUM}.bin", "8"],
    ["./quicksort_std_thread", "random_numbers_{NUM}.bin", "16"],
    ["mpirun", "-np", "2", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "4", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "8", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "16", "--hostfile", "hosts", "quicksort_mpi", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "2", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "4", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "8", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_{NUM}.bin"],
    ["mpirun", "-np", "16", "--hostfile", "hosts", "quicksort_mpi2", "random_numbers_{NUM}.bin"],
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

number_of_testcases = [
    100000,
    1000000,
    10000000,
    100000000,
]

label_of_testcases = [
    "100K",
    "1M",
    "10M",
    "100M",
]

assert(len(programs) == len(labels))
assert(len(number_of_testcases) == len(label_of_testcases))

rounds = 5

times = {label: [] for label in labels}

for program, label in zip(programs, labels):
    print("[*] Running " + label)
    for number_of_testcase, label_of_testcase in zip(number_of_testcases, label_of_testcases):
        print("[*] Running " + label_of_testcase)
        sum = 0
        for _ in range(rounds):
            print("[*] Round " + str(_ + 1))
            current_program = [x.replace("{NUM}", str(number_of_testcase)) for x in program]
            result = subprocess.run(current_program, capture_output=True, text=True)
            time_line = result.stdout.splitlines()[-1]
            try:
                time = float(time_line.split()[-2])
            except:
                print("[!] Error: " + time_line)
                exit(1)
            sum += time
        times[label].append(sum / rounds)

fig, ax = plt.subplots()

# Plotting each line
for label, index in zip(labels, range(len(labels))):
    ax.plot(label_of_testcases, times[label], label=label)

ax.set_xlabel("Number of Testcases")
ax.set_ylabel("Execution Time (ms)")
ax.set_title("Execution times by program and round")
ax.legend()

fig.tight_layout()

plt.savefig("line_chart.png")
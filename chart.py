import subprocess
import matplotlib.pyplot as plt
import numpy as np

programs = [
    ["./quicksort"],
    ["./quicksort_omp"],
    ["./quicksort_std_thread"],
    ["mpirun", "-np", "2", "--hostfile", "hosts", "quicksort_mpi"],
    ["mpirun", "-np", "4", "--hostfile", "hosts", "quicksort_mpi"],
    ["mpirun", "-np", "6", "--hostfile", "hosts", "quicksort_mpi"],
    ["mpirun", "-np", "8", "--hostfile", "hosts", "quicksort_mpi"],
]

labels = [
    "Sequential",
    "OpenMP",
    "std::thread",
    "MPI NP=2",
    "MPI NP=4",
    "MPI NP=6",
    "MPI NP=8",
]

rounds = 10

times = {label: [] for label in labels}

for program, label in zip(programs, labels):
    print("[*] Running " + label)
    for _ in range(rounds):
        print("[*] Round " + str(_ + 1))
        result = subprocess.run(program, capture_output=True, text=True)
        time_line = result.stdout.splitlines()[-1]
        time = float(time_line.split()[-2])
        times[label].append(time)

time_means = [np.mean(times[label]) for label in labels]
time_std = [np.std(times[label]) for label in labels]

x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots()
rects1 = ax.bar(x, time_means, width, label="Mean", yerr=time_std)

ax.set_ylabel("Execution Time (ms)")
ax.set_title("Execution times by program and round")
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()

fig.tight_layout()

plt.savefig("chart.png")

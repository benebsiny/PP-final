import subprocess
import matplotlib.pyplot as plt
import numpy as np

programs = ["quicksort", "quicksort_omp", "quicksort_std_thread", "quicksort_mpi"]
labels = ["Sequential", "OpenMP", "std::thread", "MPI"]

rounds = 10

times = {program: [] for program in programs}

for program in programs:
    print("[*] Running " + program)
    for _ in range(rounds):
        print("[*] Round " + str(_ + 1))
        result = subprocess.run(["./" + program], capture_output=True, text=True)
        time_line = result.stdout.splitlines()[-1]
        time = float(time_line.split()[-2])
        times[program].append(time)

time_means = [np.mean(times[program]) for program in programs]
time_std = [np.std(times[program]) for program in programs]

x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots()
rects1 = ax.bar(x, time_means, width, label='Mean', yerr=time_std)

ax.set_ylabel('Execution Time (ms)')
ax.set_title('Execution times by program and round')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()

fig.tight_layout()

plt.savefig("chart.png")
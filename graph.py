import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Given data from the user's table.
data = {
    "Number of Points": [100000, 200000, 300000, 400000, 600000, 800000, 1000000],
    "Speedup (1 Thread)": [0.78, 0.92, 1.24, 1.27, 1.11, 0.82, 0.91],
    "Speedup (5 Threads)": [4.51, 2.37, 4.37, 6.55, 5.64, 4.86, 4.71],
    "Speedup (11 Threads)": [5.81, 2.66, 5.44, 6.74, 7.27, 3.12, 4.79],
    "Speedup (22 Threads)": [6.15, 6.13, 5.81, 4.59, 4.95, 6.64, 6.50]
}

df = pd.DataFrame(data)

# Prepare the data in a suitable format for plotting
threads = ['1 Thread', '5 Threads', '11 Threads', '22 Threads']
points = df["Number of Points"].apply(lambda x: f"{x:,}")

# Data for plotting
plot_data = [df["Speedup (1 Thread)"].values,
             df["Speedup (5 Threads)"].values,
             df["Speedup (11 Threads)"].values,
             df["Speedup (22 Threads)"].values]

# Number of bar groups
num_groups = len(plot_data)

# Set up the matplotlib figure and axes
fig, ax = plt.subplots(figsize=(12, 8))

# Set the positions and width for the bars
positions = np.arange(len(points))
width = 0.2

# Plotting the bars for each thread count
for i, thread in enumerate(threads):
    ax.bar(positions - width*(num_groups/2 - i), plot_data[i], width=width, label=thread)

# Adding labels and title
ax.set_xticks(positions)
ax.set_xticklabels(points)
ax.set_xlabel('Number of Points')
ax.set_ylabel('Speedup')
ax.set_title('Speedup by Number of Points and Threads Used')
ax.legend(title="Threads")

# Adding the grid
ax.yaxis.grid(True)

# Show the plot
plt.show()

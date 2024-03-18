import matplotlib.pyplot as plt
import pandas as pd

# Data from the table of results
data = {
    "No. of points": [100000, 200000, 300000, 400000, 600000, 800000, 1000000],
    "Speed up (1 threads/n threads)": [16.21, 8.36, 9, 11.65, 11.93, 8.29, 9.24],
    "Previous speed up w/best time": [6.15, 6.13, 5.81, 6.74, 7.27, 6.64, 6.50]
}

df = pd.DataFrame(data)

# Plotting
plt.figure(figsize=(10, 6))

# Plot for speed up using optimal number of threads
plt.plot(df["No. of points"], df["Speed up (1 threads/n threads)"], label='Speed Up (Optimal Threads)', marker='o', linestyle='-')

# Plot for previous speed up with best time
plt.plot(df["No. of points"], df["Previous speed up w/best time"], label='Previous Speed Up (Best Time)', marker='x', linestyle='--')

plt.title('Speed Up Comparison')
plt.xlabel('Number of Points')
plt.ylabel('Speed Up')
plt.legend()
plt.grid(True)
plt.xticks(df["No. of points"], labels=[f"{p//1000}K" for p in df["No. of points"]])
plt.tight_layout()

plt.show()

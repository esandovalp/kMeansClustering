import pandas as pd
import matplotlib.pyplot as plt

# Path to the CSV file
csv_file = "1000000_clusters.csv"

# Reading the CSV file
points_df = pd.read_csv(csv_file, header=None, names=["x", "y", "cluster"])

# Number of clusters - adjust this if you used a different number in your C++ implementation
clusters = points_df["cluster"].max() + 1  # Assuming cluster labels start from 0

# Plotting clusters
colors = ["red", "blue", "orange", "black", "purple", "pink", "brown", "green", "yellow", "cyan"]
# Make sure you have enough colors for your clusters

for cluster in range(clusters):
    cluster_points = points_df[points_df["cluster"] == cluster]
    plt.scatter(cluster_points["x"], cluster_points["y"], marker="o", s=1, color=colors[cluster % len(colors)], alpha=0.5)

plt.title("Clusters Visualized", fontsize=20)
plt.xlabel("X Coordinate")
plt.ylabel("Y Coordinate")
plt.show()

from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def create_dataset_and_save_csv(n_points, centers=10, cluster_std=0.04, random_state=7, center_box=(0, 1.0), n_clusters=5):
    # randomly generating data points and noise
    points, _ = make_blobs(n_samples=n_points,
                           centers=centers,
                           cluster_std=cluster_std,
                           random_state=random_state,
                           center_box=center_box)

    # only positive points and with three decimals
    points = np.round(np.abs(points), 3)

    # storing points into a csv file
    np.savetxt(f"{n_points}_data.csv", points, delimiter=",", fmt="%.3f")

    # creating a dataframe
    points_df = pd.DataFrame()
    points_df["x"] = points[:, 0]
    points_df["y"] = points[:, 1]

    # clustering with kmeans
    kmeans = KMeans(n_clusters=n_clusters).fit(points)

    points_df["cluster"] = kmeans.labels_

    # Plotting clusters - optional, can be commented out to speed up the process
'''
    colors = ["red", "blue", "orange", "black", "purple", "pink", "brown"]
    for cluster in range(n_clusters):
        plt.scatter(points_df[points_df["cluster"] == cluster]["x"],
                    points_df[points_df["cluster"] == cluster]["y"],
                    marker="o", s=180, color=colors[cluster], alpha=0.05)

        plt.scatter(kmeans.cluster_centers_[cluster][0],
                    kmeans.cluster_centers_[cluster][1],
                    marker="P", s=280, color=colors[cluster])

    plt.title(f"Clusters with K-Means for {n_points} Points", fontsize=20)
    plt.xlim(-0.1, 1.1)
    plt.ylim(-0.1, 1.1)
    plt.show()
'''

# Define the different sizes of datasets to generate
point_sizes = [100000, 200000, 300000, 400000, 600000, 800000, 1000000]

for size in point_sizes:
    create_dataset_and_save_csv(n_points=size)

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <omp.h>
#include <sstream>
#include <string>
#include <vector>

struct Result {
  int threads;
  int points;
  double time;
};

struct Point {
  double x, y; // Assuming 2D points for simplicity
};

std::vector<Point> readCSV(const std::string &filename) {
  std::vector<Point> points;
  std::ifstream file(filename);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return points; // Return empty vector if file can't be opened
  }

  while (getline(file, line)) {
    std::stringstream linestream(line);
    std::string value;
    Point point;
    getline(linestream, value, ',');
    point.x = std::stod(value);
    getline(linestream, value, ',');
    point.y = std::stod(value);
    points.push_back(point);
  }
  return points;
}

double distance(const Point &p1, const Point &p2) {
  return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

void kMeansClustering(std::vector<Point> &points, int k,
                      std::vector<int> &labels) {
  std::vector<Point> centroids(k);
  int num_threads = omp_get_max_threads();
  std::vector<unsigned int> seeds(num_threads, 0);

// Initialize seeds for each thread
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    seeds[tid] = static_cast<unsigned int>(time(NULL)) + tid;
  }

// Initialize centroids
#pragma omp parallel for
  for (int i = 0; i < k; ++i) {
    int tid = omp_get_thread_num();
    centroids[i] = points[rand_r(&seeds[tid]) % points.size()];
  }

  bool changed;
  do {
    changed = false;
    std::vector<bool> local_changed(num_threads, false);

    // Reset counts for each iteration
    std::vector<int> counts(k, 0);

// Assign points to the nearest centroid
#pragma omp parallel for shared(points, centroids)
    for (size_t i = 0; i < points.size(); ++i) {
      double minDist = std::numeric_limits<double>::max();
      int label = -1;
      for (int j = 0; j < k; ++j) {
        double dist = distance(points[i], centroids[j]);
        if (dist < minDist) {
          minDist = dist;
          label = j;
        }
      }

      int tid = omp_get_thread_num();
      if (labels[i] != label) {
        labels[i] = label;
        local_changed[tid] = true;
      }
    }

    // Reduce the local changed flags
    for (bool lc : local_changed) {
      changed = changed || lc;
    }

    std::vector<std::vector<Point>> sumCentroids(
        num_threads, std::vector<Point>(k, {0.0, 0.0}));
    std::vector<std::vector<int>> sumCounts(num_threads,
                                            std::vector<int>(k, 0));

// Update centroids
#pragma omp parallel for
    for (size_t i = 0; i < points.size(); ++i) {
      int tid = omp_get_thread_num();
      int label = labels[i];
      sumCentroids[tid][label].x += points[i].x;
      sumCentroids[tid][label].y += points[i].y;
      sumCounts[tid][label]++;
    }

    // Combine results from each thread
    for (int t = 0; t < num_threads; ++t) {
      for (int j = 0; j < k; ++j) {
        centroids[j].x += sumCentroids[t][j].x;
        centroids[j].y += sumCentroids[t][j].y;
        counts[j] += sumCounts[t][j];
      }
    }

    // Recalculate centroid positions
    for (int i = 0; i < k; ++i) {
      if (counts[i] > 0) {
        centroids[i].x /= counts[i];
        centroids[i].y /= counts[i];
      }
    }
  } while (changed);
}

void writeCSV(const std::string &filename, const std::vector<Point> &points,
              const std::vector<int> &labels) {
  std::ofstream file(filename);
  for (size_t i = 0; i < points.size(); ++i) {
    file << points[i].x << "," << points[i].y << "," << labels[i] << "\n";
  }
}

int main() {
  std::srand(std::time(0));        // Initialize random seed once
  int K = 5;                       // Number of clusters
  std::vector<Result> all_results; // Vector to store all results

  // Define filenames of the datasets to test
  std::vector<std::string> filenames = {"100000_data.csv", "200000_data.csv",
                                        "300000_data.csv", "400000_data.csv",
                                        "600000_data.csv", "800000_data.csv",
                                        "1000000_data.csv"};

  // Define thread counts to test
  std::vector<int> threadCounts = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
                                   12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};

  for (const auto &filename : filenames) {
    std::vector<Point> points = readCSV(filename);
    if (points.empty()) {
      std::cerr << "Error: No points read from file " << filename << ".\n";
      continue; // Skip to the next file if the current one can't be opened or
                // is empty
    }

    std::vector<int> labels(points.size(), -1); // Initialize labels

    for (int threads : threadCounts) {
      omp_set_num_threads(threads);
      std::vector<double> times; // Store times for each number of threads

      for (int i = 0; i < 10; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        kMeansClustering(points, K, labels);
        auto stop = std::chrono::high_resolution_clock::now();
        times.push_back(
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)
                .count());
      }

      double averageTime =
          std::accumulate(times.begin(), times.end(), 0.0) / times.size();

      // Store the result
      all_results.push_back(
          {threads, static_cast<int>(points.size()), averageTime});

      // Output the result to the console
      std::cout << "Average time with " << threads << " threads for "
                << points.size() << " points: " << averageTime
                << " milliseconds.\n";
    }
  }

  // After all results are collected, write them to a CSV file
  std::ofstream result_file("kmeans_results.csv");
  result_file << "Threads,Points,Time (ms)\n";
  for (const auto &result : all_results) {
    result_file << result.threads << "," << result.points << "," << result.time
                << "\n";
  }
  result_file.close();

  return 0;
}

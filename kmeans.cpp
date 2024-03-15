#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

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
  // Simple random initialization of centroids
  for (int i = 0; i < k; ++i) {
    centroids[i] = points[rand() % points.size()];
  }

  bool changed;
  do {
    changed = false;
    // Assign points to the nearest centroid
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
      if (labels[i] != label) {
        labels[i] = label;
        changed = true;
      }
    }
    // Update centroids
    std::vector<Point> newCentroids(k);
    std::vector<int> counts(k, 0);
    for (size_t i = 0; i < points.size(); ++i) {
      newCentroids[labels[i]].x += points[i].x;
      newCentroids[labels[i]].y += points[i].y;
      counts[labels[i]]++;
    }
    for (int i = 0; i < k; ++i) {
      if (counts[i] > 0) {
        centroids[i].x = newCentroids[i].x / counts[i];
        centroids[i].y = newCentroids[i].y / counts[i];
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
  std::srand(std::time(0)); // Initialize random seed once

  int K = 5; // Number of clusters

  // Filename of the input CSV containing the points
  std::string filename = "100000_data.csv"; // Adjust this line if your
                                            // file has a different name

  std::vector<Point> points = readCSV(filename);
  if (points.empty()) {
    std::cerr << "Error: No points read from file.\n";
    return 1; // Exit if the file can't be opened or is empty
  }

  int numPoints = points.size(); // Determine the number of points dynamically
  std::string outputFilename = std::to_string(numPoints) + "_clusters.csv";
  std::vector<double> times; // Store times for each iteration

  std::vector<int> labels(points.size(), -1); // Initialize labels

  for (int i = 0; i < 10; ++i) {
    auto start = std::chrono::high_resolution_clock::now();

    kMeansClustering(points, K, labels);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    times.push_back(duration.count());

    // Optionally, write the output CSV only on the last iteration
    if (i == 9) {
      writeCSV(outputFilename, points, labels);
    }
  }

  double averageTime =
      std::accumulate(times.begin(), times.end(), 0.0) / times.size();
  std::cout << "Average time for " << numPoints << " points: " << averageTime
            << " milliseconds.\n";

  return 0;
}

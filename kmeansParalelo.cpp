#include <chrono>
#include <iostream>
#include <omp.h>
#include <thread>
#include <vector>

struct Point {
  double x, y;
  int cluster;
  Point(double x, double y) : x(x), y(y), cluster(-1) {}
};

double distance(const Point &a, const Point &b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

// Modify the kmeans function to support OpenMP here
void kmeans(std::vector<Point> &points, int K) {
  int n = points.size();
  std::vector<Point> centroids;

  // Initialize centroids randomly
  std::srand(std::time(0));
  for (int i = 0; i < K; ++i) {
    centroids.push_back(points[std::rand() % n]);
  }

  bool changed;
  do {
    changed = false;

    // Assign clusters
    for (auto &p : points) {
      double minDist = std::numeric_limits<double>::max();
      int assignedCluster = -1;
      for (int i = 0; i < K; ++i) {
        double dist = distance(p, centroids[i]);
        if (dist < minDist) {
          minDist = dist;
          assignedCluster = i;
        }
      }
      if (p.cluster != assignedCluster) {
        p.cluster = assignedCluster;
        changed = true;
      }
    }

    // Update centroids
    std::vector<int> counts(K, 0);
    std::vector<Point> newCentroids(K, Point(0, 0));
    for (const auto &p : points) {
      newCentroids[p.cluster].x += p.x;
      newCentroids[p.cluster].y += p.y;
      counts[p.cluster]++;
    }
    for (int i = 0; i < K; ++i) {
      centroids[i].x = newCentroids[i].x / counts[i];
      centroids[i].y = newCentroids[i].y / counts[i];
    }
  } while (changed);
}

std::vector<Point> loadPointsFromFile(const std::string &filename) {
  std::vector<Point> points;
  std::ifstream file(filename);
  std::string line;

  while (getline(file, line)) {
    std::stringstream linestream(line);
    std::string cell;
    double x, y;

    getline(linestream, cell, ',');
    x = std::stod(cell);

    getline(linestream, cell);
    y = std::stod(cell);

    points.emplace_back(x, y);
  }

  return points;
}

void savePointsToFile(const std::vector<Point> &points,
                      const std::string &filename) {
  std::ofstream file(filename);

  for (const auto &point : points) {
    file << point.x << "," << point.y << "," << point.cluster << std::endl;
  }
}

int main() {
  // Array of different numbers of points to be tested
  std::vector<int> pointsToTest = {100000, 200000, 300000, 400000,
                                   600000, 800000, 1000000};
  // Determine the number of virtual cores
  unsigned int cores = std::thread::hardware_concurrency();

  int K = 5; // Number of clusters

  // Arrays of different thread counts to be tested
  std::vector<int> threadsToTest = {1, cores / 2, cores, 2 * cores};

  // Iterate over each number of points to test
  for (int numPoints : pointsToTest) {
    // Generate or load the dataset for the current number of points
    std::vector<Point> points =
        loadPointsFromFile(std::to_string(numPoints) + "_data.csv");

    // Iterate over each thread count configuration
    for (int numThreads : threadsToTest) {
      omp_set_num_threads(numThreads); // Set the number of threads for OpenMP

      std::vector<double> times; // Store times for each iteration

      // Repeat the clustering ten times to get an average time
      for (int i = 0; i < 10; ++i) {
        // Reset cluster assignments if necessary

        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Perform k-means clustering with OpenMP parallelization
        kmeans(points, K);

        // Stop timing
        auto stop = std::chrono::high_resolution_clock::now();

        // Calculate duration and add to times
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        times.push_back(duration.count());
      }

      // Calculate the average time
      double averageTime =
          std::accumulate(times.begin(), times.end(), 0.0) / times.size();
      std::cout << "Average time for " << numPoints << " points with "
                << numThreads << " threads: " << averageTime
                << " milliseconds.\n";
    }
  }

  return 0;
}

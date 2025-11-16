//
// Created by Nick on 16.11.2025.
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>

class ArrayGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    ArrayGenerator() : gen(rd()) {}

    // Generate random array
    std::vector<int> generateRandom(int size, int min_val = 0, int max_val = 6000) {
        std::vector<int> arr(size);
        std::uniform_int_distribution<int> dist(min_val, max_val);

        for (int i = 0; i < size; ++i) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    // Generate reverse sorted array
    std::vector<int> generateReverseSorted(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = size - i;
        }
        return arr;
    }

    // Generate nearly sorted array
    std::vector<int> generateNearlySorted(int size, double swap_ratio = 0.01) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = i + 1;
        }

        int num_swaps = static_cast<int>(size * swap_ratio);
        std::uniform_int_distribution<int> dist(0, size - 1);

        for (int i = 0; i < num_swaps; ++i) {
            int idx1 = dist(gen);
            int idx2 = dist(gen);
            std::swap(arr[idx1], arr[idx2]);
        }

        return arr;
    }

    // Get subarray from a larger array
    std::vector<int> getSubarray(const std::vector<int>& source, int size) {
        return std::vector<int>(source.begin(), source.begin() + size);
    }
};

class SortTester {
private:
    // Standard Merge Sort implementation
    void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; ++i)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; ++j)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                ++i;
            } else {
                arr[k] = R[j];
                ++j;
            }
            ++k;
        }

        while (i < n1) {
            arr[k] = L[i];
            ++i;
            ++k;
        }

        while (j < n2) {
            arr[k] = R[j];
            ++j;
            ++k;
        }
    }

    void mergeSort(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    // Insertion Sort implementation
    void insertionSort(std::vector<int>& arr, int left, int right) {
        for (int i = left + 1; i <= right; ++i) {
            int key = arr[i];
            int j = i - 1;

            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
        }
    }

    // Hybrid Merge-Insertion Sort implementation
    void hybridSort(std::vector<int>& arr, int left, int right, int threshold) {
        if (left < right) {
            if (right - left + 1 <= threshold) {
                insertionSort(arr, left, right);
            } else {
                int mid = left + (right - left) / 2;
                hybridSort(arr, left, mid, threshold);
                hybridSort(arr, mid + 1, right, threshold);
                merge(arr, left, mid, right);
            }
        }
    }

public:
    // Test standard Merge Sort
    long long testMergeSort(std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(arr, 0, arr.size() - 1);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    // Test hybrid Merge-Insertion Sort
    long long testHybridSort(std::vector<int> arr, int threshold) {
        auto start = std::chrono::high_resolution_clock::now();
        hybridSort(arr, 0, arr.size() - 1, threshold);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    }

    // Run multiple tests and return median time
    long long runMultipleTests(const std::vector<int>& original_arr, bool use_hybrid, int threshold = 15, int num_tests = 5) {
        std::vector<long long> times;

        for (int i = 0; i < num_tests; ++i) {
            std::vector<int> arr = original_arr; // Copy for each test

            long long time_taken;
            if (use_hybrid) {
                time_taken = testHybridSort(arr, threshold);
            } else {
                time_taken = testMergeSort(arr);
            }
            times.push_back(time_taken);
        }

        // Return median time
        std::sort(times.begin(), times.end());
        return times[times.size() / 2];
    }
};

// Function to run complete analysis
void runAnalysis() {
    ArrayGenerator generator;
    SortTester tester;

    // Generate large arrays for each type
    std::vector<int> random_large = generator.generateRandom(100000);
    std::vector<int> reverse_large = generator.generateReverseSorted(100000);
    std::vector<int> nearly_large = generator.generateNearlySorted(100000);

    // Test different thresholds for hybrid sort
    std::vector<int> thresholds = {5, 10, 15, 20, 25, 30, 50};

    // Open files for output
    std::ofstream merge_file("merge_sort_results.csv");
    std::ofstream hybrid_file("hybrid_sort_results.csv");
    std::ofstream threshold_file("threshold_analysis.csv");

    // Write headers
    merge_file << "Size,Random,Reverse,NearlySorted\n";
    hybrid_file << "Size,Random,Reverse,NearlySorted\n";
    threshold_file << "Threshold,Random,Reverse,NearlySorted\n";

    // Test standard merge sort
    std::cout << "Testing Standard Merge Sort...\n";
    for (int size = 500; size <= 100000; size += 100) {
        std::vector<int> random_arr = generator.getSubarray(random_large, size);
        std::vector<int> reverse_arr = generator.getSubarray(reverse_large, size);
        std::vector<int> nearly_arr = generator.getSubarray(nearly_large, size);

        long long random_time = tester.runMultipleTests(random_arr, false);
        long long reverse_time = tester.runMultipleTests(reverse_arr, false);
        long long nearly_time = tester.runMultipleTests(nearly_arr, false);

        merge_file << size << "," << random_time << "," << reverse_time << "," << nearly_time << "\n";

        if (size % 10000 == 0) {
            std::cout << "Completed size: " << size << std::endl;
        }
    }

    // Test hybrid sort with different thresholds at fixed size
    std::cout << "\nTesting Threshold Analysis...\n";
    int test_size = 10000;
    std::vector<int> random_test = generator.getSubarray(random_large, test_size);
    std::vector<int> reverse_test = generator.getSubarray(reverse_large, test_size);
    std::vector<int> nearly_test = generator.getSubarray(nearly_large, test_size);

    for (int threshold : thresholds) {
        long long random_time = tester.runMultipleTests(random_test, true, threshold);
        long long reverse_time = tester.runMultipleTests(reverse_test, true, threshold);
        long long nearly_time = tester.runMultipleTests(nearly_test, true, threshold);

        threshold_file << threshold << "," << random_time << "," << reverse_time << "," << nearly_time << "\n";
        std::cout << "Completed threshold: " << threshold << std::endl;
    }

    // Test hybrid sort with optimal threshold (15)
    std::cout << "\nTesting Hybrid Sort (threshold=15)...\n";
    for (int size = 500; size <= 100000; size += 100) {
        std::vector<int> random_arr = generator.getSubarray(random_large, size);
        std::vector<int> reverse_arr = generator.getSubarray(reverse_large, size);
        std::vector<int> nearly_arr = generator.getSubarray(nearly_large, size);

        long long random_time = tester.runMultipleTests(random_arr, true, 15);
        long long reverse_time = tester.runMultipleTests(reverse_arr, true, 15);
        long long nearly_time = tester.runMultipleTests(nearly_arr, true, 15);

        hybrid_file << size << "," << random_time << "," << reverse_time << "," << nearly_time << "\n";

        if (size % 10000 == 0) {
            std::cout << "Completed size: " << size << std::endl;
        }
    }

    merge_file.close();
    hybrid_file.close();
    threshold_file.close();
    std::cout << "\nAnalysis complete! Results saved to CSV files.\n";
}

int main() {
    runAnalysis();
    return 0;
}

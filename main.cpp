#include <iostream>
#include <vector>

const int INSERTION_THRESHOLD = 15;

void insertion_sort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        const int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    const int n1 = mid - left + 1;
    const int n2 = right - mid;

    std::vector<int> left_arr(n1);
    std::vector<int> right_arr(n2);

    for (int i = 0; i < n1; ++i) {
        left_arr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        right_arr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            ++i;
        } else {
            arr[k] = right_arr[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        ++j;
        ++k;
    }
}

void hybrid_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        if (right - left + 1 <= INSERTION_THRESHOLD) {
            insertion_sort(arr, left, right);
        } else {
            const int mid = left + (right - left) / 2;
            hybrid_sort(arr, left, mid);
            hybrid_sort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    if (n > 0) {
        hybrid_sort(arr, 0, n - 1);
    }

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i];
        if (i < n - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}

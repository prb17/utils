#include "array.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace sort {
                template<typename T>
                void swap(structures::array<T>& arr, size_t i, size_t j) {
                    T tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }

                /**
                * best: O(n) time | O(1) space
                * avg: O(n^2) time | O(1) space
                * worst: O(n^2) time | O(1) space
                */
                void bubble(structures::array<T>& arr) {
                    int max = arr.size(); 
                    while (true) {
                        int swaps = 0;
                        for (int i=0; i<max; i++) {
                            if (i + 1 >= max) {
                                break;
                            }
                            if (arr[i] > arr[i+1]) {
                                swap(arr, i, i+1);
                                if (i + 2 >= max) {
                                    max = max-1;
                                } 
                            }
                        }
                        if (swaps == 0) {
                            break;
                        } 
                    } 
                }

                /**
                * best: O(n) time | O(1) space
                * avg: O(n^2) time | O(1) space
                * worst: O(n^2) time | O(1) space
                */
                void insertion(structures::array<T>& arr) {
                    int head = 0;
                    while(head+1 < arr.size()) {
                        for (int i=head+1; i>=0; i--) {
                            if (arr[i-1] > arr[i]) {
                                swap(arr, i-1, i);
                            }
                        }
                        head = head + 1;
                    }
                }
 
                /**
                * best: O(n^2) time | O(1) space
                * avg: O(n^2) time | O(1) space
                * worst: O(n^2) time | O(1) space
                */
                void selection(structures::array<T>& arr) {
                    int min_idx = 0;
                    int head = -1;
                    while (head+1 < arr.size()) {
                        for (int i=min_idx; i<arr.size(); i++) {
                            if (arr[min_idx] > arr[i]) {
                                min_idx = i;
                            }
                        }
                        swap(arr, head+1, min_idx);
                        head = head+1;
                        min_idx = head+1;
                    }
                }
            }
        }
    }
}

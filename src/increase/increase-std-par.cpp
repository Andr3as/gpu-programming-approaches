#include <algorithm>
#include <execution>

#include "increase-util.h"


inline void increase(double *data, size_t nx) {
    // version 1: std::transform
    std::transform(std::execution::par_unseq, data, data + nx, data,
               [=](auto data_item) {
                   return data_item + 1;
               });

    // // version 2: std::for_each with index reconstruction
    // std::for_each(std::execution::par_unseq, data, data + nx,
    //               [=](const auto& data_item) {
    //                   const size_t i0 = &data_item - data;
    //                   data[i0] += 1;
    //               });

    // // version 3: std::for_each with counting iterator
    // std::for_each(std::execution::par_unseq, thrust::make_counting_iterator<size_t>(0), thrust::make_counting_iterator<size_t>(nx),
    //               [=](const auto &i0) {
    //                   data[i0] += 1;
    //               });
}

int main(int argc, char *argv[]) {
    size_t nx, nItWarmUp, nIt;
    parseCLA_1d(argc, argv, nx, nItWarmUp, nIt);

    double *data;
    data = new double[nx];

    // init
    initIncrease(data, nx);

    // warm-up
    for (size_t i = 0; i < nItWarmUp; ++i) {
        increase(data, nx);
    }

    // measurement
    auto start = std::chrono::steady_clock::now();

    for (size_t i = 0; i < nIt; ++i) {
        increase(data, nx);
    }

    auto end = std::chrono::steady_clock::now();

    printStats(end - start, nIt, nx, sizeof(double) + sizeof(double), 1);

    // check solution
    checkSolutionIncrease(data, nx, nIt + nItWarmUp);

    delete[] data;

    return 0;
}

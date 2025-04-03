#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <chrono>
#include <thread>
#include "kaizen.h"

// ------------------------ VOLATILE HEAVY FUNCTION --------------------------
void test_volatile_operations(int iterations) {
    volatile int a = 1, b = 2, c = 3, d = 4, e = 5;
    for (int i = 0; i < iterations; ++i) {
        a += 1;
        b = a + 1;
        // c = b + 1;
        // d = c + 1;
        // e = d + 1;
        // b += 2;
        // c += 3;
        // d += 4;
        // a = b + (c%d)*e;
        // e += 5;
        // d = (a+b)%c;
        // a = (b+c)%d;
        // b = (c+d)%e;
        // c = (d+e)%a;
        // d = (e+a)%b;
        // e = (a+b)%c;
        // a = (b+c)%d;
        // b = (c+d)%e;    
        // a++;
        // b++; 
    }
}

// ------------------------ OPTIMIZED FUNCTION -------------------------------
void test_optimized_operations(int iterations) {
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    for (int i = 0; i < iterations; ++i) {
        a += 1; 
        b += 2;
        c += 3;
        d += 4;
        e += 5;
    }
}

// ------------------------ TIMING  TEMPLATE ----------------------------------
template <typename F>
std::vector<long long> time_function(F func, int iterations, int trials) {
    std::vector<long long> times;
    for (int i = 0; i < trials; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func(iterations);
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }
    return times;
}

// ------------------------ MAIN ---------------------------------------------
int main(int argc, char* argv[]) {
    zen::cmd_args args(argv, argc);

    if (!args.is_present("--iterations") || !args.is_present("--trials")) {
        std::cerr << "Usage: " << argv[0] << " --iterations <iterations> --trials <trials> [--verbose]" << std::endl;
        return 1;
    }

    int iterations = std::stoi(args.get_options("--iterations")[0]);
    int trials = std::stoi(args.get_options("--trials")[0]);
    bool verbose = args.is_present("--verbose");

    auto times_volatile = time_function(test_volatile_operations, iterations, trials);
    auto times_optimized = time_function(test_optimized_operations, iterations, trials);

    long long avg_vol = std::accumulate(times_volatile.begin(), times_volatile.end(), 0LL) / trials;
    long long avg_opt = std::accumulate(times_optimized.begin(), times_optimized.end(), 0LL) / trials;

    std::ofstream csv("results.csv");
    csv << "Trial,Volatile(ns),Optimized(ns)\n";
    for (int i = 0; i < trials; ++i) {
        csv << (i + 1) << "," << times_volatile[i] << "," << times_optimized[i] << "\n";
    }
    csv << "Average," << avg_vol << "," << avg_opt << "\n";
    csv.close();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);  

    ss << "| " << std::left << std::setw(35) << "Volatile operations avg (ns)"
    << " | " << std::right << std::setw(12) << avg_vol << " |\n";

    ss << "| " << std::left << std::setw(35) << "Optimized operations avg (ns)"
    << " | " << std::right << std::setw(12) << avg_opt << " |\n";

    ss << "| " << std::left << std::setw(35) << "Speedup (volatile / optimized)"
    << " | " << std::right << std::setw(12) << static_cast<double>(avg_vol) / avg_opt << " |\n";

    zen::print(ss.str());


    if (verbose) {
        std::cout << "\nTrial Results:\n";
        for (int i = 0; i < trials; ++i) {
            std::cout << "Trial " << i + 1 << ": Volatile = " << times_volatile[i]
                      << " ns, Optimized = " << times_optimized[i] << " ns\n";
        }
    }

    return 0;
}

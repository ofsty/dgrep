#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include "bmh.hpp"

#ifdef _WIN32
#include <windows.h>
size_t get_total_memory() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
        return static_cast<size_t>(status.ullTotalPhys);
    return 0;
}
#elif __linux__
#include <sstream>
size_t get_total_memory() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            std::istringstream iss(line);
            std::string key;
            size_t kb;
            std::string unit;
            iss >> key >> kb >> unit;
            return kb * 1024;
        }
    }
    return 0;
}
#else
size_t get_total_memory() {
    return 512 * 1024 * 1024; // fallback: 512MB
}
#endif

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [file] <pattern>\n";
        return 1;
    }

    std::string pattern;
    std::vector<char> buffer;

    if (argc == 2) {
        pattern = argv[1];
        if (pattern.empty()) return 1;

        char ch;
        while (std::cin.get(ch))
            buffer.push_back(ch);

        if (buffer.empty()) return 0;
    } else {
        std::string filename = argv[1];
        pattern = argv[2];
        if (pattern.empty()) return 1;

        std::ifstream file(filename, std::ios::binary);
        if (!file) return 1;

        file.seekg(0, std::ios::end);
        size_t fsize = file.tellg();
        file.seekg(0, std::ios::beg);
        if (fsize == 0) return 0;

        size_t total_mem = get_total_memory();
        size_t safe_buf_size = total_mem / 4;
        if (fsize > safe_buf_size)
            std::cout << "[!] Large file: using chunked memory for processing\n";

        buffer.resize(fsize);
        file.read(buffer.data(), fsize);
    }

    size_t size = buffer.size();
    unsigned int nthreads = std::thread::hardware_concurrency();
    if (nthreads == 0) nthreads = 4;

    size_t chunk = size / nthreads;
    std::vector<int> table;
    xx44(pattern, table);

    std::vector<std::thread> workers;
    std::vector<size_t> counts(nthreads, 0);
    std::vector<size_t> base_lines(nthreads, 1);

    for (unsigned int i = 1; i < nthreads; ++i) {
        size_t offset = i * chunk;
        size_t lines = 1;
        for (size_t j = 0; j < offset; ++j)
            if (buffer[j] == '\n') lines++;
        base_lines[i] = lines;
    }

    auto t0 = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < nthreads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == nthreads - 1) ? size : (i + 1) * chunk;

        if (i != 0)
            start = (start >= pattern.size()) ? start - pattern.size() : 0;

        workers.emplace_back(pp77, std::cref(buffer), start, end, std::cref(pattern), std::cref(table), std::ref(counts[i]), base_lines[i]);
    }

    for (auto& t : workers)
        t.join();

    auto t1 = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(t1 - t0).count();

    size_t total_matches = 0;
    for (auto c : counts) total_matches += c;

    std::cout << "\nTotal matches: " << total_matches << "\n";
    std::cout << "Elapsed time: " << elapsed << " seconds\n";

    return 0;
}

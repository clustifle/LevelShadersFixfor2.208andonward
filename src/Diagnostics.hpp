#pragma once
#include <chrono>
#include <algorithm>
#include <array>
#include <cstdint>
namespace lsf::diagnostics {
using Clock = std::chrono::steady_clock;
enum Category { Update, Visibility, Particles, ShaderCPU, Count };
struct Metric {
    double totalMs = 0; double maxMs = 0; uint64_t calls = 0;
    void add(double ms) { totalMs += ms; maxMs = std::max(maxMs, ms); ++calls; }
    double average() const { return calls ? totalMs / calls : 0.; }
};
inline bool enabled = false;
inline void* owner = nullptr; // Identity only; never dereferenced.
inline std::array<Metric, Count> metrics{};
inline Clock::time_point windowStart{};
inline void reset() { metrics = {}; owner = nullptr; windowStart = Clock::now(); }
inline void add(Category c, Clock::time_point start) {
    metrics[c].add(std::chrono::duration<double,std::milli>(Clock::now()-start).count());
}
}

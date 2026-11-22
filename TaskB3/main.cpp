#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Record { int type; double a, b; };

class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};
class OP final : public Bill {
    double a_, b_;
public:
    OP(double a, double b) : a_(a), b_(b) {}
    double total() const override { return a_ + b_; }
};
class IP final : public Bill {
    double days_, rate_, surgery_;
public:
    IP(double d, double r, double s) : days_(d), rate_(r), surgery_(s) {}
    double total() const override { return days_ * rate_ + surgery_; }
};

volatile double sink = 0.0;

template<class F>
double measure(const char* label, F fn) {
    const auto start = std::chrono::steady_clock::now();
    const double result = fn();
    const auto stop = std::chrono::steady_clock::now();
    sink = result;
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << label << ": total=" << result << ", time=" << ms << " ms\n";
    return result;
}

int main(int argc, char** argv) {
    const std::size_t n = argc > 1 ? static_cast<std::size_t>(std::stoull(argv[1])) : 1000000;
    std::vector<Record> data;
    data.reserve(n);
    for (std::size_t i = 0; i < n; ++i)
        data.push_back({static_cast<int>(i % 2), 500.0 + (i % 100), 1200.0 + (i % 50)});

    const double switchTotal = measure("switch", [&] {
        double sum = 0;
        for (const auto& x : data)
            sum += x.type == 0 ? x.a + x.b : x.a * 3.0 + x.b;
        return sum;
    });

    std::vector<Bill*> bills;
    bills.reserve(n);
    std::vector<OP> opStorage;
    std::vector<IP> ipStorage;
    opStorage.reserve((n + 1) / 2);
    ipStorage.reserve(n / 2);
    for (std::size_t i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            opStorage.emplace_back(data[i].a, data[i].b);
            bills.push_back(&opStorage.back());
        } else {
            ipStorage.emplace_back(3.0, data[i].a, data[i].b);
            bills.push_back(&ipStorage.back());
        }
    }
    const double virtualTotal = measure("virtual", [&] {
        double sum = 0;
        for (const Bill* b : bills) sum += b->total();
        return sum;
    });
    std::cout << "Totals match: " << (switchTotal == virtualTotal ? "yes" : "no") << '\n';
}

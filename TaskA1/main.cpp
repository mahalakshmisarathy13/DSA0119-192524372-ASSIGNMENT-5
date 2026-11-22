#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Bill {
protected:
    std::string patient_;
public:
    explicit Bill(std::string patient) : patient_(std::move(patient)) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual void print() const {
        std::cout << type() << " " << patient_ << ": " << total() << '\n';
    }
};

class OutPatientBill final : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(std::string p, double c, double t)
        : Bill(std::move(p)), consultation_(c), tests_(t) {}
    double total() const override { return consultation_ + tests_; }
    std::string type() const override { return "OP"; }
};

class InPatientBill final : public Bill {
    int days_;
    double perDay_, procedures_, medicines_;
public:
    InPatientBill(std::string p, int d, double rate, double procedures, double medicines)
        : Bill(std::move(p)), days_(d), perDay_(rate),
          procedures_(procedures), medicines_(medicines) {}
    double total() const override {
        return days_ * perDay_ + procedures_ + medicines_;
    }
    std::string type() const override { return "IP"; }
};

class DayCareBill final : public Bill {
    double package_;
public:
    DayCareBill(std::string p, double package)
        : Bill(std::move(p)), package_(package) {}
    double total() const override { return package_; }
    std::string type() const override { return "DayCare"; }
};

class EmergencyBill final : public Bill {
    double base_, hourly_;
    int hours_;
public:
    EmergencyBill(std::string p, double base, double hourly, int hours)
        : Bill(std::move(p)), base_(base), hourly_(hourly), hours_(hours) {}
    double total() const override { return base_ + hourly_ * hours_; }
    std::string type() const override { return "Emergency"; }
};

int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 0));
    bills.push_back(std::make_unique<DayCareBill>("Arun", 18000));
    bills.push_back(std::make_unique<EmergencyBill>("Sara", 3000, 500, 4));

    double dayTotal = 0;
    for (const auto& bill : bills) {
        bill->print();
        dayTotal += bill->total();
    }
    std::cout << "Day total: " << dayTotal << '\n';
}

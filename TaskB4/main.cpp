#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Bill {
protected:
    std::string patient_;
public:
    explicit Bill(std::string p) : patient_(std::move(p)) {}
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
};

class OutPatientBill : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(std::string p, double c, double t)
        : Bill(std::move(p)), consultation_(c), tests_(t) {}
    double total() const override { return consultation_ + tests_; }
    std::string type() const override { return "OP"; }
};

class InPatientBill : public Bill {
    int days_;
    double perDay_, procedure_;
public:
    InPatientBill(std::string p, int d, double rate, double procedure)
        : Bill(std::move(p)), days_(d), perDay_(rate), procedure_(procedure) {}
    double total() const override { return days_ * perDay_ + procedure_; }
    std::string type() const override { return "IP"; }
};

// New bill type added without modifying the existing bill classes.
class MaternityBill final : public Bill {
    double package_;
public:
    MaternityBill(std::string p, double package)
        : Bill(std::move(p)), package_(package) {}
    double total() const override { return package_; }
    std::string type() const override { return "Maternity"; }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};
class SelfPay final : public Payer {
public:
    double amountCovered(const Bill& b) const override { return b.total(); }
};

// New payer added without modifying existing payer classes.
class CorporatePayer final : public Payer {
    double coveragePercent_;
public:
    explicit CorporatePayer(double percent) : coveragePercent_(percent) {
        if (percent < 0 || percent > 100)
            throw std::invalid_argument("coverage must be 0..100");
    }
    double amountCovered(const Bill& b) const override {
        return b.total() * coveragePercent_ / 100.0;
    }
};

int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<MaternityBill>("Anu", 65000));

    SelfPay self;
    CorporatePayer corporate(80);
    for (const auto& b : bills) {
        std::cout << b->type() << " total=" << b->total()
                  << ", self-pay covers=" << self.amountCovered(*b)
                  << ", corporate covers=" << corporate.amountCovered(*b) << '\n';
    }
}

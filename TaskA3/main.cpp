#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
};

class OutPatientBill : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(double c, double t) : consultation_(c), tests_(t) {}
    double total() const override { return consultation_ + tests_; }
    std::string type() const override { return "OP"; }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class SelfPay final : public Payer {
public:
    double amountCovered(const Bill& bill) const override { return bill.total(); }
};

class Insurance final : public Payer {
    double coverLimit_, coPayPercent_;
public:
    Insurance(double limit, double coPayPercent)
        : coverLimit_(limit), coPayPercent_(coPayPercent) {}
    double amountCovered(const Bill& bill) const override {
        const double eligible = std::min(bill.total(), coverLimit_);
        return eligible * (1.0 - coPayPercent_ / 100.0);
    }
};

class GovernmentScheme final : public Payer {
    double opPackage_;
public:
    explicit GovernmentScheme(double packageRate) : opPackage_(packageRate) {}
    double amountCovered(const Bill& bill) const override {
        return bill.type() == "OP" ? opPackage_ : 0.0;
    }
};

// Extension example: new payer is added without editing existing payer classes.
class CorporateTieUp final : public Payer {
    double discountPercent_;
public:
    explicit CorporateTieUp(double discountPercent) : discountPercent_(discountPercent) {}
    double amountCovered(const Bill& bill) const override {
        return bill.total() * (1.0 - discountPercent_ / 100.0);
    }
};

int main() {
    OutPatientBill bill(500, 1200);
    std::vector<std::unique_ptr<Payer>> payers;
    payers.push_back(std::make_unique<SelfPay>());
    payers.push_back(std::make_unique<Insurance>(2000, 10));
    payers.push_back(std::make_unique<GovernmentScheme>(1500));
    payers.push_back(std::make_unique<CorporateTieUp>(15));

    for (const auto& payer : payers)
        std::cout << "Covered: " << payer->amountCovered(bill) << '\n';
}

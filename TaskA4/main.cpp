#include <iostream>
#include <stdexcept>
#include <string>

class BillingError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
class InvalidCharge : public BillingError {
public:
    using BillingError::BillingError;
};
class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};
class UnknownPackage : public BillingError {
public:
    using BillingError::BillingError;
};

double validateCharge(double amount) {
    if (amount < 0) throw InvalidCharge("negative charge");
    return amount;
}

double approveClaim(double amount, double cover) {
    validateCharge(amount);
    if (cover < 0) throw InvalidCharge("negative cover");
    if (amount > cover) throw ClaimError("claim exceeds cover");
    return amount;
}

int main() {
    const double cases[][2] = {{400, 500}, {600, 500}, {-1, 500}};
    for (const auto& c : cases) {
        try {
            std::cout << "Approved: " << approveClaim(c[0], c[1]) << '\n';
        } catch (const BillingError& e) {
            std::cout << "Billing error: " << e.what() << '\n';
        }
    }
}

//
// Created by amir on 12/1/23.
//

#include "BigNumber.h"
#include <complex>

using Complex = std::complex<double>;
using ComplexVector = std::vector<Complex>;

BigNumber::BigNumber(const std::string &n) {
    for (const auto &item: n) {
        digits.insert(digits.cbegin(), item - '0');
    }
}

std::ostream &operator<<(std::ostream &o, const BigNumber &b) {
    if (b.digits.empty()) return o;
    for (int i = static_cast<int>(b.digits.size()) - 1; i >= 0; --i) {
        o << b.digits[i];
    }
    return o;
}

static ComplexVector fft(ComplexVector x) {
    const size_t N = x.size();
    if (N <= 1) {
        return x;
    }

    // Divide
    ComplexVector even(N / 2);
    ComplexVector odd(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }

    // Conquer
    even = fft(even);
    odd = fft(odd);

    // Combine
    for (size_t k = 0; k < N / 2; ++k) {
        Complex t = std::polar(1.0, -2.0 * std::numbers::pi * static_cast<double>(k) /
                                    static_cast<double>(N)) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
    return x;
}

static ComplexVector ifft(ComplexVector x) {
    // Take the conjugate of the input
    for (auto &i: x) {
        i = std::conj(i);
    }

    // Perform FFT
    x = fft(x);

    // Take the conjugate of the result and scale by N
    for (auto &i: x) {
        i = std::conj(i) / static_cast<double>(x.size());
    }
    return x;
}

static std::vector<uint32_t> multiply(ComplexVector x, ComplexVector y) {
    int n = 1;
    while (n < std::max(x.size(), y.size())) {
        n <<= 1;
    }
    n <<= 1;
    x.resize(n, 0);
    y.resize(n, 0);
    x = fft(x);
    y = fft(y);
    ComplexVector result(x.size());
    for (int i = 0; i < x.size(); ++i) {
        result[i] = x[i] * y[i];
    }
    // Perform IFFT
    result = ifft(result);
    std::vector<uint32_t> results(result.size());
    for (int i = 0; i < result.size(); ++i) {
        results[i] = static_cast<int>(std::round(result[i].real()));
    }
    for (int i = 0; i < results.size(); ++i) {
        if (i + 1 == results.size()) continue;
        results[i + 1] += results[i] / 10;
        results[i] = results[i] % 10;
    }
    return results;
}

BigNumber BigNumber::operator*(const BigNumber &o) {
    ComplexVector x(digits.begin(), digits.end());
    ComplexVector y(o.digits.begin(), o.digits.end());
    BigNumber res;
    res.digits = multiply(x, y);
    res.normalize();
    return res;
}

void BigNumber::normalize() {
    int i = static_cast<int>(digits.size()) - 1;
    for (; i >= 0 && digits[i] == 0; --i);
    digits.resize(i + 1);

}

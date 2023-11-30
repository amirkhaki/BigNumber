#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include <iostream>
#include <complex>
#include <vector>


using Complex = std::complex<double>;
using ComplexVector = std::vector<Complex>;

// Cooley-Tukey Radix-2 Decimation in Time FFT
ComplexVector fft(ComplexVector x) {
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

// Cooley-Tukey Radix-2 Decimation in Time IFFT
ComplexVector ifft(ComplexVector x) {
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

std::vector<int> multiply(ComplexVector x, ComplexVector y) {
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
    std::vector<int> results(result.size());
    for (int i = 0; i < result.size(); ++i) {
        results[i] = static_cast<int>(std::round(result[i].real()));
    }
    std::cout << std::endl;
    for (int i = 0; i < results.size(); ++i) {
        if (i + 1 == results.size()) continue;
        results[i + 1] += results[i] / 10;
        results[i] = results[i] % 10;
    }
    return results;
}

int main() {
    // Example usage
    ComplexVector x = {1, 1};
    ComplexVector y = {1, 1};
    auto result = multiply(x, y);
    std::reverse(result.begin(), result.end());
    for (int i: result) {
        std::cout << i;
    }
    std::cout << std::endl;
    return 0;
}

#pragma clang diagnostic pop
//
// Created by amir on 12/1/23.
//

#ifndef BIGNUMBER_BIGNUMBER_H
#define BIGNUMBER_BIGNUMBER_H

#include <vector>
#include <string>
#include <iostream>

class BigNumber {
public:
    BigNumber() = default;

    explicit BigNumber(const std::string &n);

    void normalize();

    BigNumber operator*(const BigNumber &o);

    friend std::ostream &operator<<(std::ostream &o, const BigNumber &b);


private:
    std::vector<uint32_t> digits;
};


#endif //BIGNUMBER_BIGNUMBER_H
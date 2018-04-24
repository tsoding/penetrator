#ifndef UINPUT_HPP_
#define UINPUT_HPP_

#include <map>
#include <vector>
#include "uud.hpp"

class UInput
{
public:
    using Bits = std::map<int, std::vector<int>>;

    UInput(const Bits &bits, const Uud &uud);
    ~UInput();

    void emit(int type, int code, int val);

private:
    int fd;
};

#endif  // UINPUT_HPP_

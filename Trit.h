#ifndef CPP_TASK1_TRITSET_TRIT_H
#define CPP_TASK1_TRITSET_TRIT_H

#include <iostream>

enum class Trit
{
    F = 1, U = 0, T = 2
};

unsigned int TrittoInt(Trit a);
Trit inttoTrit(unsigned int x);

Trit operator &(Trit a, Trit b);
Trit operator |(Trit a, Trit b);
Trit operator !(Trit a);


#endif //CPP_TASK1_TRITSET_TRIT_H
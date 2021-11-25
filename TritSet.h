#include <vector>
#include "Trit.h"
#ifndef CPP_TASK1_TRITSET_TRITSET_H
#define CPP_TASK1_TRITSET_TRITSET_H

const unsigned int Trit_U_Int = sizeof(unsigned int) * 4;

class TritsetProxy;

enum  trit_pack {
    F = 1, U = 0, T = 2, mask = 0b11 
};

class Tritset
{
    friend class TritsetProxy;  

private:
    std::vector<unsigned> array_;
    int last_;



public:
    void update();
    Tritset();
    explicit Tritset(unsigned int size); 
    Tritset(const Tritset& other); 
    ~Tritset();

    size_t getsize() const;
    size_t length() const;

    size_t f_capacity() const;
    void extend(const size_t new_size);


    void shrink();

    Tritset& operator&=(const Tritset& other);
    Tritset& operator|=(const Tritset& other);

    Tritset operator~() const;
    Tritset operator&(const Tritset& other);
    Tritset operator|(const Tritset& other);

    Trit operator[](const int index) const;
    TritsetProxy operator[](const int index);

    std::string to_string() const;
};



class TritsetProxy
{
private:
    Tritset* tritset;
    unsigned int index;

public:
    explicit TritsetProxy(Tritset* t, unsigned int i)
        : tritset(t)
        , index(i)
    {}

    ~TritsetProxy()
    {}

    operator Trit() const;

    Trit get_value() const;

    void set_value(const Trit& value);

    TritsetProxy& operator&=(const Trit& other);

    TritsetProxy& operator|=(const Trit& other);

    TritsetProxy& operator=(Trit const& t);

    bool operator==(const TritsetProxy& other);

    TritsetProxy& operator=(const TritsetProxy& other);
};



#endif //CPP_TASK1_TRITSET_TRITSET_H
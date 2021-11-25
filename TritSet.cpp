#include "TritSet.h"
#include "Trit.h"
#include <vector>


Tritset::Tritset() 
{
    array_.resize(0);
    last_ = -1;
}

Tritset::Tritset(unsigned int size) { 
    last_ = (int)size - 1;

}

Tritset::~Tritset()  
{
}

Tritset::Tritset(const Tritset& other)      
    : array_(other.array_), last_(other.last_) {
}

size_t Tritset::getsize() const          
{
    return array_.size();
}

size_t Tritset::length() const {        
    return size_t(last_ + 1);
}

void Tritset::shrink()                
{
    if (last_ < 0) {
        array_.resize(0);
        last_ = -1;
    }
    update();
    array_.resize(last_ + 1);
}

Trit Tritset::operator[](const int index) const {
    unsigned value = array_[index / Trit_U_Int];
    value >>= 2 * (index % Trit_U_Int);
    value &= static_cast<unsigned>(trit_pack::mask);
    if (value == static_cast<unsigned>(trit_pack::F))
        return Trit(F);
    if (value == static_cast<unsigned>(trit_pack::T))
        return Trit(T);
    return Trit(U);
}

void Tritset::update() {
    last_ = -1;
    size_t s = array_.size() * Trit_U_Int;
    for (int i = 0; i < s; i++) {
        Trit t = (*this)[i];
        if (Trit(t) != Trit::U)
            last_ = i;
    }
}

Tritset& Tritset::operator&=(const Tritset& other) {    
    size_t len = other.length();
    extend(len);
    for (int i = 0; i < len; i++)
        ((*this)[i]) &= Trit(other[i]);
    for (size_t i = len; i <= last_; i++)
        (*this)[i] &= Trit(Trit::U);
    update();
    return (*this);
}

Tritset& Tritset::operator|=(const Tritset& other) {
    size_t len = other.length();
    extend(len);
    for (size_t i = 0; i < len; i++)
        (*this)[i] |= other[i];
    for (size_t i = len; i <= last_; i++)
        (*this)[i] |= Trit(Trit::U);
    update();
    return (*this);
}

Tritset Tritset::operator~() const {        
    Tritset cp(*this);
    for (int i = 0; i <= last_; i++)
        (cp[i]) = !(cp[i]);
    cp.update();
    return cp;
}

Tritset Tritset::operator&(const Tritset& other) {
    Tritset a(*this);
    a &= other;
    return a;
}

Tritset Tritset::operator|(const Tritset& other) {         
    Tritset a(*this);
    a |= other;
    return a;
}


size_t Tritset::f_capacity() const {
    return array_.size() * Trit_U_Int;
}

void Tritset::extend(const size_t new_size) {
    size_t needed = new_size / Trit_U_Int;
    needed += ((new_size % Trit_U_Int) ? 1 : 0);
    if (needed > array_.size())
        array_.resize(needed, 0);
}

std::string Tritset::to_string() const {
    std::string str;
    size_t len = array_.size() * Trit_U_Int;
    str.resize(len);
    for (int i = 0; i < len; i++) {
        Trit t = (*this)[i];
        if (t == Trit::U)
            str[i] = '?';
        else if (t == Trit::T)
            str[i] = '1';
        else str[i] = '0';
    }
    return str;
}

                                     
Trit TritsetProxy::get_value() const {
    if (index >= tritset->f_capacity())
        return Trit(Trit::U);
    int value = tritset->array_[index / Trit_U_Int];
    value >>= 2 * (index % Trit_U_Int);          
    value &= trit_pack::mask;                                
    if (value == 1)
        return Trit(F);
    if (value == 2)
        return Trit(T);
    return Trit(U);
}

void TritsetProxy::set_value(const Trit& value) {
    if (index >= tritset->f_capacity()) {
        if (value == Trit::U)
            return;
        tritset->extend(index + 1);
    }
    size_t id = index / Trit_U_Int;
    size_t shift = 2 * (index % Trit_U_Int);

    tritset->array_[id] &= ~(static_cast<unsigned>(trit_pack::mask) << shift);
    if (value == Trit::T)                                 
        tritset->array_[id] |= static_cast<int>(trit_pack::T) << shift;
    else if (value == Trit::U)
        tritset->array_[id] |= static_cast<int>(trit_pack::U) << shift;
    else
        tritset->array_[id] |= static_cast<int>(trit_pack::F) << shift;

    if ((int)index > tritset->last_ && value != Trit::U)
        tritset->last_ = static_cast<int>(index);
}

TritsetProxy& TritsetProxy::operator=(Trit const& other)
{
    set_value(other);
    return (*this);
}
TritsetProxy::operator Trit() const {
    return get_value();
}

TritsetProxy& TritsetProxy::operator&=(const Trit& other) {
    set_value(get_value() & other);
    return (*this);
}

TritsetProxy& TritsetProxy::operator|=(const Trit& other) {
    set_value(get_value() | other);
    return (*this);
}

TritsetProxy Tritset::operator[](const int index) {
    return TritsetProxy(this, size_t(index));
}



TritsetProxy& TritsetProxy::operator=(const TritsetProxy& other) {
    return (*this) = other.get_value();
}

bool TritsetProxy::operator==(const TritsetProxy& other) {
    return other.index == index;
}

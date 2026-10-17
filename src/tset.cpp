// HHV, BMK, Курс "Методы программирования-2", C++, 001
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

// ---------------------------------------------------------------
// Конструкторы и преобразования
// ---------------------------------------------------------------

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

// ---------------------------------------------------------------
// Доступ к элементам множества
// ---------------------------------------------------------------

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        return 0;
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::InsElem: index out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::DelElem: index out of range");
    BitField.ClrBit(Elem);
}

// ---------------------------------------------------------------
// Теоретико-множественные операции
// ---------------------------------------------------------------

TSet& TSet::operator=(const TSet& s)
{
    if (this == &s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const
{
    return BitField != s.BitField;
}

// объединение с элементом
TSet TSet::operator+(const int Elem)
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

// разность с элементом
TSet TSet::operator-(const int Elem)
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

// объединение двух множеств
TSet TSet::operator+(const TSet& s)
{
    return TSet(BitField | s.BitField);
}

// пересечение двух множеств
TSet TSet::operator*(const TSet& s)
{
    return TSet(BitField & s.BitField);
}


// дополнение множества
TSet TSet::operator~(void)
{
    return TSet(~BitField);
}

// ---------------------------------------------------------------
// Ввод / вывод
// ---------------------------------------------------------------

istream& operator>>(istream& istr, TSet& s)
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << s.BitField;
    return ostr;
}
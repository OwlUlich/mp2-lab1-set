// HHV, BMK, Курс "Методы программирования-2", C++, 001
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("TBitField: negative length");

    BitLen = len;
    int bitsInElem = sizeof(TELEM) * 8;   // 32
    MemLen = (len + bitsInElem - 1) / bitsInElem;
    if (MemLen == 0) MemLen = 1;

    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const
{
    return n / (int)(sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const
{
    return (TELEM)1 << (n % (int)(sizeof(TELEM) * 8));
}

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::SetBit: index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::ClrBit: index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::GetBit: index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this == &bf)
        return *this;

    delete[] pMem;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);

    for (int i = 0; i < res.MemLen; i++) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        res.pMem[i] = a | b;
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);

    for (int i = 0; i < res.MemLen; i++) {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        res.pMem[i] = a & b;
    }
    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];

    int bitsInElem = (int)(sizeof(TELEM) * 8);
    int used = BitLen % bitsInElem;
    if (used != 0) {
        TELEM mask = ((TELEM)1 << used) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

istream& operator>>(istream& istr, TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++) {
        int bit;
        istr >> bit;
        if (bit)
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << bf.GetBit(i);
    return ostr;
}
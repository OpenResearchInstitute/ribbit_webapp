/*
Maximum length sequence

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE
{
    class MLS
    {
        // returns the position of the
        // (leftmost) highest set bit
        // of an unsigned value
        // then zeros the rest.
        static int hibit(unsigned n)
        {
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            return n ^ (n >> 1);
        }
        int poly, test, reg;

    public:
        /* 
            initializes an `MLS` object with the provided
            `poly` and `reg` values, or with default values
            if none are provided. It also calculates a
            `test` value based on the `poly` value.
        */
        MLS(int poly = 0b100000000000000001001, int reg = 1) : poly(poly), test(hibit(poly) >> 1), reg(reg) {}
        void reset(int r = 1)
        {
            reg = r;
        }

        bool bad(int r = 1)
        {
            reg = r;
            int len = hibit(poly) - 1;
            for (int i = 1; i < len; ++i)
            {
                (*this)();
                if (reg == r)
                    return true;
            }
            (*this)();
            return reg != r;
        }
        // feedback shift register
        bool operator()()
        {
            bool fb = reg & test;
            reg <<= 1;
            reg ^= fb * poly;
            return fb;
        }
    };
}

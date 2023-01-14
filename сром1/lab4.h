#pragma once
#include <iostream>
#include <algorithm>
#include <string> 
#include <array>
#include <vector>

#define p 503
#define DIM 251

using namespace std;



class ONB
{
private:

    array<bool, DIM> v;

    static array<array<bool, DIM>, DIM> mult_matrix;

    static array<bool, DIM> pow2modp;


public:

    ONB() { this->v = { false }; }

    ONB(vector<bool> a) : ONB()
    {
        ONB w;
        for (int i = 0; i < a.size(); i++)
            w.v[i] = a[i];
        *this = *this + w;
    }

    static ONB one()
    {
        ONB one;
        one.v = { true };
        return one;
    }

    static void precalc_matrix()
    {
        size_t a = 1;
        for (size_t i = 0; i < DIM; ++i)
        {
            pow2modp[i] = a;
            a = (a * 2) % p;
        }

        for (size_t i = 0; i < DIM; ++i)
        {
            for (size_t j = 0; j < DIM; ++j)
            {
                size_t a1 = (pow2modp[i] + pow2modp[j]) % p;
                size_t a2 = (pow2modp[i] - pow2modp[j]) % p;
                size_t a3 = ((-1 * pow2modp[i]) + pow2modp[j]) % p;
                size_t a4 = ((-1 * pow2modp[i]) - pow2modp[j]) % p;

                mult_matrix[i][j] = ((a1 == 1) || (a2 == 1) || (a3 == 1) || (a4 == 1));
            }
        }
    }

    ONB operator+(const ONB& a) const
    {
        ONB result;
        result.v = a.v;

        for (int i = 0; i < DIM; i++)
            result.v[i] = (result.v[i] != this->v[i]);

        return result;
    }

    ONB operator*(const ONB& a) const
    {
        ONB res;
        ONB t1 = *this;
        ONB t2 = a;

        for (int i = 0; i < DIM; i++)
        {
            res.v[i] = mult_vec(t1.v, mult_mat(t2.v));
            t1 = t1.shiftl(1);
            t2 = t1.shiftl(1);
        }

        return res;
    }

    bool operator==(const ONB& a) const
    {
        return (this->v == a.v);
    }

    ONB inv() const
    {
        array<bool, 8> m = { 1, 1, 1, 1, 1, 0, 1, 0 };  // 250 in binary
        ONB b = *this;
        ONB temp = b;
        size_t k = 1;

        for (int i = 6; i >= 0; i--)
        {
            for (int j = 0; j < k; j++)
                temp = temp.pow2();

            b = b * temp;
            k = 2 * k;

            if (m[i])
            {
                b = b.pow2() * *this;
                k = k + 1;
            }
        }

        b.pow2();

        return b;
    }

    ONB trace() const
    {
        int c = 0;
        for (int i = 0; i < DIM; i++)
        {
            if (this->v[i])
                c++;
        }
        c = c % 2;

        if (c)
            return one();
        else
            return ONB();
    }

    string to_string() const
    {
        string result;

        for (int i = DIM - 1; i >= 0; i--)
        {
            if (this->v[i])
                result.append("1");
            else
                result.append("0");
        }

        return result;
    }

    ONB pow2() const
    {
        return this->shiftr(1);
    }

private:

    size_t msb() const
    {
        auto t = this->v.end();

        while (*t != 1)
            --t;

        return t - this->v.begin();
    }

    ONB shiftr(int n) const
    {
        ONB result = *this;
        rotate(result.v.begin(), result.v.end() - n, result.v.end());
        return result;
    }

    ONB shiftl(int n) const
    {
        ONB result = *this;
        rotate(result.v.begin(), result.v.begin() + n, result.v.end());
        return result;
    }

    static array<bool, DIM> mult_mat(array<bool, DIM> a)
    {
        array<bool, DIM> res;

        for (int i = 0; i < DIM; i++)
        {
            int c = 0;
            for (int j = 0; j < DIM; j++)
            {
                if (mult_matrix[i][j] && a[j])
                    c++;
            }
            c = c % 2;
            res[i] = c;
        }

        return res;
    }

    static bool mult_vec(array<bool, DIM> a, array<bool, DIM> b)
    {
        bool res = false;

        for (size_t i = 0; i < DIM; i++)
            res = (res != (a[i] && b[i]));

        return res;
    }

};
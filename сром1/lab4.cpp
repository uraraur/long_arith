#include <iostream>
#include <algorithm>
#include <string> 
#include <array>

#define p 503
#define DIM 251

using namespace std;

array<array<bool, DIM>, DIM> mult_matrix = { {0} };

array<bool, DIM> pow2modp = { 0 };

constexpr void precalc_matrix()
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

            mult_matrix[i][j] = (a1 || a2 || a3 || a4);
        }
    }
}


class ONB
{
private:

    array<bool, DIM> v;

public:

    ONB() { this->v = { false }; }

    static ONB one() 
    {
        ONB one;
        one.v = { true };
        return one;   
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
        if (a.size() == DIM)
            return this->mult(a);
        else
            throw "Invalid size";
    }

    ONB pow(const ONB& a) const
    {

    }

    ONB inv() const
    {
        ONB a = this->pow2();
        ONB res = a;

        for (int i = 0; i < DIM; i++)
        {
            a = a.pow2();
            res = a * res;
        }

        return res;
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

        for (auto i = this->v.end(); i >= this->v.begin(); i--)
        {
            if (*i)
                result.append("1");
            else
                result.append("0");
        }

        return result;
    }

private:

    size_t msb() const
    {
        auto t = this->v.end();

        while (*t != 1)
            --t;

        return t - this->v.begin();
    }

    ONB shift(int n) const
    {
        rotate(this->v.begin(), this->v.end() - n, this->v.end());
    }

    ONB mult(const ONB& a) const
    {
        ONB res;

        for (int i = 0; i < this->size(); i++)
        {
            if (this->v[i] == 1)
                res = res.add(a.shift(i));
        }

        return res.modp();
    }

    ONB pow2() const
    {
        return this->shift(1);
    }
};
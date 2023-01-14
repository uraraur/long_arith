#include <iostream>
#include <algorithm>
#include <string> 
#include <array>

#define DIM 251

using namespace std;

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
        ONB a = *this;
        ONB res = a;

        for (int i = 0; i < DIM; i++)
        {
            a = a.pow2();
            res = a.add(res);
        }

        return res;
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

    ONB shrink_to_fit() const
    {
        ONB result;

        for (int i = 0; i < min(this->size(), result.size()); i++)
            result.v[i] = this->v[i];

        return result;
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
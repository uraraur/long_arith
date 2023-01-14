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

            mult_matrix[i][j] = ((a1 == 1) || (a2 == 1) || (a3 == 1) || (a4 == 1));
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
        ONB res;
        ONB m1;

        for (int i = 0; i < DIM; i++) 
        {


        }
        m1.v = mult_mat(a.v);
        mult_vec(this->v, m1.v);

    }

    ONB pow(const ONB& a) const
    {

    }

    ONB inv() const
    {
    
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

        for(size_t i = 0; i< DIM; i++)
            res = (res != (a[i] && b[i]));

        return res;
    }

    ONB pow2() const
    {
        return this->shift(1);
    }
};
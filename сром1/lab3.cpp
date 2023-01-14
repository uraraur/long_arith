#include <iostream>
#include <vector>
#include <string> 
 
#define DIM 251

using namespace std;

vector<bool> polymod()
{
    vector<bool> poly(252, false);
    poly.at(251) = 1;
    poly.at(14) = 1;
    poly.at(4) = 1;
    poly.at(1) = 1;
    poly.at(0) = 1;
    return poly;
}

class VPB
{
private:

    vector<bool> v;

public:

    VPB(){ this->v = vector<bool>(252, false); }

    static VPB one() {

        VPB one;
        one.v.at(0) = 1;
        return one;
    }

    VPB operator+(const VPB& a) const
    {
        if(a.size() == DIM)
            return this->add(a);
        else
            throw "Invalid size";
    }

    VPB operator*(const VPB& a) const
    {
        if (a.size() == DIM)
            return this->mult(a);
        else
            throw "Invalid size";
    }

    VPB pow(const VPB& a) const
    {

    }

    VPB inv() const
    {
        VPB a = this->pow2();
        VPB res = a;

        for (int i = 0; i < DIM; i++)
        {
            a = a.pow2();
            res = a * res;
        }

        return res;
    }

    VPB trace() const
    {
        VPB a = *this;
        VPB res = a;

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

    size_t size() const { return this->v.size(); }

    size_t msb() const
    {
        auto t = this->v.end();

        while (*t != 1)
            --t;

        return t - this->v.begin();
    }

    VPB shrink_to_fit() const
    {
        VPB result;

        for (int i = 0; i < min(this->size(), result.size()); i++)
            result.v[i] = this->v[i];
        
        return result;
    }


    VPB add(const VPB& a) const
    {
        VPB result;

        if(a.size() > this->size())
        {
            result.v = a.v;
            for(int i = 0; i < this->size(); i++)
                result.v[i] = (result.v[i] != this->v[i]); 
        }
        else
        {
            result.v = this->v;
            for(int i = 0; i < a.size(); i++)
                result.v[i] = (result.v[i] != a.v[i]); 
        }

        return result;
    }

    VPB modp() const
    {
        VPB m;
        m.v = polymod();
        VPB res = *this;

        while (res.msb() >= m.msb())
        {
            int c = res.size() - m.size();
            res = res.add(m.shift(c));
        }
        return res.shrink_to_fit();
    }

    VPB shift(int n) const
    {
        VPB result = *this;
        result.v.insert(result.v.begin(), n, false);
    }

    VPB mult(const VPB& a) const
    {
        VPB res;

        for (int i = 0; i < this->size(); i++) 
        {
            if(this->v[i] == 1)
                res = res.add(a.shift(i));
        }

        return res.modp();
    }

    VPB pow2() const
    {
        VPB res;
        res.v = vector<bool>(this->size() * 2, false);
        
        for (int i = 0; i < this->size(); i++) 
            res.v[2 * i] = this->v[i];
        
        return res.modp();
    }
};
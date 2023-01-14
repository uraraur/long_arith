#pragma once
#include <iostream>
#include <vector>
#include <string> 
 
#define DIM 251

using namespace std;

class PBV
{
private:

    vector<bool> v;

    static vector<bool> polymod()
    {
        vector<bool> poly(252, false);
        poly.at(251) = 1;
        poly.at(14) = 1;
        poly.at(4) = 1;
        poly.at(1) = 1;
        poly.at(0) = 1;
        return poly;
    }

public:

    PBV(){ this->v = vector<bool>(DIM, false); }

    PBV(vector<bool> a) : PBV()
    {
        PBV p;
        p.v = a;
        *this = this->add(p);
    }

    static PBV one() {

        PBV one;
        one.v.at(0) = 1;
        return one;
    }

    PBV operator+(const PBV& a) const
    {
        if(a.size() == DIM)
            return this->add(a);
        else
            throw "Invalid size";
    }

    PBV operator*(const PBV& a) const
    {
        if (a.size() == DIM)
            return this->mult(a);
        else
            throw "Invalid size";
    }

    bool operator==(const PBV& a) const
    {
        return (this->v == a.v);
    }

    PBV inv() const
    {
        PBV a = this->pow2();
        PBV res = a;

        for (int i = 0; i < DIM; i++)
        {
            a = a.pow2();
            res = a * res;
        }

        return res;
    }

    PBV trace() const
    {
        PBV a = *this;
        PBV res = a;

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

        for (int i = this->size() - 1; i >= 0; i--)
        {
            if (this->v[i])
                result.append("1");
            else
                result.append("0");
        }

        return result;
    }

    PBV pow2() const
    {
        PBV res;
        res.v = vector<bool>(this->size() * 2, false);

        for (int i = 0; i < this->size(); i++)
            res.v[2 * i] = this->v[i];

        return res.modp();
    }

private:

    size_t size() const { return this->v.size(); }

    size_t msb() const
    {
        int t = this->size() - 1;

        while (this->v[t] != 1)
            --t;

        return t;
    }

    PBV shrink_to_fit() const
    {
        PBV result;

        for (int i = 0; i < min(this->size(), result.size()); i++)
            result.v[i] = this->v[i];
        
        return result;
    }


    PBV add(const PBV& a) const
    {
        PBV result;

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

    PBV modp() const
    {
        PBV m;
        m.v = polymod();
        PBV res = *this;

        while (res.msb() >= m.msb())
        {
            int c = res.msb() - m.msb();
            res = res.add(m.shift(c));
        }
        return res.shrink_to_fit();
    }

    PBV shift(int n) const
    {
        PBV result = *this;
        result.v.insert(result.v.begin(), n, false);
        return result;
    }

    PBV mult(const PBV& a) const
    {
        PBV res;

        for (int i = 0; i < this->size(); i++) 
        {
            if(this->v[i] == 1)
                res = res.add(a.shift(i)).modp();
        }

        return res.modp();
    }
};

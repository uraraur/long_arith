#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;
#define ARCHITECTURE_TYPE 64
#define MAX_UINT 18446744073709551615

class LongNum {
private:
    uint16_t length;
    uint64_t* number = nullptr;
    void shrink_to_fit() {
        uint16_t counter = 0;
        for (int i = length - 1; i >= 0; i--) {
            if (number[i] != 0) break;
            else counter++;
        }
        uint64_t* temp = new uint64_t[length - counter];
        for (uint16_t j = 0; j < length - counter; j++) {
            temp[j] = number[j];
        }
        delete[] number;
        number = temp;
        length = length - counter;
    };
    static string to_hex(uint64_t a) {
        int rem = 0;
        int i = 0;
        string hexNum(16, '0');
        while (a != 0){
            rem = a % 16;
            if (rem < 10)
                rem = rem + 48;
            else
                rem = rem + 55;
            hexNum[i] = rem;
            i++;
            a = a / 16;
        }
        return hexNum;
    }
    static uint64_t from_hex(string a) {
        uint64_t base = 1;
        uint64_t decNum = 0;
        for (int i = a.size() - 1; i >= 0; i--) {
            if (a[i] >= '0' && a[i] <= '9') {
                decNum += ((int(a[i]) - 48) * base);
            }
            else if (a[i] >= 'A' && a[i] <= 'F') {
                decNum += ((int(a[i]) - 55) * base);
            }
            base = base * 16;
        }
        return decNum;
    }
    LongNum& addzero(int j) {
        uint64_t* temp;
        temp = new uint64_t[this->length + j];
        for (int i = 0; i < this->length; i++) {
            temp[i] = this->number[i];
        }
        for (int i = this->length; i < this->length + j; i++) {
            temp[i] = 0;
        }
        delete[] this->number;
        this->number = temp;
        this->length = this->length + j;
        return *this;
    }
    pair<LongNum, LongNum> kar_sep(uint64_t size) const {
        pair<LongNum, LongNum> a;
        LongNum temp = *this;
        if (temp.length % 2 != 0) {
            temp.addzero(size - temp.length);
        }
        uint64_t s = (temp.length / 2) * ARCHITECTURE_TYPE;
        a.first = (temp >> s);
        a.second = ((temp << s) >> s);
        a.first.shrink_to_fit();
        a.second.shrink_to_fit();
        return a;
    }
    LongNum mult2pow(uint64_t pow) const {
        LongNum temp = *this;
        uint64_t c = pow / ARCHITECTURE_TYPE;
        LongNum res((uint64_t)0, this->length + c);
        for (int i = this->length - 1; i >= 0; i--) {
            res.number[i + c] = this->number[i];
        }
        for (int i = 0; i < res.length - this->length - 1; i++) {
            res.number[i] = 0;
        }
        return res;
    }
public:
    LongNum() : LongNum((uint64_t)0, 1) {};
    LongNum(uint64_t num, uint16_t len = 1) {
        length = len;
        number = new uint64_t[len];
        for (uint16_t i = 0; i < len; i++) {
            number[i] = 0;
        }
        number[0] = num;
    }
    LongNum(uint64_t* num, uint16_t len) {
        length = len;
        number = new uint64_t[len];
        for (uint16_t i = 0; i < len; i++) {
            number[i] = num[i];
        }
    }
    LongNum(LongNum&& b) {
        this->number = b.number;
        b.number = nullptr;
        this->length = b.length;
    }
    LongNum(const LongNum& b) {
        this->length = b.length;
        number = new uint64_t[length];
        for (uint16_t i = 0; i < length; i++) {
            this->number[i] = b.number[i];
        }
    }
    ~LongNum() {
        if (number) {
            delete[] number;
        }
    };
    void print() const {
        cout << endl;
        for (int i = length - 1; i >= 0; i--) {
            cout << number[i] << " ";
        }
        cout << endl;
    }
    void printhex() const {
        cout << endl;
        cout << this->gethex();
        cout << endl;
    }
    string gethex() const {
        string temp;
        string tohex;
        for (int i = length - 1; i >= 0; i--) {
            tohex = to_hex(number[i]);
            std::reverse(tohex.begin(), tohex.end());
            temp = temp + tohex;
        }
       while (temp[0] == '0')
         temp.erase(0, 1);
        return temp;
    }
    static LongNum readhex(string a) {
        string temp(16, '0');
        while (a.size() % 16 != 0) {
            a.insert(0, "0");
        }
        LongNum result((uint64_t)0, a.size() / 16);
        for (int i = a.size() - 1, j = 0; i >= 0; i = i - 16, j++) {
            for (int c = 0; c < 16; c++) {
                temp[15 - c] = a[i - c];
            }
            result.number[j] = from_hex(temp);
        }
        return result;
    }
    LongNum operator+(const LongNum& b) const {
        uint16_t c = 0;
        uint16_t min = this->length;
        uint16_t max = b.length;
        const LongNum* max_num = &b;
        if (b.length < this->length) {
            min = b.length;
            max = this->length;
            max_num = this;
        }
        LongNum sum(*max_num);
        uint16_t i = 0;
        uint64_t this_temp = this->number[0];
        uint64_t b_temp = b.number[0];
        for (; i < min; i++) {
            this_temp = this->number[i];
            b_temp = b.number[i];
            sum.number[i] = this_temp + b_temp + c;
            c = ((this_temp & b_temp) | (~sum.number[i] & (this_temp | b_temp))) >> (ARCHITECTURE_TYPE - 1);
        }
        while (c != 0 && i != max) {
            c = (sum.number[i] == MAX_UINT);
            sum.number[i] = sum.number[i] + 1;
            i++;
        }
        if (c != 0) {
            uint64_t* temp = new uint64_t[max + 1];
            for (uint16_t i = 0; i < max; i++) {
                temp[i] = sum.number[i];
            }
            temp[max] = 1;
            delete[] sum.number;
            sum.number = temp;
            ++sum.length;
        }
        return sum;
    };
    LongNum operator-(const LongNum& b) const {
        uint16_t d = 0;
        uint16_t max = this->length;
        uint16_t min = b.length;
        const LongNum* max_num = this;
        if (b > *this) {
            throw exception("Negative number!1");
        }
        LongNum diff(*max_num);
        uint16_t i = 0;
        uint64_t this_temp = this->number[0];
        uint64_t b_temp = b.number[0];
        for (; i < min; i++) {
            this_temp = this->number[i];
            b_temp = b.number[i];
            diff.number[i] = this_temp - b_temp - d;
            d = ((~this_temp & (b_temp | diff.number[i]) | (b_temp & diff.number[i]))) >> (ARCHITECTURE_TYPE - 1);
        }
        while (d != 0 && i != max) {
            d = (diff.number[i] == 0);
            diff.number[i] = diff.number[i] - 1;
            i++;
        }
        if (d != 0) {
            throw exception("Negative number!2");
        }
        diff.shrink_to_fit();
        return diff;
    };


    LongNum operator*(const LongNum& b) const {
        LongNum mult((uint64_t)0, this->length + b.length);
        uint16_t max = b.length;
        uint16_t min = this->length;
        if (b.length < this->length) {
            max = this->length;
            min = b.length;
        }
        LongNum t;
        int k = 0;
        if (max < 8) {
            for (int i = 0; i < b.length; i++) {
                int c = 0;
                LongNum s((uint64_t)0, this->length + b.length);
                for (int j = 0; j < this->length; j++) {
                    LongNum temp((uint64_t)0, max + 1);
                    t = mult_for64(this->number[j], b.number[i]);
                    temp.number[0] = t.number[0];
                    temp.number[1] = t.number[1];
                    s = s + (temp << c);
                    c = c + ARCHITECTURE_TYPE;
                }
                mult = mult + (s << k);
                k = k + ARCHITECTURE_TYPE;
            }
            mult.shrink_to_fit();
            return mult;
            
        }
        else {
            LongNum temp = *this;
            LongNum a0, a1, b0, b1, c0, c1, c2;
            if (max % 2 != 0) {
                max = max + 1;
            }
            pair<LongNum, LongNum> a = temp.kar_sep(max);
            uint64_t s = (max / 2) * ARCHITECTURE_TYPE;
            a1 = a.first;
            a0 = a.second;
            a = b.kar_sep(max);
            b1 = a.first;
            b0 = a.second;
            c0 = b0 * a0;
            c1 = a1 * b1;
            LongNum x = ((a1 + a0) * (b1 + b0));
            c2 = ((x - c1) - c0);
            c1.addzero(2*s / ARCHITECTURE_TYPE);
            c2.addzero(s / ARCHITECTURE_TYPE);
            mult = ((c1 << 2*s) + (c2<<s));
            mult = mult + c0;
            mult.shrink_to_fit();
            return mult;
        }
    }

    pair<LongNum, LongNum> divide(const LongNum& div) const {
        if (this->length < div.length) {
            return {0, *this};
        }
        if (div == 1) {
            return {*this, 0};
        }
        LongNum res((uint64_t)0, 1);
        LongNum q = *this;
        LongNum temp;
        uint64_t c;
        while (q > div || q == div) {
            temp = div;
            c = q.msb() - div.msb();
            temp.shift_l(c);
            temp.shrink_to_fit();
            if (temp > q){
                c = c-1;
                temp = (temp >> 1);
            }
            q = q - temp;
            res = res + LongNum(1, 1).shift_l(c);
        }
        res.shrink_to_fit();
        return {res, q};
    }

    LongNum operator/(const LongNum& div) const {
        return this->divide(div).first;
    }

    LongNum operator%(const LongNum& div) const {
        return this->divide(div).second;
    }

    uint64_t msb() const {
        uint64_t c = 0;
        uint64_t m = 1;
        m = (m << 63);
        while (!(m & this->number[length-1])) {
            c++;
            m = (m >> 1);
        }
        return ((this->length*ARCHITECTURE_TYPE) - c);
    }

    LongNum operator<<(uint64_t s) const {
        if (s == 0) {
            return *this;
        }
        int k, r;
        k = s / ARCHITECTURE_TYPE;
        r = s % ARCHITECTURE_TYPE;
        uint64_t carry = 0;
        uint64_t temp;
        LongNum res((uint64_t)0, this->length);
        for (int i = length - 1; i >= k; i--) {
            res.number[i] = number[i - k];
        }

        if (r != 0) {
            for (int i = 0; i < length; i++) {
                temp = ((res.number[i] >> (ARCHITECTURE_TYPE - r - 1)) >> 1);
                res.number[i] = ((res.number[i] << r) | carry);
                carry = temp;
            }
        }
        return res;
    }

    LongNum operator>>(uint64_t s) const {
        if (s == 0) {
            return *this;
        }
        int k = s / ARCHITECTURE_TYPE;
        int r = s % ARCHITECTURE_TYPE;
        uint64_t carry = 0;
        uint64_t temp;
        LongNum res((uint64_t)0, this->length);
        for (int i = 0; i < length - k; i++) {
            res.number[i] = number[i + k];
        }
        if (r != 0) {
            for (int i = length - 1; i >= 0; i--) {
                temp = ((res.number[i] << (ARCHITECTURE_TYPE - r - 1)) << 1);
                res.number[i] = ((res.number[i] >> r) | carry);
                carry = temp;
            }
        }
        return res;
    }
    LongNum& operator=(const LongNum& b){
        this->length = b.length;
        number = new uint64_t[length];
        for (uint16_t i = 0; i < length; i++) {
            this->number[i] = b.number[i];
        }
        return *this;
    }
    LongNum& operator=(LongNum&& b) {
        if (this->number)
            delete[] this->number;
        this->number = b.number;
        b.number = nullptr;
        this->length = b.length;
        return *this;
    }
    static LongNum mult_for64(uint64_t a, uint64_t b) {
        uint64_t a1, a2, b1, b2;
        LongNum res((uint64_t)0, 2);
        LongNum c1((uint64_t)0, 2);
        LongNum c2((uint64_t)0, 2);
        a1 = (a >> 32);
        a2 = ((a << 32) >> 32);
        b1 = (b >> 32);
        b2 = ((b << 32) >> 32);
        c1.number[0] = a1 * b2;
        c2.number[0] = a2 * b1;
        LongNum s = (c1 + c2);
        s = (s << 32);
        res.number[1] = (a1 * b1);
        res.number[0] = (a2 * b2);
        LongNum res1 = (res + s);
        return res1;
    }

    bool operator==(const LongNum& b) const {
        if (this->length != b.length)
            return false;
        for (int i = 0; i < length; i++) {
            if (number[i] != b.number[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator>(const LongNum& b) const {
        if (this->length > b.length)
            return true;
        else if (b.length > this->length)
            return false;
        for (int i = length - 1; i >= 0; i--) {
            if (number[i] > b.number[i])
                return true;
            else if(b.number[i] > number[i])
                return false;
        }
        return false;
    }

    LongNum& shift_r(uint64_t j) {
        this->addzero(ceil((double)j / (double)ARCHITECTURE_TYPE));
        *this = (*this >> j);
        this->shrink_to_fit();
        return *this;
    }

    LongNum& shift_l(uint64_t j) {
        this->addzero(ceil((double)j / (double)ARCHITECTURE_TYPE));
        *this = (*this << j);
        this->shrink_to_fit();
        return *this;
    }


//--------------------------------------------------------------------------------------------

    static LongNum gcd(const LongNum& a, const LongNum& b){
    LongNum r1 = a;
    LongNum r2 = b;
    LongNum temp;
    while(!(r2 == 0)){
        temp = r2;
        r2 = r1 % r2;
        r1 = temp;
    }
    return r1;
    };

    static LongNum lcm(const LongNum& a, const LongNum& b){
        return ((a * b) / gcd(a,b));
    }

    static LongNum reduc_barret(const LongNum& a, const LongNum& n) {
        LongNum mu(1, 1);
        mu.shift_l(2 * n.msb());
        mu = mu / n;
        LongNum q1 = a;
        q1.shift_r(n.msb() - 1);
        q1 = q1 * mu;
        q1.shift_r(n.msb() + 1);
        LongNum r = (a - (q1 * n));
        while (r > n || r == n)
            r = r - n;
        return r;
    }

    static LongNum barret(const LongNum& a, const LongNum& n, const LongNum& mu){
        LongNum q1 = a;
        q1.shift_r(n.msb() - 1);
        q1 = q1 * mu;
        q1.shift_r(n.msb() + 1);
        LongNum r = (a - (q1 * n));
        while (r > n || r == n)
            r = r - n;
        return r;
    }


    static LongNum pow64mod(LongNum& a, uint64_t pow, const LongNum& n, const LongNum& mu){
        uint64_t m = 1;
        m = ((m << 62) << 1);
        bool t;
        LongNum res(1,1);
        for (int i = 63; i >= 0; i--){
            t = (pow & m);
            if(t != 0){
                res = barret(res * a, n, mu);
            }
            if(i != 0)
                res = barret(res * res, n, mu);
            m = (m << 1);
        }
        return res;
    }

    static LongNum powmod(const LongNum& a, const LongNum& b, const LongNum& n) {
        LongNum mu(1, 1);
        mu.shift_l(2 * n.msb());
        mu = mu / n;
        /*uint64_t m = 1;
        bool t;*/
        LongNum temp = a % n;
        LongNum res(1, 1);
        //for (int j = b.length - 1; j >= 0; j--) {
          /*  for (int i = 63; i >= 0; i--) {
                m = (m << 1);
                t = (b.number[j] & m);
                if (t == 1) {
                    res = barret(res * temp, n, mu);
                }
                if (i != 0)
                    res = barret(res * res, n, mu);
            }*/
        //}
        for (int i = b.length - 1; i >= 0; i--) {
            res = res + pow64mod(temp, b.number[i], n, mu);
        }
        return res;
    }
};



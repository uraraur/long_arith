#include <iostream>
#include <limits>
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
        while (a != 0)
        {
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
        for (int i = 0; i < this->length; i++) 
        {
            temp[i] = this->number[i];
        }
        for (int i = this->length; i < this->length + j; i++) 
        {
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
        delete[] number;
    };
    void print() const {
        for (int i = length - 1; i >= 0; i--) {
            cout << number[i] << " ";
        }
        cout << endl;
    }
    void printhex() const {
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
       // while (temp[0] == '0')
          //  temp.erase(0, 1);
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
            sum.number[i] = sum.number[i] + c;
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
        if (this->length < b.length) {
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
            diff.number[i] = diff.number[i] - d;
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
        LongNum tilt;
        int k = 0;
        if (this->length < 8) {
            for (int i = 0; i < b.length; i++) {
                int c = 0;
                LongNum s((uint64_t)0, this->length + b.length);
                for (int j = 0; j < this->length; j++) {
                    LongNum temp((uint64_t)0, max + 1);
                    tilt = mult_for64(this->number[j], b.number[i]);
                    temp.number[0] = tilt.number[0];
                    temp.number[1] = tilt.number[1];
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
            a1 = a.first;
            a1.print();
            cout << endl;
            a0 = a.second;
            a0.print();
            cout << endl;
            a = b.kar_sep(max);
            b1 = a.first;
            b1.print();
            cout << endl;
            b0 = a.second;
            b0.print();
            cout << endl;
            c0 = b0 * a0;
            c0.print();
            cout << endl;
            c1 = a1 * b1;
            c1.print();
            cout << endl;
            c2 = ((((a1 + a0) * (b1 + b0)) - c1) - c2);
            c2.print();
            cout << endl;
            uint64_t s = (max / 2) * ARCHITECTURE_TYPE;
            mult = (c1 >> (s*2)) + (c2 >> s) + c0;
            return mult;
        }
    }
    LongNum operator<<(uint64_t s) const {
        if (s == 0) {
            return *this;
        }
        uint64_t k, r;
        k = s / ARCHITECTURE_TYPE;
        r = s % ARCHITECTURE_TYPE;
        uint64_t carry = 0;
        uint64_t temp;
        LongNum res((uint64_t)0, this->length);
        if (k != 0) {
            for (int i = length - 1; i >= k; i--) {
                res.number[i] = number[i - k];
            }
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
        uint64_t k = s / ARCHITECTURE_TYPE;
        uint64_t r = s % ARCHITECTURE_TYPE;
        uint64_t carry = 0;
        uint64_t temp;
        LongNum res((uint64_t)0, this->length);
        if (k != 0) {
            for (int i = 0; i < length - k ; i++) {
                res.number[i] = number[i + k];
            }
        }
        if (r != 0) {
            for (int i = length - 1; i > 0; i--) {
                temp = (res.number[i] << (ARCHITECTURE_TYPE - r));
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
        LongNum c1((uint64_t)0, 1);
        LongNum c2((uint64_t)0, 1);
        a1 = (a >> 32);
        a2 = ((a << 32) >> 32);
        b1 = (b >> 32);
        b2 = ((b << 32) >> 32);
        c1.number[0] = a1 * b2;
        c2.number[0] = a2 * b1;
        LongNum s((uint64_t)0, 2);
        s = c1 + c2;
        s << 32;
        res.number[1] = (a1 * b1);
        res.number[0] = (a2 * b2);
        LongNum res1 = res + s;
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

};

int main() {
    /*
    string A = "D4D2110984907B5625309D956521BAB4157B8B1ECE04043249A3D379AC112E5B9AF44E721E148D88A942744CF56A06B92D28A0DB950FE4CED2B41A0BD38BCE7D0BE1055CF5DE38F2A588C2C9A79A75011058C320A7B661C6CE1C36C7D870758307E5D2CF07D9B6E8D529779B6B2910DD17B6766A7EFEE215A98CAC300F2827DB";
    string B = "3A7EF2554E8940FA9B93B2A5E822CC7BB262F4A14159E4318CAE3ABF5AEB1022EC6D01DEFAB48B528868679D649B445A753684C13F6C3ADBAB059D635A2882090FC166EA9F0AAACD16A062149E4A0952F7FAAB14A0E9D3CB0BE9200DBD3B0342496421826919148E617AF1DB66978B1FCD28F8408506B79979CCBCC7F7E5FDE7";
    string C = "10F51035ED319BC50C0C4503B4D44872FC7DE7FC00F5DE863D6520E3906FC3E7E8761505118C918DB31AADBEA5A054B13A25F259CD47C1FAA7DB9B76F2DB450861BA26C4794E8E3BFBC2924DE45E47E5408536E3548A03591DA0556D595AB78C55149F45170F2CB7736A46976D1C09BFCE4DF6EAB040599AF235968F8070E25C2";
    LongNum AA = LongNum::readhex(A);
    LongNum BB = LongNum::readhex(B);
    LongNum CC = LongNum::readhex(C);
    LongNum CCC = AA + BB;
    //AA.print()
    CC.printhex();
    cout << endl;
    CCC.printhex();
    cout << endl;
    cout << (CCC == CC) << endl;
    */
    uint64_t* number1 = new uint64_t[10];
    uint64_t* number2 = new uint64_t[10];
    for (int i = 0; i < 10; i++)
    {
        number1[i] = rand() % 100;
    }
    for (int i = 0; i < 10; i++)
    {
        number2[i] = rand() % 100;
    }
    LongNum a(number1, 10);
    LongNum b(number2, 10);
    a.print();
    cout << endl;
    a.printhex();
    cout << endl;
    b.print();
    cout << endl;
    b.printhex();
    cout << endl;
    LongNum c = a * b;
    c.print();
    cout << endl;
    c.printhex();
  

}
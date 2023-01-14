#include "lab1_2.cpp"
#include "lab3.cpp"
#include "lab4.cpp"

int main() {
    //string A = "4D3C9";
    //string B = "DAF1ABDA4AD4D9FE3E36A529210C2AE99B905922FC0519798A26E351FE23AF375AD6BA288EE030B70DF0CE1CDF1E8B75BA56494DC6ED36B181814CD5783E6C81";
    //string N = "2AB3";
    //////std::transform(C.begin(), C.end(), C.begin(), [](char c) { return std::toupper(c); });
    //LongNum AA = LongNum::readhex(A);
    ////AA.print();
    //LongNum BB = LongNum::readhex(B);
    ////BB.print();
    //LongNum NN = LongNum::readhex(N);
    //LongNum C = LongNum::powmod(AA, LongNum(1, 1), NN);
    //C.printhex();
    //cout << endl;
    //LongNum CC = AA % NN;
    //CC.printhex();
    //cout << endl;
    //cout << (CC == C) << endl;

    array<int, 3> hi = { 1, 2, 3 };
    for (auto c : hi)
        cout << c;
    cout << endl;
    rotate(hi.begin(), hi.end() - 2, hi.end());
    for (auto c : hi)
        cout << c;

}
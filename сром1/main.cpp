
//#include "lab1_2.cpp"
//#include "lab3.cpp"
#include "lab4.cpp"


int main() {
    //cout << " + ";
    //string A = "D4D2110984907B5625309D956521BAB4157B8B1ECE04043249A3D379AC112E5B9AF44E721E148D88A942744CF56A06B92D28A0DB950FE4CED2B41A0BD38BCE7D0BE1055CF5DE38F2A588C2C9A79A75011058C320A7B661C6CE1C36C7D870758307E5D2CF07D9B6E8D529779B6B2910DD17B6766A7EFEE215A98CAC300F2827DB";
    //string B = "3A7EF2554E8940FA9B93B2A5E822CC7BB262F4A14159E4318CAE3ABF5AEB1022EC6D01DEFAB48B528868679D649B445A753684C13F6C3ADBAB059D635A2882090FC166EA9F0AAACD16A062149E4A0952F7FAAB14A0E9D3CB0BE9200DBD3B0342496421826919148E617AF1DB66978B1FCD28F8408506B79979CCBCC7F7E5FDE7";
    //string AplusB = "10F51035ED319BC50C0C4503B4D44872FC7DE7FC00F5DE863D6520E3906FC3E7E8761505118C918DB31AADBEA5A054B13A25F259CD47C1FAA7DB9B76F2DB450861BA26C4794E8E3BFBC2924DE45E47E5408536E3548A03591DA0556D595AB78C55149F45170F2CB7736A46976D1C09BFCE4DF6EAB040599AF235968F8070E25C2";
    //LongNum AA = LongNum::readhex(A);
    //LongNum BB = LongNum::readhex(B);
    //LongNum AAplusBB = LongNum::readhex(AplusB);
    //LongNum C1 = AA + BB;
    //C1.printhex();
    //cout << (AAplusBB == C1) << endl;

    //cout << " - ";
    //string AminusB = "9A531EB436073A5B899CEAEF7CFEEE386318967D8CAA2000BCF598BA51261E38AE874C932360023620DA0CAF90CEC25EB7F21C1A55A3A9F327AE7CA879634C73FC1F9E7256D38E258EE860B509506BAE185E180C06CC8DFBC23316BA1B357240BE81B14C9EC0A25A73AE85C0049185BD4A8D7E29F9F82A7C2FBFEF68174229F4";
    //LongNum AAminusBB = LongNum::readhex(AminusB);
    //LongNum C2 = AA - BB;
    //C2.printhex();
    //cout << (AAminusBB == C2) << endl;

    //cout << " * ";
    //string AmultB = "30A120B609DCBE28B09CA92E12DD29D77AE6400DC22B026AFB5FB945AAF62B57F4E48BD299261F02BBB35DD2495B5CD2713BF0E30192DAE1B334659160C8552423F0AD7FB82870920DF4E9B57980EAD2ADA9F3EF4B5D0718AB7F1053700395278998CB9AD48498D65150E3E837B0BB169D432B441424557061F838A17C65F90A31105F599BF69B87485BF9C70F51D37A417E476E372558C26782AC8C8F35C3D1227E851D8A72CD708700FD90C5E17F22C4EA15730345E56BD76F04B54580813CBE306B4404C6F34BCD9840D2911E6B3CF6DE3EE428C274EDF0A97335D8256DA26FCD67BA5450593A15F6B527ECE76FBBE20F7A882347614AF4B7FAF55086659D";
    //LongNum AAmultBB = LongNum::readhex(AmultB);
    //LongNum C3 = AA * BB;
    //C3.printhex();
    //cout << (AAmultBB == C3) << endl;

    //cout << " (a*b)/b";
    //LongNum C4 = (AA * BB) / BB;
    //C4.printhex();
    //cout << (AA == C4) << endl;

    //cout << "mod" << endl;
    //LongNum Cr = (C3 + C2) % BB;
    //LongNum Cq = (C3 + C2) / BB;
    //cout << (Cr + (BB*Cq) == C3 + C2) << endl;

    //cout << "^" << endl;
    //LongNum C5 = LongNum::reduc_barret(AA, BB);
    //LongNum C6 = AA % BB;
    //cout << (C6 == C5) << endl;

    //cout << "gcd" << endl;
    //LongNum C7 = LongNum::gcd(AA, BB);
    //C7.printhex();

    //cout << "lcm" << endl;
    //LongNum C8 = LongNum::lcm(AA, BB);
    //C8.printhex();



    //cout << "--------------------------------------------------" << endl;

    //vector<bool> v =  { 1, 1, 0 , 0 , 1, 1};
    //PBV a(v);
    //cout << a.to_string() << endl;
    //PBV a1 = a + a;
    //cout << "                           +" << endl;
    //cout << (a1 == PBV()) << endl;
    //PBV a2 = a.inv();
    //cout << a2.to_string() << endl;
    //PBV a3 = a * a;
    //cout << (a3 == a.pow2()) << endl;
    //cout << "inserse:    " << (a * a2).to_string() << endl;
    //cout << "trace:   " <<  a.trace().to_string() << endl;
    //cout << "pow2:  " << a.pow2().to_string() << endl;

    //cout << "--------------------------------------------------" << endl;
    
    vector<bool> v = { 1, 1, 0 , 0 , 1, 1 };
    ONB::precalc_matrix();
    ONB b(v);
    ONB b1 = b + b;
    cout << "                           +" << endl;
    cout << (b1 == ONB()) << endl;
    ONB b2 = b.inv();
    cout << "inserse:    " << (b * b2).to_string() << endl;
    cout << "trace:   " << b.trace().to_string() << endl;
    cout << "pow2:  " << b.pow2().to_string() << endl;


}
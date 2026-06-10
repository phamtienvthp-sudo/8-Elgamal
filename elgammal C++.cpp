#include <bits/stdc++.h>

using namespace std;

class ElGamal {
public:
	  bool isPrime(long long n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (long long i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }
    long long modPow(long long base, long long exp, long long mod) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return result;
    }

    long long extendedGCD(long long a, long long b, long long &x, long long &y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        long long x1, y1;
        long long gcd = extendedGCD(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return gcd;
    }

    long long modInverse(long long a, long long mod) {
        long long x, y;
        long long gcd = extendedGCD(a, mod, x, y);
        if (gcd != 1) return -1;
        return (x % mod + mod) % mod;
    }
};

string base64Encode(const vector<unsigned char>& data) {
    static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string ret;
    int i = 0, j = 0;
    unsigned char char_array_3[3], char_array_4[4];

    for (size_t k = 0; k < data.size(); k++) {
        char_array_3[i++] = data[k];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for(i = 0; i < 4; i++) ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        for(j = i; j < 3; j++) char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];
        while((i++ < 3)) ret += '=';
    }
    return ret;
}

vector<unsigned char> base64Decode(const string& encoded_string) {
    static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i = 0, j = 0;
    unsigned char char_array_4[4], char_array_3[3];
    vector<unsigned char> ret;

    for (size_t in_ = 0; in_ < encoded_string.size(); in_++) {
        char c = encoded_string[in_];
        if (c == '=') break;
        if (!isalnum(c) && c != '+' && c != '/') continue;

        char_array_4[i++] = c;
        if (i == 4) {
            for (i = 0; i < 4; i++) char_array_4[i] = base64_chars.find(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; i < 3; i++) ret.push_back(char_array_3[i]);
            i = 0;
        }
    }
    if (i) {
        for (j = i; j < 4; j++) char_array_4[j] = 0;
        for (j = 0; j < 4; j++) char_array_4[j] = base64_chars.find(char_array_4[j]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (j = 0; j < i - 1; j++) ret.push_back(char_array_3[j]);
    }
    return ret;
}

string elgamalEncrypt(const string& banRo, long long q, long long a, long long ya, long long k, long long &outC1, long long &outC2) {
    ElGamal elgamal;
    vector<unsigned char> danhSachC1C2;

    for (size_t idx = 0; idx < banRo.size(); idx++) {
        long long m = static_cast<unsigned char>(banRo[idx]);
        long long c1 = elgamal.modPow(a, k, q);
        long long c2 = (m * elgamal.modPow(ya, k, q)) % q;

        if (idx == banRo.size() - 1) {
            outC1 = c1;
            outC2 = c2;
        }

        danhSachC1C2.push_back((c1 >> 24) & 0xFF);
        danhSachC1C2.push_back((c1 >> 16) & 0xFF);
        danhSachC1C2.push_back((c1 >> 8) & 0xFF);
        danhSachC1C2.push_back(c1 & 0xFF);

        danhSachC1C2.push_back((c2 >> 24) & 0xFF);
        danhSachC1C2.push_back((c2 >> 16) & 0xFF);
        danhSachC1C2.push_back((c2 >> 8) & 0xFF);
        danhSachC1C2.push_back(c2 & 0xFF);
    }

    if (banRo.empty()) {
        outC1 = elgamal.modPow(a, k, q);
        outC2 = 0;
    }

    return base64Encode(danhSachC1C2);
}

string elgamalDecrypt(const string& banMaBase64, long long q, long long xa) {
    ElGamal elgamal;
    vector<unsigned char> danhSachC1C2 = base64Decode(banMaBase64);
    string ketQuaBanRo = "";

    for (size_t i = 0; i < danhSachC1C2.size(); i += 8) {
        if (i + 7 >= danhSachC1C2.size()) break;

        long long c1 = 0;
        c1 |= static_cast<long long>(danhSachC1C2[i]) << 24;
        c1 |= static_cast<long long>(danhSachC1C2[i+1]) << 16;
        c1 |= static_cast<long long>(danhSachC1C2[i+2]) << 8;
        c1 |= static_cast<long long>(danhSachC1C2[i+3]);

        long long b_val = 0;
        b_val |= static_cast<long long>(danhSachC1C2[i+4]) << 24;
        b_val |= static_cast<long long>(danhSachC1C2[i+5]) << 16;
        b_val |= static_cast<long long>(danhSachC1C2[i+6]) << 8;
        b_val |= static_cast<long long>(danhSachC1C2[i+7]);

        long long s = elgamal.modPow(c1, xa, q);
        long long s_inv = elgamal.modInverse(s, q);
        
        if (s_inv == -1) continue;

        long long m = (b_val * s_inv) % q;
        if (m < 0) m = (m + q) % q;

        ketQuaBanRo += static_cast<char>(m);
    }
    return ketQuaBanRo;
}

int main() {
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    ElGamal elgamal;
    long long q, a, xa, k;
      do {
        cout << "Nhap so nguyen to Q: ";
        cin >> q;
        if (!elgamal.isPrime(q)) {
            cout << "Q chua phai so nguyen to, moi ban nhap lai." << endl << endl;
        }
    } while (!elgamal.isPrime(q));
    cout << "Nhap can nguyen thuy a: ";
    cin >> a;
    cout << "Nhap khoa bi mat Xa: ";
    cin >> xa;
    cout << "Nhap khoa ngau nhien k: ";
    cin >> k;
    
    string dummy;
    getline(cin, dummy);

    long long ya = elgamal.modPow(a, xa, q);

    cout << "\n=== HE THONG KHOA DA TINH TOAN ===" << endl;
    cout << "-> Gia tri Ya tinh duoc: " << ya << endl;
    cout << "-> Khoa cong khai Kpub: (" << q << ", " << a << ", " << ya << ")" << endl;
    cout << "-> Khoa bi mat Kpr: (" << q << ", " << xa << ")" << endl;
    cout << "==================================" << endl << endl;

    string banRoNhap;
    cout << "Nhap ban ro bat ky (Moi ky tu tren ban phim): ";
    getline(cin, banRoNhap);

    long long hienC1 = 0, hienC2 = 0;
    string banMaBase64 = elgamalEncrypt(banRoNhap, q, a, ya, k, hienC1, hienC2);

    cout << "\n-> Ket qua C1: " << hienC1 << endl;
    cout << "-> Ket qua C2: " << hienC2 << endl;
    cout << "-> BAN MA THU DUOC (Chuoi chu Base64):\n" << banMaBase64 << endl << endl;

    string banRoGiaiMa = elgamalDecrypt(banMaBase64, q, xa);
    cout << "-> KET QUA SAU KHI GIAI MA:\n" << banRoGiaiMa << endl;

    return 0;
}

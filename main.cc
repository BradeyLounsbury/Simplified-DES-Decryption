// This is a program that performs simplified DES decryption
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

void p10(bitset<10> &key_bits);
bitset<5> shift(bitset<5> half_key);
bitset<8> p8(bitset<5> left, bitset<5> right);
bitset<8> IP(bitset<8> ch);
bitset<8> EP(bitset<4> half);
bitset<4> Sub(bitset<8> exp, bitset<8> key, bitset<4> left, bitset<4> right);
bitset<2> get_lsub(bitset<4> left);
bitset<2> get_rsub(bitset<4> right);
bitset<4> p4(bitset<2> left, bitset<2> right);
bitset<8> IP_1(bitset<4> left, bitset<4> right);


int main() {
    string cipher_text, key;

    getline(cin, cipher_text);
    getline(cin, key);
    
    if (!(key.length() == 10)) {
        cout << "key not 10 bits\n";
        return -1;
    }
    
    bitset<10> key_bits (key);

    // key gen
    p10(key_bits);
    
    bitset<5> left_half, right_half;
    
    for (size_t i = 0; i < 5; i++) {
        right_half[i] = key_bits[i];
        left_half[i] = key_bits[i+5];
    }
    
    bitset<5> left_shift_1 = shift(left_half);
    bitset<5> right_shift_1 = shift(right_half);
    bitset<8> k1 = p8(left_shift_1, right_shift_1);

    bitset<5> left_shift_2 = shift(left_shift_1);
    left_shift_2 = shift(left_shift_2);
    bitset<5> right_shift_2 = shift(right_shift_1);
    right_shift_2 = shift(right_shift_2);
    bitset<8> k2 = p8(left_shift_2, right_shift_2);
    // key gen done

    cout << "key: " << key << endl;
    cout << "key_bits after p10: " << key_bits << endl;
    cout << "left half: " << left_half << endl;
    cout << "right half: " << right_half << endl;
    cout << "left half shifted 1: " << left_shift_1 << endl;
    cout << "right half shifted 1: " << right_shift_1 << endl;
    cout << "left half shifted 2: " << left_shift_2 << endl;
    cout << "right half shifted 2: " << right_shift_2 << endl;
    cout << "k1: " << k1 << endl;
    cout << "k2: " << k2 << endl;

    // decrypt
    bitset<8> payload;
    for (size_t i = 0; i < cipher_text.length(); i++) {
        bitset<8> cipher_ch ((int)cipher_text[i]);
        cout << "cipher_ch: " << cipher_ch << endl;
 
        cipher_ch = IP(cipher_ch);
        cout << "IP: " << cipher_ch << endl;

        bitset<4> left, right;
        for (size_t i = 0; i < 4; i++) {
            right[i] = cipher_ch[i];
            left[i] = cipher_ch[i+4];
        }
        cout << "left of IP: " << left << endl;
        cout << "right of IP: " << right << endl;

        bitset<8> expand (EP(right));
        cout << "expand of right half: " << expand << endl;
        bitset<4> feistel (Sub(expand, k1, left, right));
        cout << "feistel 1: " << feistel << endl;

        //feistel and right swap
        bitset<8> expand_2 (EP(feistel));
        cout << "expand 2 of right half: " << expand_2 << endl;
        bitset<4> feistel_2 (Sub(expand_2, k2, right, feistel));
        cout << "feistel 2: " << feistel_2 << endl;

        payload = IP_1(feistel, feistel_2);
    }
    
    cout << payload << endl;
    
    
    return 0;
}

void p10(bitset<10> &key_bits) {
    bitset<10> key_perm = key_bits;
    key_bits[0] = key_perm[4];
    key_bits[1] = key_perm[2];
    key_bits[2] = key_perm[1];
    key_bits[3] = key_perm[9];
    key_bits[4] = key_perm[0];
    key_bits[5] = key_perm[6];
    key_bits[6] = key_perm[3];
    key_bits[7] = key_perm[8];
    key_bits[8] = key_perm[5];
    key_bits[9] = key_perm[7];
}

bitset<5> shift(bitset<5> half_key) {
    if (half_key.test(4)) {
        half_key = half_key<<1;
        half_key.set(0,1);
    }
    else {
        half_key = half_key<<1;
    }
    return half_key;
}

bitset<8> p8(bitset<5> left, bitset<5> right) {
    bitset<8> perm;

    perm[7] = right[4];
    perm[6] = left[2];
    perm[5] = right[3];
    perm[4] = left[1];
    perm[3] = right[2];
    perm[2] = left[0];
    perm[1] = right[0];
    perm[0] = right[1];
    return perm;
}

bitset<8> IP(bitset<8> ch) {
    bitset<8> perm;

    perm[7] = ch[6];
    perm[6] = ch[2];
    perm[5] = ch[5];
    perm[4] = ch[7];
    perm[3] = ch[4];
    perm[2] = ch[0];
    perm[1] = ch[3];
    perm[0] = ch[1];
    return perm;
}

bitset<8> EP(bitset<4> half) {
    bitset<8> exp;

    exp[7] = half[0];
    exp[6] = half[3];
    exp[5] = half[2];
    exp[4] = half[1];
    exp[3] = half[2];
    exp[2] = half[1];
    exp[1] = half[0];
    exp[0] = half[3];
    return exp;
}

bitset<4> Sub(bitset<8> exp, bitset<8> key, bitset<4> left, bitset<4> right) {
    exp = exp ^ key;    // xor

    cout << "xor: " << exp << endl;

    bitset<2> lsub, rsub;
    lsub = get_lsub(left);
    rsub = get_rsub(right);

    cout << "lsub: " << lsub << endl;
    cout << "rsub: " << rsub << endl;
    
    bitset<4> perm;
    perm = p4(lsub, rsub);

    cout << "p4: " << perm << endl;

    return perm ^ left;
}

bitset<2> get_lsub(bitset<4> left) {
    string sub;
    if (left.test(3)) {
        if (left.test(0)) {         // row: 3
            if (left.test(2)) {
                if (left.test(1)) { // column: 3
                    sub = "10";
                }
                else {              // column: 2
                    sub = "11";
                }
            }
            else {
                if (left.test(1)) { // column: 1
                    sub = "01";
                }
                else {              // column: 0
                    sub = "11";
                }
            }
        }
        else {                      // row: 2
            if (left.test(2)) {
                if (left.test(1)) { // column: 3
                    sub = "11";
                }
                else {              // column: 2
                    sub = "01";
                }
            }
            else {
                if (left.test(1)) { // column: 1
                    sub = "10";
                }
                else {              // column: 0
                    sub = "00";
                }
            }
        }
    }
    else {
        if (left.test(0)) {         // row: 1
            if (left.test(2)) {
                if (left.test(1)) { // column: 3
                    sub = "00";
                }
                else {              // column: 2
                    sub = "01";
                }
            }
            else {
                if (left.test(1)) { // column: 1
                    sub = "10";
                }
                else {              // column: 0
                    sub = "11";
                }
            }
        }
        else {                      // row: 0
            if (left.test(2)) {
                if (left.test(1)) { // column: 3
                    sub = "10";
                }
                else {              // column: 2
                    sub = "11";
                }
            }
            else {
                if (left.test(1)) { // column: 1
                    sub = "00";
                }
                else {              // column: 0
                    sub = "01";
                }
            }
        }
    }
    bitset<2> lsub (sub);
    return lsub;
}

bitset<2> get_rsub(bitset<4> right) {
    string sub;
    if (right.test(3)) {
        if (right.test(0)) {         // row: 3
            if (right.test(2)) {
                if (right.test(1)) { // column: 3
                    sub = "11";
                }
                else {              // column: 2
                    sub = "00";
                }
            }
            else {
                if (right.test(1)) { // column: 1
                    sub = "01";
                }
                else {              // column: 0
                    sub = "10";
                }
            }
        }
        else {                      // row: 2
            if (right.test(2)) {
                if (right.test(1)) { // column: 3
                    sub = "00";
                }
                else {              // column: 2
                    sub = "01";
                }
            }
            else {
                if (right.test(1)) { // column: 1
                    sub = "00";
                }
                else {              // column: 0
                    sub = "11";
                }
            }
        }
    }
    else {
        if (right.test(0)) {         // row: 1
            if (right.test(2)) {
                if (right.test(1)) { // column: 3
                    sub = "11";
                }
                else {              // column: 2
                    sub = "01";
                }
            }
            else {
                if (right.test(1)) { // column: 1
                    sub = "00";
                }
                else {              // column: 0
                    sub = "10";
                }
            }
        }
        else {                      // row: 0
            if (right.test(2)) {
                if (right.test(1)) { // column: 3
                    sub = "11";
                }
                else {              // column: 2
                    sub = "10";
                }
            }
            else {
                if (right.test(1)) { // column: 1
                    sub = "01";
                }
                else {              // column: 0
                    sub = "00";
                }
            }
        }
    }
    bitset<2> lsub (sub);
    return lsub;
}

bitset<4> p4(bitset<2> left, bitset<2> right) {
    bitset<4> perm;

    perm[3] = left[0];
    perm[2] = right[0];
    perm[1] = right[1];
    perm[0] = left[1];
    return perm;
}

bitset<8> IP_1(bitset<4> left, bitset<4> right) {
    bitset<8> perm;

    perm[7] = left[0];
    perm[6] = left[3];
    perm[5] = left[1];
    perm[4] = right[3];
    perm[3] = right[1];
    perm[2] = left[2];
    perm[1] = right[0];
    perm[0] = right[2];
    return perm;
}
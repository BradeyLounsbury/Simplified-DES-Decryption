// This is a program that performs simplified DES decryption
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

void p10(bitset<10> &key_bits);
bitset<5> shift(bitset<5> half_key);
bitset<8> p8(bitset<5> left, bitset<5> right);


int main() {
    string cipher_text, key;

    // getline(cin, cipher_text);
    getline(cin, key);
    
    if (!(key.length() == 10)) {
        cout << "key not 10 bits\n";
        return -1;
    }
    
    bitset<10> key_bits (key);

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
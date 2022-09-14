// This is a program that performs simplified DES decryption
#include <iostream>
#include <string>
#include <bitset>

using namespace std;

int* p10(int key_bits[10]);
int* shift_1(int half_key[5]);
int* shift_2(int half_key[5]);
int* p8(int* left, int* right);


int main() {
    string cipher_text, key;
    int cipher_text_bits[8], key_bits[10];

    getline(cin, cipher_text);
    getline(cin, key);
    
    for (size_t i = 0; i < 10; i++) {
        if(key[i] == '0') {
            key_bits[i] = 0;
        }
        else if (key[i] == '1') {
            key_bits[i] = 1;
        }
        else {
            cout << "key incorrectly formatted\n";
            return -1;
        }
    }

    int* key_perm = p10(key_bits);
    int left_half[5], right_half[5];
    
    for (size_t i = 0; i < 5; i++) {
        left_half[i] = key_bits[i];
        right_half[i] = key_bits[i+5];
    }
    
    int* left_shift_1 = shift_1(left_half);
    int* right_shift_1 = shift_1(right_half);
    int* k1 = p8(left_shift_1, right_shift_1);

    int* left_shift_2 = shift_2(left_half);
    int* right_shift_2 = shift_2(right_half);
    int* k2 = p8(left_shift_2, right_shift_2);


    
    return 0;
}

int* p10(int key_bits[10]) {
    static int key_perm[10];
    key_perm[0] = key_bits[2];
    key_perm[1] = key_bits[4];
    key_perm[2] = key_bits[1];
    key_perm[3] = key_bits[6];
    key_perm[4] = key_bits[3];
    key_perm[5] = key_bits[9];
    key_perm[6] = key_bits[0];
    key_perm[7] = key_bits[8];
    key_perm[8] = key_bits[7];
    key_perm[9] = key_bits[5];
    return key_perm;
}

int* shift_1(int half_key[5]) {
    static int shifted[5];
    shifted[0] = half_key[1];
    shifted[1] = half_key[2];
    shifted[2] = half_key[3];
    shifted[3] = half_key[4];
    shifted[4] = half_key[0];
    return shifted;
}

int* shift_2(int half_key[5]) {
    static int shifted[5];
    shifted[0] = half_key[2];
    shifted[1] = half_key[3];
    shifted[2] = half_key[4];
    shifted[3] = half_key[0];
    shifted[4] = half_key[1];
    return shifted;
}

int* p8(int* left, int* right) {
    static int perm[8];
    perm[0] = *(right);
    perm[1] = *(left + 2);
    perm[2] = *(right + 1);
    perm[3] = *(left + 3);
    perm[4] = *(right + 2);
    perm[5] = *(left + 4);
    perm[6] = *(right + 4);
    perm[7] = *(right + 3);
    return perm;
}
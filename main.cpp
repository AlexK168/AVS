#include <iostream>
#include <mmintrin.h>

void calculate(const int8_t *A, const int8_t *B, const int8_t *C, const int16_t *D) {
    // required operation:
    // F[i]=(A[i]-B[i])*(C[i]-D[i]) , i=1...8;

    // moving data to registers
    auto mm_0 = (__m64*)A;
    auto mm_1 = (__m64*)B;
    auto mm_2 = (__m64*)C;
    auto mm_4 = (__m64*)D;
    auto mm_3 = (__m64*)(&D[4]);
    auto mm_5 = _mm_cvtsi64_m64(0);
    auto mm_6 = _mm_cvtsi64_m64(0);
    auto mm_7 = _mm_cvtsi64_m64(0);

    // executing (A[i]-B[i]). storing result in mm_0
    *mm_0 = _mm_subs_pi8(*mm_0, *mm_1);

    // compare with zero to determine signs
    *mm_1 = _m_pcmpgtb(mm_7, *mm_2);

    // unpack C to mm_5 and mm_6
    mm_6 = _mm_unpacklo_pi8(*mm_2, *mm_1);
    mm_5 = _mm_unpackhi_pi8(*mm_2, *mm_1);

    // executing (C[i]-D[i]). storing result in mm_3, mm_4
    *mm_3 = _mm_subs_pi16(mm_5, *mm_3);
    *mm_4 = _mm_subs_pi16(mm_6, *mm_4);

    // compare with zero to determine signs
    *mm_1 = _m_pcmpgtb(mm_7, *mm_0);

    // unpack A - B to mm_1 and mm_2
    *mm_2 = _mm_unpacklo_pi8(*mm_0, *mm_1);
    *mm_1 = _mm_unpackhi_pi8(*mm_0, *mm_1);

    // (A[i]-B[i]) is stored in mm_1 and mm_2
    // (C[i]-D[i]) is stored in mm_3 and mm_4

    // multiplication
    mm_6 = _mm_mullo_pi16(*mm_1, *mm_3);
    mm_7 = _mm_mullo_pi16(*mm_2, *mm_4);

    // storing result in F
    int64_t r1 = _m_to_int64(mm_6);
    int64_t r2 = _m_to_int64(mm_7);

    int16_t F[8];
    F[0] = (int16_t)(r1 >> 48);
    F[1] = (int16_t)(r1 << 16 >> 48);
    F[2] = (int16_t)(r1 << 32 >> 48);
    F[3] = (int16_t)(r1 << 48 >> 48);
    F[4] = (int16_t)(r2 >> 48);
    F[5] = (int16_t)(r2 << 16 >> 48);
    F[6] = (int16_t)(r2 << 32 >> 48);
    F[7] = (int16_t)(r2 << 48 >> 48);

    for (auto i : F)
        std::cout << i << " ";

    std::cout << "\n";
}

int main() {
    //initial data
    /*
    124 -102 101 14 -18 58 6 11
    100 -7 -6 0 -4 3 -12 10
    8 7 6 5 4 3 2 1
    -3 4 5 -6 7 -8 9 10
     */

    int8_t A[8], B[8], C[8];
    int16_t D[8];
    int tmp = 0;

    std::cout << "Enter 8 nums from A \n";
    for (int i = 7; i >= 0; i--) {
        std::cin >> tmp;
        A[i] = tmp;
    }

    std::cout << "Enter 8 nums from B \n";
    for (int i = 7; i >= 0; i--) {
        std::cin >> tmp;
        B[i] = tmp;
    }

    std::cout << "Enter 8 nums from C \n";
    for (int i = 7; i >= 0; i--) {
        std::cin >> tmp;
        C[i] = tmp;
    }

    std::cout << "Enter 8 nums from D \n";
    for (int i = 7; i >= 0; i--) {
        std::cin >> tmp;
        D[i] = tmp;
    }

    for (int i = 7; i >= 0; i--)
        std::cout << (A[i] - B[i]) * (C[i] - D[i]) << " ";
    std::cout << "\n";

    calculate(A, B, C, D);



    return 0;
}

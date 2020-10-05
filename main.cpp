#include <iostream>
#include <mmintrin.h>

int main() {
    //initial data
    int8_t A[8]{ 124, -102, 101, 14, -18, 58, 6, 11 };
    int8_t B[8]{ 100, -7, -6, 0, -4, 3, -12, 10 };
    int8_t C[8]{ 8, 7, 6, 5, 4, 3, 2, 1 };
    int16_t D[8]{ -3, 4, 5, -6, 7, -8, 9, 10 };

    //resulting array
    int16_t F[8];

    // required operation:
    // F[i]=(A[i]-B[i])*(C[i]-D[i]) , i=1...8;

    // moving data to registers
    auto mm_0 = _mm_set_pi8(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7]);
    auto mm_1 = _mm_set_pi8(B[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7]);
    auto mm_2 = _mm_set_pi8(C[0], C[1], C[2], C[3], C[4], C[5], C[6], C[7]);
    auto mm_3 = _mm_set_pi16(D[0], D[1], D[2], D[3]);
    auto mm_4 = _mm_set_pi16(D[4], D[5], D[6], D[7]);
    auto mm_5 = _mm_cvtsi64_m64(0);
    auto mm_6 = _mm_cvtsi64_m64(0);
    auto mm_7 = _mm_cvtsi64_m64(0);

    // executing (A[i]-B[i]). storing result in mm_0
    mm_0 = _mm_subs_pi8(mm_0, mm_1);

    // compare with zero to determine signs
    mm_1 = _m_pcmpgtb(mm_7, mm_2);

    // unpack C to mm_5 and mm_6
    mm_6 = _mm_unpacklo_pi8(mm_2, mm_1);
    mm_5 = _mm_unpackhi_pi8(mm_2, mm_1);

    // executing (C[i]-D[i]). storing result in mm_3, mm_4
    mm_3 = _mm_subs_pi16(mm_5, mm_3);
    mm_4 = _mm_subs_pi16(mm_6, mm_4);

    // compare with zero to determine signs
    mm_1 = _m_pcmpgtb(mm_7, mm_0);

    // unpack A - B to mm_1 and mm_2
    mm_2 = _mm_unpacklo_pi8(mm_0, mm_1);
    mm_1 = _mm_unpackhi_pi8(mm_0, mm_1);

    // (A[i]-B[i]) is stored in mm_1 and mm_2
    // (C[i]-D[i]) is stored in mm_3 and mm_4

    // multiplication
    mm_6 = _mm_mullo_pi16(mm_1, mm_3);
    mm_7 = _mm_mullo_pi16(mm_2, mm_4);

    // storing result in F
    int64_t r1 = _m_to_int64(mm_6);
    int64_t r2 = _m_to_int64(mm_7);

    F[0] = (int16_t)(r1 >> 48);
    F[1] = (int16_t)(r1 << 16 >> 48);
    F[2] = (int16_t)(r1 << 32 >> 48);
    F[3] = (int16_t)(r1 << 48 >> 48);
    F[4] = (int16_t)(r2 >> 48);
    F[5] = (int16_t)(r2 << 16 >> 48);
    F[6] = (int16_t)(r2 << 32 >> 48);
    F[7] = (int16_t)(r2 << 48 >> 48);

    for (auto i : F) {
        std::cout << i << " ";
    }

    std::cout << "---- ";
    for (int i = 0; i < 8; i++) {
        std::cout << (A[i] - B[i]) * (C[i] - D[i]) << " ";
    }

    return 0;
}

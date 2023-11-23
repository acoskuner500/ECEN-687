#include <stdio.h>
#include <cmath>
#include <utility>
using namespace std;

pair<double,double> getB(double m0, double m1, double m2, double m3) {
    double b1 = (m1 * m2 - m0 * m3) / (m0 * m2 - m1 * m1);
    double b2 = (m2 + m1 * b1) / -m0;
    return make_pair(b1, b2);
}

pair<double, double> getRoot(double b1, double b2) {
    double p1 = (-b1 + sqrt(b1 * b1 - 4 * b2))/(2 * b2);
    double p2 = (-b1 - sqrt(b1 * b1 - 4 * b2))/(2 * b2);
    return make_pair(p1, p2);
}

pair<double,double> getK(double p1, double p2, double m0, double m1) {
    double a, b, c, d, e, f;
    a = pow(p1,-1);
    b = pow(p2,-1);
    c = pow(p1,-2);
    d = pow(p2,-2);
    e = -m0;
    f = -m1;
    double k2 = (a*f - c*e) / (a*d - b*c);
    double k1 = (e - b * k2) / a;
    return make_pair(k1, k2);
}

int main() {
    double C0 = 400e-15;
    double C1 = 1.4e-12;
    double C2 = 20e-15;
    double C3 = 20e-15;
    double R0 = 100;
    double R1 = 40;
    double R2 = 80;
    double R3 = 20;
    
    double D0 = R0 * (C0 + C1 + C2 + C3);
    double D1 = D0 + R1 * (C1 + C2 + C3);
    double D2 = D1 + R2 * C2;
    double D3 = D1 + R3 * C3;

    // [node #][order #]
    double m00, m10, m20, m30;
    double m01, m11, m21, m31;
    double m02, m12, m22, m32;
    double m03, m13, m23, m33;
    
    // Calculate m0
    m00 = m10 = m20 = m30 = 1;

    // Calculate m1
    m01 =   0 - R0 * (m00 * C0 + m10 * C1 + m20 * C2 + m30 * C3);
    m11 = m01 - R1 * (m10 * C1 + m20 * C2 + m30 * C3);
    m21 = m11 - R2 * (m20 * C2);
    m31 = m11 - R3 * (m30 * C3);

    // Calculate m2
    m02 =   0 - R0 * (m01 * C0 + m11 * C1 + m21 * C2 + m31 * C3);
    m12 = m02 - R1 * (m11 * C1 + m21 * C2 + m31 * C3);
    m22 = m12 - R2 * (m21 * C2);
    m32 = m12 - R3 * (m31 * C3);

    // Calculate m3
    m03 =   0 - R0 * (m01 * C0 + m12 * C1 + m22 * C2 + m32 * C3);
    m13 = m03 - R1 * (m12 * C1 + m22 * C2 + m32 * C3);
    m23 = m13 - R2 * (m22 * C2);
    m33 = m13 - R3 * (m32 * C3);

    auto[b01, b02] = getB(m00, m01, m02, m03);
    auto[b11, b12] = getB(m10, m11, m12, m13);
    auto[b21, b22] = getB(m20, m21, m22, m23);
    auto[b31, b32] = getB(m30, m31, m32, m33);

    auto[p01, p02] = getRoot(b01, b02);
    auto[p11, p12] = getRoot(b11, b12);
    auto[p21, p22] = getRoot(b21, b22);
    auto[p31, p32] = getRoot(b31, b32);

    auto[k01, k02] = getK(p01, p02, m00, m01);
    auto[k11, k12] = getK(p11, p12, m10, m11);
    auto[k21, k22] = getK(p21, p22, m20, m21);
    auto[k31, k32] = getK(p31, p32, m30, m31);

    printf("D0 = %.6e\t", D0);
    printf("D1 = %.6e\t", D1);
    printf("D2 = %.6e\t", D2);
    printf("D3 = %.6e\n\n", D3);
    
    printf("m00 = %.6e\t", m00);
    printf("m10 = %.6e\t", m10);
    printf("m20 = %.6e\t", m20);
    printf("m30 = %.6e\n", m30);
    printf("m01 = %.6e\t", m01);
    printf("m11 = %.6e\t", m11);
    printf("m21 = %.6e\t", m21);
    printf("m31 = %.6e\n", m31);
    printf("m02 = %.6e\t", m02);
    printf("m12 = %.6e\t", m12);
    printf("m22 = %.6e\t", m22);
    printf("m32 = %.6e\n", m32);
    printf("m03 = %.6e\t", m03);
    printf("m13 = %.6e\t", m13);
    printf("m23 = %.6e\t", m23);
    printf("m33 = %.6e\n\n", m33);

    printf("b01 = %.6e\t", b01);
    printf("b11 = %.6e\t", b11);
    printf("b21 = %.6e\t", b21);
    printf("b31 = %.6e\n", b31);
    printf("b02 = %.6e\t", b02);
    printf("b12 = %.6e\t", b12);
    printf("b22 = %.6e\t", b22);
    printf("b32 = %.6e\n\n", b32);

    printf("p01 = %.6e\t", p01);
    printf("p11 = %.6e\t", p11);
    printf("p21 = %.6e\t", p21);
    printf("p31 = %.6e\n", p31);
    printf("p02 = %.6e\t", p02);
    printf("p12 = %.6e\t", p12);
    printf("p22 = %.6e\t", p22);
    printf("p32 = %.6e\n\n", p32);

    printf("k01 = %.6e\t", k01);
    printf("k11 = %.6e\t", k11);
    printf("k21 = %.6e\t", k21);
    printf("k31 = %.6e\n", k31);
    printf("k02 = %.6e\t", k02);
    printf("k12 = %.6e\t", k12);
    printf("k22 = %.6e\t", k22);
    printf("k32 = %.6e\n\n", k32);


    return 0;
}
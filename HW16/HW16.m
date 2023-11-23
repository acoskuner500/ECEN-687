clear; clc;
C0 = 400e-15;
C1 = 1.4e-12;
C2 = 20e-15;
C3 = 20e-15;
R0 = 100;
R1 = 40;
R2 = 80;
R3 = 20;
D0 = R0 * (C0 + C1 + C2 + C3);
D1 = D0 + R1 * (C1 + C2 + C3);
D2 = D1 + R2 * C2;
D3 = D1 + R3 * C3;
Elmore = [D0; D1; D2; D3]

% Calculate m0
m00 = 1;
m10 = 1;
m20 = 1;
m30 = 1;

% Calculate m1
m01 =   0 - R0 * (m00 * C0 + m10 * C1 + m20 * C2 + m30 * C3);
m11 = m01 - R1 * (m10 * C1 + m20 * C2 + m30 * C3);
m21 = m11 - R2 * (m20 * C2);
m31 = m11 - R3 * (m30 * C3);

% Calculate m2
m02 = vpa(  0 - R0 * (m01 * C0 + m11 * C1 + m21 * C2 + m31 * C3));
m12 = vpa(m02 - R1 * (m11 * C1 + m21 * C2 + m31 * C3));
m22 = vpa(m12 - R2 * (m21 * C2));
m32 = vpa(m12 - R3 * (m31 * C3));

% Calculate m3
m03 = vpa(  0 - R0 * (m01 * C0 + m12 * C1 + m22 * C2 + m32 * C3));
m13 = vpa(m03 - R1 * (m12 * C1 + m22 * C2 + m32 * C3));
m23 = vpa(m13 - R2 * (m22 * C2));
m33 = vpa(m13 - R3 * (m32 * C3));

A0 = [m00 m01; m01 m02];
A1 = [m10 m11; m11 m12];
A2 = [m20 m21; m21 m22];
A3 = [m30 m31; m31 m32];
Y0 = [-m02; -m03];
Y1 = [-m12; -m13];
Y2 = [-m22; -m23];
Y3 = [-m32; -m33];
B0 = linsolve(A0, Y0);
B1 = linsolve(A1, Y1);
B2 = linsolve(A2, Y2);
B3 = linsolve(A3, Y3);

P0 = roots([B0(1) B0(2) 1]);
P1 = roots([B1(1) B1(2) 1]);
P2 = roots([B2(1) B2(2) 1]);
P3 = roots([B3(1) B3(2) 1]);

A0 = [P0(1)^-1 P0(2)^-1; P0(1)^-2 P0(2)^-2];
A1 = [P1(1)^-1 P1(2)^-1; P1(1)^-2 P1(2)^-2];
A2 = [P2(1)^-1 P2(2)^-1; P2(1)^-2 P2(2)^-2];
A3 = [P3(1)^-1 P3(2)^-1; P3(1)^-2 P3(2)^-2];
Y0 = [-m00; -m01];
Y1 = [-m10; -m11];
Y2 = [-m20; -m21];
Y3 = [-m30; -m31];
K0 = linsolve(A0, Y0);
K1 = linsolve(A1, Y1);
K2 = linsolve(A2, Y2);
K3 = linsolve(A3, Y3);

syms s t
V0 = sum(K0./P0)*(-1/s) + (K0(1)/P0(1))*(1/(s-P0(1))) + (K0(2)/P0(2))*(1/(s-P0(2)));
V1 = sum(K1./P1)*(-1/s) + (K1(1)/P1(1))*(1/(s-P1(1))) + (K1(2)/P1(2))*(1/(s-P1(2)));
V2 = sum(K2./P2)*(-1/s) + (K2(1)/P2(1))*(1/(s-P2(1))) + (K2(2)/P2(2))*(1/(s-P2(2)));
V3 = sum(K3./P3)*(-1/s) + (K3(1)/P3(1))*(1/(s-P3(1))) + (K3(2)/P3(2))*(1/(s-P3(2)));

V0 = ilaplace(V0);
V1 = ilaplace(V1);
V2 = ilaplace(V2);
V3 = ilaplace(V3);

AWE0 = solve(V0 == 0.5, t);
AWE1 = solve(V1 == 0.5, t);
AWE2 = solve(V2 == 0.5, t);
AWE3 = solve(V3 == 0.5, t);
AWE = [AWE0; AWE1; AWE2; AWE3]

delayU = 1.72206e-10;
delayV = 1.71019e-10;
PEU_Elm = 100*abs(D2 - delayU)/delayU;
PEV_Elm = 100*abs(D3 - delayV)/delayV;
PEU_AWE = 100*abs(AWE2 - delayU)/delayU;
PEV_AWE = 100*abs(AWE3 - delayV)/delayV;
fprintf("Percent Errors:\n")
fprintf("\t\tSink U\t\tSink V\n")
fprintf("Elmore:\t%f%%\t%f%%\n", PEU_Elm, PEV_Elm)
fprintf("AWE:\t%f%%\t%f%%\n", PEU_AWE, PEV_AWE)
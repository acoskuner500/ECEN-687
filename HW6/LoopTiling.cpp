#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <array>
using namespace std;

int main() {
    const int k[2] = {1024, 4096};
    const int Bx[4] = {1,16,64,256};
    for (int r=0; r<3; r++) {
        for (int i=0; i<2; i++) {
            for (int j=0; j<4; j++) {
                vector<vector<int>> A(k[i], vector<int>(k[i]));
                vector<vector<int>> B(k[i], vector<int>(k[i]));
                vector<vector<int>> Y(k[i], vector<int>(k[i]));
                for (int ii=0; ii<k[i]; ii++) {
                    for (int jj=0; jj<k[i]; jj++) {
                        A[ii][jj] = rand();
                        B[ii][jj] = rand();
                        
                        // Uncomment to check a few cells for random 
                        // values in A,B and whether Y is init to 0

                        // if (ii < 10 && jj < 10) {
                        //     cout << A[ii][jj] << "\t" << B[ii][jj] << "\t" << Y[ii][jj] << endl;
                        // }
                    }
                }
                clock_t start = clock();
                for (int ii=0; ii<k[i]; ii+=Bx[j]) {
                    for (int jj=0; jj<k[i]; jj+=Bx[j]) {
                        for (int kk=0; kk<k[i]; kk+=Bx[j]) {
                            for (int iii=ii; iii<min(ii+Bx[j],k[i]);iii++) {
                                for (int jjj=jj; jjj<min(jj+Bx[j],k[i]);jjj++) {
                                    for (int kkk=kk; kkk<min(kk+Bx[j],k[i]);kkk++) {
                                        Y[iii][jjj] += A[iii][kkk]*B[kkk][jjj];
                }}}}}}
                start = clock() - start;
                cout << "CPU time for k=" << k[i] << " and B=" << Bx[j] << ":\t" << (0.001*start) << " s\n";
            }
        }
    }
}
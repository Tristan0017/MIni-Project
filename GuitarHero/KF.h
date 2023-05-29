#ifndef KF_H_
#define KF_H_
#include <iostream>
#include <BasicLinearAlgebra.h>
using namespace BLA;

class KF
{
public:

void KFPredict();

void KFUpdate(BLA::Matrix<2,1> Z);

float KFGetY();

private:
    float Var_P = 8; // Measurement noise (position)
    float Var_V = 10; // Meaurement noise (Angular velocity)
    float Ts = 0.02;
    BLA::Matrix<2,1> X_hat = {0, 0};
    BLA::Matrix<2,1> X = {0, 0};    
    BLA::Matrix<2,2> H = {1, 0, 0, 1};
    BLA::Matrix<2,2> A = {0.99, 0, Ts, 1}; // 0.99 is the lowpass filter ensuring it will reach 0 angular velocity over time.
    BLA::Matrix<2,2> Cov_hat = {0, 0, 0, 0};
    BLA::Matrix<2,2> Cov = {Var_V , 0, 0, Var_P};
    BLA::Matrix<2,2> Q = {2, 0, 0, 0.64};  // (Q) 2 is found by tuning, since it was hard to reason what a good angular acceleration is. 0.64 is found by assuming the maximum
    BLA::Matrix<2,2> I = {1, 0, 0, 1};     // angular velocity is 120 degrees/s. This was assumed to be 3*sigma (99.7%), after which the variance is obtained by 
    BLA::Matrix<2,1> Y_tilde = {0, 0};     //  scaling with our sample rate, dividing by three and squaring.
    BLA::Matrix<1,1> Y = { 0 };
    BLA::Matrix<2,2> R = {Var_V, 0, 0, Var_P};
    BLA::Matrix<2,2> K = {0, 0, 0, 0};
    BLA::Matrix<2,2> S = {0, 0, 0, 0};
    BLA::Matrix<1,2> H_Y = {0, 1};         // Only angular position is desired.
};
#endif



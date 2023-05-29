#include "KF.h"
void KF::KFPredict()
{
    X_hat = A * X;
    Cov_hat = A*Cov* ~A + Q; 
}

void KF::KFUpdate(BLA::Matrix<2,1> Z) 
{
    Y_tilde = Z - H * X_hat;
    S = H * Cov_hat * ~H + R;
    bool is_nonsingular = Invert(S); // Inverts S. Is saved in S itself.
    K = Cov_hat * ~H * S;
    X = X_hat + K * Y_tilde; 
    Cov = (I - K * H) * Cov_hat;
    Y_tilde = Z - H * X;
}

float KF::KFGetY() 
{
    Y = H_Y * X;
    float Y_Position = Y(0);
    return Y_Position;
}

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

/// \brief Test the real solution of system Ax = b
/// \return the relative error for PALU solver
/// \return the relative error for QR solver
void TestSolution(const MatrixXd& A,
                  const VectorXd& b,
                  const VectorXd& solution,
                  double& errRelPALU,
                  double& errRelQR);

/// \brief Solve linear system with PALU
/// \return the solution
VectorXd SolveSystemPALU(const MatrixXd& A,
                         const VectorXd& b);

/// \brief Solve linear system with PALU
/// \return the solution
VectorXd SolveSystemQR(const MatrixXd& A,
                       const VectorXd& b);

int main()
{
    Vector2d solution(-1.0000e+0, -1.0000e+00);

    Matrix2d A1 = Matrix2d::Zero();
    A1 << 5.547001962252291e-01, -3.770900990025203e-02,
          8.320502943378437e-01, -9.992887623566787e-01;
    Vector2d b1 = Vector2d::Zero();
    b1 << -5.169911863249772e-01, 1.672384680188350e-01;

    double errRel1PALU, errRel1QR;
    TestSolution(A1, b1, solution, errRel1PALU, errRel1QR);

    if (errRel1PALU < 1e-15 && errRel1QR < 1e-15)
        cout<< scientific<< "1 - "<< "PALU: "<< errRel1PALU<< " QR: "<< errRel1QR<< endl;
    else
    {
        cerr<< "1 - Wrong system solution found"<< endl;
        return -1;
    }

    Matrix2d A2 = Matrix2d::Zero();
    A2 << 5.547001962252291e-01, -5.540607316466765e-01,
          8.320502943378437e-01, -8.324762492991313e-01;
    Vector2d b2 = Vector2d::Zero();
    b2 << -6.394645785530173e-04, 4.259549612877223e-04;

    double errRel2PALU, errRel2QR;
    TestSolution(A2, b2, solution, errRel2PALU, errRel2QR);

    if (errRel2PALU < 1e-11 && errRel2QR < 1e-11)
        cout<< scientific<< "2 - "<< "PALU: "<< errRel2PALU<< " QR: "<< errRel2QR<< endl;
    else
    {
        cerr<< "2 - Wrong system solution found"<< endl;
        return -1;
    }

    Matrix2d A3 = Matrix2d::Zero();
    A3 << 5.547001962252291e-01, -5.547001955851905e-01,
          8.320502943378437e-01, -8.320502947645361e-01;
    Vector2d b3 = Vector2d::Zero();
    b3 << -6.400391328043042e-10, 4.266924591433963e-10;

    double errRel3PALU, errRel3QR;

    TestSolution(A3, b3, solution, errRel3PALU, errRel3QR);

    if (errRel3PALU < 1e-5 && errRel3QR < 1e-5)
        cout<< scientific<< "3 - "<< "PALU: "<< errRel3PALU<< " QR: "<< errRel3QR<< endl;
    else
    {
        cerr<< "3 - Wrong system solution found"<< endl;
        return -1;
    }

    return 0;
}

void TestSolution(const MatrixXd& A,
                  const VectorXd& b,
                  const VectorXd& solution,
                  double& errRelPALU,
                  double& errRelQR)
{
    errRelPALU = 0.0;
    errRelQR = 0.0;

    VectorXd xPALU = SolveSystemPALU(A, b);    
        for (int i = 0; i < A.rows(); i++)
        {
            errRelPALU += abs(xPALU(i) - solution(i)) / abs(solution(i));
        }
    VectorXd xQR = SolveSystemQR(A, b);
    for (int i = 0; i < A.rows(); i++)
    {
        errRelQR += abs(xQR(i) - solution(i)) / abs(solution(i));
    }
}

VectorXd SolveSystemPALU(const MatrixXd& A,
                         const VectorXd& b)
{
    int n = A.rows();
    VectorXd bCopy = b;
    VectorXd x(n);
    MatrixXd P = MatrixXd::Identity(n, n);

    // Definisci la matrice L
    MatrixXd L = MatrixXd::Zero(n, n);

    // Definisci la matrice U
    MatrixXd U = A;

    // Esegui la decomposizione PA=LU
    for (int k = 0; k < A.rows(); ++k) {
        // Trova l'elemento massimo nella colonna k
        int maxRow = k;
        for (int i = k + 1; i < A.rows(); ++i) {
            if (abs(U(i, k)) > abs(U(maxRow, k))) {
                maxRow = i;
            }
        }

        // Scambia la riga corrente con la riga contenente l'elemento massimo
        if (maxRow != k) {
            U.row(k).swap(U.row(maxRow));
            P.row(k).swap(P.row(maxRow));
            if (k >= 1) {
                L.row(k).swap(L.row(maxRow));
            }
        }

        // Calcola gli elementi della matrice L e U
        for (int i = k + 1; i < A.rows(); ++i) {
            L(i, k) = U(i, k) / U(k, k);
            U.row(i) -= L(i, k) * U.row(k);
        }
    }
    L = L + MatrixXd::Identity(n, n);
    bCopy = P * bCopy;

    //cout << "P: " << P << "\nA: " << A << "\nL: " << L << "\nU: " << U << "\nb: " << bCopy << endl;

    VectorXd y(n);
    for (int i = 0; i < n; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < i; j++)
        {
            sum += L(i, j) * y(j);
        }
        y(i) = (bCopy(i) - sum) / L(i, i);
    }

    // Risoluzione del sistema Ux = y
    for (int i = n - 1; i >= 0; i--)
    {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
        {
            sum += U(i, j) * x(j);
        }
        x(i) = (y(i) - sum) / U(i, i);
    }

    return x;
}

VectorXd SolveSystemQR(const MatrixXd& A,
                       const VectorXd& b)
{
    int n = A.rows();
    VectorXd x(n);

    MatrixXd Q(A.rows(), A.cols());  // Matrice Q
    Q.setIdentity();

    MatrixXd R(A.rows(), A.cols());  // Matrice R
    R.setZero();

    for (int i = 0; i < A.cols(); i++) {
        VectorXd v = A.col(i);

        for (int j = 0; j < i; j++) {
            R(j, i) = Q.col(j).dot(A.col(i));
            v -= R(j, i) * Q.col(j);
        }

        R(i, i) = v.norm();
        Q.col(i) = v.normalized();
    }

    VectorXd y = Q.transpose() * b;

    // Risoluzione del sistema lineare Rx = y
    for (int i = A.cols() - 1; i >= 0; i--) {
        double sum = 0.0;

        for (int j = i + 1; j < A.cols(); j++) {
            sum += R(i, j) * x(j);
        }

        x(i) = (y(i) - sum) / R(i, i);
    }
    //cout << "\nA: " << A << "\nQ: " << Q << "\nR: " << R << endl;
    return x;
}

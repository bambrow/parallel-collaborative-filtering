
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Sparse>

using namespace std;

int main(int argc, char const *argv[])
{
    Eigen::MatrixXi m1(3,3);
    m1.setZero(3,3);
    m1(0,0) = 1;
    m1(1,1) = 2;
    m1(2,2) = 3;
    Eigen::SparseMatrix<int> m2(3,3);
    m2.insert(0,0) = 1;
    m2.insert(1,1) = 2;
    m2.insert(2,2) = 3;

    cout << m1 << endl;
    cout << m2 << endl;

    return 0;
}

#include<iostream>
#include<cmath>
#include<Eigen/Dense>
#include<cstdlib>//防止迭代次数过多时输出乱码

int main(){

    system("chcp 65001 > nul");//防止迭代次数过多时输出乱码

    // 定义 QP 标准型的矩阵 P, q, A, l, u 
    // Hessian矩阵，必须是对称正定（或半正定）的
    Eigen::Matrix2d P;
    P << 1.0,  0.0,
          0.0, 10.0;

    // q 向量 (2x1) — 线性项系数
    Eigen::Vector2d q;
    q << -3.0, -30.0;

    // A 矩阵 (1x2) — 约束矩阵
    Eigen::Matrix<double, 1, 2> A;
    A << 1.0, 1.0;

    // l, u 向量 (1x1) — 约束下界和上界（相等表示等式约束）
    Eigen::VectorXd l(1), u(1);
    l << 4.0;
    u << 4.0;

    // 构建 3x3 的 KKT 矩阵
    Eigen::Matrix3d KKT;
    KKT << P(0,0), P(0,1), A(0,0),   
           P(1,0), P(1,1), A(0,1),   
           A(0,0), A(0,1),    0.0;  

    // 构建右端向量
    Eigen::Vector3d rhs;
    rhs << -q(0), -q(1), l(0);  // [-q; b],  b = l = u = 4
    // 即 rhs = [3, 30, 4]

    // 使用 Eigen 的线性求解器求解 KKT 系统
    Eigen::Vector3d sol = KKT.colPivHouseholderQr().solve(rhs);

    // 提取解
    double x = sol(0);
    double y = sol(1);
    double lambda = sol(2);  // 拉格朗日乘子

    std::cout << "  x = " << x << std::endl;
    std::cout << "  y = " << y << std::endl;
    std::cout << "  λ = " << lambda << std::endl;

    return 0;
}

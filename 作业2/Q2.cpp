#include<iostream>
#include<cmath>
#include<Eigen/Dense>
#include<cstdlib>//防止迭代次数过多时输出乱码

Eigen::Vector2d f_tidu(double x, double y){//自行构造梯度函数
    Eigen::Vector2d f(2,1);
    f(0) = x-3;
    f(1) = 10*(y-3);
    return f;
}

Eigen::Vector2d h_tidu(){// h(x,y)=x+y-4 的梯度
    Eigen::Vector2d h(2,1);
    h(0) = 1;
    h(1) = 1;
    return h;
}

double h(double x, double y){// h 的函数值：需要 -4 （标量）
    return x + y - 4;
}

/*
如果用这个函数，那需要事先构造f和h，也很麻烦，所以舍弃
Eigen::Vector2d L(Eigen::Vector2d f, Eigen::Vector2d h,double u){//拉格朗日乘子法的梯度函数
    Eigen::Vector2d L;
    L(0)=f(0)+ u * h(0);
    L(1)=f(1)+ u * h(1);
    return L;
}
*/
int main(){
    
    system("chcp 65001 > nul");//防止迭代次数过多时输出乱码

    double learning_rate = 0.08;   
    double u_rate = 2;   // u 是拉格朗日乘子（对偶变量）。原问题是最小化f，满足h(x,y)=0,他的对偶问题是最大化对偶函数u
    int max_iter = 10000; //设置最大迭代次数，防止死循环

    double x = 0.0, y = 0.0, u = 0.0;
    Eigen::Vector2d X(2,1);
    X(0) = x;
    X(1) = y;

    int count = 0;
    // double Cost_Function = 0.0;
    double kkt_err ;
    double h_err ;
    do {
        if (count > max_iter) {
            std::cout << "迭代次数过多" << std::endl;
            break;
        }

        // 定义拉格朗日函数的梯度
        Eigen::Vector2d grad_L = f_tidu(X(0), X(1)) + u * h_tidu();

        // 更新 x, y (梯度下降)
        X = X - learning_rate * grad_L;

        // 更新乘子 u (梯度上升，因为是对偶问题)
        // 目的是最小化f，而u大于0，h小于等于0，所以让h=0时，f最小
        u = u + u_rate * h(X(0), X(1));//h>0, u增大，增强约束，让h往0移动；h<0, u减小，减弱约束，让h往0移动

        //Cost_Function = 0.5*(X(0)-3)*(X(0)-3) + 5*(X(1)-3)*(X(1)-3);
        h_err = std::abs(h(X(0), X(1)));

        kkt_err = (f_tidu(X(0), X(1)) + u * h_tidu()).norm(); //把向量转成模，判断它是否等于零，也就是是否满足KKT条件

        count++;
    } while (h_err > 0.001 || kkt_err > 0.001);
    // h_err 控制h等于0，kkt_err 控制拉格朗日函数梯度等于0

    std::cout << "迭代次数: " << count << std::endl;
    std::cout << " x = " << X(0) << std::endl;
    std::cout << " y = " << X(1) << std::endl;
    std::cout << " u = " << u << std::endl;
    std::cout << " h_err = " << h_err << std::endl;
    std::cout << " kkt_err = " << kkt_err << std::endl;
    std::cout << " h = " << h(X(0), X(1)) << std::endl;

    return 0;
}
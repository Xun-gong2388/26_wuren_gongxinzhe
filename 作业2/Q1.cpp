#include<iostream>
#include<cmath>
#include<Eigen/Dense>

Eigen::Vector2d tidu(Eigen::Vector2d X){
    X(0) = X(0) - 3;
    X(1) = 10*(X(1) - 3);
    return X;
}

int main(){
    double x=0.0;
    double y=0.0;
    double Learning_Rate = 0.1;//对于学习率，当它为0.2时，就会报错，查资料得知是因为学习率太大，一直在震荡，形成死循环，所以输出的是nan
    int count=0;
    Eigen::Vector2d X(2,1);
    X(0)=x;
    X(1)=y;
    int maxtest=10000;//在测试过程中，学习率为0.1，0.2都可运行，但是为0.05时就会报错，查资料得知是因为迭代次数过多，形成死循环，被识别成了恶意程序
    //所以要设置最大迭代次数
    //std::cout << X << std::endl;
    //std::cout << X.size() << std::endl;
    double Cost_Function = 0.5*(x-3)*(x-3)+5*(y-3)*(y-3);
    do{
       if(count > maxtest){
           std::cout << "迭代次数过多，无法收敛" << std::endl;
           break;
        }
        X = X - Learning_Rate * tidu(X);
        Cost_Function = 0.5*(X(0)-3)*(X(0)-3)+5*(X(1)-3)*(X(1)-3);
        count++;
    }while(Cost_Function > 0.001); //让损失函数极小
    //while(X(0) != 3 || X(1) != 3);//若用这个条件，因为很难达到，所以迭代数很大，几乎形成死循环，因此要换成损失函数在一个很小的范围内
    
    std::cout << count << std::endl;
    std::cout << X << std::endl;
    return 0;
}
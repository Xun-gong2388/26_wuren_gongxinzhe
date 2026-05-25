//
//  main.cpp
//  Astar
//
//  Created by xujw on 15/4/9.
//  Copyright (c) 2015年 xujw. All rights reserved.
//

#include <iostream>
#include "CAstar.h"

/*
这个函数的作用是打印地图，就是在终端让每一个元素都显示出来
*/
void printMap(char map[MAX_X][MAX_Y],int width,int height)
{
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height; j++)
        {
            printf("%c\t",map[i][j]);//打印当前字符，\t表示制表符，用于对齐
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    
    cout<<"---map---"<<endl;
    //初始化地图矩阵 0代表障碍
    char mapdata[MAX_X][MAX_Y] =
    {
        {'1','0','0','1','0','1','1','1','1','1'},
        {'1','1','1','1','0','1','1','1','1','1'},
        {'0','0','0','1','0','1','1','1','1','1'},
        {'1','0','0','1','0','1','1','1','1','0'},
        {'1','1','1','1','0','1','1','1','1','1'},
        {'1','1','0','0','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1'},
        {'1','0','0','1','1','1','1','1','1','1'},
        {'1','1','0','0','1','1','1','1','1','1'},
        {'1','0','1','1','1','1','1','1','1','1'},
    };
    
    printMap(mapdata, MAX_X, MAX_Y);
    //创建地图
    vector< vector<APoint*> > map;
    /*vector是向量，所以这里指的是声明一个二维向量，
    其元素是vector<APoint*>，也就是每一行都存放的是一个APoint指针向量
    */



    /*
    这个循环的作用是创建地图，设置好地图的宽度和高度，并且设置好每个节点的坐标，类型
    并且判断是否为障碍，随后这个地图会传给CAstar类的findWay函数来进行寻路
    */
    for (int i = 0; i<MAX_X; i++)
    {
        vector<APoint*> tmp;//创建一个临时的向量 tmp，用于存储当前行的所有 APoint* 指针
        for (int j = 0; j<MAX_Y; j++)
        {
            APoint *point = new APoint();//对于APoint类的定义在CAstar.h里有体现
            point->x = i;
            point->y = j;//设置该节点坐标
            if (mapdata[i][j]=='0')//判断在这个位置的字符是不是0（障碍）
            {
                point->type = AType::ATYPE_BARRIER;//如果是障碍，就设置为brier状态
            }
            tmp.push_back(point);//把当前节点加入到 tmp 临时向量中
        }
        map.push_back(tmp);//把当前行的所有节点加入到 map 向量中，作为其中一行
    }
    
    //开始寻路
    auto star = new CAstar();//CAstar类的声明在CAstar.h里有体现
    auto point = star->findWay(map[0][0], map[9][9], map); //从起点到终点的路径点
    //如果路径点为空，就说明没有路径，找到路径就返回重点节点的指针，通过回溯父节点可得到完整路径
    if (!point)//如果 point 为空指针（即未找到路径），则程序直接返回 0 结束。
    {
        return 0;
    }
    
    cout<<"---下面是路径点(倒序)---"<<endl;
    
    while (point)//当 point 不为空指针时，说明路径存在，继续回溯父节点，从而得到完整路径
    {
        mapdata[point->x][point->y] = '*';
        cout<<point->x<<","<<point->y<<endl;
        point = point->parent;
    }
    cout<<"---打印路径---"<<endl;
    printMap(mapdata, MAX_X, MAX_Y);
    
    
    //-------------释放内存----------
    delete star;//前面的star是临时存储的，所以这里要释放内存
    


    /*
    这个循环的作用是释放地图中的所有节点，因为每个节点都是动态分配的，所以这里要释放内存
    */
    for (int i = 0; i<MAX_X; i++)
    {
        for (int j = 0; j<MAX_Y; j++)
        {
            delete map[i][j];
            map[i][j] = nullptr;
        }
    }
    
    return 0;
}

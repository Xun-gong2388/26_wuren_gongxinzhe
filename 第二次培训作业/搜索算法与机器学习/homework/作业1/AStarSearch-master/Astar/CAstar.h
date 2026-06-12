//
//  CAstar.h
//  Astar
//
//  Created by xujw on 15/4/9.
//  Copyright (c) 2015年 xujw. All rights reserved.
//
/*
    F:路径评分 = g+h
    G:走一格格子的花销
    H:当前格子到目标格子的估算花销
 
    上下左右走一格花销为10，斜着走一格花销为14，以方便计算
    即格子宽高为10 对角线为14
 */
#ifndef __Astar__CAstar__
#define __Astar__CAstar__  

#include <stdio.h> //用于printf输出
#include <vector> //用于动态数组
#include <iostream>  //用于cout输出
using namespace std;  

//地图最大值
#define MAX_X 10
#define MAX_Y 10

/* 
这个函数的作用是定义节点的类型，包括unknown,closed,opened,brier，作用是方便之后找到最优路径（可以舍去一些不可行的节点）
*/
enum class AType //节点类型
{
    ATYPE_UNKNOWN, //未被考察过的节点
    ATYPE_CLOSED, //关闭列表，节点以及其周边节点都被考察过，不被再次考察
    ATYPE_OPENED, //开放列表，节点已经被发现，但是周围节点还没有被考察
    ATYPE_BARRIER   //障碍，不可通过
};
/*每个节点的搜索过程会经历以下过程：
  1.初始为unknown状态
  2.从某一节点开始探索，该节点被发现后，转换为opened状态
  3.当节点周围的所有节点都被考察后，转换为closed状态，并且之后不再考虑这里
  4.如果节点为障碍，就直接标记为brier状态，并且不被加入opened列表，即不被考虑
*/




/*
这个类的作用是表示地图上的节点，定义他的坐标，类型（是否值得考虑），以及父节点（用于回溯路径）
最后再判断这个点是不是等于终点来决定是否终止进程
*/
class APoint //定义一个APoint类，用于表示地图上的节点
{
public:
    APoint();//作用是初始化：节点的类型为unknown，f,g,h都为0
    ~APoint();//析构函数，用于释放内存
    int x;
    int y;//x，y是节点坐标
    AType type;   //这个节点的类型只能从上面的函数所定义的几个里面选:unknown,closed,opened,brier
    int f;  //f = g+h，f是从起点经过当前节点再到终点的总代价，f越小，节点越优先被考察（表示路径最短）
    int g;//从起点到当前节点的实际代价
    int h;//从当前节点到终点的估计的剩余代价
    APoint *parent;//指向父节点的指针。当算法从起点扩展到当前节点时，parent 记录了来路。当搜索到终点后，可以通过不断访问 parent 回溯出完整路径。
    bool operator == (const APoint& po)//判断当前点是不是等于另一个点
    //整体的使用应该是逐个节点判断他们是否等于终点，若不等于就根据f值确定下一个节点，然后重复以上操作
    {
        if (x == po.x && y == po.y)
        {
            return true;
        }
        return false;
    }
   
};



/*
这个函数通过先检查当前节点周围的节点，排除掉障碍节点和已考察的不可行的节点，
最后返回当前节点的所有周边节点的指针，也就是找到所有可走路径
*/
class CAstar
{
    vector<APoint*> _openList;      //存放开放列表的节点指针
    vector<APoint*> _closeList;     //存放关闭列表的节点指针
    vector<APoint*> _neighbourList; //存放周边节点
    APoint* _endPoint;//指向终点的指针
    APoint* _curPoint;//指向当前节点的指针
    vector< vector<APoint*> > _allPoints;//二维向量，存放地图上的所有节点
public:
    CAstar();
    ~CAstar();
    APoint* findWay(APoint* beginPoint,APoint* endPoint,vector< vector<APoint*> >& allPoints);
    //执行A*算法，等找到路径后，返回从起点到终点的路径
    //    APoint* findWay(int beginX,int beginY,int endX,int endY);
private:
    int getF(APoint *point);//计算节点的F值
    int getH(APoint *point);//计算节点的H值
    vector<APoint*> getNeighboringPoint(APoint* point);//返回当前节点的所有周边节点的指针（表示所有可走路径）
};




#endif /* defined(__Astar__CAstar__) */

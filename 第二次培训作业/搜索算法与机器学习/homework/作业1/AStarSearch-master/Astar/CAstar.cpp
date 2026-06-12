//
//  CAstar.cpp
//  Astar
//
//  Created by xujw on 15/4/9.
//  Copyright (c) 2015年 xujw. All rights reserved.
//
//  上下左右走一格花销为10，斜着走一格花销为14，以方便计算
//  即格子宽高为10 对角线为14
#include "CAstar.h"
#include <algorithm>

//自定义排序函数
/*  
这个函数的作用是通过比较两个APoint类型的常量的f值，来判断是否需要交换
*/
bool mySort(const APoint* p1,const APoint* p2)
{
    return p1->f < p2->f;
}

/*
这个函数的作用是初始化APoint类型的成员变量
*/
APoint::APoint():x(0)
                ,y(0)
                ,h(0)
                ,f(0)
                ,g(0)
                ,parent(nullptr)
                ,type(AType::ATYPE_UNKNOWN)
{
}
APoint::~APoint()//析构函数
{
}


#pragma mark------CAstar-------//添加分割线，可忽略

/*
这个函数的作用是初始化CAstar类型的成员变量，让重点指针和当前节点指针都为空
避免未定义导致产生随机垃圾值
*/
CAstar::CAstar():_endPoint(nullptr)
                ,_curPoint(nullptr)
{
}   

/*
这个函数的作用是析构CAstar类型的成员变量，清空所有向量
*/
CAstar::~CAstar()//析构函数
{
    _openList.clear();
    _closeList.clear();
    _neighbourList.clear();
    _allPoints.clear();
}


/*
这个函数的作用是寻路函数，用于寻路
接收起点、终点以及整个地图节点矩阵
通过开放列表和关闭列表不断扩展节点，直到找到从起点到终点的最短路径
最后可通过parent回溯路径，若找不到路径则返回nullptr
*/
APoint* CAstar::findWay(APoint *beginPoint, APoint *endPoint,vector< vector<APoint*> >& allPoints)
{
    //传递地图
    _allPoints = allPoints;//将地图赋值给成员变量_allPoints，用于后续的寻路
    
    _endPoint = endPoint;//将终点赋值给成员变量_endPoint，用于后续的寻路

    if (_endPoint->type == AType::ATYPE_BARRIER)//检查终点是不是障碍物，如果是，则永远无法到达
    {
        cout<<"终点是障碍"<<endl;
        return nullptr;
    }
    if (*_endPoint == *beginPoint)//检查终点是否与起点相同，如果是，则直接返回起点
    {
        cout<<"起始点相同"<<endl;
        return nullptr;
    }
    
    _openList.push_back(beginPoint);//将起点添加到开放列表里
    beginPoint->type = AType::ATYPE_OPENED;//将起点的类型设置为已开放
    beginPoint->f = getF(beginPoint);//计算起点的F值，此时g=0
    //---------
    do//开始do while循环，直到开放列表为空
    {
        //获取最小值的节点
        _curPoint = _openList[0];//将开放列表的第一个节点赋值给当前节点指针
        _openList.erase(_openList.begin());//从开放列表里删除当前节点
        _curPoint->type = AType::ATYPE_CLOSED;//将当前节点的类型设置为已关闭
        _closeList.push_back(_curPoint);//将当前节点添加到关闭列表里
        
        if (*_curPoint == *_endPoint)//判断当前节点是不是终点
        {
            cout<<"have find way"<<endl;
            return _curPoint;
        }
        //获取相邻的节点
        vector<APoint*> neVec = getNeighboringPoint(_curPoint);//获取现在所有可走的周围节点
        for (int i = 0; i<neVec.size(); i++)
        {
            auto tmpoint = neVec[i];
            if (tmpoint->type == AType::ATYPE_CLOSED)//如果该周围节点已经在关闭列表中，跳过它（不再处理）
            {
                continue;
            }
            //是否在开放列表里
            if (tmpoint->type != AType::ATYPE_OPENED)//如果该周围节点不在开放列表里
            {
                tmpoint->parent = _curPoint;//将当前节点设置为该周围节点的父节点
                tmpoint->g = _curPoint->g + 10;//计算该周围节点的g值，即当前节点的g值加上10
                //计算H值
                tmpoint->h = getH(tmpoint);
                //添加到开放列表里
                _openList.push_back(tmpoint);
                tmpoint->type = AType::ATYPE_OPENED;
            }
            else
            {
                //已经在开放列表里
                if (tmpoint->h < _curPoint->h)
                {
                    tmpoint->parent = _curPoint;
                    tmpoint->g = _curPoint->g + 10;
                }
            }
        }
        //排序 F值最小的排在前面
        sort(_openList.begin(), _openList.end(), mySort);
        
    } while (_openList.size()>0);
    
    
    cout<<"---can not find way---"<<endl;
    
    return nullptr;
}

//APoint* CAstar::findWay(int beginX, int beginY, int endX, int endY)
//{
//    
//    
//    return nullptr;
//}


/*  
下面两个函数分别是得到节点的F值和得到H值的函数
*/
int CAstar::getF(APoint *point)
{
    return (point->g + getH(point));
}
int CAstar::getH(APoint *point)
{
    //曼哈顿城市街区估算法
    return (abs(_endPoint->y - point->y) + abs(_endPoint->x - point->x))*10;
}


/*  
这个函数的作用是得到节点的所有可走的周围节点
*/
vector<APoint*> CAstar::getNeighboringPoint(APoint *point)
{
    _neighbourList.clear();//清空周围节点列表
//    cout<<"nei size:"<<_neighbourList.size()<<endl;
    if (point->x < MAX_X-1)//确保不超出右边界
    {
        if (_allPoints[point->x+1][point->y]->type != AType::ATYPE_BARRIER)//如果该节点不是障碍物，则将其指针加入 _neighbourList
        {
            _neighbourList.push_back(_allPoints[point->x+1][point->y]);
        }
    }
    if (point->x >0)//确保不超出左边界
    {
        if (_allPoints[point->x-1][point->y]->type != AType::ATYPE_BARRIER)//如果该节点不是障碍物，则将其指针加入 _neighbourList
        {
            _neighbourList.push_back(_allPoints[point->x-1][point->y]);
        }
    }
    if (point->y < MAX_Y-1)//确保不超出下边界
    {
        if (_allPoints[point->x][point->y+1]->type != AType::ATYPE_BARRIER)//如果该节点不是障碍物，则将其指针加入 _neighbourList
        {
            _neighbourList.push_back(_allPoints[point->x][point->y+1]);
        }
    }
    if (point->y >0)//确保不超出上边界
    {
        if (_allPoints[point->x][point->y-1]->type != AType::ATYPE_BARRIER)//如果该节点不是障碍物，则将其指针加入 _neighbourList
        {
            _neighbourList.push_back(_allPoints[point->x][point->y-1]);
        }
    }
    
    return _neighbourList;//返回所有可走的周围节点
}



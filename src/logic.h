#ifndef LOGIC_H
#define LOGIC_H

#include<stdio.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
using namespace std;

#include "basicSettings.h"
#include "ronia.h"
#include "playerAI.h"

//选手不必了解这个类
class logic{
private:
	int comingCard[TURN_COUNT];//每回合将要降落的麻将牌种类，0表示无牌降落
	Player Players[2];//玩家信息
	Map MapData;//地图信息
public:
	logic(){memset(comingCard,0,sizeof(comingCard));}
	void initGame();//初始化游戏
	Operation validate(Operation op,int sign);//将选手的非法操作变为STAY，合法操作保留
	void DBG_outputMap();//地图的简易输出
	void GameRun();//主游戏循环
};

#endif
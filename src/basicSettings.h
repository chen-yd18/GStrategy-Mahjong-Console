#define _CRT_SECURE_NO_WARNINGS
#ifndef BASIC_SETTINGS_H
#define BASIC_SETTINGS_H

#include<string.h>

const int TURN_COUNT=36*4;//回合数，必须是36的倍数
const int MAP_SIZE=13;//正方形地图的边长

enum Sign{RED,BLUE};//玩家势力，红/蓝
enum opType{STAY,MOVE,MAGNET,FIRE};//操作类型，原地不动/普通移动/碰杠立直/麻将弹射
enum direction{RIGHT,UP,LEFT,DOWN};//方向（普通移动时需选择）：右(y+)/上(x-)/左(y-)/下(x+)
const int dx[4]={0,-1,0,1},dy[4]={1,0,-1,0};
struct Point{//坐标点类
	int x;//行（从0开始）
	int y;//列（从0开始）
	Point():x(0),y(0){}
	Point(int sign):x(sign?(MAP_SIZE-2):1),y((MAP_SIZE-1)/2){}
	//红方初始位置为第1行的中心，蓝方初始位置与红方中心对称
};
struct Player{
	int sign;//势力
	int remainStep;//剩余回合数
	int score;//当前分数
	int ronCount;//已胡次数
	int pocket;//雀士携带的牌，0表示未携带任何牌
	int ting;//当前听的牌，用一个9位二进制数表示，若ting&(1<<(i-1))则听“i条”
	Point pos;//雀士所在位置
	Player(int s=RED):sign(s),remainStep(TURN_COUNT),
	score(0),ronCount(0),pocket(0),ting(0),pos(s){}
};
struct Map{
	int data[MAP_SIZE][MAP_SIZE];//地图信息，data[i][j]非零则表示该格上的麻将牌种类，为零则表示该格为空
	int thisTurnDrop;//本回合空降的牌的种类，为0则表示本轮无牌空降
	Point thisTurnDropPoint;//在thisTurnDrop非零时有意义，表示本回合空降牌的位置
	bool haveChargeChicken;//“冲锋鸡”是否已经产生
	Map():thisTurnDrop(0),haveChargeChicken(false){
		memset(data,0,sizeof(data));
	}
};
struct Operation{
	int type;//操作种类（共四种，见上）
	int dir;//移动方向（普通移动时需改变此变量的值，取值共有四种，见上）
	Operation():type(STAY),dir(-1){}
};

#endif
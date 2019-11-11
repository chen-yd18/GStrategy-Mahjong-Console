#ifndef RONIA_H
#define RONIA_H

#include<stdio.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
using namespace std;

//选手不必了解这个类
class ronia{
private:
	static const int mianzi[16];
	//用一个9位的八进制数来描述一组麻将牌。从低到高数的第i位表示“i条”的张数（在0~4间取值）。
	static const int MZ_COUNT;//可能出现的“面子”种数
	int *cards;//需要计算听牌或番数的摆牌区牌面信息
public:
	ronia(int* const ca):cards(ca){}
	bool dfs(int remain);//判断八进制数remain对应的牌是否胡牌
	int calcTing();//计算听哪些牌
	bool yakuma(int addcard);//判断是否役满
};

#endif
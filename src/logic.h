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

//ѡ�ֲ����˽������
class logic{
private:
	int comingCard[TURN_COUNT];//ÿ�غϽ�Ҫ������齫�����࣬0��ʾ���ƽ���
	Player Players[2];//�����Ϣ
	Map MapData;//��ͼ��Ϣ
public:
	logic(){memset(comingCard,0,sizeof(comingCard));}
	void initGame();//��ʼ����Ϸ
	Operation validate(Operation op,int sign);//��ѡ�ֵķǷ�������ΪSTAY���Ϸ���������
	void DBG_outputMap();//��ͼ�ļ������
	void GameRun();//����Ϸѭ��
};

#endif
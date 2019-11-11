#define _CRT_SECURE_NO_WARNINGS
#ifndef BASIC_SETTINGS_H
#define BASIC_SETTINGS_H

#include<string.h>

const int TURN_COUNT=36*4;//�غ�����������36�ı���
const int MAP_SIZE=13;//�����ε�ͼ�ı߳�

enum Sign{RED,BLUE};//�����������/��
enum opType{STAY,MOVE,MAGNET,FIRE};//�������ͣ�ԭ�ز���/��ͨ�ƶ�/������ֱ/�齫����
enum direction{RIGHT,UP,LEFT,DOWN};//������ͨ�ƶ�ʱ��ѡ�񣩣���(y+)/��(x-)/��(y-)/��(x+)
const int dx[4]={0,-1,0,1},dy[4]={1,0,-1,0};
struct Point{//�������
	int x;//�У���0��ʼ��
	int y;//�У���0��ʼ��
	Point():x(0),y(0){}
	Point(int sign):x(sign?(MAP_SIZE-2):1),y((MAP_SIZE-1)/2){}
	//�췽��ʼλ��Ϊ��1�е����ģ�������ʼλ����췽���ĶԳ�
};
struct Player{
	int sign;//����
	int remainStep;//ʣ��غ���
	int score;//��ǰ����
	int ronCount;//�Ѻ�����
	int pocket;//ȸʿЯ�����ƣ�0��ʾδЯ���κ���
	int ting;//��ǰ�����ƣ���һ��9λ����������ʾ����ting&(1<<(i-1))������i����
	Point pos;//ȸʿ����λ��
	Player(int s=RED):sign(s),remainStep(TURN_COUNT),
	score(0),ronCount(0),pocket(0),ting(0),pos(s){}
};
struct Map{
	int data[MAP_SIZE][MAP_SIZE];//��ͼ��Ϣ��data[i][j]�������ʾ�ø��ϵ��齫�����࣬Ϊ�����ʾ�ø�Ϊ��
	int thisTurnDrop;//���غϿս����Ƶ����࣬Ϊ0���ʾ�������ƿս�
	Point thisTurnDropPoint;//��thisTurnDrop����ʱ�����壬��ʾ���غϿս��Ƶ�λ��
	bool haveChargeChicken;//����漦���Ƿ��Ѿ�����
	Map():thisTurnDrop(0),haveChargeChicken(false){
		memset(data,0,sizeof(data));
	}
};
struct Operation{
	int type;//�������ࣨ�����֣����ϣ�
	int dir;//�ƶ�������ͨ�ƶ�ʱ��ı�˱�����ֵ��ȡֵ�������֣����ϣ�
	Operation():type(STAY),dir(-1){}
};

#endif
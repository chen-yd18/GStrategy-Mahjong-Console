#ifndef RONIA_H
#define RONIA_H

#include<stdio.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
using namespace std;

//ѡ�ֲ����˽������
class ronia{
private:
	static const int mianzi[16];
	//��һ��9λ�İ˽�����������һ���齫�ơ��ӵ͵������ĵ�iλ��ʾ��i��������������0~4��ȡֵ����
	static const int MZ_COUNT;//���ܳ��ֵġ����ӡ�����
	int *cards;//��Ҫ�������ƻ����İ�����������Ϣ
public:
	ronia(int* const ca):cards(ca){}
	bool dfs(int remain);//�жϰ˽�����remain��Ӧ�����Ƿ����
	int calcTing();//��������Щ��
	bool yakuma(int addcard);//�ж��Ƿ�����
};

#endif
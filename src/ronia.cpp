#include "ronia.h"

const int ronia::MZ_COUNT=16;
const int ronia::mianzi[16]={0000000111,0000001110,0000011100,0000111000,0001110000,0011100000,0111000000,
	0000000003,0000000030,0000000300,0000003000,0000030000,0000300000,0003000000,0030000000,0300000000};
bool ronia::dfs(int remain){
	for(int i=0;i<9;i++)
	if(remain==(2<<(3*i)))return 1;
	for(int i=0;i<MZ_COUNT;i++){
		bool ok=1;
		for(int j=0;j<9;j++)
		if((mianzi[i]&(7<<(3*j)))>(remain&(7<<(3*j))))
		{ok=0;break;}
		if(ok&&dfs(remain-mianzi[i]))return 1;
	}
	return 0;
}
int ronia::calcTing(){
	int remain=0;
	int cardcount[9]={0};
	for(int i=0;i<13;i++)
	cardcount[cards[i]-1]++;
	for(int i=0;i<9;i++)
	remain|=(cardcount[i]<<(3*i));
	int ans=0;
	for(int i=0;i<9;i++){
		if(cardcount[i]<4){
			//printf("i=%d\n",i);
		remain+=(1<<(3*i));
		if(dfs(remain))ans|=(1<<i);
		remain-=(1<<(3*i));
		}
	}
	return ans;
}
bool ronia::yakuma(int addcard){
	int *cardcount = new int[9];
	for(int i=0;i<9;i++)
		cardcount[i]=0;
	for(int i=0;i<13;i++)
	cardcount[cards[i]-1]++;

	int ke=0;
	for(int i=0;i<9;i++)
	if(cardcount[i]>=3)ke++;

	//for(int i=0;i<9;i++)
	//	printf("%d ",cardcount[i]);
	//printf("\n");
	if(cardcount[0]==3&&cardcount[1]==1&&cardcount[2]==1
	&&cardcount[3]==1&&cardcount[4]==1&&cardcount[5]==1
	&&cardcount[6]==1&&cardcount[7]==1&&cardcount[8]==3){
		printf("纯九莲!\n");
		//_sleep(1000);
		delete[] cardcount;
		return true;
	}

	cardcount[addcard-1]++;

	if(ke==4){
		for(int i=0;i<9;i++)
		if(cardcount[i]==2){
		printf("对对和!\n");
		//_sleep(1000);
		delete[] cardcount;
		return true;
		}
	}
	
	if(cardcount[0]==0&&cardcount[4]==0
	&&cardcount[6]==0&&cardcount[8]==0){
		printf("绿一色!\n");
		//_sleep(1000);
		delete[] cardcount;
		return true;
	}
	if(cardcount[1]==2&&cardcount[2]==2
	&&cardcount[3]==2&&cardcount[4]==2&&cardcount[5]==2
	&&cardcount[6]==2&&cardcount[7]==2){
		printf("大竹林!\n");
		//_sleep(1000);
		delete[] cardcount;
		return true;
	}
	delete[] cardcount;
	return false;
}
#include "playerAI.h"

#include<queue>
#include<conio.h>
using namespace std;

bool noblock(int x,int y,int sign,Map &M){
	if(sign==RED){
		for(int i=x-1;i>=0;i--)
		if(M.data[i][y])return 0;
		return 1;
	}
}
int bfs(int sx,int sy,int tx,int ty,Map &M){
	const int dy[4]={1,0,-1,0},dx[4]={0,-1,0,1};
	queue<pair<int,int> > Q;
	Q.push(make_pair(tx,ty));
	bool vis[MAP_SIZE][MAP_SIZE]={0};
	vis[tx][ty]=1;
	for(int i=1;i<=MAP_SIZE-2;i++)
	for(int j=0;j<=MAP_SIZE-1;j++)
	if((i!=sx||j!=sy)&&M.data[i][j]!=0)vis[i][j]=1;
	while(!Q.empty()){
		int hx=Q.front().first;
		int hy=Q.front().second;
		//printf("%d %d\n",hx,hy);
		Q.pop();
		for(int d=0;d<4;d++){
			int x=hx+dx[d];
			int y=hy+dy[d];
			if(x==sx&&y==sy){
				//printf("%d %d %d %d %d\n",sx,sy,tx,ty,d);
				return d^2;
			}
			if(!vis[x][y]&&
			(x>=1&&x<=MAP_SIZE-2&&y>=0&&y<=MAP_SIZE-1)){
				vis[x][y]=1;
				Q.push(make_pair(x,y));
			}
		}
	}
	return -1;
}
int remaining=0311111113;
int destx=-100,desty=-100,desttype=-1;
Operation PlayerRedAI(Player plr,Map M){
	Operation op;
	op.type=STAY;

	//---键盘事件响应开始
	//注释掉该部分，可以观赏两个AI对打，九莲大战大竹林
	char c = getch();
	if(c=='w'||c=='W')op.type=MOVE,op.dir=1;
	if(c=='a'||c=='A')op.type=MOVE,op.dir=2;
	if(c=='s'||c=='S')op.type=MOVE,op.dir=3;
	if(c=='d'||c=='D')op.type=MOVE,op.dir=0;
	if(c=='q'||c=='Q')op.type=MAGNET;
	if(c=='e'||c=='E')op.type=FIRE;
	return op;
	//---键盘事件响应结束

	if(destx==plr.pos.x&&desty==plr.pos.y){
		if(desttype==1){
			destx=desty=-100;
			if(remaining)
			remaining-=(1<<(3*(plr.pocket-1)));
			op.type=FIRE;
		}
		else if(desttype==0){
			destx=desty=-100;
			//printf("233\n");
			goto label1;
		}
	}
	else if(plr.pocket&&noblock(plr.pos.x,plr.pos.y,RED,M)){
		destx=desty=-100;
		if(remaining)
		remaining-=(1<<(3*(plr.pocket-1)));
		op.type=FIRE;
	}
	else if(destx!=-100&&desty!=-100){
		op.type=MOVE;
		op.dir=bfs(plr.pos.x,plr.pos.y,destx,desty,M);
	}
	else{
label1:if(plr.pocket==0){
		desttype=0;
		for(int i=1;i<=MAP_SIZE-2;i++)
		for(int j=0;j<=MAP_SIZE-1;j++)
		if((i!=plr.pos.x||j!=plr.pos.y)&&
		M.data[i][j]!=0&&
		(remaining==0||(remaining&(7<<(3*(M.data[i][j]-1)))))&&
		(abs(i-plr.pos.x)+abs(j-plr.pos.y)
		<abs(destx-plr.pos.x)+abs(desty-plr.pos.y)))
		{destx=i;desty=j;}
		//printf("%d %d\n",destx,desty);
		}
		else{
		//printf("666\n");
		desttype=1;
		for(int i=1;i<=1;i++)
		for(int j=0;j<=MAP_SIZE-1;j++)
		if(M.data[plr.sign?MAP_SIZE-1:0][j]==0&&
		abs(i-plr.pos.x)+abs(j-plr.pos.y)
		<abs(destx-plr.pos.x)+abs(desty-plr.pos.y))
		{destx=i;desty=j;}
		}
		int dir=bfs(plr.pos.x,plr.pos.y,destx,desty,M);
		if(dir==-1)op.type=STAY;
		else{
			op.type=MOVE;
			op.dir=dir;
		}
	}
	return op;
}
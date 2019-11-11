#include "playerAI.h"

#include<queue>
using namespace std;

bool Bnoblock(int x,int y,int sign,Map &M){
	if(sign==BLUE){
		for(int i=x+1;i<MAP_SIZE;i++)
		if(M.data[i][y])return 0;
		return 1;
	}
}
int Bbfs(int sx,int sy,int tx,int ty,Map &M){
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
int Bremaining=0022222220;
int Bdestx=-100,Bdesty=-100,Bdesttype=-1;
Operation PlayerBlueAI(Player plr,Map M){
	Operation op;
	/*op.type=STAY;
	char c;
	scanf("%c%*c",&c);
	if(c=='w'||c=='W')op.type=MOVE,op.dir=1;
	if(c=='a'||c=='A')op.type=MOVE,op.dir=2;
	if(c=='s'||c=='S')op.type=MOVE,op.dir=3;
	if(c=='d'||c=='D')op.type=MOVE,op.dir=0;
	if(c=='q'||c=='Q')op.type=MAGNET;
	if(c=='e'||c=='E')op.type=FIRE;*/
	if(Bdestx==plr.pos.x&&Bdesty==plr.pos.y){
		if(Bdesttype==1){
			Bdestx=Bdesty=-100;
			if(Bremaining)
			Bremaining-=(1<<(3*(plr.pocket-1)));
			op.type=FIRE;
		}
		else if(Bdesttype==0){
			Bdestx=Bdesty=-100;
			//printf("233\n");
			goto label1;
		}
	}
	else if(plr.pocket&&Bnoblock(plr.pos.x,plr.pos.y,BLUE,M)){
		Bdestx=Bdesty=-100;
		if(Bremaining)
		Bremaining-=(1<<(3*(plr.pocket-1)));
		op.type=FIRE;
	}
	else if(Bdestx!=-100&&Bdesty!=-100){
		op.type=MOVE;
		op.dir=Bbfs(plr.pos.x,plr.pos.y,Bdestx,Bdesty,M);
	}
	else{
label1:if(plr.pocket==0){
		Bdesttype=0;
		for(int i=1;i<=MAP_SIZE-2;i++)
		for(int j=0;j<=MAP_SIZE-1;j++)
		if((i!=plr.pos.x||j!=plr.pos.y)&&
		M.data[i][j]!=0&&
		(Bremaining==0||(Bremaining&(7<<(3*(M.data[i][j]-1)))))&&
		(abs(i-plr.pos.x)+abs(j-plr.pos.y)
		<abs(Bdestx-plr.pos.x)+abs(Bdesty-plr.pos.y)))
		{Bdestx=i;Bdesty=j;}
		}
		else{
		Bdesttype=1;
		for(int i=MAP_SIZE-2;i<=MAP_SIZE-2;i++)
		for(int j=0;j<=MAP_SIZE-1;j++)
		if(M.data[plr.sign?MAP_SIZE-1:0][j]==0&&
		abs(i-plr.pos.x)+abs(j-plr.pos.y)
		<abs(Bdestx-plr.pos.x)+abs(Bdesty-plr.pos.y))
		{Bdestx=i;Bdesty=j;}
		}
		int dir=Bbfs(plr.pos.x,plr.pos.y,Bdestx,Bdesty,M);
		if(dir==-1)op.type=STAY;
		else{
			op.type=MOVE;
			op.dir=dir;
		}
	}
	return op;
}
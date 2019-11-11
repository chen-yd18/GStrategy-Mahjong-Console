#include "logic.h"

void logic::initGame(){
	Player red(RED),blue(BLUE);
	Players[RED]=red;
	Players[BLUE]=blue;
	memset(MapData.data,0,sizeof(MapData.data));
	for (int i = 0; i < 36; i++)
		comingCard[i] = i % 9 + 1;
	srand(time(NULL));
	random_shuffle(comingCard,comingCard+9);
	random_shuffle(comingCard+9, comingCard + 18);
	random_shuffle(comingCard+18, comingCard + 27);
	random_shuffle(comingCard+27, comingCard + 36);
	for(int i=TURN_COUNT-TURN_COUNT/36;i>=0;i-=TURN_COUNT/36)
	comingCard[i]=comingCard[i/(TURN_COUNT/36)];
	for(int i=0;i<36;i++)
	if(i%(TURN_COUNT/36)!=0)comingCard[i]=0;
}
Operation logic::validate(Operation op,int sign){
	if(op.type==MOVE){
		if(op.dir<0||op.dir>3){
			printf("Player %s: Illegal Direction\n",sign?"Blue":"Red");
			op.type=STAY;
		}
		else{
			int x=Players[sign].pos.x+dx[op.dir];
			int y=Players[sign].pos.y+dy[op.dir];
			if(x==0||x==MAP_SIZE-1){
				printf("Player %s: Cannot Touch Card-placing Areas\n",sign?"Blue":"Red");
				op.type=STAY;
			} 
			else if(y<0||y>=MAP_SIZE){
				printf("Player %s: Trying to Step out of the Map\n",sign?"Blue":"Red");
				op.type=STAY;
			}
			else if(MapData.data[x][y]>=1&&MapData.data[x][y]<=9){
				if(Players[sign].pocket!=0){
				printf("Player %s: Already brought a Card\n",sign?"Blue":"Red");
				op.type=STAY;
				}
				else if(Players[sign].ting!=0
				&&(Players[sign].ting&(1<<(MapData.data[x][y]-1)))==0){
				printf("Player %s: You don't Need Card #%d!\n",
				sign?"Blue":"Red",MapData.data[x][y]);
				op.type=STAY;
				}
			}
		}
	}
	else if(op.type==MAGNET){
		if(MapData.thisTurnDrop==0){
			printf("Player %s: No Card Drops\n",sign?"Blue":"Red");
			op.type=STAY;
		}
		else{
		int bonia_index=-1;
		for(int i=1;i<=MAP_SIZE-2;i++)
		if(MapData.data[sign?MAP_SIZE-1:0][i-1]==MapData.thisTurnDrop
		&&MapData.data[sign?MAP_SIZE-1:0][i+1]==MapData.thisTurnDrop
		&&MapData.data[sign?MAP_SIZE-1:0][i]==0)
		{bonia_index=i;break;}
		if(bonia_index==-1){
			printf("Player %s: Cannot Pung\n",sign?"Blue":"Red");
			op.type=STAY;
		}
		}
	}
	else if(op.type==FIRE){
		if(Players[sign].pocket==0){
			printf("Player %s: No Card to Fire\n",sign?"Blue":"Red");
			op.type=STAY;
		}
		else{
		int y=Players[sign].pos.y;
		for(int x=Players[sign].pos.x+(sign?1:-1);x!=(sign?MAP_SIZE:-1);x+=(sign?1:-1)){
			if(MapData.data[x][y]!=0){
				//printf("%d %d\n",x,y);
				printf("Player %s: Fire Blocked by Other Cards\n",sign?"Blue":"Red");
				op.type=STAY;
				break;
			}
		}
		}	
	}
	return op;
}
void logic::DBG_outputMap(){
	for(int i=0;i<MAP_SIZE;i++){
	for(int j=0;j<MAP_SIZE;j++){
		if(Players[RED].pos.x==i&&Players[RED].pos.y==j)putchar('R');
		else if(Players[BLUE].pos.x==i&&Players[BLUE].pos.y==j)putchar('B');
		else if(MapData.data[i][j]==0)putchar('.');
		else putchar(MapData.data[i][j]+'0');
	}
	putchar('\n');
	}
}
void logic::GameRun(){
	FILE* fp = fopen("ops.visual", "w");
	initGame();
	bool visited[MAP_SIZE][MAP_SIZE]={0};
	bool RON = false;
	for(int turn=0;turn<TURN_COUNT;turn++,Players[RED].remainStep--,Players[BLUE].remainStep--){
		//回合开始时，先向文件输出地图信息
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++)
				if (RON)fprintf(fp, "-1 ");//如果上回合有人胡牌，则整个矩阵为-1，可视化时显示全屏特效
				else if (Players[RED].pos.x == i && Players[RED].pos.y == j)
					fprintf(fp, "10 ");//矩阵中10表示红方玩家
				else if (Players[BLUE].pos.x == i && Players[BLUE].pos.y == j)
					fprintf(fp, "11 ");//矩阵中11表示蓝方玩家
				else fprintf(fp, "%d ", MapData.data[i][j]);//0表示空地，1~9表示麻将牌
			fprintf(fp, "\n");
		}
		fprintf(fp, "2 %d %d\n", Players[RED].pocket, Players[BLUE].pocket);
			RON = false;
		//向控制台输出回合编号
		printf("Round %d:\n",turn+1);
		MapData.thisTurnDrop=comingCard[turn];
		if(comingCard[turn]!=0){
			//printf("233\n");
			int xx,yy;
			do{
			xx=rand()%(MAP_SIZE-4)+2;
			yy=rand()%MAP_SIZE;
			}while(visited[xx][yy]);
			visited[xx][yy]=true;
			MapData.thisTurnDropPoint.x=xx;
			MapData.thisTurnDropPoint.y=yy;
			printf("Card #%d Drops on (%d,%d)\n",MapData.thisTurnDrop,
			MapData.thisTurnDropPoint.x,MapData.thisTurnDropPoint.y);
			MapData.data[xx][yy]=comingCard[turn];
		}
		//向控制台输出地图信息
		DBG_outputMap();
	Operation opred=PlayerRedAI(Players[RED],MapData);
	Operation opblue=PlayerBlueAI(Players[BLUE],MapData);
	//fprintf(fp, "%d %d\n", opred.type, opred.dir);
	//fprintf(fp, "%d %d\n", opblue.type, opblue.dir);
	opred=validate(opred,RED);
	opblue=validate(opblue,BLUE);
	//printf("RED %d %d\n",opred.type,opred.dir);
	//printf("BLUE %d %d\n",opblue.type,opblue.dir);
	int redpickx=-1,redpicky=-1,redpriority=19260817;
	int bluepickx=-2,bluepicky=-2,bluepriority=19260817;
	if(opred.type==STAY){
		printf("RED STAY\n");
		fprintf(fp, "0 0\n");
	}
	else if(opred.type==MOVE){
		Players[RED].pos.x+=dx[opred.dir];
		Players[RED].pos.y+=dy[opred.dir];
		int x=Players[RED].pos.x,y=Players[RED].pos.y;
		printf("RED MOVE TO (%d,%d)\n",x,y);
		fprintf(fp, "0 1 %d %d %d %d\n", x - dx[opred.dir], y - dy[opred.dir], x, y);
		if(MapData.data[x][y]!=0){
			printf("RED PICK Card #%d\n",MapData.data[x][y]);
			fprintf(fp, "0 11 %d %d %d\n", x, y, MapData.data[x][y]);
			redpickx=x;redpicky=y;
		}
	}
	else if(opred.type==MAGNET){
		int bonia_index=-1;
		for(int i=1;i<=MAP_SIZE-2;i++)
		if(MapData.data[0][i-1]==MapData.thisTurnDrop
		&&MapData.data[0][i+1]==MapData.thisTurnDrop
		&&MapData.data[0][i]==0)
		{bonia_index=i;break;}
				printf("RED BONIA!\n");
				fprintf(fp, "0 2 %d %d %d %d\n", MapData.thisTurnDrop, MapData.thisTurnDropPoint.x,
					MapData.thisTurnDropPoint.y, bonia_index);
				MapData.data[0][bonia_index]=MapData.thisTurnDrop;
				MapData.data[MapData.thisTurnDropPoint.x][MapData.thisTurnDropPoint.y]=0;
				bool ok=1;
				for(int i=0;i<MAP_SIZE;i++)
				if(MapData.data[0][i]==0){ok=0;break;}
				if(Players[RED].ting==0&&ok){
					ronia ronJudger(MapData.data[0]);
					Players[RED].ting
						=ronJudger.calcTing();
					if(Players[RED].pocket!=0){
						int x,y;
						do{
							x=rand()%(MAP_SIZE-4)+2;
							y=rand()%MAP_SIZE;
						}while(MapData.data[x][y]!=0);
						MapData.data[x][y]=Players[RED].pocket;
						Players[RED].pocket=0;
					}
					if (Players[RED].ting) {
						printf("RED READY for RON!\n");
						fprintf(fp, "0 99 %d\n",Players[RED].ting);
					}
				
			
		}
	}
	else if(opred.type==FIRE){
		printf("RED FIRE Card #%d into Slot %d\n",
		Players[RED].pocket,Players[RED].pos.y);
		fprintf(fp, "0 3 %d %d %d\n", Players[RED].pocket, Players[RED].pos.x, Players[RED].pos.y);
		MapData.data[0][Players[RED].pos.y]=Players[RED].pocket;
		if(Players[RED].pocket==1&&!MapData.haveChargeChicken){
			MapData.haveChargeChicken=true;
			Players[RED].score+=1;
			printf("RED CHARGE CHICKEN!\n");
			fprintf(fp, "0 31\n");
		}
		Players[RED].pocket=0;
		bool ok=1;
		for(int i=0;i<MAP_SIZE;i++)
		if(MapData.data[0][i]==0){ok=0;break;}
		if(Players[RED].ting==0&&ok){
			ronia ronJudger(MapData.data[0]);
			Players[RED].ting
			=ronJudger.calcTing();
			if (Players[RED].ting) {
				printf("RED READY for RON!\n");
				fprintf(fp, "0 99 %d\n", Players[RED].ting);
			}
		}
	}
	
	if(opblue.type==STAY){
		printf("BLUE STAY\n");
		fprintf(fp, "1 0\n");
	}
	else if(opblue.type==MOVE){
		Players[BLUE].pos.x+=dx[opblue.dir];
		Players[BLUE].pos.y+=dy[opblue.dir];
		int x=Players[BLUE].pos.x,y=Players[BLUE].pos.y;
		printf("BLUE MOVE TO (%d,%d)\n",x,y);
		fprintf(fp, "1 1 %d %d %d %d\n", x - dx[opblue.dir], y - dy[opblue.dir], x, y);
		if(MapData.data[x][y]!=0){
			printf("BLUE PICK Card #%d\n",MapData.data[x][y]);
			fprintf(fp, "1 11 %d %d %d\n", x, y, MapData.data[x][y]);
			bluepickx=x;bluepicky=y;
		}
	}
	else if(opblue.type==MAGNET){
		int bonia_index=-1;
		for(int i=1;i<=MAP_SIZE-2;i++)
		if(MapData.data[MAP_SIZE-1][i-1]==MapData.thisTurnDrop
		&&MapData.data[MAP_SIZE-1][i+1]==MapData.thisTurnDrop
		&&MapData.data[MAP_SIZE-1][i]==0)
		{bonia_index=i;break;}
				printf("BLUE BONIA!\n");
				fprintf(fp, "1 2 %d %d %d %d\n", MapData.thisTurnDrop, MapData.thisTurnDropPoint.x,
					MapData.thisTurnDropPoint.y, bonia_index);
				MapData.data[MAP_SIZE-1][bonia_index]=MapData.thisTurnDrop;
				MapData.data[MapData.thisTurnDropPoint.x][MapData.thisTurnDropPoint.y]=0;
				bool ok=1;
				for(int i=0;i<MAP_SIZE;i++)
				if(MapData.data[MAP_SIZE-1][i]==0){ok=0;break;}
				if(Players[BLUE].ting==0&&ok){
					ronia ronJudger(MapData.data[MAP_SIZE-1]);
					Players[BLUE].ting
						=ronJudger.calcTing();
					if(Players[BLUE].pocket!=0){
						int x,y;
						do{
							x=rand()%(MAP_SIZE-4)+2;
							y=rand()%MAP_SIZE;
						}while(MapData.data[x][y]!=0);
						MapData.data[x][y]=Players[BLUE].pocket;
						Players[RED].pocket=0;
					}
					if (Players[BLUE].ting) {
						printf("BLUE READY for RON!\n");
						fprintf(fp, "1 99 %d\n", Players[BLUE].ting);
					}
				
			
		}
	}
	else if(opblue.type==FIRE){
		printf("BLUE FIRE Card #%d into Slot %d\n",
		Players[BLUE].pocket,Players[BLUE].pos.y);
		MapData.data[MAP_SIZE-1][Players[BLUE].pos.y]=Players[BLUE].pocket;
		fprintf(fp, "1 3 %d %d %d\n", Players[BLUE].pocket, 
			Players[BLUE].pos.x, Players[BLUE].pos.y);
		if(Players[BLUE].pocket==1&&!MapData.haveChargeChicken){
			MapData.haveChargeChicken=true;
			Players[BLUE].score+=1;
			printf("BLUE CHARGE CHICKEN!\n");
			fprintf(fp, "1 31\n");
		}
		Players[BLUE].pocket=0;
		bool ok=1;
		for(int i=0;i<MAP_SIZE;i++)
		if(MapData.data[MAP_SIZE-1][i]==0){ok=0;break;}
		if(Players[BLUE].ting==0&&ok){
			ronia ronJudger(MapData.data[MAP_SIZE-1]);
			Players[BLUE].ting
			=ronJudger.calcTing();
			if (Players[BLUE].ting) {
				printf("BLUE READY for RON!\n");
				fprintf(fp, "1 99 %d\n", Players[BLUE].ting);
			}
		}
	}
	
	if(redpickx>=0&&redpicky>=0&&MapData.data[redpickx][redpicky]>0
	&&(Players[RED].ting&(1<<(MapData.data[redpickx][redpicky]-1)))){
		RON = true;
		ronia ronJudger(MapData.data[0]);
		if(ronJudger.yakuma(MapData.data[redpickx][redpicky])){
			printf("RED YAKUMA!!!\n");
			getchar();
			fprintf(fp,"999 0\n");
			fclose(fp);
			exit(0);
		}
		else{
			printf("RED RONIA!!!\n");
			Players[RED].ronCount++;
			Players[RED].score+=Players[RED].ronCount;
			MapData.data[redpickx][redpicky]=0;
			continue;
		}
	}
	if(bluepickx>=0&&bluepicky>=0&&MapData.data[bluepickx][bluepicky]>0
	&&(Players[BLUE].ting&(1<<(MapData.data[bluepickx][bluepicky]-1)))){
		RON = true;
		ronia ronJudger(MapData.data[MAP_SIZE-1]);
		if(ronJudger.yakuma(MapData.data[bluepickx][bluepicky])){
			printf("BLUE YAKUMA!!!\n");
			getchar();
			fprintf(fp,"0 999\n");
			fclose(fp);
			exit(0);
		}
		else{
			printf("BLUE RONIA!!!\n");
			Players[BLUE].ronCount++;
			Players[BLUE].score+=Players[BLUE].ronCount;
			MapData.data[bluepickx][bluepicky]=0;
			continue;
		}
	}
	
	if(redpickx==bluepickx&&redpicky==bluepicky){
		if(redpriority<bluepriority){
			printf("RED GET Card #%d\n",
			Players[RED].pocket=MapData.data[redpickx][redpicky]);
			MapData.data[redpickx][redpicky]=0;
		}
		else if(redpriority>bluepriority){
			printf("BLUE GET Card #%d\n",
			Players[BLUE].pocket=MapData.data[bluepickx][bluepicky]);
			MapData.data[bluepickx][bluepicky]=0;
		}
		else if(rand()%2){
			printf("BLUE GET Card #%d\n",
			Players[BLUE].pocket=MapData.data[bluepickx][bluepicky]);
			MapData.data[bluepickx][bluepicky]=0;
		}
		else{
			printf("RED GET Card #%d\n",
			Players[RED].pocket=MapData.data[redpickx][redpicky]);
			MapData.data[redpickx][redpicky]=0;
		}
	}
	else{
		if(redpickx>=0&&redpicky>=0){
			printf("RED GET Card #%d\n",
			Players[RED].pocket=MapData.data[redpickx][redpicky]);
			MapData.data[redpickx][redpicky]=0;
		}
		if(bluepickx>=0&&bluepicky>=0){
			printf("BLUE GET Card #%d\n",
			Players[BLUE].pocket=MapData.data[bluepickx][bluepicky]);
			MapData.data[bluepickx][bluepicky]=0;
		}
	}
	fprintf(fp, "-1\n");//回合结束
	}
	if(Players[RED].ting)Players[RED].score+=2;
	if(Players[BLUE].ting)Players[BLUE].score+=2;
	printf("FINAL SCORE: %d - %d\n",Players[RED].score,Players[BLUE].score);
	getchar();
	fprintf(fp, "%d %d\n", Players[RED].score, Players[BLUE].score);
	fclose(fp);
}
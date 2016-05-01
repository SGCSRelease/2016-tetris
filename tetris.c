#include "tetris.h"

static struct sigaction act, oact;

int main(){/*{{{*/
	int exit=0;

	initscr();
	noecho();
    curs_set(0);
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	writeRankFile();
	endwin();
	system("clear");
	return 0;
}/*}}}*/

void InitTetris(void){/*{{{*/
	int i,j,window;
    for(window = 0; window < 2; ++window)
        for(j=0;j<HEIGHT;j++)
            for(i=0;i<WIDTH;i++)
                field[window][j][i]=0;

	for(i=0;i<BLOCK_NUM;++i)
		nextBlock[i] = rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
	total_seconds = 0.0;
	total_moves = 0;
	
    // Player 1
	move(20,WIDTH+10);
	printw("total time(s) =");
	move(21,WIDTH+10);
	printw("score/time(s) =");
	move(22,WIDTH+10);
	printw("score/space(byte) =");
    // Player 2
	move(20,WINSPAN + WIDTH+10);
	printw("total time(s) =");
	move(21,WINSPAN + WIDTH+10);
	printw("score/time(s) =");
	move(22,WINSPAN + WIDTH+10);
	printw("score/space(byte) =");
	DrawOutline();
	DrawField(PLAYER1);
	DrawField(PLAYER2);
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate,PLAYER1);
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate,PLAYER2);
	DrawNextBlock(nextBlock, PLAYER1);
	DrawNextBlock(nextBlock, PLAYER2);
	PrintScore(score, PLAYER1);
	PrintScore(score, PLAYER2);
}/*}}}*/

void DrawOutline(void){	/*{{{*/
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH, PLAYER1);
	DrawBox(0,0,HEIGHT,WIDTH, PLAYER2);

	/* next block을 보여주는 공간의 테두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	move(2,WINSPAN + WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8, PLAYER1);
	DrawBox(3,WIDTH+10,4,8, PLAYER2);

	/* next next block을 보여주는 공간의 테두리를 그린다.*/
	move(9,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8, PLAYER1);
	DrawBox(9,WIDTH+10,4,8, PLAYER2);
	
	/* score를 보여주는 공간의 테두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	move(16,WINSPAN + WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8, PLAYER1);
	DrawBox(17,WIDTH+10,1,8, PLAYER2);
}/*}}}*/

int GetCommand(void){/*{{{*/
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		for(;CheckToMove(field[PLAYER1],nextBlock[0],blockRotate,blockY+1,blockX)!=0;++blockY);
		timed_out = 1;
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}/*}}}*/

int ProcessCommand(int command){/*{{{*/
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field[PLAYER1],nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field[PLAYER1],nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field[PLAYER1],nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field[PLAYER1],nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	case ' ':
		break;
	default:
		break;
	}
	if(drawFlag) {
        DrawChange(field[PLAYER1],command,nextBlock[0],blockRotate,blockY,blockX, PLAYER1);
        DrawChange(field[PLAYER2],command,nextBlock[0],blockRotate,blockY,blockX, PLAYER2);
    }
	return ret;	
}/*}}}*/

void DrawField(int selectPlayer){/*{{{*/
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,selectPlayer*WINSPAN + 1);
		for(i=0;i<WIDTH;i++){
			if(field[selectPlayer][j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}/*}}}*/

void PrintScore(int score, int selectPlayer){/*{{{*/
	move(18,selectPlayer*WINSPAN + WIDTH+11);
	printw("%8d",score);
}/*}}}*/

void DrawNextBlock(int *nextBlock, int selectPlayer){/*{{{*/
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,selectPlayer*WINSPAN + WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(10+i,selectPlayer*WINSPAN + WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}/*}}}*/

void DrawBlock(int y, int x, int blockID,
        int blockRotate, char tile, int selectPlayer){/*{{{*/
	int i,j;
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,selectPlayer*WINSPAN + j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}

	move(HEIGHT,selectPlayer*WINSPAN + WIDTH+10);
}/*}}}*/

void DrawBox(int y,int x, int height, int width, int selectPlayer){/*{{{*/
	int i,j;
	move(y,selectPlayer*WINSPAN + x);
	addch(ACS_ULCORNER);
	for(i=selectPlayer*WINSPAN;i<selectPlayer*WINSPAN + width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,selectPlayer*WINSPAN + x);
		addch(ACS_VLINE);
		move(y+j+1,selectPlayer*WINSPAN + x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,selectPlayer*WINSPAN + x);
	addch(ACS_LLCORNER);
	for(i=selectPlayer*WINSPAN;i<selectPlayer*WINSPAN + width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}/*}}}*/

void play(){/*{{{*/
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10, PLAYER1);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10, PLAYER2);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			move(HEIGHT/2,WINSPAN + WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
			newRank(score);

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10, PLAYER1);
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10, PLAYER2);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	move(HEIGHT/2,WINSPAN + WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}/*}}}*/

void playPLAYER1(){
    

}

void playPLAYER2(){


}


char menu(){/*{{{*/
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. exit\n");
	return wgetch(stdscr);
}/*}}}*/

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate,int blockY, int blockX){/*{{{*/
	int i,j;

	for(i=0;i<4;++i)
		for(j=0;j<4;++j)
			if(block[currentBlock][blockRotate][i][j])
				if(blockY+i>=HEIGHT || blockX+j<0 || blockX+j>=WIDTH || f[blockY+i][blockX+j]) return 0;
	
	return 1;
}/*}}}*/

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,
        int blockRotate, int blockY, int blockX, int selectPlayer){/*{{{*/
	int i,j,x,y,rot,pre;

	switch(command){
		case KEY_UP:    x =blockX;   y = blockY;   rot = (blockRotate+3)%4; break;
		case KEY_DOWN:  x =blockX;   y = blockY-1; rot = blockRotate;       break;
		case KEY_LEFT:  x =blockX+1; y = blockY;   rot = blockRotate;       break;
		case KEY_RIGHT: x =blockX-1; y = blockY;   rot = blockRotate;       break;
		default:        x =blockX;   y = blockY;   rot = blockRotate;       break;
	}
	
	for(pre=y;CheckToMove(field[selectPlayer],currentBlock,rot,pre+1,x)!=0;++pre);
	
    x += selectPlayer*WINSPAN;

	for(i=0;i<4;++i)
		for(j=0;j<4;++j)
		{
			if(block[currentBlock][rot][i][j]==1)
			{
				move(y+i+1,x+j+1);
				printw(".");
				move(pre+i+1,x+j+1);
				printw(".");
			}
		}
	
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate, selectPlayer);
}/*}}}*/

void BlockDown(int sig){/*{{{*/
	int i;
	
	if(CheckToMove(field[PLAYER1],nextBlock[0],blockRotate,blockY+1,blockX))
	{
		blockY++;
		DrawChange(field[PLAYER1],KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX, PLAYER1);
		DrawChange(field[PLAYER2],KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX, PLAYER2);
	}
	else
	{
		if(blockY==-1) gameOver = true;
		
		score += AddBlockToField(field[PLAYER1],nextBlock[0],blockRotate,blockY,blockX);
		score += AddBlockToField(field[PLAYER2],nextBlock[0],blockRotate,blockY,blockX);
		score += DeleteLine(field[PLAYER1]);
		score += DeleteLine(field[PLAYER2]);
		
		for(i=0;i<BLOCK_NUM-1;++i)
			nextBlock[i] = nextBlock[i+1];
		nextBlock[i] = rand()%7;

		DrawNextBlock(nextBlock, PLAYER1);
        DrawNextBlock(nextBlock, PLAYER2);
		DrawField(PLAYER1);
        DrawField(PLAYER2);
		PrintScore(score, PLAYER1);
        PrintScore(score, PLAYER2);
	
		blockY = -1;
		blockX = WIDTH/2-2;
		blockRotate = 0;
	}
	timed_out = 0;
}/*}}}*/

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){/*{{{*/
	int i,j,score=0;

	for(i=0;i<4;++i)
		for(j=0;j<4;++j)
			if(block[currentBlock][blockRotate][i][j])
			{
				f[blockY+i][blockX+j] = 1;
				if(blockY+i+1==HEIGHT||f[blockY+i+1][blockX+j+1]) score+=10;
			}
	return score;
}/*}}}*/

int DeleteLine(char f[HEIGHT][WIDTH]){/*{{{*/
	int i,j,k;
	int cnt = 0;

	for(i=1;i<HEIGHT;++i)
	{
		bool is_done = true;
		for(j=0;j<WIDTH;++j)
			if(f[i][j]==0) is_done = false;
		if(is_done)
		{
			for(k=i;k>0;--k)
				for(j=0;j<WIDTH;++j)
					f[k][j] = f[k-1][j];
			cnt++;
		}
	}

	return cnt*cnt*100;
}/*}}}*/

void DrawShadow(int y, int x, int blockID,int blockRotate, int selectPlayer){/*{{{*/
	for(;CheckToMove(field[selectPlayer],blockID,blockRotate,y+1,x)!=0;++y);
	DrawBlock(y,x,blockID,blockRotate,'\\', selectPlayer);
}/*}}}*/

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate, int selectPlayer){/*{{{*/
	DrawBlock(y,x,blockID,blockRotate,' ', selectPlayer);
	DrawShadow(y,x,blockID,blockRotate,selectPlayer);
}/*}}}*/

void createRankList(){/*{{{*/
	FILE* fp = fopen("rank.txt","r");
	int i,tot=-1;
	
	if(fp==NULL)
	{
		fclose(fp);
		fp = fopen("rank.txt","w");
		fprintf(fp,"0");
		fclose(fp);
		fp = fopen("rank.txt","r");
	}

	if(rankRoot==NULL)
	{
		fscanf(fp,"%d\n",&tot);
		if(tot==-1)
		{
			tot = 0;
			fclose(fp);
			fp = fopen("rank.txt","w");
			fprintf(fp,"0");
			fclose(fp);
			fp = fopen("rank.txt","r");
		}
		rankRoot = (RankNode*)malloc(sizeof(RankNode)*(tot+1));
		rankRoot[0].score = tot;
		for(i=1;i<=tot;++i) fscanf(fp,"%s %d\n",rankRoot[i].name,&rankRoot[i].score);
		qsort(rankRoot+1,tot,sizeof(RankNode),compareRank);
	}

	fclose(fp);
}

int compareRank(const void* l, const void* r){
	return ((RankNode*)r)->score - ((RankNode*)l)->score;
}/*}}}*/

void rank(){/*{{{*/
	int s,f,i;
	char input[20];

	createRankList();
	
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	switch(wgetch(stdscr))
	{
		case '1':
			s = 1;
			f = rankRoot[0].score;
			echo();
			printw("X: ");
			scanw("%d",&s);
			printw("Y: ");
			scanw("%d",&f);
			noecho();
			printw("       name         |   score\n");
			printw("--------------------------------\n");
			if(s>f || s<1 || f>rankRoot[0].score) printw("\nsearch failure: no rank in the list\n");
			else for(;s<=f;++s) printw(" %-19s| %d\n",rankRoot[s].name,rankRoot[s].score);
			getch();
			break;
		case '2':
			s = 0;
			echo();
			printw("input the name: ");
			scanw("%19s",input);
			noecho();
			printw("       name         |   score\n");
			printw("--------------------------------\n");
			for(i=1;i<=rankRoot[0].score;++i)
				if(strcmp(rankRoot[i].name,input)==0)
				{
					s++;
					printw(" %-19s| %d\n",rankRoot[i].name,rankRoot[i].score);
				}
			if(s==0) printw("\nsearch failure: no rank in the list\n");
			getch();
			break;
		case '3':
			echo();
			printw("input the rank: ");
			scanw("%d",&s);
			noecho();
			if(s>0&&s<=rankRoot[0].score)
			{
				for(i=s;i<rankRoot[0].score;++i) rankRoot[i] = rankRoot[i+1];
				rankRoot[0].score--;
				printw("\nresult: the rank deleted.\n");
			}
			else printw("\nsearch failure: the rank not in the list\n");
			getch();
			break;
		default: return;
	}
}/*}}}*/

void writeRankFile(){/*{{{*/
	int i,tot;
	FILE* fp = fopen("rank.txt","w");

	if(rankRoot!=NULL)
	{
		tot = rankRoot[0].score;
		fprintf(fp,"%d",tot);
		for(i=1;i<=tot;++i) fprintf(fp,"\n%s %d",rankRoot[i].name,rankRoot[i].score);
		free(rankRoot);
		rankRoot = NULL;
	}
	fclose(fp);
}/*}}}*/

void newRank(int score){/*{{{*/
	int i,j,tot;
	char str[20];
	
	clear();
	printw("your name: ");
	echo();
	scanw("%19s",str);
	noecho();

	if(rankRoot==NULL) createRankList();
	rankRoot[0].score++;
	tot = rankRoot[0].score;

	for(i=1;i<tot;++i) if(rankRoot[i].score<score) break;

	rankRoot = (RankNode*) realloc(rankRoot,(tot+1)*sizeof(RankNode));

	for(j=tot;j>i;--j) rankRoot[j] = rankRoot[j-1];

	rankRoot[i].score = score;
	strcpy(rankRoot[i].name,str);
}/*}}}*/

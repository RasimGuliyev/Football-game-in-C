#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <stdbool.h>

typedef struct Player{
	int x;
	int y;
	char number;
	char team;
}Player;
Player black[8];
Player white[8];

typedef struct Ball{
	int x;
	int y;
	char shape;
}Ball;
Ball ball;

int bpass=0;int wpass=0;int bshot=0;int wshot=0;//for stats

int Timer=0;//Game clock

char field[16][81];//I didn't use define, because it wasn't stated in assignment

void CreateField();//Fills field[][]

void PrintField();//Prints field for once


void CreateTeams(Player*,Player*);//Creates 2 teams

void PrintDefaultPlayers(Player[],Player[],Ball*);//For printing default player positions after every goal

void PlayerMove(Player*,int X,int Y);//Move player by 1 


int CheckPlayerX(Player [],Ball*);//Checks if ball is around any player for same Y
int CheckPlayerY(Player [],Ball*);//Checks if ball is around any player for same X

int CheckKick(Player [], Ball*);//If one of the check functions return value then this function returns non-zero value

int BallMove(Player[],Player[],Ball*);//Checks CheckKick function for 2 teams

void BallMovement(Ball *ball,int j,int k);//Moves ball any direction by given parameters


float DistanceBallPlayer(Player,Ball);//Returns distance of any player and ball

int DistancePlayerPlayer(Player,Player);//Returns distance of any player and any player

bool IsTherePlayer(int,int);//Checks if there is any player in given coordinates

void Gotoyx(int,int);//Brings the cursor to given y/x coordinates

void HideCursor();//For hiding cursor while printing

void Pass(Player,Ball*);//Every player passes to specific teammate

void Shot(Player,Ball*);//Shots +-10 x direction

void Charachteristic0(Player*,Ball);//Goalkeepers behaivier

void Charachteristic12(Player*,Ball);//Defances

void Charachteristic4(Player*,Ball);//Middle

void Charachteristic35(Player*,Ball);//Upper middle

void Charachteristic67(Player*,Ball);//Attack

void Default();//Default field and player settings

void Game();//Starts all players and ball

int main() {
	
	system("mode con cols=180 lines=40");//Defines window's measures
	HideCursor();
	
	srand(time(NULL));
	Default();
	
	Game();
	return 0;
}

void CreateField(){
	int i,j;
	for(i=0;i<16;i++)
		for(j=0;j<81;j++)
			field[i][j]=' ';
	
	//x lines
	for(i=1;i<80;i++)
	{
		field[0][i]='_';
		field[15][i]='_';
	} 
	for(i=1;i<8;i++)
	{
		field[3][i]='_';
		field[3][72+i]='_';
		field[12][i]='_';
		field[12][72+i]='_';
	}
	for(i=0;i<3;i++)
	{
		field[4][37+i]='_';
		field[4][41+i]='_';	
		field[11][37+i]='_';
		field[11][41+i]='_';
	}
	
	field[5][1]='_';
	field[10][1]='_';
	field[5][79]='_';
	field[10][79]='_';
	
	//y lines
	for(j=1;j<16;j++)
	{
		field[j][0]=179;
		field[j][40]=179;
		field[j][80]=179;
	}
	for(j=0;j<9;j++)
	{
		field[4+j][8]=179;
		field[4+j][72]=179;
	}
	for(j=0;j<7;j++)
	{
		field[5+j][36]=179;
		field[5+j][44]=179;
	}
}

void PrintField(){
	
	int i,j;
	Gotoyx(0,0);
	printf("\n\n");
	for(i=0;i<16;i++)//Print Field
	{
		
		for(j=0;j<81;j++)
		{
			printf("%c",field[i][j]);
		}
		printf("\n");
	}
}

void CreateTeams(Player *b,Player *w){
	int i;
	
	for(i=0;i<8;i++){
		b[i].number=(char)(i+48);
		w[i].number=(char)(i+48);
		b[i].team='b';
		w[i].team='w';
	}
	b[0].x=2; b[0].y=8;
	w[0].x=78; w[0].y=8;//GoalKeepers
	
	//Defance
	b[1].x=11; b[1].y=5;
	w[1].x=69; w[1].y=5;
	
	b[2].x=11; b[2].y=11;
	w[2].x=69; w[2].y=11;
	//
	
	//Middle
	b[3].x=26; b[3].y=13;
	w[3].x=54; w[3].y=13;
	
	b[4].x=20; b[4].y=8;
	w[4].x=60; w[4].y=8;
	
	b[5].x=26; b[5].y=3;
	w[5].x=54; w[5].y=3;
	//
	
	//Attack
	b[6].x=35; b[6].y=5;
	w[6].x=45; w[6].y=5;
	
	b[7].x=35; b[7].y=11;
	w[7].x=45; w[7].y=11;
}

void PrintDefaultPlayers(Player b[],Player w[],Ball *ball){
	int i;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//For backgorund color
	
	for(i=0;i<8;i++){
		Gotoyx(b[i].y+2,b[i].x);
		SetConsoleTextAttribute(hConsole,15);//Background black
		printf("%c",b[i].number);//Black
		
		Gotoyx(w[i].y+2,w[i].x);
		SetConsoleTextAttribute(hConsole,240);
		printf("%c",w[i].number);//White
	}
	SetConsoleTextAttribute(hConsole,0x2f);//Green bg, white text
	ball->shape='o';
	Gotoyx(10,40);
	ball->x=40;
	ball->y=10;
	printf("%c",ball->shape);
}

bool IsTherePlayer(int X,int Y){
	int i,j;
	
	for(i=0;i<8;i++){
		if(black[i].x==X && black[i].y==Y) return true;
		if(white[i].x==X && white[i].y==Y) return true;
	}
	return false;
}

//Move player, X and Y only take 0,1,-1
void PlayerMove(Player *p,int X,int Y){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	
	if(BallMove(black,white,&ball)!=0 && p->team=='b'){
		if(p->x>68){Shot(*p,&ball);bshot++;}
		else {Pass(*p,&ball);bpass++;}
	} 
	else if(BallMove(black,white,&ball)!=0 && p->team=='w'){
		if(p->x<12){Shot(*p,&ball);wshot++;} 
		else {Pass(*p,&ball);wpass++;}
	} 
	else if(IsTherePlayer(p->x+X,p->y+Y) == false && (ball.x == p->x+X && ball.y == p->y+Y+2)==false){
	Gotoyx(p->y+2,p->x);
	SetConsoleTextAttribute(hConsole,0x2f);//Green bg, white fg
	printf("%c",field[p->y][p->x]);
	
	
	if(1<p->x+X && p->x+X<79)p->x+=X;
	if(1<p->y+Y && p->y+Y<15)p->y+=Y;
	
	Gotoyx(p->y+2,p->x);
	if(p->team=='b') SetConsoleTextAttribute(hConsole,15);//Black bg, white fg
	else SetConsoleTextAttribute(hConsole,240);//White bg, black fg
	printf("%c",p->number);
	Sleep(20);
	
	}
}

float DistanceBallPlayer(Player p,Ball b){
	float distance=sqrt(pow((p.y-b.y),2.0) + (pow((p.x-b.x),2.0)));//Pythagorean
	return (distance);
}

int DistancePlayerPlayer(Player p1,Player p2){
	int distance=sqrt(pow(p1.y-p2.y,2) + (pow(p1.x-p2.x,2)));//Pythagorean
	return (distance);
}

void BallMovement(Ball *ball,int j,int k){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if(IsTherePlayer(ball->x+j,ball->y+k-2)==false){
	
	Sleep(20);
	Gotoyx(ball->y,ball->x);
	SetConsoleTextAttribute(hConsole,0x2f);//Green bg, white fg
	printf("%c",field[ball->y-2][ball->x]);
	
	if(ball->x==80 ) j=-1;
	if(ball->y==16 ) k=-1; 
	if(ball->x==1 ) j=1;
	if(ball->y==4 ) k=1;
			
	if(0<ball->x+j && ball->x+j<80 && 3<ball->y+k && ball->y+k<17){
		ball->x+=j;
		ball->y+=k;
	}
	Gotoyx(ball->y,ball->x);
	SetConsoleTextAttribute(hConsole,0x2f);//Green bg, white fg
	printf("%c",ball->shape);
	
	}
}

int CheckPlayerX(Player p[],Ball *ball){
	int i;
	int j;
	for(i=0;i<8;i++){
		
		if(p[i].y+2== ball->y && p[i].x+1 == ball->x){j=1; return j;
		}
		if(p[i].y+2== ball->y && p[i].x-1 == ball->x){j=-1;return j;
		}
		
	}
	return 0;
}

int CheckPlayerY(Player p[],Ball *ball){
	int i;
	int k;
	for(i=0;i<8;i++){
		
		if(p[i].y+3== ball->y && p[i].x== ball->x){k=-1; return k;
		}
		if(p[i].y+1== ball->y && p[i].x== ball->x){k=1;return k;
		}
	}
	return 0;
}

int CheckKick(Player p[], Ball *ball){
	
	if(CheckPlayerX(p,ball)==1) return 1;//+x
	if(CheckPlayerX(p,ball)==-1) return 2;//-x
	if(CheckPlayerY(p,ball)==1) return 3;//+y
	if(CheckPlayerY(p,ball)==-1) return 4;//-y
	return 0;
}

int BallMove(Player b[],Player w[],Ball *ball){
	if(CheckKick(b, ball)==0 && CheckKick(w, ball)==0) return 0;
	return 1;
}

void Pass(Player p1,Ball *ball){
    int i,number=0;
    if(p1.number=='0') 
		number = rand()%2 ? 1 : 2;
    if(p1.number=='1' || p1.number=='2') number=4;
    if(p1.number=='3' || p1.number=='5' || p1.number=='4') 
    	number = rand()%2 ? 6 : 7;
    if(p1.number=='6') number=7; 
    if(p1.number=='7') number=6;
	int dx,dy;
	if(p1.team=='b'){
		dx = ball->x - black[number].x - 1;
		dy = ball->y - black[number].y - 3;
	}
	else{
		dx = ball->x - white[number].x - 1;
		dy = ball->y - white[number].y - 3;
	}
	
    if(dx>=0 && dy>=0){
    	if(dx==0 && dy>0){
    		BallMovement(ball,0,-1);
    		BallMovement(ball,0,-1);
		}
    	else if(dx>0 && dy==0) {
    		BallMovement(ball,-1,0);
    		BallMovement(ball,-1,0);
		}
    	else if(dx>0 && dy>0){
    		BallMovement(ball,-1,0);
    		BallMovement(ball,0,-1);
    		BallMovement(ball,-1,0);
    		BallMovement(ball,0,-1);
		}
	}
	if(dx<=0 && dy<=0){
		if(dx==0 && dy<0){
			BallMovement(ball,0,1);
			BallMovement(ball,0,1);
		}
    	else if(dx<0 && dy==0){
    		BallMovement(ball,1,0);
    		BallMovement(ball,1,0);
		}
    	else if(dx<0 && dy<0){
    		BallMovement(ball,1,0);
    		BallMovement(ball,0,1);
    		BallMovement(ball,1,0);
    		BallMovement(ball,0,1);
		}
	}
    else BallMovement(ball,rand()%2,rand()%2);
}

void Shot(Player p1,Ball *ball){
	int i;
	if(p1.team=='b'){
		if(p1.y<8){
			for(i=0;i<10;i++){
				if(ball->y<12)
					if(!rand()%5)BallMovement(ball,1,1);
					else BallMovement(ball,1,0);	
			}
		}
		else{
			for(i=0;i<10;i++){
				if(ball->y>8)
					if(!rand()%5)BallMovement(ball,1,-1);
					else BallMovement(ball,1,0);	
			}
		}
	}
	else{
		if(p1.y<8){
			for(i=0;i<10;i++){
				if(ball->y<12)
				if(!rand()%5)BallMovement(ball,-1,1);
				else BallMovement(ball,-1,0);	
			}
		}else{
			for(i=0;i<10;i++){
				if(ball->y>8)
				if(!rand()%5)BallMovement(ball,1,-1);
				else BallMovement(ball,-1,0);
				
			}
		}
	}
	
}

void Charachteristic0(Player *zero,Ball ball){
	
	if(zero->y+2 >ball.y && zero->y>6){
		PlayerMove(zero,0,-1);
	}
	if(zero->y+2 < ball.y && zero->y<10){
		PlayerMove(zero,0,1);
	}
	if(zero->team=='b'){
		if(zero->x<6 && DistanceBallPlayer(*zero,ball)<15){
		PlayerMove(zero,1,0);
		}
		else if(zero->x>2) PlayerMove(zero,-1,0);
	}
	else{
		if(zero->x>73 && DistanceBallPlayer(*zero,ball)<15){
		PlayerMove(zero,-1,0);
		}
		else if(zero->x<78) PlayerMove(zero,1,0);
	}
}

void Charachteristic12(Player *one,Ball ball){
	if(one->number=='2'){
		if(one->y+2 >ball.y && one->y>9){
			rand()%2 ? PlayerMove(one,rand()%2,-1) : PlayerMove(one,rand()%2-1,-1);
		}
	
		if(one->y+2 < ball.y && one->y<13){
			rand()%2 ? PlayerMove(one,rand()%2,1) : PlayerMove(one,rand()%2-1,1);
		}
	}
	else{
		if(one->y+2 <ball.y && one->y<7){
			rand()%2 ? PlayerMove(one,rand()%2,1) : PlayerMove(one,rand()%2-1,1);
		}
		if(one->y+2 > ball.y && one->y>3){
			PlayerMove(one,0,-1);
		}
	}
	
	if(one->team=='b'){
		if(one->x <17 && one->x < ball.x && DistanceBallPlayer(*one,ball)<7){
			rand()%5 ? PlayerMove(one,1,0) : (rand()%2 ? PlayerMove(one,1,rand()%2-1) : PlayerMove(one,1,rand()%2));
		}
		if(one->x >1  && one->x > ball.x){
			rand()%5 ? PlayerMove(one,-1,0) : (rand()%2 ? PlayerMove(one,-1,rand()%2) : PlayerMove(one,-1,rand()%2-1));
		}
	}
	if(one->team=='w'){
		if(one->x <79 && one->x < ball.x ){
			rand()%5 ? PlayerMove(one,1,0) : (rand()%2 ? PlayerMove(one,1,rand()%2) : PlayerMove(one,1,rand()%2-1));
		}
		if(one->x >61  && one->x > ball.x && DistanceBallPlayer(*one,ball)<7){
			rand()%5 ? PlayerMove(one,-1,0) : (rand()%2 ? PlayerMove(one,-1,rand()%2) : PlayerMove(one,-1,rand()%2-1));
		}
	}
	
}

void Charachteristic4(Player *four,Ball ball){
	if(four->y+2 >ball.y && four->y>5){
		PlayerMove(four,0,-1);
	}	//20 8 60 8
	if(four->y+2 < ball.y && four->y<11){
		PlayerMove(four,0,1);
	}
	if(four->team=='b'){
		if(four->x < ball.x && four->x < 30 && (DistanceBallPlayer(*four,ball)<9)){
			rand()%5 ? PlayerMove(four,1,0) : (rand()%2 ? PlayerMove(four,1,rand()%2-1) : PlayerMove(four,1,rand()%2));
		}
		if(four->x > ball.x && four->x > 12){
			rand()%5 ? PlayerMove(four,-1,0) : (rand()%2 ? PlayerMove(four,-1,rand()%2-1) : PlayerMove(four,-1,rand()%2));
		}
	}
	if(four->team=='w'){
		if(four->x > ball.x && four->x > 50 && (DistanceBallPlayer(*four,ball)<9)){
			rand()%5 ? PlayerMove(four,-1,0) : (rand()%2 ? PlayerMove(four,-1,rand()%2-1) : PlayerMove(four,-1,rand()%2));
		}
		if(four->x < ball.x && four->x < 68){
			rand()%5 ? PlayerMove(four,1,0) : (rand()%2 ? PlayerMove(four,1,rand()%2-1) : PlayerMove(four,1,rand()%2));
		}
	}
}

void Charachteristic35(Player *five,Ball ball){
	//26 13 54 13
	//26 3 54 3 
	if(five->number=='3'){
		if(five->y+2 <ball.y && five->y<14){
			PlayerMove(five,0,1);
		}
	
		if(five->y+2 > ball.y && five->y>9){
			PlayerMove(five,0,-1);
		}
	}
	else{
		if(five->y+2 >ball.y && five->y>2){
			PlayerMove(five,0,-1);
		}
	
		if(five->y+2 < ball.y && five->y<7){
			PlayerMove(five,0,1);
		}
	}
	
	if(five->team=='b'){
		if(five->x <36 && five->x < ball.x && DistanceBallPlayer(*five,ball)<15){
			rand()%5 ? PlayerMove(five,1,0) : (rand()%2 ? PlayerMove(five,1,rand()%2-1) : PlayerMove(five,1,rand()%2));
		}
		if(five->x >14  && five->x > ball.x){
			rand()%5 ? PlayerMove(five,-1,0) : (rand()%2 ? PlayerMove(five,1,rand()%2-1) : PlayerMove(five,1,rand()%2));
		}
	}
	if(five->team=='w'){
		if(five->x <66 && five->x < ball.x ){
			rand()%5 ? PlayerMove(five,1,0) : (rand()%2 ? PlayerMove(five,1,rand()%2-1) : PlayerMove(five,1,rand()%2));
		}
		if(five->x >44  && five->x > ball.x && DistanceBallPlayer(*five,ball)<15){
			rand()%5 ? PlayerMove(five,-1,0) : (rand()%2 ? PlayerMove(five,1,rand()%2-1) : PlayerMove(five,1,rand()%2));
		}
	}
}

void Charachteristic67(Player *six,Ball ball){
	//35 5b6 45 5w6 35 11b7 45 11w7
	
	if(six->number=='6'){
		if(six->y+2 < ball.y && six->y <7){
			PlayerMove(six,0,1);
		}
		if(six->y+2 > ball.y && six->y >3){
			PlayerMove(six,rand()%2,-1);
		}
	}
	else{
		if(six->y+2 < ball.y && six->y <14){
			PlayerMove(six,0,1);
		}
		if(six->y+2 > ball.y && six->y >9){
			PlayerMove(six,0,-1);
		}
	}
	if(six->team=='b'){
		if(six->x < ball.x){
			rand()%5 ? PlayerMove(six,1,0) : (rand()%2 ? PlayerMove(six,1,rand()%2-1) : PlayerMove(six,1,rand()%2));
		}
		if(six->x > ball.x && six->x > 38){
			rand()%5 ? PlayerMove(six,-1,0) : (rand()%2 ? PlayerMove(six,-1,rand()%2-1) : PlayerMove(six,-1,rand()%2));
		}
	}
	else{
		if(six->x > ball.x ){
			rand()%5 ? PlayerMove(six,-1,0) : (rand()%2 ? PlayerMove(six,-1,rand()%2-1) : PlayerMove(six,-1,rand()%2));
		}
		if(six->x < ball.x && six->x <42 ){
			rand()%5 ? PlayerMove(six,1,0) : (rand()%2 ? PlayerMove(six,1,rand()%2-1) : PlayerMove(six,1,rand()%2));
		}
	}
}

void Default(){
	system("color 2F");
	CreateTeams(black,white);
	CreateField();
	
	PrintField();
	PrintDefaultPlayers(black,white,&ball);
}

void Game(){
	int b=0,w=0;

	Gotoyx(1,5);
	printf("BLC 0 - 0 WHT Time 0");
	
	int a=0;
	Gotoyx(10,33);
	printf("Press SPACE for START");
	char key='a';
	while(key!=' ') key=getch();
	Default();
	while (Timer<90){
		a++;
		if(a==4){
			Timer+=1;
			a=0;
			Gotoyx(1,24); printf("%d",Timer);
			if(Timer==45){
			Gotoyx(10,33);
			printf("Press SPACE for continue");
			char key='a';
			while(key!=' ') key=getch();
			Default();
		}
		}
		
		if(rand()%2){
			Charachteristic0(&white[0],ball);
			Charachteristic0(&black[0],ball);
		} 
		else{
			Charachteristic0(&black[0],ball);
			Charachteristic0(&white[0],ball);
		}
		if(rand()%2){
			Charachteristic12(&white[1],ball);
			Charachteristic12(&black[1],ball);
		} 
		else{
			Charachteristic12(&black[1],ball);
			Charachteristic12(&white[1],ball);
		}
		
		
		if(rand()%2){
			Charachteristic12(&white[2],ball);
			Charachteristic12(&black[2],ball);
		}
		else{
			Charachteristic12(&black[2],ball);
			Charachteristic12(&white[2],ball);
		}
		
		if(rand()%2){
			Charachteristic4(&white[4],ball);
			Charachteristic4(&black[4],ball);
		} 
		else{
			Charachteristic4(&black[4],ball);
			Charachteristic4(&white[4],ball);
		}
		
		if(rand()%2){
			Charachteristic35(&black[3],ball);
			Charachteristic35(&white[3],ball);
		} 
		else{
			Charachteristic35(&white[3],ball);
			Charachteristic35(&black[3],ball);
		}
		
		if(!rand()%2){
			Charachteristic35(&black[5],ball);
			Charachteristic35(&white[5],ball);
		} 
		else{
			Charachteristic35(&white[5],ball);
			Charachteristic35(&black[5],ball);
		} 
	
		if(!rand()%2){
			Charachteristic67(&white[6],ball);
			Charachteristic67(&black[6],ball);
		} 
		else{
			Charachteristic67(&black[6],ball);
			Charachteristic67(&white[6],ball);
		} 
	
		if(!rand()%2){
			Charachteristic67(&white[7],ball);
			Charachteristic67(&black[7],ball);
		} 
		else{
			Charachteristic67(&black[7],ball);
			Charachteristic67(&white[7],ball);
		} //Decides which player will be move first
		
		if(ball.x==1 && ball.y>6 && ball.y<11){
			Gotoyx(1,5);
			w++;
			printf("BLC %d - %d WHT",b,w);
			Default();
			Gotoyx(10,33);
			printf("Press SPACE for continue");
			char key='a';
			while(key!=' ') key=getch();	
			Default();
		}
		if(ball.x==79 && ball.y>6 && ball.y<11){
			Gotoyx(1,5);
			b++;
			printf("BLC %d - %d WHT",b,w,Timer);
			Default();
			Gotoyx(10,33);
			printf("Press SPACE for continue");
			char key='a';
			while(key!=' ') key=getch();
			Default();
		}
		
		Sleep(30);
	}
	Default();
	Gotoyx(10,33);
	if(b>w){
		printf("Black Team won");
	}
	else if(b<w){
		printf("White Team won");
	}
	else printf("It is Draw    Press Esc for leaving...");
	
	Gotoyx(20,38);
	printf("Stats");
	Gotoyx(21,33);
	printf("Black ---- White");
	Gotoyx(22,22);printf("Posession     %.0f  --  %.0f",(float)bpass/(bpass+wpass)*100,(float)wpass/(bpass+wpass)*100);
	Gotoyx(23,22);printf("Passes       %d  --  %d",bpass,wpass);
	Gotoyx(24,22);printf("Shots          %d  --  %d",bshot,wshot);
	Gotoyx(25,21);printf("Shot Accuracy %2.0f  --  %2.0f",(float)b/bshot*100,(float)w/wshot*100);
	
	key='a';
	while(key!=27) key=getch();
}

void Gotoyx(int y, int x){
    COORD coord;
    coord.Y = y;
    coord.X = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HideCursor(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    
    cursorInfo.dwSize = 1;     
    cursorInfo.bVisible = FALSE;
    
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}




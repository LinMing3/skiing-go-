
//#include "../include/main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#undef main

#define LEFT 1
#define WRIGHT 2
#define UP 3
#define DOWN 4
#define UP_DOWN 5
#define DOWN_UP 6
#define STONE_A 7
#define STONE_B 8
#define STONE_C 9
#define BIRD 10


const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 900;
const int TOP = 60;
int Quit=0;
int GameStart=0;
int Score=0;
int delay=1;
int speed=1;
Uint32 Time=0;
Uint32 start=0;
Uint32 pause_time=0;
Uint32 invincible_start=0;
int backGround_y=0;


typedef struct character{
  int track;
  int x,y;
  int height,width;
  int state;
}Character;
Character hero={2, -60, 640, 120, 120, 0};

typedef struct coin{
  int x,y;
  int height,width;
  int state;
  SDL_Rect rect;
}Coin;
Coin coin[10];

typedef struct barrier{
  int x,y;
  int height,width;
  int type;
  int track;
  int state;
  SDL_Surface *surface;
  SDL_Rect rect;
  SDL_Texture *texture;
}Barrier;
Barrier barrier[3];

typedef struct superStar{
  int x,y;
  int state;
  SDL_Rect rect;
}SuperStar;
SuperStar star={75,TOP-100,50,50,1,1};

SDL_Window *window=NULL;
SDL_Renderer *renderer=NULL;
TTF_Font *font=NULL;
SDL_Color color_title={0,0,0,0};

SDL_Surface *surface_coin ;
SDL_Surface *surface_stone1 ;
SDL_Surface *surface_stone2;
SDL_Surface *surface_stone3;
SDL_Surface *surface_bird;
SDL_Surface *surface_chr ;
SDL_Surface *surface_chr_down ;
SDL_Surface *surface_chr_up ;
SDL_Surface *surface_chr_left ;
SDL_Surface *surface_chr_wright ;
SDL_Surface *surface_superStar ;
SDL_Surface *surface_backGround;
SDL_Surface *screen_surface;
SDL_Surface *surface_score;
SDL_Surface *surface_time;

SDL_Rect rect_backGround={0,6296-WINDOW_HEIGHT,2398,900};
SDL_Rect rect_continue={WINDOW_WIDTH/2-125,WINDOW_HEIGHT/3-25,250,75};
SDL_Rect rect_restart={WINDOW_WIDTH/2-125, WINDOW_HEIGHT/3*2-120, 250, 75};
SDL_Rect rect_score={0,0,100,TOP};
SDL_Rect rect_time={WINDOW_WIDTH/2,0,100,TOP};

SDL_Texture *texture_backGround=NULL;
SDL_Texture *texture_score=NULL;
SDL_Texture *texture_time=NULL;
SDL_Texture *texture_chr=NULL;
SDL_Texture *texture_chr_left=NULL;
SDL_Texture *texture_chr_wright=NULL;
SDL_Texture *texture_chr_up=NULL;
SDL_Texture *texture_chr_down=NULL;
SDL_Texture *texture_coin=NULL;
SDL_Texture *texture_superstar=NULL;

void Init_coins();
void Init_superstar();
void Init_barriers();
void createBarrier(int i);
void loadSurface();
void Init_texture();
int init();

void destroy_texture();
void freeSurface();

void renderBackGround();
void draw_menu();
void draw_gameOver();
void draw_start();

void draw_coins();
void draw_barriers();
void draw_superstar();
void draw_character();
void draw();

void do_keydown(SDL_KeyboardEvent keyType);
void do_restart();

SDL_bool ifPause(SDL_MouseButtonEvent event);
SDL_bool ifContinue(SDL_MouseButtonEvent event);
SDL_bool ifRestart(SDL_MouseButtonEvent event);

int checkCoin(Coin theCoin);
int checkBarriers(Barrier theBarrier);
SDL_bool checkSuperstar();

int pause();
int gameOver();

void run_coins();
void run_barriers();
void run_superstar();
void run();

int event_loop();

int main(int argc, char *args[]) {
  //初始化
  if(init()<0){
	printf("init failed");
  }

  //新建surface
  loadSurface();
  //渲染背景
  renderBackGround();
  //事件
  event_loop();

  //销毁
  freeSurface();
  SDL_DestroyWindow(window);//退出窗口
  SDL_DestroyRenderer(renderer);//退出渲染

  destroy_texture();

  //退出
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return 0;
}

void Init_coins(){
  //新建coins
  for (int i = 0; i < 10; ++i) {
	coin[i].x=rand()%3*200+75;
	coin[i].y=TOP-200*i;
	coin[i].height=50;
	coin[i].width=50;
	SDL_Rect rect={coin[i].x,coin[i].y,50,50};
	coin[i].rect=rect;
  }
}
void Init_superstar(){
  star.x=rand()%3*200+75;
  star.state=1;
  SDL_Rect rect={star.x,star.y,50,50};
  star.rect=rect;
}
void createBarrier(int i){
  SDL_DestroyTexture(barrier[i].texture);
  barrier[i].state=1;
  barrier[i].height=50;
  barrier[i].type=rand()%4;
  barrier[i].track=rand()%3;
  barrier[i].x=barrier[i].track*200+50;
  barrier[i].width=100;
  SDL_Rect rect={barrier[i].x,barrier[i].y,barrier[i].width,barrier[i].height};
  barrier[i].rect=rect;
  switch (barrier[i].type){
	case 0:{
	  barrier[i].type=STONE_A;
	  barrier[i].surface=surface_stone1;
	}
	  break;
	case 1:{
	  barrier[i].type=STONE_B;
	  barrier[i].surface=surface_stone2;
	}
	  break;
	case 2:{
	  barrier[i].type=BIRD;
	  barrier[i].surface=surface_bird;
	} break;
	case 3:{
	  barrier[i].type=STONE_C;
	  barrier[i].x=barrier[i].track*200+20;
	  barrier[i].width=160;
	  barrier[i].height=70;
	  barrier[i].surface=surface_stone3;
	  SDL_Rect rect_c={barrier[i].x,barrier[i].y,barrier[i].width,barrier[i].height};
	  barrier[i].rect=rect_c;
	}
  }
  barrier[i].texture= SDL_CreateTextureFromSurface(renderer,barrier[i].surface);
}
void Init_barriers(){
  for (int i = 0; i < 3; ++i) {
	barrier[i].y=TOP-WINDOW_HEIGHT/3*i;
	createBarrier(i);
  }
}
void loadSurface(){
  surface_coin = IMG_Load("./res/images/coin.png");
  surface_stone1 = IMG_Load("./res/images/stone1.png");
  surface_stone2 = IMG_Load("./res/images/stone2.png");
  surface_stone3 = IMG_Load("./res/images/stone3.png");
  surface_bird = IMG_Load("./res/images/bird.png");
  surface_chr = IMG_Load("./res/images/chr.png");
  surface_chr_down = IMG_Load("./res/images/chr_down.png");
  surface_chr_up = IMG_Load("./res/images/chr_up.png");
  surface_chr_left = IMG_Load("./res/images/chr_left.png");
  surface_chr_wright = IMG_Load("./res/images/chr_wright.png");
  surface_superStar = IMG_Load("./res/images/superStar.png");
  surface_backGround= IMG_Load("./res/images/backGround.png");
  surface_score= TTF_RenderUTF8_Blended(font,"分数：",color_title);
  surface_time= TTF_RenderUTF8_Blended(font,"时间：",color_title);
}
void Init_texture(){
  texture_backGround= SDL_CreateTextureFromSurface(renderer,surface_backGround);
  texture_score= SDL_CreateTextureFromSurface(renderer,surface_score);
  texture_time= SDL_CreateTextureFromSurface(renderer,surface_time);
  texture_chr= SDL_CreateTextureFromSurface(renderer,surface_chr);
  texture_chr_left= SDL_CreateTextureFromSurface(renderer, surface_chr_left);
  texture_chr_wright= SDL_CreateTextureFromSurface(renderer,surface_chr_wright);
  texture_chr_up= SDL_CreateTextureFromSurface(renderer, surface_chr_up);
  texture_chr_down= SDL_CreateTextureFromSurface(renderer,surface_chr_down);
  texture_coin= SDL_CreateTextureFromSurface(renderer,surface_coin);
  texture_superstar= SDL_CreateTextureFromSurface(renderer,surface_superStar);
}

int init(){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
	SDL_Log("SDL Init failed:%s", SDL_GetError());
	return -1;
  }
  if(IMG_Init(IMG_INIT_PNG)<0){
	SDL_Log("IMG Init failed:%s",SDL_GetError());
	return -1;
  }
  if(TTF_Init()<0){
	SDL_Log("TTF Init failed:%s",SDL_GetError());
	return -1;
  }

  //创建窗口,渲染器
  window = SDL_CreateWindow("Skiing Go!", 500, 50, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  screen_surface= SDL_GetWindowSurface(window);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  font= TTF_OpenFont("./res/ttf/font.ttf",60);
  loadSurface();
  Init_coins();
  Init_barriers();
  Init_superstar();
  Init_texture();

  return 0;
}

void destroy_texture(){
  for (int i = 0; i < 3; ++i) {
	SDL_DestroyTexture(barrier[i].texture);
  }
  SDL_DestroyTexture(texture_coin);
  SDL_DestroyTexture(texture_superstar);

  SDL_DestroyTexture(texture_backGround);
  SDL_DestroyTexture(texture_score);
  SDL_DestroyTexture(texture_time);

  SDL_DestroyTexture(texture_chr);
  SDL_DestroyTexture(texture_chr_left);
  SDL_DestroyTexture(texture_chr_wright);
  SDL_DestroyTexture(texture_chr_up);
  SDL_DestroyTexture(texture_chr_down);
}
void freeSurface(){
  SDL_FreeSurface(surface_coin);
  SDL_FreeSurface(surface_stone1);
  SDL_FreeSurface(surface_stone2);
  SDL_FreeSurface(surface_stone3);
  SDL_FreeSurface(surface_bird);
  SDL_FreeSurface(surface_chr);
  SDL_FreeSurface(surface_chr_down);
  SDL_FreeSurface(surface_chr_up);
  SDL_FreeSurface(surface_chr_left);
  SDL_FreeSurface(surface_chr_wright);
  SDL_FreeSurface(surface_superStar);
  SDL_FreeSurface(surface_backGround);
  SDL_FreeSurface(screen_surface);
  SDL_FreeSurface(surface_time);
  SDL_FreeSurface(surface_score);

}

void renderBackGround(){

  SDL_RenderCopy(renderer,texture_backGround,&rect_backGround,NULL);

  //文字
  SDL_SetRenderDrawColor(renderer,0,0,0,0);

  SDL_RenderCopy(renderer,texture_score,NULL,&rect_score);
  SDL_RenderCopy(renderer,texture_time,NULL,&rect_time);

  //分数
  SDL_Rect rect_score_s={100,0,100,TOP};
  char score[6]={"00000"};
  itoa(Score,score,10);
  SDL_Surface *surface_score_s= TTF_RenderUTF8_Blended(font,score,color_title);
  SDL_Texture *texture_score_s= SDL_CreateTextureFromSurface(renderer,surface_score_s);
  SDL_RenderCopy(renderer,texture_score_s,NULL,&rect_score_s);

  //时间
  SDL_Rect rect_time_s={WINDOW_WIDTH/2+100,0,100,TOP};
  Time=SDL_GetTicks()-start-pause_time;
  char time[6]={"00000"};
  int time_ms=(int)Time/1000;
  itoa(time_ms,time,10);
  SDL_Surface *surface_time_s= TTF_RenderUTF8_Blended(font,time,color_title);
  SDL_Texture *texture_time_s= SDL_CreateTextureFromSurface(renderer,surface_time_s);
  SDL_RenderCopy(renderer,texture_time_s,NULL,&rect_time_s);

  //暂停区域
  SDL_Rect rect_pause={500,0,100,TOP};
  SDL_Surface *surface_pause= TTF_RenderUTF8_Blended(font,"暂停",color_title);
  SDL_Texture *texture_pause= SDL_CreateTextureFromSurface(renderer,surface_pause);
  SDL_RenderCopy(renderer,texture_pause,NULL,&rect_pause);

  SDL_FreeSurface(surface_score_s);
  SDL_DestroyTexture(texture_score_s);
  SDL_FreeSurface(surface_pause);
  SDL_DestroyTexture(texture_pause);
  SDL_FreeSurface(surface_time_s);
  SDL_DestroyTexture(texture_time_s);
}

void draw_menu(){
  SDL_Rect rect={0,backGround_y,2398,6296-900};
  SDL_RenderCopy(renderer,texture_backGround,&rect,NULL);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_Surface *surface_continue= TTF_RenderUTF8_Blended(font, "CONTINUE", color_title);
  SDL_Texture *texture_continue= SDL_CreateTextureFromSurface(renderer, surface_continue);
  SDL_RenderCopy(renderer, texture_continue, NULL, &rect_continue);


  SDL_Surface *surface_restart= TTF_RenderUTF8_Blended(font, "RESTART", color_title);
  SDL_Texture *texture_restart= SDL_CreateTextureFromSurface(renderer, surface_restart);
  SDL_RenderCopy(renderer, texture_restart, NULL, &rect_restart);

  SDL_DestroyTexture(texture_restart);
  SDL_FreeSurface(surface_restart);
  SDL_DestroyTexture(texture_continue);
  SDL_FreeSurface(surface_continue);
}
void draw_coins(){
  for (int i = 0; i < 10; ++i) {
	if(coin[i].state){
	  SDL_RenderCopy(renderer,texture_coin,NULL,&coin[i].rect);
	}
  }
}
void draw_barriers(){
  for (int i = 0; i < 3; ++i) {
	SDL_RenderCopy(renderer,barrier[i].texture,NULL,&barrier[i].rect);
  }
}
void draw_superstar(){
  if(star.state){
	SDL_RenderCopy(renderer,texture_superstar,NULL,&star.rect);
  }
}

void do_keydown(SDL_KeyboardEvent keyType) {
  switch (keyType.keysym.scancode) {
	case SDL_SCANCODE_LEFT:{
	  if(WINDOW_WIDTH/2+hero.x-200>0)hero.state=LEFT;
	}
	  break;
	case SDL_SCANCODE_RIGHT: {
	  if(WINDOW_WIDTH/2+hero.x+hero.width+200<WINDOW_WIDTH)hero.state=WRIGHT;
	}
	  break;
	case SDL_SCANCODE_UP: {
	  hero.state=UP;
	}
	  break;
	case SDL_SCANCODE_DOWN: {
	  hero.state=DOWN;
	}
	  break;
	default:
	  break;
  }
}

void do_restart(){
  hero.track=2;
  hero.x=-60;
  hero.y=640;
  hero.state=0;

  Score=0;
  pause_time=0;
  delay=1;
  speed=1;
  start=SDL_GetTicks();

  Init_coins();
  Init_barriers();
  Init_superstar();
}

SDL_bool ifPause(SDL_MouseButtonEvent event){
  SDL_Point point={event.x,event.y};
  SDL_Rect rect_pause={500,0,100,TOP};
  if(SDL_PointInRect(&point,&rect_pause)){
	return SDL_TRUE;
  }
  return SDL_FALSE;
}
SDL_bool ifContinue(SDL_MouseButtonEvent event){
  SDL_Point point={event.x,event.y};
  if(SDL_PointInRect(&point,&rect_continue)){
	return SDL_TRUE;
  }
  return SDL_FALSE;
}
SDL_bool ifRestart(SDL_MouseButtonEvent event){
  SDL_Point point={event.x,event.y};
  if(SDL_PointInRect(&point,&rect_restart)){
	return SDL_TRUE;
  }
  return SDL_FALSE;
}

void draw_character(){
  SDL_Rect rect_chr={WINDOW_WIDTH/2+hero.x,hero.y,hero.width,hero.height};
  switch (hero.state) {
	case 0:{
	  SDL_RenderCopy(renderer,texture_chr,NULL,&rect_chr);
	}
	  break;
	case LEFT:{
	  SDL_RenderCopy(renderer, texture_chr_left, NULL, &rect_chr);
	  if(hero.track==2){
		if(hero.x==-260){
		  hero.track=1;
		  hero.state=0;
		}
		if(hero.x>-260)hero.x-=10;
	  } else if(hero.track==3){
		if(hero.x==-60){
		  hero.track=2;
		  hero.state=0;
		}
		if(hero.x>-60)hero.x-=10;
	  }
	}
	  break;
	case WRIGHT:{
	  SDL_RenderCopy(renderer, texture_chr_wright, NULL, &rect_chr);
	  if(hero.track==2){
		if(hero.x==140){
		  hero.track=3;
		  hero.state=0;
		}
		if(hero.x<140)hero.x+=10;
	  } else if(hero.track==1){
		if(hero.x==-60){
		  hero.track=2;
		  hero.state=0;
		}
		if(hero.x<-60)hero.x+=10;
	  }
	}
	  break;
	case UP:{
	  SDL_RenderCopy(renderer, texture_chr_up, NULL, &rect_chr);
	  if(hero.y==580)hero.state=UP_DOWN;
	  if(hero.y>580)hero.y--;
	}
	  break;
	case DOWN:{
	  SDL_RenderCopy(renderer,texture_chr_down,NULL,&rect_chr);
	  if (hero.y>=700)hero.state=DOWN_UP;
	  if(hero.y<700)hero.y++;
	}
	  break;
	case UP_DOWN:{
	  SDL_RenderCopy(renderer,texture_chr_down,NULL,&rect_chr);
	  if(hero.y==640)hero.state=0;
	  if(hero.y<640)hero.y++;
	}
	  break;
	case DOWN_UP:{
	  SDL_RenderCopy(renderer,texture_chr_up,NULL,&rect_chr);
	  if(hero.y==640)hero.state=0;
	  if(hero.y>640)hero.y--;
	}
	  break;
  }
}

void draw(){
  renderBackGround();
  draw_barriers();
  draw_coins();
  draw_superstar();
  draw_character();
}

int checkCoin(Coin theCoin){//是否与人物相交
  if(!theCoin.state){
	return 0;
  }
  if(//在人物外面
	  (theCoin.rect.y+theCoin.height<hero.y)||
		  (theCoin.rect.y>hero.y+hero.height)||
		  (theCoin.x>WINDOW_WIDTH/2+hero.x+hero.width)||
		  (theCoin.x+theCoin.width<WINDOW_WIDTH/2+hero.x)
	  )
	return 0;
  return 1;
}
int checkBarriers(Barrier theBarrier){
  if(theBarrier.rect.y-25>hero.y){
	return 0;
  }
  if(!star.state){
	return 2;
  }
  if(theBarrier.state==1&&theBarrier.rect.y+theBarrier.height>=hero.y){
	switch (theBarrier.type) {
	  case STONE_C:{
		if(theBarrier.track+1==hero.track){
		  return -1;
		} else{
		  return 1;
		}
	  }
	  case STONE_A:{
		if(theBarrier.track+1==hero.track){
		  if (hero.state==UP||hero.state==UP_DOWN){
			return 1;
		  } else{
			return -1;
		  }
		}
	  }
	  case STONE_B:{
		if(theBarrier.track+1==hero.track){
		  if (hero.state==UP||hero.state==UP_DOWN){
			return 1;
		  } else{
			return -1;
		  }
		}
	  }
	  case BIRD:{
		if(theBarrier.track+1==hero.track){
		  if (hero.state==DOWN||hero.state==DOWN_UP){
			return 1;
		  } else{
			return -1;
		  }
		}
	  }
	}
  }
  return 0;
}
SDL_bool checkSuperstar(){
  SDL_Point point={star.rect.x+25,star.rect.y+50};
  SDL_Rect rect_chr={WINDOW_WIDTH/2+hero.x,hero.y,hero.width,hero.height};
  if(star.state&&SDL_PointInRect(&point,&rect_chr)){
	return SDL_TRUE;
  }
  return SDL_FALSE;
}


int pause(){
  Uint32 pause_start=SDL_GetTicks();
  draw_menu();
  SDL_RenderPresent(renderer);
  while (1){
	SDL_Event event;
	if (SDL_PollEvent(&event)){
	  switch (event.type) {
		case SDL_QUIT: return 0;
		case SDL_MOUSEBUTTONDOWN:
		{
		  if(ifContinue(event.button)){
			pause_time+=SDL_GetTicks()-pause_start;
			return 1;
		  }
		  if(ifRestart(event.button)){
			return 2;
		  }
		}
		  break;
		default:break;
	  }
	} else{
	  SDL_Delay(5);
	}
  }
}

void draw_gameOver(){
  SDL_Rect rect={0,backGround_y,2398,6296-900};
  SDL_RenderCopy(renderer,texture_backGround,&rect,NULL);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_Rect rect_GameOver={WINDOW_WIDTH/2-200,WINDOW_HEIGHT/3-50,400,100};
  SDL_Surface *surface_GameOver= TTF_RenderUTF8_Blended(font, "GAME OVER", color_title);
  SDL_Texture *texture_GameOver= SDL_CreateTextureFromSurface(renderer, surface_GameOver);
  SDL_RenderCopy(renderer, texture_GameOver, NULL, &rect_GameOver);


  SDL_Surface *surface_restart= TTF_RenderUTF8_Blended(font, "RESTART", color_title);
  SDL_Texture *texture_restart= SDL_CreateTextureFromSurface(renderer, surface_restart);
  SDL_RenderCopy(renderer, texture_restart, NULL, &rect_restart);

  SDL_DestroyTexture(texture_restart);
  SDL_FreeSurface(surface_restart);
  SDL_DestroyTexture(texture_GameOver);
  SDL_FreeSurface(surface_GameOver);
}
void draw_start(){
  SDL_Rect rect={0,backGround_y,2398,6296-900};
  SDL_RenderCopy(renderer,texture_backGround,&rect,NULL);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_Rect rect_gameStart={WINDOW_WIDTH/2-200, WINDOW_HEIGHT/3-60, 400, 120};
  SDL_Surface *surface_GameStart= TTF_RenderUTF8_Blended(font, "GAME START", color_title);
  SDL_Texture *texture_GameStart= SDL_CreateTextureFromSurface(renderer, surface_GameStart);
  SDL_RenderCopy(renderer, texture_GameStart, NULL, &rect_gameStart);

  SDL_DestroyTexture(texture_GameStart);
  SDL_FreeSurface(surface_GameStart);
}

int gameOver(){
  draw_gameOver();
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
  while (1){
	SDL_Event event;
	if (SDL_PollEvent(&event)){
	  switch (event.type) {
		case SDL_QUIT:
		  Quit=1;
		  return 0;
		case SDL_MOUSEBUTTONDOWN:{
		  SDL_Point point={event.button.x,event.button.y};
		  if (SDL_PointInRect(&point,&rect_restart)){
			do_restart();
			return 1;
		  }
		}
		  break;
		default:break;
	  }
	} else{
	  SDL_Delay(5);
	}
  }
}

void run_coins(){
  for (int i = 0; i < 10; ++i) {
	coin[i].rect.y+=speed;
	if(checkCoin(coin[i])){
	  coin[i].state=0;
	  Score+=1;
	}
	if(coin[i].rect.y>=WINDOW_HEIGHT){
	  coin[i].rect.y=TOP;
	  coin[i].x=rand()%3*200+75;
	  coin[i].rect.x=coin[i].x;
	  coin[i].state=1;
	}
  }
}
void run_barriers(){
  for (int i = 0; i < 3; ++i) {
	barrier[i].rect.y+=speed;
	int index=checkBarriers(barrier[i]);
	if(index==1){
	  Score+=1;
	  barrier[i].state=0;
	} else if(index==-1){
	  gameOver();
	  return;
	} else if(index==2){
	  barrier[i].state=0;
	}
	if(barrier[i].rect.y>=WINDOW_HEIGHT){
	  barrier[i].y=TOP;
	  createBarrier(i);
	}
  }
}
void run_superstar(){
  star.rect.y+=speed;
  if(checkSuperstar()){
	star.state=0;
	Score+=10;
	delay=0;
	invincible_start=SDL_GetTicks();
  }
  if(star.rect.y>=WINDOW_HEIGHT){
	Uint32 delta=SDL_GetTicks()-invincible_start-pause_time;
	if(delta>15000){
	  Init_superstar();
	} else if(delta>4000){
	  star.state=1;
	} else if(delta>3000){
	  delay=1;
	}
  }
}
void run(){
  run_coins();
  run_barriers();
  run_superstar();

  rect_backGround.y-=speed;
  if(Score<100)speed=1;
  if(Score>=100)speed=2;
  if(Score>=500)speed=3;
  if(rect_backGround.y<=900){
	rect_backGround.y=6296-900;
  }
}

int event_loop(){
  SDL_Event event;
  draw_start();
  SDL_RenderPresent(renderer);
  while (!GameStart){
	while (SDL_PollEvent(&event)){
	  switch (event.type) {
		case SDL_QUIT:
		  return 0;
		case SDL_MOUSEBUTTONDOWN:{
		  SDL_Point point={event.button.x,event.button.y};
		  SDL_Rect rect_start={WINDOW_WIDTH/2-200, WINDOW_HEIGHT/3-60, 400, 120};
		  if(SDL_PointInRect(&point,&rect_start)){
			GameStart=1;
			start=SDL_GetTicks();
		  }
		}
		  break;
		default: break;
	  }
	}
	SDL_Delay(5);
  }

  while (!Quit) {
	while (SDL_PollEvent(&event)) {
	  switch (event.type) {
		case SDL_QUIT:
		  return 0;
		case SDL_MOUSEBUTTONDOWN:
		{
		  if (ifPause(event.button)){
			switch (pause()) {
			  case 0:{//退出
				return 0;
			  }
			  case 1:{//继续
			  }
				break;
			  case 2:{//重开
				do_restart();
			  }
			}
		  }
		}
		  break;
		case SDL_KEYDOWN:
		  do_keydown(event.key);
		  break;
		default:break;
	  }
	}
	draw();
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	run();

	SDL_Delay(delay);

  }

}
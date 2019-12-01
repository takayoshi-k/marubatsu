/********************************************/
/*         q21.c                            */
/*   強化学習(Q学習)の例題プログラム　その１*/
/*　 単純な例題です                         */
/*使い方                                    */
/* C:\Users\odaka\ch2>q21                   */
/********************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/* 記号定数の定義             */
#define GENMAX  50 /*学習の繰り返し回数*/
#define STATENO  7  /*状態の数*/
#define ACTIONNO 2  /*行動の数*/
#define ALPHA 0.1/*学習係数*/
#define GAMMA 0.9/*割引率*/
#define EPSILON 0.3 /*行動選択のランダム性を決定*/
#define SEED 32767 /*乱数のシード*/
#define REWARD 10 /*ゴール到達時の報酬*/

#define GOAL 6/*状態６がゴール状態*/
#define UP 0/*上方向の行動*/
#define DOWN 1/*下方向の行動*/
#define LEVEL 2 /*枝分かれの深さ*/

/* 関数のプロトタイプの宣言   */
int rand0or1() ;/*0又は1を返す乱数関数*/
double frand() ;/*0～1の実数を返す乱数関数*/
void printqvalue(double qvalue[][ACTIONNO]);/*Q値出力*/
int selecta(int s,double qvalue[][ACTIONNO]);/*行動選択*/
double updateq(int s,int snext,int a,double qvalue[][ACTIONNO]);/*Q値更新*/
int set_a_by_q(int s,double qvalue[][ACTIONNO]) ;/*Q値最大値を選択*/
int nexts(int s,int a) ;/*行動によって次の状態に遷移*/

/****************/
/*  main()関数  */
/****************/
int main()
{
 int i,j;
 int s,snext;/*現在の状態と、次の状態*/
 int t;/*時刻*/
 int a;/*行動*/
 double qvalue[STATENO][ACTIONNO] ;/*Q値*/

 srand(SEED);/*乱数の初期化*/

 /*Ｑ値の初期化*/
 for(i=0;i<STATENO;++i)
  for(j=0;j<ACTIONNO;++j)
   qvalue[i][j]=frand() ;
 printqvalue(qvalue) ;

 /*学習の本体*/
 for(i=0;i<GENMAX;++i){
  s=0;/*行動の初期状態*/
  for(t=0;t<LEVEL;++t){/*最下段まで繰り返す*/
   /*行動選択*/
   a=selecta(s,qvalue) ;
   fprintf(stderr," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q値の更新*/
   qvalue[s][a]=updateq(s,snext,a,qvalue) ;
   /*行動aによって次の状態snextに遷移*/
    s=snext ;
  }
  /*Q値の出力*/
  printqvalue(qvalue) ;
 }
 return 0;
}

/****************************/
/*       updateq()関数      */
/*　     Q値を更新する      */
/****************************/
double updateq(int s,int snext,int a,double qvalue[][ACTIONNO])
{
 double qv ;/*更新されるQ値*/
 
 /*Q値の更新*/
 if(snext==GOAL)/*報酬が付与される場合*/
   qv=qvalue[s][a]+ALPHA*(REWARD-qvalue[s][a]) ;
 else/*報酬なし*/
   qv=qvalue[s][a]
     +ALPHA*(GAMMA*qvalue[snext][set_a_by_q(snext,qvalue)]-qvalue[s][a]) ;

 return qv ;
}

/****************************/
/*        selecta()関数     */
/*　     行動を選択する     */
/****************************/
int selecta(int s,double qvalue[][ACTIONNO])
{
 int a ;/*選択された行動*/
 
 /*ε-greedy法による行動選択*/
 if(frand()<EPSILON){
  /*ランダムに行動*/
  a=rand0or1();
 }
 else{
  /*Ｑ値最大値を選択*/
  a=set_a_by_q(s,qvalue) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()関数      */
/*　  Ｑ値最大値を選択      */
/****************************/
int set_a_by_q(int s,double qvalue[][ACTIONNO])
{
 if((qvalue[s][UP])>(qvalue[s][DOWN])) 
  return UP ;
 else return DOWN;
}

/****************************/
/*    nexts()関数           */
/*行動によって次の状態に遷移*/
/****************************/
int nexts(int s,int a)
{
 return s*2+1+a ;
}

/****************************/
/*    printqvalue()関数     */
/*　   Q値を出力する        */
/****************************/
void printqvalue(double qvalue[][ACTIONNO])
{
 int i,j ;
 
 for(i=0;i<STATENO;++i){
  for(j=0;j<ACTIONNO;++j)
   printf("%.3lf ",qvalue[i][j]);
  printf("\t") ;
 }
 printf("\n");
}

/****************************/
/*     frand()関数          */
/*0～1の実数を返す乱数関数  */
/****************************/
double frand()
{
 /*乱数の計算*/ 
 return (double)rand()/RAND_MAX ;
}

/****************************/
/*     rand0or1()関数         */
/*　   0又は1を返す乱数関数 */
/****************************/
int rand0or1()
{
 int rnd ;
 
 /*乱数の最大値を除く*/
 while((rnd=rand())==RAND_MAX) ;
 /*乱数の計算*/ 
 return (int)((double)rnd/RAND_MAX*2) ;
}

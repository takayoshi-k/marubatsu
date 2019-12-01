/*********************************************/
/*           j.c                             */
/*  じゃんけんを経験から学習する　           */
/* 使い方                                    */
/*C:\Users\odaka\ch1>j < text.txt            */
/* text.txtファイルには、対戦相手の手を格納  */
/* 0:グー　　1:チョキ　　　2:パー　          */
/*********************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/* 記号定数の定義             */
#define SEED 65535    /*乱数のシード*/
#define GU 0  /*グー*/
#define CYOKI 1  /*チョキ*/
#define PA 2  /*パー*/
#define WIN 1 /*勝ち*/
#define LOSE -1 /*負け*/
#define DRAW 0 /*あいこ*/
#define ALPHA  0.01  /*学習係数*/ 

/* 関数のプロトタイプの宣言   */
int hand(double rate[]) ;/*乱数と引数を利用して手を決定する*/
double frand(void) ;/* 0～1の実数乱数  */

/****************/
/*  main()関数  */
/****************/
int main()
{
 int n=0 ;/*対戦回数のカウンタ*/
 int myhand,ohand ;/*自分と相手の手*/
 double rate[3]={1,1,1} ;/*手の出現割合*/
 int gain ;/*勝ち負けの結果*/
 int payoffmatrix[3][3]={{DRAW,WIN,LOSE},
                         {LOSE,DRAW,WIN},
                         {WIN,LOSE,DRAW}} ;
           /*利得行列*/

 /*対戦と学習を繰り返す*/
 while(scanf("%d",&ohand)!=EOF){
  if((ohand<GU)||(ohand>PA)) continue ;/*不正な手の入力*/
  myhand=hand(rate) ;/*出現割合に基づく手の決定*/
  gain=payoffmatrix[myhand][ohand] ;/*勝ち負けの判定*/
  printf("%d %d %d   ",myhand,ohand,gain) ;/*結果出力*/
  rate[myhand]+=gain*ALPHA*rate[myhand] ;/*出現割合の学習*/
  printf("%lf  %lf  %lf\n",
    rate[GU],rate[CYOKI],rate[PA]) ;/*出現割合の出力*/
 }
  
 return 0;
}


/**********************************/
/*  hand()関数                    */
/*乱数と引数を利用して手を決定する*/
/**********************************/
int hand(double rate[])
{
 double gu,cyoki,pa ;/*手を決定するための値を格納*/

 gu=rate[GU]*frand() ;
 cyoki=rate[CYOKI]*frand() ;
 pa=rate[PA]*frand() ;

 if(gu>cyoki){
  if(gu>pa) return GU ;/*guが大きい*/
   else     return PA ;/*paが大きい*/
 }else {
  if(cyoki>pa) return CYOKI ;/*cyokiが大きい*/
   else        return PA ;/*paが大きい*/
 }
}

/*******************/
/* frand()関数     */
/* 0～1の実数乱数  */
/*******************/
double frand(void)
{ 
 return (double)rand()/RAND_MAX ; 
}
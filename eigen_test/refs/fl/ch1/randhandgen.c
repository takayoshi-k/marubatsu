/***************************************************/
/*           randhandgen.c                         */
/*  ある偏りのある手を1000回生成する               */
/* 使い方                                          */
/*C:\Users\odaka\ch1>randhandgen 1 1 1 >text.txt   */
/* text.txtファイルには、生成した手を格納          */
/* 0:グー　　1:チョキ　　　2:パー　                */
/***************************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>

/* 記号定数の定義             */
#define SEED   65535    /*乱数のシード*/
#define LASTNO  1000    /*手の生成回数*/
#define GU 0  /*グー*/
#define CYOKI 1  /*チョキ*/
#define PA 2  /*パー*/

/* 関数のプロトタイプの宣言   */
int hand(double rate[]) ;/*乱数と引数を利用して手を決定する*/
double frand(void) ;/* 0～1の実数乱数  */

/****************/
/*  main()関数  */
/****************/
int main(int argc,char *argv[])
{
 int n ;/*出力回数のカウンタ*/
 double rate[3] ;/*手の出現割合*/

 /*乱数の初期化*/
 srand(SEED) ;

 /*手の生成割合を設定*/
 if(argc<4){/*生成割合の指定がおかしい*/
  fprintf(stderr,"使い方　randhandgen (グーの割合）（チョキの割合）（パーの割合）\n") ;
  exit(1) ;
 }
 rate[GU]=atof(argv[1]) ;/*グーの割合*/
 rate[CYOKI]=atof(argv[2]) ;/*チョキの割合*/
 rate[PA]=atof(argv[3]) ;/*パーの割合*/

 /*出力を繰り返す*/
 for(n=0;n<LASTNO;++n){
  printf("%d\n",hand(rate)) ;
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
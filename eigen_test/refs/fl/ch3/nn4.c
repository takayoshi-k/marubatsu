/***********************************************************/
/*                       nn4.c                             */
/*　畳み込みニューラルネット                           　  */
/* 使い方　                                            　  */
/*  C:\Users\odaka\ch3>nn4 < (学習データセットのファイル名)*/
/*　誤差の推移や，学習結果となる結合係数などを出力します　 */
/***********************************************************/
 
/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS
 
/* ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
/*記号定数の定義*/
/*畳み込み演算関連*/
#define IMAGESIZE 8 /*入力画像の1辺のピクセル数*/
#define F_SIZE 3 /*畳み込みフィルタのサイズ*/
#define F_NO 2 /*フィルタの個数*/
#define POOLOUTSIZE 3 /*プーリング層の出力のサイズ*/
/*全結合増関連*/
#define INPUTNO 18   /*入力層のセル数*/
#define HIDDENNO 6  /*中間層のセル数*/
#define OUTPUTNO 4  /*出力層のセル数*/
#define ALPHA  1    /*学習係数*/
#define MAXNO 100   /*学習データの最大個数*/
#define BIGNUM 100  /*誤差の初期値*/
#define LIMIT 0.001 /*誤差の上限値*/
//#define SEED 65535  /*乱数のシード*/
#define SEED 32767  /*乱数のシード*/
 
/*関数のプロトタイプの宣言*/
/*畳み込み演算関連*/
void initfilter(double filter[F_NO][F_SIZE][F_SIZE]) ;
                          /*畳み込みフィルタの初期化*/
int getdata(double image[][IMAGESIZE][IMAGESIZE]
                  ,double t[][OUTPUTNO]) ; /*データ読み込み*/ 
void conv(double filter[F_SIZE][F_SIZE]
                ,double e[][IMAGESIZE]
                ,double convout[][IMAGESIZE]) ; /*畳み込みの計算*/
double calcconv(double filter[][F_SIZE]
               ,double e[][IMAGESIZE],int i,int j) ;/*  フィルタの適用  */
void pool(double convout[][IMAGESIZE],double poolout[][POOLOUTSIZE]) ; 
                                     /*プーリングの計算*/ 
double calcpooling(double convout[][IMAGESIZE]
                 ,int x,int y) ;/* 平均値プーリング */

/*全結合層関連*/
void initwh(double wh[HIDDENNO][INPUTNO+1]) ;
                         /*中間層の重みの初期化*/
void initwo(double wo[OUTPUTNO][HIDDENNO+1]) ;
                        /*出力層の重みの初期化*/
double forward(double wh[HIDDENNO][INPUTNO+1]
         ,double [HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO]) ; /*順方向の計算*/
void olearn(double wo[HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO],double o,int k) ;
                                  /*出力層の重みの学習*/
void hlearn(double wh[HIDDENNO][INPUTNO+1]
         ,double wo[HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO],double o,int k) ;
                                  /*中間層の重みの学習*/
void printweight(double wh[HIDDENNO][INPUTNO+1]
          ,double wo[OUTPUTNO][HIDDENNO+1]) ; /*結果の出力*/
double s(double u) ; /*シグモイド関数*/
double drand(void) ;/*-1から1の間の乱数を生成 */
 
/*******************/
/*    main()関数   */
/*******************/
int main()
{
 /*畳み込み演算関連*/
 double filter[F_NO][F_SIZE][F_SIZE] ;/*畳み込みフィルタ*/
 double image[MAXNO][IMAGESIZE][IMAGESIZE] ;/*入力データ*/
 double t[MAXNO][OUTPUTNO] ;/*教師データ*/
 double convout[IMAGESIZE][IMAGESIZE] ;/*畳み込み出力*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;/*出力データ*/

 /*全結合層関連*/
 double wh[HIDDENNO][INPUTNO+1] ;/*中間層の重み*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*出力層の重み*/
 double e[MAXNO][INPUTNO+OUTPUTNO] ;/*学習データセット*/
 double hi[HIDDENNO+1] ;/*中間層の出力*/
 double o[OUTPUTNO]  ;/*出力*/
 double err=BIGNUM ;/*誤差の評価*/
 int i,j,m,n ;/*繰り返しの制御*/
 int n_of_e ;/*学習データの個数*/
 int count=0 ;/*繰り返し回数のカウンタ*/
 
 /*乱数の初期化*/
 srand(SEED) ;
 
 /*畳み込みフィルタの初期化*/
 initfilter(filter) ;

 /*重みの初期化*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;


 /*学習データの読み込み*/
 n_of_e=getdata(image,t) ;
 printf("学習データの個数:%d\n",n_of_e) ;

 /*畳み込み処理*/
 for(i=0;i<n_of_e;++i){/*学習データ毎の繰り返し*/
  for(j=0;j<F_NO;++j){/*フィルタ毎の繰り返し*/
    /*畳み込みの計算*/
    conv(filter[j],image[i],convout) ;
    /*プーリングの計算*/
    pool(convout,poolout) ;
    /*プーリング出力を全結合層の入力へコピー*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      e[i][j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    for(m=0;m<OUTPUTNO;++m)
     e[i][POOLOUTSIZE*POOLOUTSIZE*F_NO+m]=t[i][m] ;/*教師データ*/
  }
 }
 
 /*学習*/
 while(err>LIMIT){
  /*i個の出力層に対応*/
  for(i=0;i<OUTPUTNO;++i){
   err=0.0 ;
   for(j=0;j<n_of_e;++j){
    /*順方向の計算*/
    o[i]=forward(wh,wo[i],hi,e[j]) ;
    /*出力層の重みの調整*/
    olearn(wo[i],hi,e[j],o[i],i) ;
    /*中間層の重みの調整*/
    hlearn(wh,wo[i],hi,e[j],o[i],i) ;
    /*誤差の積算*/
    err+=(o[i]-e[j][INPUTNO+i])*(o[i]-e[j][INPUTNO+i]) ;
   }
   ++count ;
   /*誤差の出力*/
   printf("%d\t%lf\n",count,err) ;
  }
 }/*学習終了*/
 
 /*重みの出力*/
 printweight(wh,wo) ;
 
 /*学習データに対する出力*/
 for(i=0;i<n_of_e;++i){
  printf("%d\n",i) ;
  for(j=0;j<INPUTNO;++j)
   printf("%lf ",e[i][j]) ;/*学習データ*/
  printf("\n") ;
  for(j=INPUTNO;j<INPUTNO+OUTPUTNO;++j)/*教師データ*/
   printf("%lf ",e[i][j]) ;
  printf("\n") ;
  for(j=0;j<OUTPUTNO;++j)/*ネットワーク出力*/
   printf("%lf ",forward(wh,wo[j],hi,e[i])) ;
  printf("\n") ;
 }
 
 return 0 ;
}

/**********************/
/*  initfilter()関数  */
/* 　フィルタの初期化 */
/**********************/
void initfilter(double filter[F_NO][F_SIZE][F_SIZE])
{
 int i,j,k ;/*繰り返しの制御*/
 
 for(i=0;i<F_NO;++i)
  for(j=0;j<F_SIZE;++j)
   for(k=0;k<F_SIZE;++k)
    filter[i][j][k]=drand() ;
} 

/**********************/
/*    initwh()関数    */
/*中間層の重みの初期化*/
/**********************/
void initwh(double wh[HIDDENNO][INPUTNO+1])
{
 int i,j ;/*繰り返しの制御*/
 
 /*　乱数による重みの初期化*/
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<INPUTNO+1;++j)
   wh[i][j]=drand() ;
}
 
/**********************/
/*    initwo()関数    */
/*出力層の重みの初期化*/
/**********************/
void initwo(double wo[OUTPUTNO][HIDDENNO+1])
{
 int i,j ;/*繰り返しの制御*/
 
 /*　乱数による重みの初期化*/
 for(i=0;i<OUTPUTNO;++i)
  for(j=0;j<HIDDENNO+1;++j)
   wo[i][j]=drand() ;
}
 
/**********************/
/*  getdata()関数     */
/*入力データの読み込み*/
/**********************/
int getdata(double image[][IMAGESIZE][IMAGESIZE]
                  ,double t[][OUTPUTNO])
{
 int i=0,j=0,k=0 ;/*繰り返しの制御用*/

 /*データの入力*/
 while(scanf("%lf",&t[i][j])!=EOF){
  /*教師データの読み込み*/
  ++j ;
  while(scanf("%lf",&t[i][j])!=EOF){
   ++j ;
   if(j>=OUTPUTNO) break ;
  }

  /*画像データの読み込み*/
  j=0 ;
  while(scanf("%lf",&image[i][j][k])!=EOF){
   ++ k ;
   if(k>=IMAGESIZE){/*次のデータ*/
    k=0 ;
    ++j ;
    if(j>=IMAGESIZE) break ;/*入力終了*/
   }
  }
  j=0; k=0 ;
  ++i ; 
 }
 return i ;
}

/**********************/
/*  conv()関数        */
/*  畳み込みの計算    */
/**********************/
void conv(double filter[][F_SIZE]
         ,double e[][IMAGESIZE],double convout[][IMAGESIZE])
{
 int i=0,j=0 ;/*繰り返しの制御用*/
 int startpoint=F_SIZE/2 ;/*畳み込み範囲の下限*/

 for(i=startpoint;i<IMAGESIZE-startpoint;++i)
  for(j=startpoint;j<IMAGESIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv()関数    */
/*  フィルタの適用    */
/**********************/
double calcconv(double filter[][F_SIZE]
             ,double e[][IMAGESIZE],int i,int j)
{
 int m,n ;/*繰り返しの制御用*/
 double sum=0 ;/*和の値*/
 
 for(m=0;m<F_SIZE;++m)
  for(n=0;n<F_SIZE;++n)
   sum+=e[i-F_SIZE/2+m][j-F_SIZE/2+n]*filter[m][n];
   
 return sum ;
}

/**********************/
/*  pool()関数        */
/* プーリングの計算   */
/**********************/
void pool(double convout[][IMAGESIZE]
         ,double poolout[][POOLOUTSIZE]) 
{
 int i,j ;/*繰り返しの制御*/

 for(i=0;i<POOLOUTSIZE;++i)
  for(j=0;j<POOLOUTSIZE;++j)
   poolout[i][j]=calcpooling(convout,i*2+1,j*2+1) ;
}
 
/**********************/
/* calcpooling()関数  */
/* 平均値プーリング   */
/**********************/
double calcpooling(double convout[][IMAGESIZE]
                 ,int x,int y)
{
 int m,n ;/*繰り返しの制御用*/
 double ave=0.0 ;/*平均値*/
 
 for(m=x;m<=x+1;++m)
  for(n=y;n<=y+1;++n)
   ave+=convout[m][n] ;

 return ave/4.0 ;
}
               
/**********************/
/*  forward()関数     */
/*  順方向の計算      */
/**********************/
double forward(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[])
{
 int i,j ;/*繰り返しの制御*/
 double u ;/*重み付き和の計算*/
 double o ;/*出力の計算*/
 
 /*hiの計算*/
 for(i=0;i<HIDDENNO;++i){
  u=0 ;/*重み付き和を求める*/
  for(j=0;j<INPUTNO;++j)
   u+=e[j]*wh[i][j] ;
  u-=wh[i][j] ;/*しきい値の処理*/
  hi[i]=s(u) ;
 }
 /*出力oの計算*/
 o=0 ;
 for(i=0;i<HIDDENNO;++i)
  o+=hi[i]*wo[i] ;
 o-=wo[i] ;/*しきい値の処理*/
 
 return s(o) ;
}
 
/**********************/
/*  olearn()関数      */
/*  出力層の重み学習  */
/**********************/
void olearn(double wo[HIDDENNO+1]
    ,double hi[],double e[],double o,int k)
{
 int i ;/*繰り返しの制御*/
 double d ;/*重み計算に利用*/
 
 d=(e[INPUTNO+k]-o)*o*(1-o) ;/*誤差の計算*/
 for(i=0;i<HIDDENNO;++i){
  wo[i]+=ALPHA*hi[i]*d ;/*重みの学習*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;/*しきい値の学習*/
}
 
/**********************/
/*  hlearn()関数      */
/*  中間層の重み学習  */
/**********************/
void hlearn(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1]
                ,double hi[],double e[],double o,int k)
{
 int i,j ;/*繰り返しの制御*/
 double dj ;/*中間層の重み計算に利用*/
 
 for(j=0;j<HIDDENNO;++j){/*中間層の各セルjを対象*/
  dj=hi[j]*(1-hi[j])*wo[j]*(e[INPUTNO+k]-o)*o*(1-o) ;
  for(i=0;i<INPUTNO;++i)/*i番目の重みを処理*/
   wh[j][i]+=ALPHA*e[i]*dj ;
  wh[j][i]+=ALPHA*(-1.0)*dj ;/*しきい値の学習*/
 }
}
 
/**********************/
/*  printweight()関数 */
/*   結果の出力       */
/**********************/
void printweight(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[OUTPUTNO][HIDDENNO+1])
{
 int i,j ;/*繰り返しの制御*/
 
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<INPUTNO+1;++j)
   printf("%lf ",wh[i][j]) ;
 printf("\n") ;
 for(i=0;i<OUTPUTNO;++i){
  for(j=0;j<HIDDENNO+1;++j)
   printf("%lf ",wo[i][j]) ;
 }
 printf("\n") ;
}
 
/*******************/
/* s()関数         */
/* シグモイド関数  */
/*******************/
double s(double u)
{
 return 1.0/(1.0+exp(-u)) ;
}
 
/*************************/
/* drand()関数           */
/*-1から1の間の乱数を生成*/
/*************************/
double drand(void)
{
 double rndno ;/*生成した乱数*/
 
 while((rndno=(double)rand()/RAND_MAX)==1.0) ;
 rndno=rndno*2-1 ;/*-1から1の間の乱数を生成*/
 return rndno;
}

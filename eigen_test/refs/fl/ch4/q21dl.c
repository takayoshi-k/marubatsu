/********************************************/
/*         q21dl.c                          */
/*   強化学習とNNの例題プログラム　         */
/*　 q21.cの発展版                          */
/*使い方                                    */
/* C:\Users\odaka\ch4>q21dl                 */
/********************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 記号定数の定義             */
/*強化学習関連*/
#define GENMAX  4000 /*学習の繰り返し回数*/
#define STATENO  7  /*状態の数*/
#define ACTIONNO 2  /*行動の数*/
#define ALPHA 0.1/*学習係数*/
#define GAMMA 0.9/*割引率*/
#define EPSILON 0.3 /*行動選択のランダム性を決定*/
#define SEED 65535 /*乱数のシード*/
#define REWARD 1 /*ゴール到達時の報酬*/
#define GOAL 6 /*状態６がゴール状態*/
#define UP 0/*上方向の行動*/
#define DOWN 1/*下方向の行動*/
#define LEVEL 2 /*枝分かれの深さ*/
/*ニューラルネット関連*/
#define INPUTNO 7   /*入力層のセル数*/
#define HIDDENNO 2  /*中間層のセル数*/
#define OUTPUTNO 2  /*出力層のセル数*/
#define NNALPHA  3    /*学習係数*/

/* 関数のプロトタイプの宣言   */
/*強化学習関連*/
int rand0or1() ;/*0又は1を返す乱数関数*/
double frand() ;/*0～1の実数を返す乱数関数*/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q値出力*/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*行動選択*/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q値更新*/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q値最大値を選択*/
int nexts(int s,int a) ;/*行動によって次の状態に遷移*/
/*ニューラルネット関連*/
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
 
/****************/
/*  main()関数  */
/****************/
int main()
{
 /*強化学習関連*/
 int i;
 int s,snext;/*現在の状態と、次の状態*/
 int t;/*時刻*/
 int a;/*行動*/
 /*ニューラルネット関連*/
 double wh[HIDDENNO][INPUTNO+1] ;/*中間層の重み*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*出力層の重み*/
 double e[INPUTNO+OUTPUTNO] ;/*学習データセット*/
 double hi[HIDDENNO+1] ;/*中間層の出力*/
 double o[OUTPUTNO]  ;/*出力*/
 int i_n ;/*繰り返しの制御*/
 int count=0 ;/*繰り返し回数のカウンタ*/

 /*乱数の初期化*/
 srand(SEED);

 /*重みの初期化*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;
 
 /*学習の本体*/
 for(i=0;i<GENMAX;++i){
  s=0;/*行動の初期状態*/
  for(t=0;t<LEVEL;++t){/*最下段まで繰り返す*/
   /*行動選択*/
   a=selecta(s,wh,wo,hi) ;
   fprintf(stderr," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q値の更新*/
   /*ネットワーク入力データeの設定*/
   for(i_n=0;i_n<INPUTNO+OUTPUTNO;++i_n)
    e[i_n]=0 ;
   e[s]=1 ;/*現在の状態*/
   e[INPUTNO+a]=updateq(s,snext,a,wh,wo,hi) ;/*行動*/
   /*順方向の計算*/
   o[a]=forward(wh,wo[a],hi,e) ;
   /*出力層の重みの調整*/
   olearn(wo[a],hi,e,o[a],a) ;
   /*中間層の重みの調整*/
   hlearn(wh,wo[a],hi,e,o[a],a) ;
   /*行動aによって次の状態snextに遷移*/
   s=snext ;
  }
  /*Q値の出力*/
  printqvalue(wh,wo,hi) ;
 }
 return 0;
}

/******************/
/*　下請け関数　　*/
/*  強化学習関連  */
/******************/

/****************************/
/*       updateq()関数      */
/*　     Q値を更新する      */
/****************************/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
                       ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 double qv ;/*更新されるQ値*/
 double qvalue_sa ;/*現在のQ値*/
 double qvalue_snexta ;/*次の状態での最大Q値*/
 double e[INPUTNO+1]={0}  ;

 /*現在状態sでのQ値を求める*/
 e[s]=1 ;/*ニューラルネットへの入力設定*/
 qvalue_sa=forward(wh,wo[a],hi,e) ;/*行動a*/ 
 e[s]=0 ;/*入力のクリア*/

 /*次の状態snextでの最大Q値を求める*/
 e[snext]=1 ;/*ニューラルネットへの入力設定*/
 qvalue_snexta=forward(wh,wo[set_a_by_q(snext,wh,wo,hi)],hi,e) ;

 /*Q値の更新*/
 if(snext==GOAL)/*報酬が付与される場合*/
   qv=qvalue_sa+ALPHA*(REWARD-qvalue_sa) ;
 else/*報酬なし*/
   qv=qvalue_sa
     +ALPHA*(GAMMA*qvalue_snexta-qvalue_sa) ;

 return qv ;
}

/****************************/
/*        selecta()関数     */
/*　     行動を選択する     */
/****************************/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 int a ;/*選択された行動*/
 
 /*ε-greedy法による行動選択*/
 if(frand()<EPSILON){
  /*ランダムに行動*/
  a=rand0or1();
 }
 else{
  /*Ｑ値最大値を選択*/
  a=set_a_by_q(s,wh,wo,hi) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()関数      */
/*　  Ｑ値最大値を選択      */
/****************************/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 double upqvalue,downqvalue ;
 double e[INPUTNO+1]={0}  ;

 /*Q値の計算*/
 e[s]=1 ;/*ニューラルネットへの入力設定*/
 upqvalue=forward(wh,wo[UP],hi,e) ;/*UP*/
 downqvalue=forward(wh,wo[DOWN],hi,e) ;/*DOWN*/
 /*Q値による判定*/
 if((upqvalue)>(downqvalue)) 
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
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 int i,j ;
 double e[INPUTNO+1]={0}  ;

 for(i=0;i<STATENO;++i){
  for(j=0;j<ACTIONNO;++j){
   e[i]=1 ;/*出力すべき状態の番号*/
   printf("%.3lf ",forward(wh,wo[j],hi,e));
   e[i]=0 ;/*元に戻す*/
  }
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

/**************************/
/*　下請け関数　　        */
/*  ニューラルネット関連  */
/**************************/
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
  wo[i]+=NNALPHA*hi[i]*d ;/*重みの学習*/
 }
 wo[i]+=NNALPHA*(-1.0)*d ;/*しきい値の学習*/
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
   wh[j][i]+=NNALPHA*e[i]*dj ;
  wh[j][i]+=NNALPHA*(-1.0)*dj ;/*しきい値の学習*/
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

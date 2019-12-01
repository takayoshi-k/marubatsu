/********************************************/
/*         q22dl.c                          */
/*   強化学習とNNの例題プログラム　         */
/*　 q22.cの発展版                          */
/*使い方                                    */
/* C:\Users\odaka\ch4>q22dl                 */
/********************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/*ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 記号定数の定義             */
/*強化学習関連*/
#define GENMAX  100000 /*学習の繰り返し回数*/
#define STATENO  64  /*状態の数*/
#define ACTIONNO 4  /*行動の数*/
#define ALPHA 0.1/*学習係数*/
#define GAMMA 0.9/*割引率*/
#define EPSILON 0.3 /*行動選択のランダム性を決定*/
//#define SEED 65535 /*乱数のシード*/
#define SEED 32767 /*乱数のシード*/
#define REWARD 1 /*ゴール到達時の報酬*/
#define GOAL 54 /*状態54がゴール状態*/
#define UP 0/*上方向の行動*/
#define DOWN 1/*下方向の行動*/
#define LEFT 2/*左方向の行動*/
#define RIGHT 3/*右方向の行動*/
#define LEVEL 512 /*1試行における最大ステップ数*/
/*ニューラルネット関連*/
/*畳み込み演算関連*/
#define IMAGESIZE 8 /*入力画像の1辺のピクセル数*/
#define F_SIZE 3 /*畳み込みフィルタのサイズ*/
#define F_NO 2 /*フィルタの個数*/
#define POOLOUTSIZE 3 /*プーリング層の出力のサイズ*/
/*全結合増関連*/
#define INPUTNO 18   /*入力層のセル数*/
#define HIDDENNO 6  /*中間層のセル数*/
#define OUTPUTNO 4  /*出力層のセル数*/
#define NNALPHA  1    /*学習係数*/

/* 関数のプロトタイプの宣言   */
/*強化学習関連*/
int rand03() ;/*0～3の値を返す乱数関数*/
double frand() ;/*0～1の実数を返す乱数関数*/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q値出力*/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*行動選択*/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q値更新*/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q値最大値を選択*/
int nexts(int s,int a) ;/*行動によって次の状態に遷移*/
double calcqvalue(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[],int s,int a) ;
                                               /*Q値の計算 */

/*ニューラルネット関連*/
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

void set_e_by_s(int s,double filter[F_NO][F_SIZE][F_SIZE]
                ,double e[]) ;/*畳み込みを用いたNN入力データの設定  */

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
 /*畳み込み演算関連*/
 double filter[F_NO][F_SIZE][F_SIZE] ;/*畳み込みフィルタ*/

 /*全結合層関連*/
 double wh[HIDDENNO][INPUTNO+1] ;/*中間層の重み*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*出力層の重み*/
 double e[INPUTNO+OUTPUTNO] ;/*学習データセット*/
 double hi[HIDDENNO+1] ;/*中間層の出力*/
 double o[OUTPUTNO]  ;/*出力*/
 int count=0 ;/*繰り返し回数のカウンタ*/

 /*乱数の初期化*/
 srand(SEED);

 /*畳み込みフィルタの初期化*/
 initfilter(filter) ;

 /*重みの初期化*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;
 
 /*学習の本体*/
 for(i=0;i<GENMAX;++i){
  if(i%1000==0) fprintf(stderr,"%d000 step\n",i/1000) ;
  s=0;/*行動の初期状態*/
  for(t=0;t<LEVEL;++t){/*最大ステップ数まで繰り返す*/
   /*行動選択*/
   a=selecta(s,wh,wo,hi,filter) ;
   fprintf(stdout," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q値の更新*/
   /*ネットワーク入力データeの設定*/
   set_e_by_s(s,filter,e) ;
   e[INPUTNO+a]=updateq(s,snext,a,wh,wo,hi,filter) ;/*行動*/
   /*順方向の計算*/
   o[a]=forward(wh,wo[a],hi,e) ;
   /*出力層の重みの調整*/
   olearn(wo[a],hi,e,o[a],a) ;
   /*中間層の重みの調整*/
   hlearn(wh,wo[a],hi,e,o[a],a) ;
   /*行動aによって次の状態snextに遷移*/
   s=snext ;
   /*ゴールに到達したら初期状態に戻る*/
   if(s==GOAL) break ;
  }
  /*Q値の出力*/
  printqvalue(wh,wo,hi,filter) ;
 }
 return 0;
}

/******************/
/*　下請け関数　　*/
/*  強化学習関連  */
/******************/

/******************/
/*calcqvalue()関数*/
/*Q値の計算       */
/******************/
double calcqvalue(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[],int s,int a)
{
 
 /*移動できない方向へのQ値を0にする*/
 if((s<=7)&&(a==UP)) return 0 ;/*最上段ではUP方向に進まない*/
 if((s>=56)&&(a==DOWN)) return 0 ;/*最下段ではDOWN方向に進まない*/  
 if((s%8==0)&&(a==LEFT)) return 0 ;/*左端ではLEFT方向に進まない*/
 if((s%8==7)&&(a==RIGHT)) return 0 ;/*右端ではRIGHT方向に進まない*/

 /*移動できる場合のQ値*/
 return forward(wh,wo,hi,e) ;
}

/****************************/
/*       updateq()関数      */
/*　     Q値を更新する      */
/****************************/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
               ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
               ,double filter[F_NO][F_SIZE][F_SIZE])
{
 double qv ;/*更新されるQ値*/
 double qvalue_sa ;/*現在のQ値*/
 double qvalue_snexta ;/*次の状態での最大Q値*/
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*現在状態sでのQ値を求める*/
 /*ネットワーク入力データeの設定*/
 set_e_by_s(s,filter,e) ;
 qvalue_sa=calcqvalue(wh,wo[a],hi,e,s,a) ;/*行動a*/

 /*次の状態snextでの最大Q値を求める*/
 /*ネットワーク入力データeの設定*/
 set_e_by_s(snext,filter,e) ;
 qvalue_snexta=calcqvalue(wh,wo[set_a_by_q(snext,wh,wo,hi,filter)]
                          ,hi,e,snext,set_a_by_q(snext,wh,wo,hi,filter)) ;

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
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
             ,double filter[F_NO][F_SIZE][F_SIZE])
{
 int a ;/*選択された行動*/
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*ニューラルネットへの入力設定*/
 set_e_by_s(s,filter,e) ;
 /*ε-greedy法による行動選択*/
 if(frand()<EPSILON){
  /*ランダムに行動*/
  do{
   a=rand03() ;
  }while(calcqvalue(wh,wo[a],hi,e,s,a)==0) ;/*移動できない方向ならやり直し*/
 }
 else{
  /*Ｑ値最大値を選択*/
  a=set_a_by_q(s,wh,wo,hi,filter) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()関数      */
/*　  Ｑ値最大値を選択      */
/****************************/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
             ,double filter[F_NO][F_SIZE][F_SIZE])
{
 double maxq=0 ;/*Q値の最大値候補*/
 int maxaction=0 ;/*Q値最大に対応する行動*/
 int i ;
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*ネットワーク入力データeの設定*/
 set_e_by_s(s,filter,e) ;
 for(i=0;i<ACTIONNO;++i)
  if(calcqvalue(wh,wo[i],hi,e,s,i)>maxq){

   maxq=calcqvalue(wh,wo[i],hi,e,s,i) ;/*最大値の更新*/
   maxaction=i ;/*対応する行動*/
  }

 return maxaction ;

}

/****************************/
/*    nexts()関数           */
/*行動によって次の状態に遷移*/
/****************************/
int nexts(int s,int a)
{
 int next_s_value[]={-8,8,-1,1} ;
      /*行動aに対応して次の状態に遷移するための加算値*/

 return s+next_s_value[a] ;
}

/****************************/
/*    printqvalue()関数     */
/*　   Q値を出力する        */
/****************************/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
             ,double filter[F_NO][F_SIZE][F_SIZE])
{
 int i,j ;
 double e[INPUTNO+OUTPUTNO]={0}  ;

 for(i=0;i<STATENO;++i){
  for(j=0;j<ACTIONNO;++j){
   set_e_by_s(i,filter,e) ;
   printf("%.3lf ",forward(wh,wo[j],hi,e));
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
/*     rand03()関数         */
/*　 0～3の値を返す乱数関数 */
/****************************/
int rand03()
{
 int rnd ;
 
 /*乱数の最大値を除く*/
 while((rnd=rand())==RAND_MAX) ;
 /*乱数の計算*/
 return (int)((double)rnd/RAND_MAX*4) ;
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
 
/************************************/
/* set_e_by_s()関数                 */
/* 畳み込みを用いたNN入力データの設定  */
/************************************/
void set_e_by_s(int s,double filter[F_NO][F_SIZE][F_SIZE]
               ,double e[])
{
 int i,j,m,n ;/*繰り返しの制御用*/
 double image[IMAGESIZE][IMAGESIZE] ;/*入力データ*/
 double convout[IMAGESIZE][IMAGESIZE] ;/*畳み込み出力*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;/*出力データ*/
 
 /*畳み込み部への入力の設定*/
 for(i=0;i<IMAGESIZE;++i)
  for(j=0;j<IMAGESIZE;++j){
   if((i+j*IMAGESIZE)==s) image[i][j]=1 ;
   else image[i][j]=0 ;
  }

 for(j=0;j<F_NO;++j){/*フィルタ毎の繰り返し*/
  /*畳み込みの計算*/
  conv(filter[j],image,convout) ;
  /*プーリングの計算*/
  pool(convout,poolout) ;
  /*プーリング出力を全結合層の入力へコピー*/
  for(m=0;m<POOLOUTSIZE;++m)
   for(n=0;n<POOLOUTSIZE;++n)
    e[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    for(m=0;m<OUTPUTNO;++m)
     e[POOLOUTSIZE*POOLOUTSIZE*F_NO+m]=0 ;/*教師データ部のクリア*/
 }
}
  
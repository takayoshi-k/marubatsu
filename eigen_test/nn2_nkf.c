/**********************************************************/
/*                       nn2.c                            */
/*　バックプロパゲーションによるニューラルネットの学習 　 */
/* 使い方　                                               */
/* C:\Users\odaka\ch3>nn2 < (学習データセットのファイル名)*/
/* 誤差の推移や，学習結果となる結合係数などを出力します　 */
/**********************************************************/

/*Visual Studioとの互換性確保 */
#define _CRT_SECURE_NO_WARNINGS

/* ヘッダファイルのインクルード*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*記号定数の定義*/
#define INPUTNO 2   /*入力層のセル数*/
#define HIDDENNO 2  /*中間層のセル数*/
#define ALPHA  1    /*学習係数*/
#define MAXNO 100   /*学習データの最大個数*/
#define BIGNUM 100  /*誤差の初期値*/
#define LIMIT 0.001 /*誤差の上限値*/
#define SEED 65535  /*乱数のシード*/
//#define SEED 32767  /*乱数のシード*/

/*関数のプロトタイプの宣言*/
void initwh(double wh[HIDDENNO][INPUTNO+1]) ;
/*中間層の重みの初期化*/
void initwo(double wo[HIDDENNO+1]) ;/*出力層の重みの初期化*/
int getdata(double e[][INPUTNO+1]) ; /*学習データの読み込み*/
double forward(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[HIDDENNO+1],double hi[]
                ,double e[INPUTNO+1]) ; /*順方向の計算*/
void olearn(double wo[HIDDENNO+1],double hi[]
                ,double e[INPUTNO+1],double o) ; /*出力層の重みの学習*/
void hlearn(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[HIDDENNO+1],double hi[]
                ,double e[INPUTNO+1],double o) ; /*中間層の重みの学習*/
void printweight(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[HIDDENNO+1]) ; /*結果の出力*/
double s(double u) ; /*シグモイド関数*/
double drand(void) ;/*-1から1の間の乱数を生成 */

/*******************/
/*    main()関数   */
/*******************/
int main()
{
        double wh[HIDDENNO][INPUTNO+1] ;/*中間層の重み*/
        double wo[HIDDENNO+1] ;/*出力層の重み*/
        double e[MAXNO][INPUTNO+1] ;/*学習データセット*/
        double hi[HIDDENNO+1] ;/*中間層の出力*/
        double o ;/*出力*/
        double err=BIGNUM ;/*誤差の評価*/
        int i,j ;/*繰り返しの制御*/
        int n_of_e ;/*学習データの個数*/
        int count=0 ;/*繰り返し回数のカウンタ*/

        /*乱数の初期化*/
        srand(SEED) ;

        /*重みの初期化*/
        initwh(wh) ;
        initwo(wo) ;
        printweight(wh,wo) ;

        /*学習データの読み込み*/
        n_of_e=getdata(e) ;
        printf("学習データの個数:%d\n",n_of_e) ;

        /*学習*/
        while(err>LIMIT){
                err=0.0 ;
                for(j=0;j<n_of_e;++j){
                        /*順方向の計算*/
                        o=forward(wh,wo,hi,e[j]) ;
                        /*出力層の重みの調整*/
                        olearn(wo,hi,e[j],o) ;
                        /*中間層の重みの調整*/
                        hlearn(wh,wo,hi,e[j],o) ;
                        /*誤差の積算*/
                        err+=(o-e[j][INPUTNO])*(o-e[j][INPUTNO]) ;
                }
                ++count ;
                /*誤差の出力*/
                printf("%d\t%lf\n",count,err) ;
        }/*学習終了*/

        /*重みの出力*/
        printweight(wh,wo) ;

        /*学習データに対する出力*/
        for(i=0;i<n_of_e;++i){
                printf("%d ",i) ;
                for(j=0;j<INPUTNO+1;++j)
                        printf("%lf ",e[i][j]) ;
                o=forward(wh,wo,hi,e[i]) ;
                printf("%lf\n",o) ;
        }

        return 0 ;
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
/*出力層の重みの初期化 */
/**********************/
void initwo(double wo[HIDDENNO+1])
{
        int i ;/*繰り返しの制御*/

        for(i=0;i<HIDDENNO+1;++i)
                wo[i]=drand() ;
}

/**********************/
/*  getdata()関数     */
/*学習データの読み込み*/
/**********************/
int getdata(double e[][INPUTNO+1])
{
        int n_of_e=0 ;/*データセットの個数*/
        int j=0 ;/*繰り返しの制御用*/

        /*データの入力*/
        while(scanf("%lf",&e[n_of_e][j])!=EOF){
                ++ j ;
                if(j>INPUTNO){/*次のデータ*/
                        j=0 ;
                        ++n_of_e ;
                        if(n_of_e>=MAXNO){/*入力数が上限に達した*/
                                fprintf(stderr,"入力数が上限に達しました\n") ;
                                break ;
                        }
                }
        }
        return n_of_e ;
}

/**********************/
/*  forward()関数     */
/*  順方向の計算      */
/**********************/
double forward(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[HIDDENNO+1],double hi[],double e[INPUTNO+1])
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
                ,double hi[],double e[INPUTNO+1],double o)
{
        int i ;/*繰り返しの制御*/
        double d ;/*重み計算に利用*/

        d=(e[INPUTNO]-o)*o*(1-o) ;/*誤差の計算*/
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
                ,double hi[],double e[INPUTNO+1],double o)
{
        int i,j ;/*繰り返しの制御*/
        double dj ;/*中間層の重み計算に利用*/

        for(j=0;j<HIDDENNO;++j){/*中間層の各セルjを対象*/
                dj=hi[j]*(1-hi[j])*wo[j]*(e[INPUTNO]-o)*o*(1-o) ;
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
                ,double wo[HIDDENNO+1])
{
        int i,j ;/*繰り返しの制御*/

        for(i=0;i<HIDDENNO;++i)
                for(j=0;j<INPUTNO+1;++j)
                        printf("%lf ",wh[i][j]) ;
        printf("\n") ;
        for(i=0;i<HIDDENNO+1;++i)
                printf("%lf ",wo[i]) ;
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


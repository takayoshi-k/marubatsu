/***********************************************************/
/*                       nn4.c                             */
/*�@��ݍ��݃j���[�����l�b�g                           �@  */
/* �g�����@                                            �@  */
/*  C:\Users\odaka\ch3>nn4 < (�w�K�f�[�^�Z�b�g�̃t�@�C����)*/
/*�@�덷�̐��ڂ�C�w�K���ʂƂȂ錋���W���Ȃǂ��o�͂��܂��@ */
/***********************************************************/
 
/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS
 
/* �w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
/*�L���萔�̒�`*/
/*��ݍ��݉��Z�֘A*/
#define IMAGESIZE 8 /*���͉摜��1�ӂ̃s�N�Z����*/
#define F_SIZE 3 /*��ݍ��݃t�B���^�̃T�C�Y*/
#define F_NO 2 /*�t�B���^�̌�*/
#define POOLOUTSIZE 3 /*�v�[�����O�w�̏o�͂̃T�C�Y*/
/*�S�������֘A*/
#define INPUTNO 18   /*���͑w�̃Z����*/
#define HIDDENNO 6  /*���ԑw�̃Z����*/
#define OUTPUTNO 4  /*�o�͑w�̃Z����*/
#define ALPHA  1    /*�w�K�W��*/
#define MAXNO 100   /*�w�K�f�[�^�̍ő��*/
#define BIGNUM 100  /*�덷�̏����l*/
#define LIMIT 0.001 /*�덷�̏���l*/
//#define SEED 65535  /*�����̃V�[�h*/
#define SEED 32767  /*�����̃V�[�h*/
 
/*�֐��̃v���g�^�C�v�̐錾*/
/*��ݍ��݉��Z�֘A*/
void initfilter(double filter[F_NO][F_SIZE][F_SIZE]) ;
                          /*��ݍ��݃t�B���^�̏�����*/
int getdata(double image[][IMAGESIZE][IMAGESIZE]
                  ,double t[][OUTPUTNO]) ; /*�f�[�^�ǂݍ���*/ 
void conv(double filter[F_SIZE][F_SIZE]
                ,double e[][IMAGESIZE]
                ,double convout[][IMAGESIZE]) ; /*��ݍ��݂̌v�Z*/
double calcconv(double filter[][F_SIZE]
               ,double e[][IMAGESIZE],int i,int j) ;/*  �t�B���^�̓K�p  */
void pool(double convout[][IMAGESIZE],double poolout[][POOLOUTSIZE]) ; 
                                     /*�v�[�����O�̌v�Z*/ 
double calcpooling(double convout[][IMAGESIZE]
                 ,int x,int y) ;/* ���ϒl�v�[�����O */

/*�S�����w�֘A*/
void initwh(double wh[HIDDENNO][INPUTNO+1]) ;
                         /*���ԑw�̏d�݂̏�����*/
void initwo(double wo[OUTPUTNO][HIDDENNO+1]) ;
                        /*�o�͑w�̏d�݂̏�����*/
double forward(double wh[HIDDENNO][INPUTNO+1]
         ,double [HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO]) ; /*�������̌v�Z*/
void olearn(double wo[HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO],double o,int k) ;
                                  /*�o�͑w�̏d�݂̊w�K*/
void hlearn(double wh[HIDDENNO][INPUTNO+1]
         ,double wo[HIDDENNO+1],double hi[]
         ,double e[INPUTNO+OUTPUTNO],double o,int k) ;
                                  /*���ԑw�̏d�݂̊w�K*/
void printweight(double wh[HIDDENNO][INPUTNO+1]
          ,double wo[OUTPUTNO][HIDDENNO+1]) ; /*���ʂ̏o��*/
double s(double u) ; /*�V�O���C�h�֐�*/
double drand(void) ;/*-1����1�̊Ԃ̗����𐶐� */
 
/*******************/
/*    main()�֐�   */
/*******************/
int main()
{
 /*��ݍ��݉��Z�֘A*/
 double filter[F_NO][F_SIZE][F_SIZE] ;/*��ݍ��݃t�B���^*/
 double image[MAXNO][IMAGESIZE][IMAGESIZE] ;/*���̓f�[�^*/
 double t[MAXNO][OUTPUTNO] ;/*���t�f�[�^*/
 double convout[IMAGESIZE][IMAGESIZE] ;/*��ݍ��ݏo��*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;/*�o�̓f�[�^*/

 /*�S�����w�֘A*/
 double wh[HIDDENNO][INPUTNO+1] ;/*���ԑw�̏d��*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*�o�͑w�̏d��*/
 double e[MAXNO][INPUTNO+OUTPUTNO] ;/*�w�K�f�[�^�Z�b�g*/
 double hi[HIDDENNO+1] ;/*���ԑw�̏o��*/
 double o[OUTPUTNO]  ;/*�o��*/
 double err=BIGNUM ;/*�덷�̕]��*/
 int i,j,m,n ;/*�J��Ԃ��̐���*/
 int n_of_e ;/*�w�K�f�[�^�̌�*/
 int count=0 ;/*�J��Ԃ��񐔂̃J�E���^*/
 
 /*�����̏�����*/
 srand(SEED) ;
 
 /*��ݍ��݃t�B���^�̏�����*/
 initfilter(filter) ;

 /*�d�݂̏�����*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;


 /*�w�K�f�[�^�̓ǂݍ���*/
 n_of_e=getdata(image,t) ;
 printf("�w�K�f�[�^�̌�:%d\n",n_of_e) ;

 /*��ݍ��ݏ���*/
 for(i=0;i<n_of_e;++i){/*�w�K�f�[�^���̌J��Ԃ�*/
  for(j=0;j<F_NO;++j){/*�t�B���^���̌J��Ԃ�*/
    /*��ݍ��݂̌v�Z*/
    conv(filter[j],image[i],convout) ;
    /*�v�[�����O�̌v�Z*/
    pool(convout,poolout) ;
    /*�v�[�����O�o�͂�S�����w�̓��͂փR�s�[*/
    for(m=0;m<POOLOUTSIZE;++m)
     for(n=0;n<POOLOUTSIZE;++n)
      e[i][j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    for(m=0;m<OUTPUTNO;++m)
     e[i][POOLOUTSIZE*POOLOUTSIZE*F_NO+m]=t[i][m] ;/*���t�f�[�^*/
  }
 }
 
 /*�w�K*/
 while(err>LIMIT){
  /*i�̏o�͑w�ɑΉ�*/
  for(i=0;i<OUTPUTNO;++i){
   err=0.0 ;
   for(j=0;j<n_of_e;++j){
    /*�������̌v�Z*/
    o[i]=forward(wh,wo[i],hi,e[j]) ;
    /*�o�͑w�̏d�݂̒���*/
    olearn(wo[i],hi,e[j],o[i],i) ;
    /*���ԑw�̏d�݂̒���*/
    hlearn(wh,wo[i],hi,e[j],o[i],i) ;
    /*�덷�̐ώZ*/
    err+=(o[i]-e[j][INPUTNO+i])*(o[i]-e[j][INPUTNO+i]) ;
   }
   ++count ;
   /*�덷�̏o��*/
   printf("%d\t%lf\n",count,err) ;
  }
 }/*�w�K�I��*/
 
 /*�d�݂̏o��*/
 printweight(wh,wo) ;
 
 /*�w�K�f�[�^�ɑ΂���o��*/
 for(i=0;i<n_of_e;++i){
  printf("%d\n",i) ;
  for(j=0;j<INPUTNO;++j)
   printf("%lf ",e[i][j]) ;/*�w�K�f�[�^*/
  printf("\n") ;
  for(j=INPUTNO;j<INPUTNO+OUTPUTNO;++j)/*���t�f�[�^*/
   printf("%lf ",e[i][j]) ;
  printf("\n") ;
  for(j=0;j<OUTPUTNO;++j)/*�l�b�g���[�N�o��*/
   printf("%lf ",forward(wh,wo[j],hi,e[i])) ;
  printf("\n") ;
 }
 
 return 0 ;
}

/**********************/
/*  initfilter()�֐�  */
/* �@�t�B���^�̏����� */
/**********************/
void initfilter(double filter[F_NO][F_SIZE][F_SIZE])
{
 int i,j,k ;/*�J��Ԃ��̐���*/
 
 for(i=0;i<F_NO;++i)
  for(j=0;j<F_SIZE;++j)
   for(k=0;k<F_SIZE;++k)
    filter[i][j][k]=drand() ;
} 

/**********************/
/*    initwh()�֐�    */
/*���ԑw�̏d�݂̏�����*/
/**********************/
void initwh(double wh[HIDDENNO][INPUTNO+1])
{
 int i,j ;/*�J��Ԃ��̐���*/
 
 /*�@�����ɂ��d�݂̏�����*/
 for(i=0;i<HIDDENNO;++i)
  for(j=0;j<INPUTNO+1;++j)
   wh[i][j]=drand() ;
}
 
/**********************/
/*    initwo()�֐�    */
/*�o�͑w�̏d�݂̏�����*/
/**********************/
void initwo(double wo[OUTPUTNO][HIDDENNO+1])
{
 int i,j ;/*�J��Ԃ��̐���*/
 
 /*�@�����ɂ��d�݂̏�����*/
 for(i=0;i<OUTPUTNO;++i)
  for(j=0;j<HIDDENNO+1;++j)
   wo[i][j]=drand() ;
}
 
/**********************/
/*  getdata()�֐�     */
/*���̓f�[�^�̓ǂݍ���*/
/**********************/
int getdata(double image[][IMAGESIZE][IMAGESIZE]
                  ,double t[][OUTPUTNO])
{
 int i=0,j=0,k=0 ;/*�J��Ԃ��̐���p*/

 /*�f�[�^�̓���*/
 while(scanf("%lf",&t[i][j])!=EOF){
  /*���t�f�[�^�̓ǂݍ���*/
  ++j ;
  while(scanf("%lf",&t[i][j])!=EOF){
   ++j ;
   if(j>=OUTPUTNO) break ;
  }

  /*�摜�f�[�^�̓ǂݍ���*/
  j=0 ;
  while(scanf("%lf",&image[i][j][k])!=EOF){
   ++ k ;
   if(k>=IMAGESIZE){/*���̃f�[�^*/
    k=0 ;
    ++j ;
    if(j>=IMAGESIZE) break ;/*���͏I��*/
   }
  }
  j=0; k=0 ;
  ++i ; 
 }
 return i ;
}

/**********************/
/*  conv()�֐�        */
/*  ��ݍ��݂̌v�Z    */
/**********************/
void conv(double filter[][F_SIZE]
         ,double e[][IMAGESIZE],double convout[][IMAGESIZE])
{
 int i=0,j=0 ;/*�J��Ԃ��̐���p*/
 int startpoint=F_SIZE/2 ;/*��ݍ��ݔ͈͂̉���*/

 for(i=startpoint;i<IMAGESIZE-startpoint;++i)
  for(j=startpoint;j<IMAGESIZE-startpoint;++j)
   convout[i][j]=calcconv(filter,e,i,j) ;
}

/**********************/
/*  calcconv()�֐�    */
/*  �t�B���^�̓K�p    */
/**********************/
double calcconv(double filter[][F_SIZE]
             ,double e[][IMAGESIZE],int i,int j)
{
 int m,n ;/*�J��Ԃ��̐���p*/
 double sum=0 ;/*�a�̒l*/
 
 for(m=0;m<F_SIZE;++m)
  for(n=0;n<F_SIZE;++n)
   sum+=e[i-F_SIZE/2+m][j-F_SIZE/2+n]*filter[m][n];
   
 return sum ;
}

/**********************/
/*  pool()�֐�        */
/* �v�[�����O�̌v�Z   */
/**********************/
void pool(double convout[][IMAGESIZE]
         ,double poolout[][POOLOUTSIZE]) 
{
 int i,j ;/*�J��Ԃ��̐���*/

 for(i=0;i<POOLOUTSIZE;++i)
  for(j=0;j<POOLOUTSIZE;++j)
   poolout[i][j]=calcpooling(convout,i*2+1,j*2+1) ;
}
 
/**********************/
/* calcpooling()�֐�  */
/* ���ϒl�v�[�����O   */
/**********************/
double calcpooling(double convout[][IMAGESIZE]
                 ,int x,int y)
{
 int m,n ;/*�J��Ԃ��̐���p*/
 double ave=0.0 ;/*���ϒl*/
 
 for(m=x;m<=x+1;++m)
  for(n=y;n<=y+1;++n)
   ave+=convout[m][n] ;

 return ave/4.0 ;
}
               
/**********************/
/*  forward()�֐�     */
/*  �������̌v�Z      */
/**********************/
double forward(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[])
{
 int i,j ;/*�J��Ԃ��̐���*/
 double u ;/*�d�ݕt���a�̌v�Z*/
 double o ;/*�o�͂̌v�Z*/
 
 /*hi�̌v�Z*/
 for(i=0;i<HIDDENNO;++i){
  u=0 ;/*�d�ݕt���a�����߂�*/
  for(j=0;j<INPUTNO;++j)
   u+=e[j]*wh[i][j] ;
  u-=wh[i][j] ;/*�������l�̏���*/
  hi[i]=s(u) ;
 }
 /*�o��o�̌v�Z*/
 o=0 ;
 for(i=0;i<HIDDENNO;++i)
  o+=hi[i]*wo[i] ;
 o-=wo[i] ;/*�������l�̏���*/
 
 return s(o) ;
}
 
/**********************/
/*  olearn()�֐�      */
/*  �o�͑w�̏d�݊w�K  */
/**********************/
void olearn(double wo[HIDDENNO+1]
    ,double hi[],double e[],double o,int k)
{
 int i ;/*�J��Ԃ��̐���*/
 double d ;/*�d�݌v�Z�ɗ��p*/
 
 d=(e[INPUTNO+k]-o)*o*(1-o) ;/*�덷�̌v�Z*/
 for(i=0;i<HIDDENNO;++i){
  wo[i]+=ALPHA*hi[i]*d ;/*�d�݂̊w�K*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;/*�������l�̊w�K*/
}
 
/**********************/
/*  hlearn()�֐�      */
/*  ���ԑw�̏d�݊w�K  */
/**********************/
void hlearn(double wh[HIDDENNO][INPUTNO+1],double wo[HIDDENNO+1]
                ,double hi[],double e[],double o,int k)
{
 int i,j ;/*�J��Ԃ��̐���*/
 double dj ;/*���ԑw�̏d�݌v�Z�ɗ��p*/
 
 for(j=0;j<HIDDENNO;++j){/*���ԑw�̊e�Z��j��Ώ�*/
  dj=hi[j]*(1-hi[j])*wo[j]*(e[INPUTNO+k]-o)*o*(1-o) ;
  for(i=0;i<INPUTNO;++i)/*i�Ԗڂ̏d�݂�����*/
   wh[j][i]+=ALPHA*e[i]*dj ;
  wh[j][i]+=ALPHA*(-1.0)*dj ;/*�������l�̊w�K*/
 }
}
 
/**********************/
/*  printweight()�֐� */
/*   ���ʂ̏o��       */
/**********************/
void printweight(double wh[HIDDENNO][INPUTNO+1]
                ,double wo[OUTPUTNO][HIDDENNO+1])
{
 int i,j ;/*�J��Ԃ��̐���*/
 
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
/* s()�֐�         */
/* �V�O���C�h�֐�  */
/*******************/
double s(double u)
{
 return 1.0/(1.0+exp(-u)) ;
}
 
/*************************/
/* drand()�֐�           */
/*-1����1�̊Ԃ̗����𐶐�*/
/*************************/
double drand(void)
{
 double rndno ;/*������������*/
 
 while((rndno=(double)rand()/RAND_MAX)==1.0) ;
 rndno=rndno*2-1 ;/*-1����1�̊Ԃ̗����𐶐�*/
 return rndno;
}

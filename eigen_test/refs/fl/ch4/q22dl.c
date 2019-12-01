/********************************************/
/*         q22dl.c                          */
/*   �����w�K��NN�̗��v���O�����@         */
/*�@ q22.c�̔��W��                          */
/*�g����                                    */
/* C:\Users\odaka\ch4>q22dl                 */
/********************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* �L���萔�̒�`             */
/*�����w�K�֘A*/
#define GENMAX  100000 /*�w�K�̌J��Ԃ���*/
#define STATENO  64  /*��Ԃ̐�*/
#define ACTIONNO 4  /*�s���̐�*/
#define ALPHA 0.1/*�w�K�W��*/
#define GAMMA 0.9/*������*/
#define EPSILON 0.3 /*�s���I���̃����_����������*/
//#define SEED 65535 /*�����̃V�[�h*/
#define SEED 32767 /*�����̃V�[�h*/
#define REWARD 1 /*�S�[�����B���̕�V*/
#define GOAL 54 /*���54���S�[�����*/
#define UP 0/*������̍s��*/
#define DOWN 1/*�������̍s��*/
#define LEFT 2/*�������̍s��*/
#define RIGHT 3/*�E�����̍s��*/
#define LEVEL 512 /*1���s�ɂ�����ő�X�e�b�v��*/
/*�j���[�����l�b�g�֘A*/
/*��ݍ��݉��Z�֘A*/
#define IMAGESIZE 8 /*���͉摜��1�ӂ̃s�N�Z����*/
#define F_SIZE 3 /*��ݍ��݃t�B���^�̃T�C�Y*/
#define F_NO 2 /*�t�B���^�̌�*/
#define POOLOUTSIZE 3 /*�v�[�����O�w�̏o�͂̃T�C�Y*/
/*�S�������֘A*/
#define INPUTNO 18   /*���͑w�̃Z����*/
#define HIDDENNO 6  /*���ԑw�̃Z����*/
#define OUTPUTNO 4  /*�o�͑w�̃Z����*/
#define NNALPHA  1    /*�w�K�W��*/

/* �֐��̃v���g�^�C�v�̐錾   */
/*�����w�K�֘A*/
int rand03() ;/*0�`3�̒l��Ԃ������֐�*/
double frand() ;/*0�`1�̎�����Ԃ������֐�*/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q�l�o��*/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*�s���I��*/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q�l�X�V*/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
    ,double filter[F_NO][F_SIZE][F_SIZE]);/*Q�l�ő�l��I��*/
int nexts(int s,int a) ;/*�s���ɂ���Ď��̏�ԂɑJ��*/
double calcqvalue(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[],int s,int a) ;
                                               /*Q�l�̌v�Z */

/*�j���[�����l�b�g�֘A*/
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

void set_e_by_s(int s,double filter[F_NO][F_SIZE][F_SIZE]
                ,double e[]) ;/*��ݍ��݂�p����NN���̓f�[�^�̐ݒ�  */

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
 
/****************/
/*  main()�֐�  */
/****************/
int main()
{
 /*�����w�K�֘A*/
 int i;
 int s,snext;/*���݂̏�ԂƁA���̏��*/
 int t;/*����*/
 int a;/*�s��*/
 /*�j���[�����l�b�g�֘A*/
 /*��ݍ��݉��Z�֘A*/
 double filter[F_NO][F_SIZE][F_SIZE] ;/*��ݍ��݃t�B���^*/

 /*�S�����w�֘A*/
 double wh[HIDDENNO][INPUTNO+1] ;/*���ԑw�̏d��*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*�o�͑w�̏d��*/
 double e[INPUTNO+OUTPUTNO] ;/*�w�K�f�[�^�Z�b�g*/
 double hi[HIDDENNO+1] ;/*���ԑw�̏o��*/
 double o[OUTPUTNO]  ;/*�o��*/
 int count=0 ;/*�J��Ԃ��񐔂̃J�E���^*/

 /*�����̏�����*/
 srand(SEED);

 /*��ݍ��݃t�B���^�̏�����*/
 initfilter(filter) ;

 /*�d�݂̏�����*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;
 
 /*�w�K�̖{��*/
 for(i=0;i<GENMAX;++i){
  if(i%1000==0) fprintf(stderr,"%d000 step\n",i/1000) ;
  s=0;/*�s���̏������*/
  for(t=0;t<LEVEL;++t){/*�ő�X�e�b�v���܂ŌJ��Ԃ�*/
   /*�s���I��*/
   a=selecta(s,wh,wo,hi,filter) ;
   fprintf(stdout," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q�l�̍X�V*/
   /*�l�b�g���[�N���̓f�[�^e�̐ݒ�*/
   set_e_by_s(s,filter,e) ;
   e[INPUTNO+a]=updateq(s,snext,a,wh,wo,hi,filter) ;/*�s��*/
   /*�������̌v�Z*/
   o[a]=forward(wh,wo[a],hi,e) ;
   /*�o�͑w�̏d�݂̒���*/
   olearn(wo[a],hi,e,o[a],a) ;
   /*���ԑw�̏d�݂̒���*/
   hlearn(wh,wo[a],hi,e,o[a],a) ;
   /*�s��a�ɂ���Ď��̏��snext�ɑJ��*/
   s=snext ;
   /*�S�[���ɓ��B�����珉����Ԃɖ߂�*/
   if(s==GOAL) break ;
  }
  /*Q�l�̏o��*/
  printqvalue(wh,wo,hi,filter) ;
 }
 return 0;
}

/******************/
/*�@�������֐��@�@*/
/*  �����w�K�֘A  */
/******************/

/******************/
/*calcqvalue()�֐�*/
/*Q�l�̌v�Z       */
/******************/
double calcqvalue(double wh[HIDDENNO][INPUTNO+1]
 ,double wo[HIDDENNO+1],double hi[],double e[],int s,int a)
{
 
 /*�ړ��ł��Ȃ������ւ�Q�l��0�ɂ���*/
 if((s<=7)&&(a==UP)) return 0 ;/*�ŏ�i�ł�UP�����ɐi�܂Ȃ�*/
 if((s>=56)&&(a==DOWN)) return 0 ;/*�ŉ��i�ł�DOWN�����ɐi�܂Ȃ�*/  
 if((s%8==0)&&(a==LEFT)) return 0 ;/*���[�ł�LEFT�����ɐi�܂Ȃ�*/
 if((s%8==7)&&(a==RIGHT)) return 0 ;/*�E�[�ł�RIGHT�����ɐi�܂Ȃ�*/

 /*�ړ��ł���ꍇ��Q�l*/
 return forward(wh,wo,hi,e) ;
}

/****************************/
/*       updateq()�֐�      */
/*�@     Q�l���X�V����      */
/****************************/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
               ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
               ,double filter[F_NO][F_SIZE][F_SIZE])
{
 double qv ;/*�X�V�����Q�l*/
 double qvalue_sa ;/*���݂�Q�l*/
 double qvalue_snexta ;/*���̏�Ԃł̍ő�Q�l*/
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*���ݏ��s�ł�Q�l�����߂�*/
 /*�l�b�g���[�N���̓f�[�^e�̐ݒ�*/
 set_e_by_s(s,filter,e) ;
 qvalue_sa=calcqvalue(wh,wo[a],hi,e,s,a) ;/*�s��a*/

 /*���̏��snext�ł̍ő�Q�l�����߂�*/
 /*�l�b�g���[�N���̓f�[�^e�̐ݒ�*/
 set_e_by_s(snext,filter,e) ;
 qvalue_snexta=calcqvalue(wh,wo[set_a_by_q(snext,wh,wo,hi,filter)]
                          ,hi,e,snext,set_a_by_q(snext,wh,wo,hi,filter)) ;

 /*Q�l�̍X�V*/
 if(snext==GOAL)/*��V���t�^�����ꍇ*/
   qv=qvalue_sa+ALPHA*(REWARD-qvalue_sa) ;
 else/*��V�Ȃ�*/
   qv=qvalue_sa
     +ALPHA*(GAMMA*qvalue_snexta-qvalue_sa) ;

 return qv ;
}

/****************************/
/*        selecta()�֐�     */
/*�@     �s����I������     */
/****************************/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
             ,double filter[F_NO][F_SIZE][F_SIZE])
{
 int a ;/*�I�����ꂽ�s��*/
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*�j���[�����l�b�g�ւ̓��͐ݒ�*/
 set_e_by_s(s,filter,e) ;
 /*��-greedy�@�ɂ��s���I��*/
 if(frand()<EPSILON){
  /*�����_���ɍs��*/
  do{
   a=rand03() ;
  }while(calcqvalue(wh,wo[a],hi,e,s,a)==0) ;/*�ړ��ł��Ȃ������Ȃ��蒼��*/
 }
 else{
  /*�p�l�ő�l��I��*/
  a=set_a_by_q(s,wh,wo,hi,filter) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()�֐�      */
/*�@  �p�l�ő�l��I��      */
/****************************/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]
             ,double filter[F_NO][F_SIZE][F_SIZE])
{
 double maxq=0 ;/*Q�l�̍ő�l���*/
 int maxaction=0 ;/*Q�l�ő�ɑΉ�����s��*/
 int i ;
 double e[INPUTNO+OUTPUTNO]={0}  ;

 /*�l�b�g���[�N���̓f�[�^e�̐ݒ�*/
 set_e_by_s(s,filter,e) ;
 for(i=0;i<ACTIONNO;++i)
  if(calcqvalue(wh,wo[i],hi,e,s,i)>maxq){

   maxq=calcqvalue(wh,wo[i],hi,e,s,i) ;/*�ő�l�̍X�V*/
   maxaction=i ;/*�Ή�����s��*/
  }

 return maxaction ;

}

/****************************/
/*    nexts()�֐�           */
/*�s���ɂ���Ď��̏�ԂɑJ��*/
/****************************/
int nexts(int s,int a)
{
 int next_s_value[]={-8,8,-1,1} ;
      /*�s��a�ɑΉ����Ď��̏�ԂɑJ�ڂ��邽�߂̉��Z�l*/

 return s+next_s_value[a] ;
}

/****************************/
/*    printqvalue()�֐�     */
/*�@   Q�l���o�͂���        */
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
/*     frand()�֐�          */
/*0�`1�̎�����Ԃ������֐�  */
/****************************/
double frand()
{
 /*�����̌v�Z*/
 return (double)rand()/RAND_MAX ;
}

/****************************/
/*     rand03()�֐�         */
/*�@ 0�`3�̒l��Ԃ������֐� */
/****************************/
int rand03()
{
 int rnd ;
 
 /*�����̍ő�l������*/
 while((rnd=rand())==RAND_MAX) ;
 /*�����̌v�Z*/
 return (int)((double)rnd/RAND_MAX*4) ;
}

/**************************/
/*�@�������֐��@�@        */
/*  �j���[�����l�b�g�֘A  */
/**************************/
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
  wo[i]+=NNALPHA*hi[i]*d ;/*�d�݂̊w�K*/
 }
 wo[i]+=NNALPHA*(-1.0)*d ;/*�������l�̊w�K*/
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
   wh[j][i]+=NNALPHA*e[i]*dj ;
  wh[j][i]+=NNALPHA*(-1.0)*dj ;/*�������l�̊w�K*/
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
 
/************************************/
/* set_e_by_s()�֐�                 */
/* ��ݍ��݂�p����NN���̓f�[�^�̐ݒ�  */
/************************************/
void set_e_by_s(int s,double filter[F_NO][F_SIZE][F_SIZE]
               ,double e[])
{
 int i,j,m,n ;/*�J��Ԃ��̐���p*/
 double image[IMAGESIZE][IMAGESIZE] ;/*���̓f�[�^*/
 double convout[IMAGESIZE][IMAGESIZE] ;/*��ݍ��ݏo��*/
 double poolout[POOLOUTSIZE][POOLOUTSIZE] ;/*�o�̓f�[�^*/
 
 /*��ݍ��ݕ��ւ̓��͂̐ݒ�*/
 for(i=0;i<IMAGESIZE;++i)
  for(j=0;j<IMAGESIZE;++j){
   if((i+j*IMAGESIZE)==s) image[i][j]=1 ;
   else image[i][j]=0 ;
  }

 for(j=0;j<F_NO;++j){/*�t�B���^���̌J��Ԃ�*/
  /*��ݍ��݂̌v�Z*/
  conv(filter[j],image,convout) ;
  /*�v�[�����O�̌v�Z*/
  pool(convout,poolout) ;
  /*�v�[�����O�o�͂�S�����w�̓��͂փR�s�[*/
  for(m=0;m<POOLOUTSIZE;++m)
   for(n=0;n<POOLOUTSIZE;++n)
    e[j*POOLOUTSIZE*POOLOUTSIZE+POOLOUTSIZE*m+n]
           =poolout[m][n] ;
    for(m=0;m<OUTPUTNO;++m)
     e[POOLOUTSIZE*POOLOUTSIZE*F_NO+m]=0 ;/*���t�f�[�^���̃N���A*/
 }
}
  
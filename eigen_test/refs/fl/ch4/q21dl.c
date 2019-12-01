/********************************************/
/*         q21dl.c                          */
/*   �����w�K��NN�̗��v���O�����@         */
/*�@ q21.c�̔��W��                          */
/*�g����                                    */
/* C:\Users\odaka\ch4>q21dl                 */
/********************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* �L���萔�̒�`             */
/*�����w�K�֘A*/
#define GENMAX  4000 /*�w�K�̌J��Ԃ���*/
#define STATENO  7  /*��Ԃ̐�*/
#define ACTIONNO 2  /*�s���̐�*/
#define ALPHA 0.1/*�w�K�W��*/
#define GAMMA 0.9/*������*/
#define EPSILON 0.3 /*�s���I���̃����_����������*/
#define SEED 65535 /*�����̃V�[�h*/
#define REWARD 1 /*�S�[�����B���̕�V*/
#define GOAL 6 /*��ԂU���S�[�����*/
#define UP 0/*������̍s��*/
#define DOWN 1/*�������̍s��*/
#define LEVEL 2 /*�}������̐[��*/
/*�j���[�����l�b�g�֘A*/
#define INPUTNO 7   /*���͑w�̃Z����*/
#define HIDDENNO 2  /*���ԑw�̃Z����*/
#define OUTPUTNO 2  /*�o�͑w�̃Z����*/
#define NNALPHA  3    /*�w�K�W��*/

/* �֐��̃v���g�^�C�v�̐錾   */
/*�����w�K�֘A*/
int rand0or1() ;/*0����1��Ԃ������֐�*/
double frand() ;/*0�`1�̎�����Ԃ������֐�*/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q�l�o��*/
int selecta(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*�s���I��*/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q�l�X�V*/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
    ,double wo[OUTPUTNO][HIDDENNO+1],double hi[]);/*Q�l�ő�l��I��*/
int nexts(int s,int a) ;/*�s���ɂ���Ď��̏�ԂɑJ��*/
/*�j���[�����l�b�g�֘A*/
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
 double wh[HIDDENNO][INPUTNO+1] ;/*���ԑw�̏d��*/
 double wo[OUTPUTNO][HIDDENNO+1] ;/*�o�͑w�̏d��*/
 double e[INPUTNO+OUTPUTNO] ;/*�w�K�f�[�^�Z�b�g*/
 double hi[HIDDENNO+1] ;/*���ԑw�̏o��*/
 double o[OUTPUTNO]  ;/*�o��*/
 int i_n ;/*�J��Ԃ��̐���*/
 int count=0 ;/*�J��Ԃ��񐔂̃J�E���^*/

 /*�����̏�����*/
 srand(SEED);

 /*�d�݂̏�����*/
 initwh(wh) ;
 initwo(wo) ;
 printweight(wh,wo) ;
 
 /*�w�K�̖{��*/
 for(i=0;i<GENMAX;++i){
  s=0;/*�s���̏������*/
  for(t=0;t<LEVEL;++t){/*�ŉ��i�܂ŌJ��Ԃ�*/
   /*�s���I��*/
   a=selecta(s,wh,wo,hi) ;
   fprintf(stderr," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q�l�̍X�V*/
   /*�l�b�g���[�N���̓f�[�^e�̐ݒ�*/
   for(i_n=0;i_n<INPUTNO+OUTPUTNO;++i_n)
    e[i_n]=0 ;
   e[s]=1 ;/*���݂̏��*/
   e[INPUTNO+a]=updateq(s,snext,a,wh,wo,hi) ;/*�s��*/
   /*�������̌v�Z*/
   o[a]=forward(wh,wo[a],hi,e) ;
   /*�o�͑w�̏d�݂̒���*/
   olearn(wo[a],hi,e,o[a],a) ;
   /*���ԑw�̏d�݂̒���*/
   hlearn(wh,wo[a],hi,e,o[a],a) ;
   /*�s��a�ɂ���Ď��̏��snext�ɑJ��*/
   s=snext ;
  }
  /*Q�l�̏o��*/
  printqvalue(wh,wo,hi) ;
 }
 return 0;
}

/******************/
/*�@�������֐��@�@*/
/*  �����w�K�֘A  */
/******************/

/****************************/
/*       updateq()�֐�      */
/*�@     Q�l���X�V����      */
/****************************/
double updateq(int s,int snext,int a,double wh[HIDDENNO][INPUTNO+1]
                       ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 double qv ;/*�X�V�����Q�l*/
 double qvalue_sa ;/*���݂�Q�l*/
 double qvalue_snexta ;/*���̏�Ԃł̍ő�Q�l*/
 double e[INPUTNO+1]={0}  ;

 /*���ݏ��s�ł�Q�l�����߂�*/
 e[s]=1 ;/*�j���[�����l�b�g�ւ̓��͐ݒ�*/
 qvalue_sa=forward(wh,wo[a],hi,e) ;/*�s��a*/ 
 e[s]=0 ;/*���͂̃N���A*/

 /*���̏��snext�ł̍ő�Q�l�����߂�*/
 e[snext]=1 ;/*�j���[�����l�b�g�ւ̓��͐ݒ�*/
 qvalue_snexta=forward(wh,wo[set_a_by_q(snext,wh,wo,hi)],hi,e) ;

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
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 int a ;/*�I�����ꂽ�s��*/
 
 /*��-greedy�@�ɂ��s���I��*/
 if(frand()<EPSILON){
  /*�����_���ɍs��*/
  a=rand0or1();
 }
 else{
  /*�p�l�ő�l��I��*/
  a=set_a_by_q(s,wh,wo,hi) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()�֐�      */
/*�@  �p�l�ő�l��I��      */
/****************************/
int set_a_by_q(int s,double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 double upqvalue,downqvalue ;
 double e[INPUTNO+1]={0}  ;

 /*Q�l�̌v�Z*/
 e[s]=1 ;/*�j���[�����l�b�g�ւ̓��͐ݒ�*/
 upqvalue=forward(wh,wo[UP],hi,e) ;/*UP*/
 downqvalue=forward(wh,wo[DOWN],hi,e) ;/*DOWN*/
 /*Q�l�ɂ�锻��*/
 if((upqvalue)>(downqvalue)) 
  return UP ;
 else return DOWN;
}

/****************************/
/*    nexts()�֐�           */
/*�s���ɂ���Ď��̏�ԂɑJ��*/
/****************************/
int nexts(int s,int a)
{
 return s*2+1+a ;
}

/****************************/
/*    printqvalue()�֐�     */
/*�@   Q�l���o�͂���        */
/****************************/
void printqvalue(double wh[HIDDENNO][INPUTNO+1]
             ,double wo[OUTPUTNO][HIDDENNO+1],double hi[])
{
 int i,j ;
 double e[INPUTNO+1]={0}  ;

 for(i=0;i<STATENO;++i){
  for(j=0;j<ACTIONNO;++j){
   e[i]=1 ;/*�o�͂��ׂ���Ԃ̔ԍ�*/
   printf("%.3lf ",forward(wh,wo[j],hi,e));
   e[i]=0 ;/*���ɖ߂�*/
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
/*     rand0or1()�֐�         */
/*�@   0����1��Ԃ������֐� */
/****************************/
int rand0or1()
{
 int rnd ;
 
 /*�����̍ő�l������*/
 while((rnd=rand())==RAND_MAX) ;
 /*�����̌v�Z*/ 
 return (int)((double)rnd/RAND_MAX*2) ;
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

/*********************************************************/
/*                       nn1.c                           */
/*�@�o�͑w�j���[�����̊w�K �@�@�@�@�@�@�@�@�@            */
/* �g�����@                                            �@*/
/* C:\Users\odaka\ch3>nn1< (�w�K�f�[�^�Z�b�g�̃t�@�C����)*/
/*�@�덷�̐��ڂ�C�w�K���ʂƂȂ錋���W���Ȃǂ��o�͂��܂� */
/*********************************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/* �w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*�L���萔�̒�`*/
#define INPUTNO 2   /*���͐�*/
#define ALPHA  1    /*�w�K�W��*/
#define MAXNO 100   /*�w�K�f�[�^�̍ő��*/
#define BIGNUM 100  /*�덷�̏����l*/
#define LIMIT 0.001 /*�덷�̏���l*/
#define SEED 65535  /*�����̃V�[�h*/
//#define SEED 32767  /*�����̃V�[�h*/

/*�֐��̃v���g�^�C�v�̐錾*/
void initwo(double wo[INPUTNO+1]) ;/*�o�͑w�̏d�݂̏�����*/
int getdata(double e[][INPUTNO+1]) ; /*�w�K�f�[�^�̓ǂݍ���*/
double forward(double wo[INPUTNO+1],double e[INPUTNO+1]) ;
                                     /*�������̌v�Z*/
void olearn(double wo[INPUTNO+1],double e[INPUTNO+1],
                         double o) ; /*�o�͑w�̏d�݂̊w�K*/
void printweight(double wo[INPUTNO+1]) ; /*���ʂ̏o��*/
double s(double u) ; /*�V�O���C�h�֐�*/
double drand(void) ;/*-1����1�̊Ԃ̗����𐶐� */

/*******************/
/*    main()�֐�   */
/*******************/
int main()
{
 double wo[INPUTNO+1] ;/*�o�͑w�̏d��*/
 double e[MAXNO][INPUTNO+1] ;/*�w�K�f�[�^�Z�b�g*/
 double o ;/*�o��*/
 double err=BIGNUM ;/*�덷�̕]��*/
 int i,j ;/*�J��Ԃ��̐���*/
 int n_of_e ;/*�w�K�f�[�^�̌�*/
 int count=0 ;/*�J��Ԃ��񐔂̃J�E���^*/

 /*�����̏�����*/
 srand(SEED) ;

 /*�d�݂̏�����*/
 initwo(wo) ;
 printweight(wo) ;

 /*�w�K�f�[�^�̓ǂݍ���*/
 n_of_e=getdata(e) ;
 printf("�w�K�f�[�^�̌�:%d\n",n_of_e) ;

 /*�w�K*/
 while(err>LIMIT){
  err=0.0 ;
  for(j=0;j<n_of_e;++j){
   /*�������̌v�Z*/
   o=forward(wo,e[j]) ;
   /*�o�͑w�̏d�݂̒���*/
   olearn(wo,e[j],o) ;
   /*�덷�̐ώZ*/
   err+=(o-e[j][INPUTNO])*(o-e[j][INPUTNO]) ;
  }
  ++count ;
  /*�덷�̏o��*/
  printf("%d\t%lf\n",count,err) ;
 }/*�w�K�I��*/

 /*�d�݂̏o��*/
 printweight(wo) ;

 /*�w�K�f�[�^�ɑ΂���o��*/
 for(i=0;i<n_of_e;++i){
  printf("%d ",i) ;
  for(j=0;j<INPUTNO+1;++j)
   printf("%lf ",e[i][j]) ;
  o=forward(wo,e[i]) ;
  printf("%lf\n",o) ;
 }

 return 0 ;
}

/**********************/
/*    initwo()�֐�    */
/*�o�͑w�̏d�݂̏�����*/
/**********************/
void initwo(double wo[INPUTNO+1])
{
 int i ;/*�J��Ԃ��̐���*/

 for(i=0;i<INPUTNO+1;++i)
   wo[i]=drand() ;
}

/**********************/
/*  getdata()�֐�     */
/*�w�K�f�[�^�̓ǂݍ���*/
/**********************/
int getdata(double e[][INPUTNO+1])
{
 int n_of_e=0 ;/*�f�[�^�Z�b�g�̌�*/
 int j=0 ;/*�J��Ԃ��̐���p*/

 /*�f�[�^�̓���*/
 while(scanf("%lf",&e[n_of_e][j])!=EOF){
  ++ j ;
  if(j>INPUTNO){/*���̃f�[�^*/
   j=0 ;
   ++n_of_e ;
   if(n_of_e>=MAXNO){/*���͐�������ɒB����*/
    fprintf(stderr,"���͐�������ɒB���܂���\n") ;
    break ;
   }
  }
 }
 return n_of_e ;
}

/**********************/
/*  forward()�֐�     */
/*  �������̌v�Z      */
/**********************/
double forward(double wo[INPUTNO+1],double e[INPUTNO+1])
{
 int i ;/*�J��Ԃ��̐���*/
 double o ;/*�o�͂̌v�Z*/

 /*�o��o�̌v�Z*/
 o=0 ;
 for(i=0;i<INPUTNO;++i)
  o+=e[i]*wo[i] ;
 o-=wo[i] ;/*�������l�̏���*/

 return s(o) ;
}

/**********************/
/*  olearn()�֐�      */
/*  �o�͑w�̏d�݊w�K  */
/**********************/
void olearn(double wo[INPUTNO+1],double e[INPUTNO+1],double o)
{
 int i ;/*�J��Ԃ��̐���*/
 double d ;/*�d�݌v�Z�ɗ��p*/

 d=(e[INPUTNO]-o)*o*(1-o) ;/*�덷�̌v�Z*/
 for(i=0;i<INPUTNO;++i){
  wo[i]+=ALPHA*e[i]*d ;/*�d�݂̊w�K*/
 }
 wo[i]+=ALPHA*(-1.0)*d ;/*�������l�̊w�K*/
}

/**********************/
/*  printweight()�֐� */
/*   ���ʂ̏o��       */
/**********************/
void printweight(double wo[INPUTNO+1])
{
 int i ;/*�J��Ԃ��̐���*/

 for(i=0;i<INPUTNO+1;++i)
  printf("%lf ",wo[i]) ;
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



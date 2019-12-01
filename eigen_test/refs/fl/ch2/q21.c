/********************************************/
/*         q21.c                            */
/*   �����w�K(Q�w�K)�̗��v���O�����@���̂P*/
/*�@ �P���ȗ��ł�                         */
/*�g����                                    */
/* C:\Users\odaka\ch2>q21                   */
/********************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>

/* �L���萔�̒�`             */
#define GENMAX  50 /*�w�K�̌J��Ԃ���*/
#define STATENO  7  /*��Ԃ̐�*/
#define ACTIONNO 2  /*�s���̐�*/
#define ALPHA 0.1/*�w�K�W��*/
#define GAMMA 0.9/*������*/
#define EPSILON 0.3 /*�s���I���̃����_����������*/
#define SEED 32767 /*�����̃V�[�h*/
#define REWARD 10 /*�S�[�����B���̕�V*/

#define GOAL 6/*��ԂU���S�[�����*/
#define UP 0/*������̍s��*/
#define DOWN 1/*�������̍s��*/
#define LEVEL 2 /*�}������̐[��*/

/* �֐��̃v���g�^�C�v�̐錾   */
int rand0or1() ;/*0����1��Ԃ������֐�*/
double frand() ;/*0�`1�̎�����Ԃ������֐�*/
void printqvalue(double qvalue[][ACTIONNO]);/*Q�l�o��*/
int selecta(int s,double qvalue[][ACTIONNO]);/*�s���I��*/
double updateq(int s,int snext,int a,double qvalue[][ACTIONNO]);/*Q�l�X�V*/
int set_a_by_q(int s,double qvalue[][ACTIONNO]) ;/*Q�l�ő�l��I��*/
int nexts(int s,int a) ;/*�s���ɂ���Ď��̏�ԂɑJ��*/

/****************/
/*  main()�֐�  */
/****************/
int main()
{
 int i,j;
 int s,snext;/*���݂̏�ԂƁA���̏��*/
 int t;/*����*/
 int a;/*�s��*/
 double qvalue[STATENO][ACTIONNO] ;/*Q�l*/

 srand(SEED);/*�����̏�����*/

 /*�p�l�̏�����*/
 for(i=0;i<STATENO;++i)
  for(j=0;j<ACTIONNO;++j)
   qvalue[i][j]=frand() ;
 printqvalue(qvalue) ;

 /*�w�K�̖{��*/
 for(i=0;i<GENMAX;++i){
  s=0;/*�s���̏������*/
  for(t=0;t<LEVEL;++t){/*�ŉ��i�܂ŌJ��Ԃ�*/
   /*�s���I��*/
   a=selecta(s,qvalue) ;
   fprintf(stderr," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q�l�̍X�V*/
   qvalue[s][a]=updateq(s,snext,a,qvalue) ;
   /*�s��a�ɂ���Ď��̏��snext�ɑJ��*/
    s=snext ;
  }
  /*Q�l�̏o��*/
  printqvalue(qvalue) ;
 }
 return 0;
}

/****************************/
/*       updateq()�֐�      */
/*�@     Q�l���X�V����      */
/****************************/
double updateq(int s,int snext,int a,double qvalue[][ACTIONNO])
{
 double qv ;/*�X�V�����Q�l*/
 
 /*Q�l�̍X�V*/
 if(snext==GOAL)/*��V���t�^�����ꍇ*/
   qv=qvalue[s][a]+ALPHA*(REWARD-qvalue[s][a]) ;
 else/*��V�Ȃ�*/
   qv=qvalue[s][a]
     +ALPHA*(GAMMA*qvalue[snext][set_a_by_q(snext,qvalue)]-qvalue[s][a]) ;

 return qv ;
}

/****************************/
/*        selecta()�֐�     */
/*�@     �s����I������     */
/****************************/
int selecta(int s,double qvalue[][ACTIONNO])
{
 int a ;/*�I�����ꂽ�s��*/
 
 /*��-greedy�@�ɂ��s���I��*/
 if(frand()<EPSILON){
  /*�����_���ɍs��*/
  a=rand0or1();
 }
 else{
  /*�p�l�ő�l��I��*/
  a=set_a_by_q(s,qvalue) ;
 }

 return a ;
}

/****************************/
/*    set_a_by_q()�֐�      */
/*�@  �p�l�ő�l��I��      */
/****************************/
int set_a_by_q(int s,double qvalue[][ACTIONNO])
{
 if((qvalue[s][UP])>(qvalue[s][DOWN])) 
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
void printqvalue(double qvalue[][ACTIONNO])
{
 int i,j ;
 
 for(i=0;i<STATENO;++i){
  for(j=0;j<ACTIONNO;++j)
   printf("%.3lf ",qvalue[i][j]);
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

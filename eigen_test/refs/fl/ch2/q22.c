/********************************************/
/*         q22.c                            */
/*   �����w�K(Q�w�K)�̗��v���O�����@���̂Q*/
/*�@ �������G�ȗ��ł��@�@�@               */
/*�g����                                    */
/* C:\Users\odaka\ch2>q22                   */
/********************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>

/* �L���萔�̒�`             */
#define GENMAX  100 /*�w�K�̌J��Ԃ���*/
#define STATENO  64  /*��Ԃ̐�*/
#define ACTIONNO 4  /*�s���̐�*/
#define ALPHA 0.1/*�w�K�W��*/
#define GAMMA 0.9/*������*/
#define EPSILON 0.3 /*�s���I���̃����_����������*/
#define SEED 32767 /*�����̃V�[�h*/
#define REWARD 10 /*�S�[�����B���̕�V*/

#define GOAL 54/*���54���S�[�����*/
#define UP 0/*������̍s��*/
#define DOWN 1/*�������̍s��*/
#define LEFT 2/*�������̍s��*/
#define RIGHT 3/*�E�����̍s��*/
#define LEVEL 512 /*1���s�ɂ�����ő�X�e�b�v��*/

/* �֐��̃v���g�^�C�v�̐錾   */
int rand03() ;/*0�`3�̒l��Ԃ������֐�*/
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
  for(j=0;j<ACTIONNO;++j){
   qvalue[i][j]=frand() ;
   if(i<=7) qvalue[i][UP]=0 ;/*�ŏ�i�ł�UP�����ɐi�܂Ȃ�*/
   if(i>=56) qvalue[i][DOWN]=0 ;/*�ŉ��i�ł�DOWN�����ɐi�܂Ȃ�*/
   if(i%8==0) qvalue[i][LEFT]=0 ;/*���[�ł�LEFT�����ɐi�܂Ȃ�*/
   if(i%8==7) qvalue[i][RIGHT]=0 ;/*�E�[�ł�RIGHT�����ɐi�܂Ȃ�*/
  }
 printqvalue(qvalue) ;

 /*�w�K�̖{��*/
 for(i=0;i<GENMAX;++i){
  s=0;/*�s���̏������*/
  for(t=0;t<LEVEL;++t){/*�ő�X�e�b�v���܂ŌJ��Ԃ�*/
   /*�s���I��*/
   a=selecta(s,qvalue) ;
   fprintf(stderr," s= %d a=%d\n",s,a) ;
   snext=nexts(s,a) ;

   /*Q�l�̍X�V*/
   qvalue[s][a]=updateq(s,snext,a,qvalue) ;
   /*�s��a�ɂ���Ď��̏��snext�ɑJ��*/
    s=snext ;
   /*�S�[���ɓ��B�����珉����Ԃɖ߂�*/
   if(s==GOAL) break ;
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
  do
   a=rand03() ;
  while(qvalue[s][a]==0) ;/*�ړ��ł��Ȃ������Ȃ��蒼��*/
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
 double maxq=0 ;/*Q�l�̍ő�l���*/
 int maxaction=0 ;/*Q�l�ő�ɑΉ�����s��*/
 int i ;

 for(i=0;i<ACTIONNO;++i)
  if((qvalue[s][i])>maxq){
   maxq=qvalue[s][i] ;/*�ő�l�̍X�V*/
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
void printqvalue(double qvalue[][ACTIONNO])
{
 int i,j ;
 
 for(i=0;i<STATENO;++i){
  printf("%d:",i) ;/*DEBUG*/
  for(j=0;j<ACTIONNO;++j)
   printf("%.3lf ",qvalue[i][j]);
//  printf("\t") ;
  printf("\n") ;/*DEBUG*/
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

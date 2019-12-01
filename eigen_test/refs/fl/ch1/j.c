/*********************************************/
/*           j.c                             */
/*  ����񂯂���o������w�K����@           */
/* �g����                                    */
/*C:\Users\odaka\ch1>j < text.txt            */
/* text.txt�t�@�C���ɂ́A�ΐ푊��̎���i�[  */
/* 0:�O�[�@�@1:�`���L�@�@�@2:�p�[�@          */
/*********************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>

/* �L���萔�̒�`             */
#define SEED 65535    /*�����̃V�[�h*/
#define GU 0  /*�O�[*/
#define CYOKI 1  /*�`���L*/
#define PA 2  /*�p�[*/
#define WIN 1 /*����*/
#define LOSE -1 /*����*/
#define DRAW 0 /*������*/
#define ALPHA  0.01  /*�w�K�W��*/ 

/* �֐��̃v���g�^�C�v�̐錾   */
int hand(double rate[]) ;/*�����ƈ����𗘗p���Ď�����肷��*/
double frand(void) ;/* 0�`1�̎�������  */

/****************/
/*  main()�֐�  */
/****************/
int main()
{
 int n=0 ;/*�ΐ�񐔂̃J�E���^*/
 int myhand,ohand ;/*�����Ƒ���̎�*/
 double rate[3]={1,1,1} ;/*��̏o������*/
 int gain ;/*���������̌���*/
 int payoffmatrix[3][3]={{DRAW,WIN,LOSE},
                         {LOSE,DRAW,WIN},
                         {WIN,LOSE,DRAW}} ;
           /*�����s��*/

 /*�ΐ�Ɗw�K���J��Ԃ�*/
 while(scanf("%d",&ohand)!=EOF){
  if((ohand<GU)||(ohand>PA)) continue ;/*�s���Ȏ�̓���*/
  myhand=hand(rate) ;/*�o�������Ɋ�Â���̌���*/
  gain=payoffmatrix[myhand][ohand] ;/*���������̔���*/
  printf("%d %d %d   ",myhand,ohand,gain) ;/*���ʏo��*/
  rate[myhand]+=gain*ALPHA*rate[myhand] ;/*�o�������̊w�K*/
  printf("%lf  %lf  %lf\n",
    rate[GU],rate[CYOKI],rate[PA]) ;/*�o�������̏o��*/
 }
  
 return 0;
}


/**********************************/
/*  hand()�֐�                    */
/*�����ƈ����𗘗p���Ď�����肷��*/
/**********************************/
int hand(double rate[])
{
 double gu,cyoki,pa ;/*������肷�邽�߂̒l���i�[*/

 gu=rate[GU]*frand() ;
 cyoki=rate[CYOKI]*frand() ;
 pa=rate[PA]*frand() ;

 if(gu>cyoki){
  if(gu>pa) return GU ;/*gu���傫��*/
   else     return PA ;/*pa���傫��*/
 }else {
  if(cyoki>pa) return CYOKI ;/*cyoki���傫��*/
   else        return PA ;/*pa���傫��*/
 }
}

/*******************/
/* frand()�֐�     */
/* 0�`1�̎�������  */
/*******************/
double frand(void)
{ 
 return (double)rand()/RAND_MAX ; 
}
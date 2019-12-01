/***************************************************/
/*           randhandgen.c                         */
/*  ����΂�̂�����1000�񐶐�����               */
/* �g����                                          */
/*C:\Users\odaka\ch1>randhandgen 1 1 1 >text.txt   */
/* text.txt�t�@�C���ɂ́A������������i�[          */
/* 0:�O�[�@�@1:�`���L�@�@�@2:�p�[�@                */
/***************************************************/

/*Visual Studio�Ƃ̌݊����m�� */
#define _CRT_SECURE_NO_WARNINGS

/*�w�b�_�t�@�C���̃C���N���[�h*/
#include <stdio.h>
#include <stdlib.h>

/* �L���萔�̒�`             */
#define SEED   65535    /*�����̃V�[�h*/
#define LASTNO  1000    /*��̐�����*/
#define GU 0  /*�O�[*/
#define CYOKI 1  /*�`���L*/
#define PA 2  /*�p�[*/

/* �֐��̃v���g�^�C�v�̐錾   */
int hand(double rate[]) ;/*�����ƈ����𗘗p���Ď�����肷��*/
double frand(void) ;/* 0�`1�̎�������  */

/****************/
/*  main()�֐�  */
/****************/
int main(int argc,char *argv[])
{
 int n ;/*�o�͉񐔂̃J�E���^*/
 double rate[3] ;/*��̏o������*/

 /*�����̏�����*/
 srand(SEED) ;

 /*��̐���������ݒ�*/
 if(argc<4){/*���������̎w�肪��������*/
  fprintf(stderr,"�g�����@randhandgen (�O�[�̊����j�i�`���L�̊����j�i�p�[�̊����j\n") ;
  exit(1) ;
 }
 rate[GU]=atof(argv[1]) ;/*�O�[�̊���*/
 rate[CYOKI]=atof(argv[2]) ;/*�`���L�̊���*/
 rate[PA]=atof(argv[3]) ;/*�p�[�̊���*/

 /*�o�͂��J��Ԃ�*/
 for(n=0;n<LASTNO;++n){
  printf("%d\n",hand(rate)) ;
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
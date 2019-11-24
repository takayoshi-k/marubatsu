/*
 * MIT License
 * 
 * Copyright (c) 2019 takayoshi.koizumi@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __MARUBATSU_GAME_STAGE_H__
#define __MARUBATSU_GAME_STAGE_H__

#include <stdio.h>
#include "GameGrid.h"

class GameStage {
  public:
    static const int GRID_WIDTH  = 3;
    static const int GRID_HEIGHT = 3;

    static const int CURSOR_DIRECTION_UP     = 'k';
    static const int CURSOR_DIRECTION_DOWN   = 'j';
    static const int CURSOR_DIRECTION_LEFT   = 'h';
    static const int CURSOR_DIRECTION_RIGHT  = 'l';
    static const int CURSOR_DIRECTION_DECIDE = ' ';

  private:
    GameGrid stage[GRID_HEIGHT][GRID_WIDTH];

    int pos_x, pos_y;

  public:
    GameStage() : pos_x(0), pos_y(0) {};

    bool isInside(int pos_w, int pos_h) {
      return  pos_w >= 0         &&
              pos_w < GRID_WIDTH &&
              pos_h >= 0         &&
              pos_h < GRID_HEIGHT;
    };

    int getGridStatus(int pos_w, int pos_h){
      int ret = GameGrid::GRID_STATUS_ERROR;
      if ( isInside(pos_w, pos_h) ) {
        ret = stage[pos_h][pos_w].getGridStatus();
      }
      return ret;
    };

    bool setGridStatus(int pos_w, int pos_h, int state){
      if ( isInside(pos_w, pos_h) ) {
        return stage[pos_h][pos_w].setGridStatus(state);
      }else{
        return false;
      }
    };

    const char *toStr(int pos_w, int pos_h){
      if ( isInside(pos_w, pos_h) ) {
        return stage[pos_h][pos_w].toStr();
      }else{
        return "";
      }
    };

    const char *posCol(int x, int y){
      return (x==pos_x && y==pos_y) ? "\033[41m" : "";
    };

    void printStage() {
      int i, j;
      printf("\033[2J");    // Clear Screen
      printf("\033[0;0H");  // Move Cursor to 0,0
      for(j=0; j<GRID_HEIGHT; j++){
        for(i=0; i<GRID_WIDTH; i++){
          printf("%s%s\033[49m", posCol(i,j), toStr(i,j));
          if ( i != (GRID_WIDTH-1) ){
            printf("|");
          }
        }
        printf("\n");
        if ( j != (GRID_HEIGHT-1) ){
          for(i=0; i<(GRID_WIDTH*2-1); i++) printf("-");
          printf("\n");
        }
      }
    };

    int checkWinner() {
      // This is just 3x3 field case...
      static const int winning_pattern[][GRID_HEIGHT][GRID_WIDTH] = {
        { {1, 1, 1},
          {0, 0, 0},
          {0, 0, 0} },
        { {0, 0, 0},
          {1, 1, 1},
          {0, 0, 0} },
        { {0, 0, 0},
          {0, 0, 0},
          {1, 1, 1} },
        { {1, 0, 0},
          {1, 0, 0},
          {1, 0, 0} },
        { {0, 1, 0},
          {0, 1, 0},
          {0, 1, 0} },
        { {0, 0, 1},
          {0, 0, 1},
          {0, 0, 1} },
        { {0, 0, 1},
          {0, 1, 0},
          {1, 0, 0} },
        { {1, 0, 0},
          {0, 1, 0},
          {0, 0, 1} }
      };
      #define PTN_SIZE (sizeof(winning_pattern)/sizeof(int[GRID_HEIGHT][GRID_WIDTH]))

      int p, i, j, s;
      bool win;
      for(p=0; p<PTN_SIZE; p++){
        s = -1;
        win = true;
        for(j=0; j<GRID_HEIGHT; j++){
          for(i=0; i<GRID_WIDTH; i++){
            if (s == -1 && winning_pattern[p][j][i]){
              s = getGridStatus(j, i);
            }else if ( s!=-1 && s!=getGridStatus(j,i) && winning_pattern[p][j][i] ){
              win = false;
            }
          }
        }
        if (win && (s!=-1 || s!=GameGrid::GRID_STATUS_NONE) ){
          return s;
        }
      }

      return GameGrid::GRID_STATUS_NONE;
    };

    bool moveCursor(int direction, int current_player){
      bool change_player = false;
      switch(direction){
        case CURSOR_DIRECTION_UP:
          pos_y--; if (pos_y < 0) pos_y = 0;
          break;
        case CURSOR_DIRECTION_DOWN:
          pos_y++; if (pos_y >= GRID_HEIGHT) pos_y = GRID_HEIGHT-1;
          break;
        case CURSOR_DIRECTION_LEFT:
          pos_x--; if (pos_x < 0) pos_x = 0;
          break;
        case CURSOR_DIRECTION_RIGHT:
          pos_x++; if (pos_x >= GRID_WIDTH) pos_x = GRID_WIDTH-1;
          break;
        case CURSOR_DIRECTION_DECIDE:
          if (setGridStatus(pos_x, pos_y, current_player) ) {
            change_player = true;
          }
          break;
        default:
          break;
      }
      return change_player;
    };

    void hideCursor(){
      pos_x = -1;
      pos_y = -1;
    };

    void dispCursor(){
      pos_x = 0;
      pos_y = 0;
    };

};

#endif  // __MARUBATSU_GAME_STAGE_H__

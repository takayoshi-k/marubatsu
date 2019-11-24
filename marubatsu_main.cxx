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

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "GameStage.h"

struct termios oldt, newt;

void init_termios(void)
{
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void final_termios(void)
{
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


int main(void)
{
  int c;
  int player;
  bool playing = true;
  GameStage stage;

  player = GameGrid::GRID_STATUS_MARU;
  init_termios();
  while(playing){
    stage.printStage();
    c = getchar();
    if (stage.moveCursor(c, player)) {
      if( stage.checkWinner() != GameGrid::GRID_STATUS_NONE ){
        stage.printStage();
        printf("Player %s Win!!\n", GameGrid::toStr(player));
        playing = false;
      }else{
        player = (player == GameGrid::GRID_STATUS_MARU) ? GameGrid::GRID_STATUS_BATSU : GameGrid::GRID_STATUS_MARU;
      }
    }else if (c == 'q'){
      playing = false;
    }
  }
  final_termios();

  return 0;
}


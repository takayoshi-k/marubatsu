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

/* Eigen includes */
#define EIGEN_NO_DEBUG // コード内のassertを無効化．
#define EIGEN_DONT_VECTORIZE // SIMDを無効化．
#define EIGEN_DONT_PARALLELIZE // 並列を無効化．
#define EIGEN_MPL2_ONLY // LGPLライセンスのコードを使わない．

#include <Eigen/Core>

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


bool human_input(GameStage *stage, int c, int player)
{
  return stage->handleInput(c, player);
}

bool machine_input(GameStage *stage, int c, int player)
{
  return false;
}

int main(void)
{
  int c;
  int player;
  bool playing = true;
  GameStage stage;

  player = GameGrid::GRID_STATUS_MARU;
  init_termios();
  while(playing)
    {
      stage.printStage();
      c = getchar();
      // if (stage.handleInput(c, player))
      if (human_input(&stage, c, player))
        {
          if( stage.checkWinner() != GameGrid::GRID_STATUS_NONE )
            {
              stage.printStage();
              printf("Player %s Win!!\n", GameGrid::toStr(player));
              playing = false;
            }
          else if( stage.checkDraw() )
            {
              stage.printStage();
              printf("Draw this Game...\n");
              playing = false;
            }
          else
            {
              player = (player == GameGrid::GRID_STATUS_MARU) ?
                                  GameGrid::GRID_STATUS_BATSU : GameGrid::GRID_STATUS_MARU;
            }
        }
      else if (c == 'q')
        {
          playing = false;
        }
    }
  final_termios();

  return 0;
}


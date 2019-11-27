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

#ifndef __MARUBATSU_GAME_GRID_H__
#define __MARUBATSU_GAME_GRID_H__


class GameGrid {
  #define none  " "
  #define maru  "\033[34mO\033[39m"
  #define batsu "\033[33mX\033[39m"

  public:
    static const int GRID_STATUS_NONE  = 0;
    static const int GRID_STATUS_MARU  = 1;
    static const int GRID_STATUS_BATSU = 2;
    static const int GRID_STATUS_ERROR = 3;

    GameGrid() : grid_status(GRID_STATUS_NONE) {};
    bool isNone() { return grid_status == GRID_STATUS_NONE; };
    int getGridStatus(){ return grid_status; };
    bool setGridStatus(int state)
      {
        if(state >= GRID_STATUS_NONE && state < GRID_STATUS_ERROR && grid_status == GRID_STATUS_NONE)
          {
            grid_status = state; return true;
          }
        else
          {
            return false;
          }
      };

    const char *toStr()
      {
        return  (grid_status == GRID_STATUS_MARU)  ? maru  :
                (grid_status == GRID_STATUS_BATSU) ? batsu : none;
      };

    static const char *toStr(int status)
      {
        return  (status == GRID_STATUS_MARU)  ? maru  :
                (status == GRID_STATUS_BATSU) ? batsu : none;
      };


  private:
    int grid_status;
};

#endif  // __MARUBATSU_GAME_GRID_H__

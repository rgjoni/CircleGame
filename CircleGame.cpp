#include <cstdlib>
#include <ncurses.h> // input output
#include <unistd.h> // sleep function
#include <string>
#include<vector>
#include<iostream>
#include<thread>
#include <unistd.h>
#include<fstream>
unsigned int microseconds = 500000;
//Global Variable
int highscore =0;
bool gameOver = false;
bool timetoShuffle = true;
const int WIDTH = 40;
const int HEIGHT = 20;
bool checkcorrect = false;
bool newval = true;
int val = 0;
int q1 = 0;
int candidates[8] = {1,2,3,4,5,6,7,8};
int timeleft = 10;
int lives = 3;
const int mapx = WIDTH / 2;
const int mapy = HEIGHT / 2;
const int maplength = 8;
//                           1     2      3       4     5     6     7       8
int mappingsx[maplength] = {mapx,mapx+5,mapx+6,mapx+5,mapx,mapx-5,mapx-6,mapx-5};
int mappingsy[maplength] = {mapy-5,mapy-3,mapy,mapy+3,mapy+5,mapy+3,mapy,mapy-3};
int posx, posy = 0;
int score = -1;
int centerx, centery;
// might need?
typedef enum DIRECTION {
  STOP = 0, LEFT, RIGHT, ENTER
}
Dir;
Dir dir;
int val2 = 1;
//
void shuffle(int arr[], int size) {
  std::vector < int > shuffle;
  for (int i = 0; i < size; i++) {
    shuffle.push_back(i);
  }
  srand(time(NULL));
  int count = 0;
  while (!shuffle.empty()) {
    int pos = rand() % shuffle.size();
    arr[count] = shuffle[pos];
    shuffle.erase(shuffle.begin() + pos);
    count++;
    //cout<<pos;
  }
}
void getHighScore()
{
  std:: string line;
  std:: ifstream readfile ("CircleGameScores.txt");
  if (readfile.is_open())
  {
    while (getline(readfile,line))
    {
      //std::cout << line << '\n';
      highscore =stoi(line);
    }
    readfile.close();
  }
  else
  {
      highscore = 0;
  }
}
void setHighScore()
{
  std:: ofstream myfile;
  myfile.open ("CircleGameScores.txt");
  myfile << score;
  myfile.close();
}
void Initialize() {
  //Shell Screen Init, functions from ncurses.h
  initscr();
  clear();
  noecho();
  cbreak();
  //curs_set(0);

  gameOver = false;
  dir = STOP;
  centerx = WIDTH / 2; //  start xpos
  centery = HEIGHT / 2; // start ypos 
  score = 0;
}
void Render() {
  if (timetoShuffle) {
    shuffle(candidates, 8);
    timetoShuffle = false;
  }
  clear();
  //Draw map
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      //Draw map
      if (i == 0 | i == 21) {
        mvprintw(i, j, "*");
      } else if (j == 0 | j == 41) {
        mvprintw(i, j, "*");
      }
      //Draw target
      else if (i == centery && j == centerx) {
        if (newval) {
          val = rand() % 8;
          newval = false;
        }
        mvprintw(i, j, "%d", val);
      }

    }
  }

  for (int z = 0; z < maplength; z++) {

    mvprintw(mappingsy[z], mappingsx[z], "%d", candidates[z]);
  }
  mvprintw(16, 45, "                      SCORE = %d", score);
  mvprintw(17, 45, "                      TIME  = %d", timeleft);
  mvprintw(18, 45, "                      LIVES = %d", lives);
  mvprintw(19, 45, "                      HIGHSCORE = %d", highscore);

  //Draw Information

  mvprintw(5, 45, "-------------------------------------------------------------------");
  mvprintw(6, 45, "|                           Circle Game!                          |");
  mvprintw(7, 45, "-------------------------------------------------------------------");
  mvprintw(8, 45, "|             Use Left and Right Keys to get to the number        |");
  mvprintw(9, 45, "|                          <   v  >                               |");
  mvprintw(10, 45, "|If you run out of time or hit an incorrect number you lose a life|");
  mvprintw(11, 45, "|                   No lives = Game Over!                         |");
  mvprintw(12, 45, "-------------------------------------------------------------------");

  //  mvprintw(17, 29, "$$ Remain Time = %d", current_time);

  move(mappingsy[posx], mappingsx[posy]);
  refresh();
}
void gameOverRender() {
  clear();
  mvprintw(3, 29, "-------------------------------------------------------------------------------");
  mvprintw(4, 29, "|                               Game Over!                                    |");
  mvprintw(5, 29, "-------------------------------------------------------------------------------");
  mvprintw(6, 29, "-------------------------------------------------------------------------------");
  mvprintw(7, 29, "|                Press Enter to exit or SpaceBar to Continue!                 |");
  mvprintw(8, 29, "------------------------------------------------------------------------------");

  refresh();
}
void Poll_Input() {
  keypad(stdscr, TRUE); // Check whether there is input, function from ncurses
  //halfdelay(1); 

  int c = getch(); // Save the typed key by gamer

  switch (c) {
  case KEY_LEFT:
    dir = LEFT;
    break;
  case KEY_RIGHT:
    dir = RIGHT;
    break;
  case 10:
    dir = ENTER;
    val2 = 0;
    break;
  case 27: // esc key
    gameOver = true;
    break;
  default:
    dir = STOP;
    break;

  }

}
void Update() {
  switch (dir) {
  case LEFT:
    posx--;
    posy--;
    if (posx < 0) {
      posx = 7;
    }
    if (posy < 0) {
      posy = 7;
    }
    break;
  case RIGHT:
    posx++;
    posy++;
    if (posx > 7) {
      posx = 0;
    }
    if (posy > 7) {
      posy = 0;
    }
    break;
  case ENTER:
    //checkcorrect = true;
    if (candidates[posx] == val && dir == ENTER) {
      timetoShuffle = true;
      newval = true;
      Render();
      score++;
      if(score > highscore)
      {
          setHighScore();
      }
      timeleft += 2;
    } else if (candidates[posx] != val && dir == ENTER) {
      lives--;
      timeleft = 10;
    }
    if (lives == 0) {
      gameOver = true;
    }
    break;
  default:
    dir = STOP;
    checkcorrect = false;
    break;
  }

}
void countdown() {

  label2: while (timeleft >= 0 && !gameOver) {
    usleep(microseconds);
    if (!gameOver) {
      Render();
    }
    mvprintw(17, 29, "$$ TimeLeft = %d", timeleft);
    timeleft = timeleft - 1;

  }
  lives--;
  if (lives > 0) {
    timeleft = 10;
    goto label2;
  }
  gameOver = true;
  gameOverRender();

}

int main(void) {
  getHighScore();
  Initialize();
  std::thread first(countdown);
  label_name:
    while (!gameOver) {
      Render();
      Poll_Input();
      Update();
    }
  clear();
  //getch();
  bool more = true;
  while (more) {
    gameOverRender();
    int game = getch();
    switch (game) {
    case 10:
      more = false;
      break;
    case 32:
      more = false;
      gameOver = false;
      break;
    default:
      break;
    }

  }
  if (!gameOver) {
    score = 0;
    timeleft = 10;
    lives = 3;
    goto label_name;

  }
  endwin();

  return 0;
}
#ifndef SNAKE_H
#define SNAKE_H


#include <QWidget>
#include <QKeyEvent>
#include "coordinate.h"
#include <vector>
class Snake : public QWidget {

  public:
      Snake(QWidget *parent = 0);
      std::vector<coordinate> placed_obstacles;

  protected:
      void paintEvent(QPaintEvent *);
      void timerEvent(QTimerEvent *);
      void keyPressEvent(QKeyEvent *);

  private:
      QImage dot;
      QImage head;
      QImage apple;
      QImage obstacle;

      static const int B_WIDTH = 400;
      static const int B_HEIGHT = 400;
      static const int DOT_SIZE = 10;
      static const int ALL_DOTS = 900;
      static const int RAND_POS = 30;
      //static const int DELAY = 140;
      int DELAY = 100;
      int score = 0;


      int timerId;
      int dots;
      int apple_x;
      int apple_y;
      //int obstacle_x;
      //int obstacle_y;

      int x[ALL_DOTS];
      int y[ALL_DOTS];

      bool leftDirection;
      bool rightDirection;
      bool upDirection;
      bool downDirection;
      bool inGame;

      void loadImages();
      void initGame();
      void locateApple();
      void locateObstacle();
      void checkApple();
      void checkObstacle();
      void checkCollision();
      void move();
      void doDrawing();
      void gameOver(QPainter &);
};


#endif // SNAKE_H

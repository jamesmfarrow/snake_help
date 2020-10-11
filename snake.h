#ifndef SNAKE_H
#define SNAKE_H


#include <QWidget>
#include <QKeyEvent>
#include <vector>
#include "coordinate.h"

class Snake : public QWidget {

  public:
      Snake(QWidget *parent = 0);
      std::vector<coordinate> placed_obstacles;

  //protected:
      void paintEvent(QPaintEvent *);
      void timerEvent(QTimerEvent *);
      void keyPressEvent(QKeyEvent *);

  private:
      QImage dot_png;
      QImage head_png;
      QImage apple_png;
      QImage obstacle_png;

      //static const int ALL_DOTS = 900;
      static const int B_WIDTH = 600;
      static const int B_HEIGHT = 600;
      const int BODY_SIZE = 10;
      const int DELAY = 140;
      int score = 0;


      int timerId;
      int snake_size;
      coordinate apple;

      int x_axis[B_WIDTH];
      int y_axis[B_HEIGHT];

      bool leftDirection;
      bool rightDirection;
      bool upDirection;
      bool downDirection;
      bool GamePlay;

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

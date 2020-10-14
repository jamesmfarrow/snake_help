#ifndef SNAKE_H
#define SNAKE_H


#include <QWidget>
#include <QKeyEvent>
#include <vector>
#include "coordinate.h"

enum class Turn{ left, right, up, down };
enum class Movement{ north, south, east, west };



class Snake : public QWidget {

  public:
      Snake(QWidget *parent = 0);
      // store obstacles and check for duplicity
      std::vector<coordinate> placed_obstacles;
      //vevtor for snake coordinates
      std::vector<coordinate> the_snake;

      Turn change;
      Movement direction;


      void paintEvent(QPaintEvent *);
      void timerEvent(QTimerEvent *);
      void keyPressEvent(QKeyEvent *);

      QImage dot_png;
      QImage head_png;
      QImage apple_png;
      QImage obstacle_png;

      //play area width
      static const int B_WIDTH = 500;
      //play area height
      static const int B_HEIGHT = 500;

      //this variable is needed otherwise snake is drawn on top of itself
      const int BODY_SIZE = 10;

      //delay offset for the speed game/refresh
      const int DELAY = 100;
      int score = 0;


      int timerId;
      int snake_size = 3;//initial size of the snake
      coordinate apple;
      coordinate snake;


      //use bool instead of enum for directions
      bool left;
      bool right;
      bool up;
      bool down;
      //check game state playing/gameover
      bool GamePlay;

      void grow(int amount);
      void loadImages();
      void initGame();
      void placeFood();
      void placeObstacle();
      void foodCollision();
      void obstacleCollision();
      void checkCollision();
      void move();
      void doDrawing();
      void gameOver(QPainter &);
};


#endif // SNAKE_H

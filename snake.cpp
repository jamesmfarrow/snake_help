#include "snake.h"

#include <QPainter>
#include <QTime>
#include <QIcon>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QSound>
#include "snake.h"
#include <iostream>
#include <random>
#include <QGamepad>
#include <QGamepadKeyNavigation>
#include <QGamepadManager>


//constructor
Snake::Snake(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color:grey;");
    left = false;
    //start snake moving in right direction
    right = true;
    up = false;
    down = false;
    GamePlay = true;

    //set the paly area size
    resize(B_WIDTH, B_HEIGHT);
    //load the png files for the game
    loadImages();

    //ini game
    initGame();
}

void Snake::loadImages() {

    dot_png.load(":/new/prefix1/dot.png");
    head_png.load(":/new/prefix1/head.png");
    apple_png.load(":/new/prefix1/apple.png");
    obstacle_png.load(":/new/prefix1/obstacle.png");
}

void Snake::initGame() {

    for(int i = 0; i < snake_size; i++) {
            snake.m_x = 150 - (i * BODY_SIZE);
            snake.m_y = 150;
            the_snake.push_back(snake);
        }

    placeFood();
    placeObstacle();
    timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    doDrawing();
}

void Snake::doDrawing() {

    QPainter qp(this);

    std::cout << "number of obstacles->" << placed_obstacles.size() << std::endl;

    if(GamePlay) {
         //draw the obstacles
         for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
            qp.drawImage(placed_obstacles[i].m_x, placed_obstacles[i].m_y, obstacle_png);
         }
         //draw the apple
         qp.drawImage(apple.m_x, apple.m_y, apple_png);
         //draw the snake
         for (unsigned long int i = 0; i < the_snake.size(); i++) {
            if (i == 0) {
                qp.drawImage(the_snake[i].m_x, the_snake[i].m_y, head_png);
            }
            else {
                qp.drawImage(the_snake[i].m_x, the_snake[i].m_y, dot_png);
            }
        }

    }
    else {
        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp) {

    QString GameOver = "Game over";
    QString YourScore = "Your score ";
    QString display_score = QString::number(score);
    QSound::play(":/new/prefix1/fail-trombone-03.wav");
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(GameOver);

    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, GameOver);
    qp.drawText(-textWidth/2, 20, YourScore+=display_score);
}

void Snake::grow(int amount) {
       coordinate last = the_snake.back();
       for(int i{}; i < amount; i++) {
           the_snake.push_back(last);
       }
   }

void Snake::foodCollision() {

    if((the_snake[0].m_x > apple.m_x - 15 && the_snake[0].m_x < apple.m_x + 15)
            && (the_snake[0].m_y > apple.m_y - 15 && the_snake[0].m_y < apple.m_y + 15)) {
        grow(3);
        score++;
        QSound::play(":/new/prefix1/button-1.wav");
        placeFood();
        if(the_snake.size() % 2) placeObstacle();

    }
}

void Snake::obstacleCollision() {

    for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
        if ((the_snake[0].m_x > placed_obstacles[i].m_x - 10 && the_snake[0].m_x < placed_obstacles[i].m_x + 10)
            && (the_snake[0].m_y > placed_obstacles[i].m_y - 10 && the_snake[0].m_y < placed_obstacles[i].m_y + 10)) {
            QSound::play(":/new/prefix1/button-1.wav");
            GamePlay = false;
            if(!GamePlay) {
                killTimer(timerId);
            }
        }
    }
}



void Snake::move() {

    for (int i = the_snake.size()-1; i > 0; i--) {
        the_snake[i] = the_snake[i - 1];
    }

    //east
    if (left) {
        the_snake[0].m_x-=BODY_SIZE;
    }

    //west
    if (right) {
        the_snake[0].m_x+=BODY_SIZE;
    }

    //north
    if (up) {
        the_snake[0].m_y-=BODY_SIZE;
    }

    //souuth
    if (down) {
        the_snake[0].m_y+=BODY_SIZE;
    }
}

void Snake::checkCollision() {

    //below for self collision
    for(unsigned long int i{1}; i < the_snake.size() - 1; i++) {
                if(the_snake[0] == the_snake[i]) GamePlay = false;
            }


    //add GamePlay = false to each if() to disable wraparound
    //and enable wall crash
    if(the_snake[0].m_y >= B_HEIGHT){
        the_snake[0].m_y = 0;
    }

    if(the_snake[0].m_y < 0){
        the_snake[0].m_y = B_HEIGHT;
    }

    if(the_snake[0].m_x >= B_WIDTH){
        the_snake[0].m_x = 0;
    }

    if(the_snake[0].m_x < 0){
        the_snake[0].m_x = B_WIDTH;
    }

    if(!GamePlay) {
        killTimer(timerId);
    }
}

void Snake::placeFood() {


    bool notundersnake{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,B_HEIGHT);
    coordinate temp((dist(dev)), (dist(dev)));
    //check to make sure apple isnt placed under snake
    for(unsigned long int i{}; i < the_snake.size(); i++ ) {
        if((temp.m_x > the_snake[i].m_x - 15 && temp.m_x < the_snake[i].m_x + 15)
            && (temp.m_y > the_snake[i].m_y - 15 && temp.m_y < the_snake[i].m_y +15))
        { notundersnake = false; break; }
    }
    if(notundersnake) apple = temp;
}


void Snake::placeObstacle() {

    bool not_duplicate{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,B_HEIGHT);
    coordinate p((dist(dev)), (dist(dev)));
    std::cout << p.m_x << " " << p.m_y << std::endl;
    if(placed_obstacles.empty()) placed_obstacles.push_back(p);
    else {
        for(unsigned long int i{}; i < placed_obstacles.size() -1; i++) {
            if(p == placed_obstacles[i]) { not_duplicate = false; break; }
            if((placed_obstacles[i].m_x > apple.m_x - 100 && placed_obstacles[i].m_x < apple.m_x + 100)
                && (placed_obstacles[i].m_y > apple.m_y - 100 && placed_obstacles[i].m_y < apple.m_y + 100)) {
                not_duplicate = false; break;
            }
        }
        if(not_duplicate) { placed_obstacles.push_back(p); }
    }
}


void Snake::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if (GamePlay) {

        foodCollision();
        obstacleCollision();
        checkCollision();
        move();
    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e) {

    int key = e->key();

    if ((key == Qt::Key_Left) && (!right)) {
        left = true;
        up = false;
        down = false;
    }

    if ((key == Qt::Key_Right) && (!left)) {
        right = true;
        up = false;
        down = false;
    }

    if ((key == Qt::Key_Up) && (!down)) {
        up = true;
        right = false;
        left = false;
    }

    if ((key == Qt::Key_Down) && (!up)) {
        down = true;
        right = false;
        left = false;
    }

    QWidget::keyPressEvent(e);
}

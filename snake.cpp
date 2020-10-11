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
    leftDirection = false;
    //start snake moving in right direction
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    GamePlay = true;

    resize(B_WIDTH, B_HEIGHT);
    loadImages();
    initGame();
}

void Snake::loadImages() {

    dot_png.load(":/new/prefix1/dot.png");
    head_png.load(":/new/prefix1/head.png");
    apple_png.load(":/new/prefix1/apple.png");
    obstacle_png.load(":/new/prefix1/obstacle.png");
}

void Snake::initGame() {

    snake_size = 3;

    //place snake in the game
    for (int z = 0; z < snake_size; z++) {
        x_axis[z] = 150 - (z * 10);
        y_axis[z] = 150;
    }

    locateApple();
    locateObstacle();


    timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);

    doDrawing();
}

void Snake::doDrawing() {

    QPainter qp(this);

    if(GamePlay) {
         for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
            qp.drawImage(placed_obstacles[i].m_x, placed_obstacles[i].m_y, obstacle_png);
         }
         qp.drawImage(apple.m_x, apple.m_y, apple_png);
         for (int z = 0; z < snake_size; z++) {
            if (z == 0) {
                qp.drawImage(x_axis[z], y_axis[z], head_png);
            }
            else {
                qp.drawImage(x_axis[z], y_axis[z], dot_png);
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

void Snake::checkApple() {

    if((x_axis[0] > apple.m_x - 15 && x_axis[0] < apple.m_x + 15) && (y_axis[0] > apple.m_y - 15 && y_axis[0] < apple.m_y + 15)) {
        snake_size+=3;
        score++;
        QSound::play(":/new/prefix1/button-1.wav");
        locateApple();
        if(score%2 == 0) locateObstacle();

    }
}

void Snake::checkObstacle() {

  std::cout << placed_obstacles.size() << std::endl;

    for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
        if ((x_axis[0] > placed_obstacles[i].m_x - 10 && x_axis[0] < placed_obstacles[i].m_x + 10)
            && (y_axis[0] > placed_obstacles[i].m_y - 10 && y_axis[0] < placed_obstacles[i].m_y + 10)) {
            std::cout << "x[0]" << x_axis[0] << "," << "y[0]" << y_axis[0] << std::endl;
            std::cout << placed_obstacles[i].m_x << "," << placed_obstacles[i].m_y << std::endl;
            QSound::play(":/new/prefix1/button-1.wav");
            GamePlay = false;
            if(!GamePlay) {
                killTimer(timerId);
            }
        }
    }
}



void Snake::move() {

    for (int z = snake_size; z > 0; z--) {
        x_axis[z] = x_axis[(z - 1)];
        y_axis[z] = y_axis[(z - 1)];
    }

    if (leftDirection) {
        x_axis[0] -= BODY_SIZE;
    }

    if (rightDirection) {
        x_axis[0] += BODY_SIZE;
    }

    if (upDirection) {
        y_axis[0] -= BODY_SIZE;
    }

    if (downDirection) {
        y_axis[0] += BODY_SIZE;
    }
}

void Snake::checkCollision() {

    //check for self collision
    for (int z = snake_size; z > 0; z--) {
        if ((z > 4) && (x_axis[0] == x_axis[z]) && (y_axis[0] == y_axis[z])) {
            GamePlay = false;
        }
    }

    //restore GamePlay = false if no wrap around required
    if (y_axis[0] >= B_HEIGHT) {
        y_axis[0] = 0;
        //GamePlay = false;
    }

    if (y_axis[0] < 0) {
        y_axis[0] = B_HEIGHT;
        //GamePlay = false;
    }

    if (x_axis[0] >= B_WIDTH) {
        x_axis[0] = 0;
        //GamePlay = false;
    }

    if (x_axis[0] < 0) {
        x_axis[0] = B_WIDTH;
        //GamePlay = false;
    }

    if(!GamePlay) {
        killTimer(timerId);
    }
}

void Snake::locateApple() {

    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,400);

    apple.m_x = dist(dev);
    apple.m_y = dist(dev);

}


void Snake::locateObstacle() {

    bool not_duplicate{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,400);
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

        checkApple();
        checkObstacle();
        checkCollision();
        move();
    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e) {

    int key = e->key();

    if ((key == Qt::Key_Left) && (!rightDirection)) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Right) && (!leftDirection)) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }

    if ((key == Qt::Key_Up) && (!downDirection)) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    if ((key == Qt::Key_Down) && (!upDirection)) {
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }

    QWidget::keyPressEvent(e);
}

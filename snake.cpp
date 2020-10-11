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

Snake::Snake(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color:grey;");
    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;

    resize(B_WIDTH, B_HEIGHT);
    loadImages();
    initGame();
}

void Snake::loadImages() {

    dot.load(":/new/prefix1/dot.png");
    head.load(":/new/prefix1/head.png");
    apple.load(":/new/prefix1/apple.png");
    obstacle.load(":/new/prefix1/obstacle.png");
}

void Snake::initGame() {

    dots = 3;

    for (int z = 0; z < dots; z++) {
        x[z] = 50 - z * 10;
        y[z] = 50;
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

    if(inGame) {
        if(placed_obstacles.size() == 1) { qp.drawImage(placed_obstacles[0].m_x, placed_obstacles[0].m_y, obstacle); }
        else {
         for(unsigned long int i{}; i < placed_obstacles.size() - 1; i++) {
            qp.drawImage(placed_obstacles[i].m_x, placed_obstacles[i].m_y, obstacle);
         }
        }
         qp.drawImage(apple_x, apple_y, apple);
         for (int z = 0; z < dots; z++) {
            if (z == 0) {
                qp.drawImage(x[z], y[z], head);
            }
            else {
                qp.drawImage(x[z], y[z], dot);
            }
        }

    }
    else {
        std::cout << "Game Over" << std::endl;
        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp) {

    QString message = "Game over";
    QString display = "Your score ";
    QString display_score= QString::number(score);
    QSound::play(":/new/prefix1/fail-trombone-03.wav");
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
    qp.drawText(-textWidth/2, 20, display+=display_score);
}

void Snake::checkApple() {

    if ((x[0] > apple_x - 5 && x[0] < apple_x + 5 )
            && (y[0] > apple_y - 5 && y[0] < apple_y + 5)) {

        //dots++;
        dots+=3;
        std::cout << score << "," << DELAY << std::endl;
        QSound::play(":/new/prefix1/button-1.wav");
        score++;
        locateApple();
        if(dots%4) locateObstacle();

    }
}

void Snake::checkObstacle() {

  std::cout << placed_obstacles.size() << std::endl;

    for(unsigned long int i{}; i < placed_obstacles.size() ; i++) {
        std::cout << "here" << std::endl;
        if ((x[0] > placed_obstacles[i].m_x - 10 && x[0] < placed_obstacles[i].m_x + 10)
            && (y[0] > placed_obstacles[i].m_y - 10 && y[0] < placed_obstacles[i].m_y + 10))

        {
            std::cout << "x[0]" << x[0] << "," << "y[0]" << y[0] << std::endl;
            std::cout << placed_obstacles[i].m_x << "," << placed_obstacles[i].m_y << std::endl;
            QSound::play(":/new/prefix1/button-1.wav");
            inGame = false;
            if(!inGame) {
                killTimer(timerId);
            }
        }
    }

   /*if ((x[0] == placed_obstacles[0].m_x) && (y[0] == placed_obstacles[0].m_y)) {
        QSound::play(":/new/prefix1/button-1.wav");
        inGame = false;
        if(!inGame) {
            killTimer(timerId);
        }
    }*/
}



void Snake::move() {

    for (int z = dots; z > 0; z--) {
        x[z] = x[(z - 1)];
        y[z] = y[(z - 1)];
    }

    if (leftDirection) {
        x[0] -= DOT_SIZE;
    }

    if (rightDirection) {
        x[0] += DOT_SIZE;
    }

    if (upDirection) {
        y[0] -= DOT_SIZE;
    }

    if (downDirection) {
        y[0] += DOT_SIZE;
    }
}

void Snake::checkCollision() {

    //check for self collision
    for (int z = dots; z > 0; z--) {
        if ((z > 4) && (x[0] == x[z]) && (y[0] == y[z])) {
            inGame = false;
        }
    }

    //restore inGame = false if no wrap around required
    if (y[0] >= B_HEIGHT) {
        y[0] = 0;
        //inGame = false;
    }

    if (y[0] < 0) {
        y[0] = B_HEIGHT;
        //inGame = false;
    }

    if (x[0] >= B_WIDTH) {
        x[0] = 0;
        //inGame = false;
    }

    if (x[0] < 0) {
        x[0] = B_WIDTH;
        //inGame = false;
    }

    if(!inGame) {
        killTimer(timerId);
    }
}

void Snake::locateApple() {

    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());


    int r = qrand() % RAND_POS;
    apple_x = (r * DOT_SIZE);


    r = qrand() % RAND_POS;
    apple_y = (r * DOT_SIZE);

}


void Snake::locateObstacle() {

    std::cout << "locate obstacle" << std::endl;

    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());

    bool not_duplicate{true};
    std::random_device dev;
    std::uniform_int_distribution<int> dist(0,400);
    coordinate p((dist(dev)), (dist(dev)));
    std::cout << p.m_x << " " << p.m_y << std::endl;
    if(placed_obstacles.empty()) placed_obstacles.push_back(p);
    else {
        for(unsigned long int i{}; i < placed_obstacles.size() -1; i++) {
            if(p == placed_obstacles[i]) { not_duplicate = false; break; }
        }
        if(not_duplicate) { placed_obstacles.push_back(p); }
    }

    /*int r = qrand() % RAND_POS;
    obstacle_x = (r * DOT_SIZE - 100);
    r = qrand() % RAND_POS;
    obstacle_y = (r * DOT_SIZE - 100);*/

}


void Snake::timerEvent(QTimerEvent *e) {

    Q_UNUSED(e);

    if (inGame) {

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

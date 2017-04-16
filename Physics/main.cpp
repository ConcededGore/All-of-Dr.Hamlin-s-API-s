#include <iostream>
#include "graphics.h"

#define PI 3.14159;

using namespace std;
using namespace mssm;

class Ball {
public:
    double centerX;
    double centerY;
    double speed;
    double accelX;
    double accelY;
    double radius;

    Ball(Graphics &g, double centerX, double centerY, double radius);
    void draw(Graphics& g);
};

Ball::Ball(Graphics& g, double centerX, double centerY, double radius) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;

    draw(g);
}

void Ball::draw(Graphics &g) {
    g.ellipse(centerX, centerY, radius, radius);
}

void graphicsMain(Graphics& g) {
    Ball ball{g, 200, 200, 20};

    while (g.draw()) {
        g.clear();



        ball.draw(g);
    }
}

int main() {
    Graphics g("Bounce!", graphicsMain, 640, 420);
}

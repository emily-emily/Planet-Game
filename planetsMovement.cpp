#include <allegro5/allegro.h>
#include<cmath>
#include"planets.h"

#include<stdio.h>

void getNewCoordinates(Person &p){
    p.xPos += p.xVel;
    p.yPos += p.yVel;
}

void jump(Person &p, Circle c){
    float x, y;
    float b;
    x = p.xPos - c.x;
    y = p.yPos - c.y;

    b = atan(y / x);

    if (isGrounded(p, c)){
        if (x < 0){
            p.xVel -= jumpVel * cos(b) / FPS;
            p.yVel -= jumpVel * sin(b) / FPS;
        }
        else{
            p.xVel += jumpVel * cos(b) / FPS;
            p.yVel += jumpVel * sin(b) / FPS;
        }
    }


    printf("jump p.xVel is %f\n", jumpVel * cos(b) / FPS);
    printf("jump p.yVel is %f\n", jumpVel * sin(b) / FPS);

}

bool isGrounded(Person p, Circle c){
    float d;
    d = sqrt(pow(p.xPos - c.x, 2) + pow(p.yPos - c.y, 2));

    if (d <= c.r){
        return true;
    }
    else return false;
}

void gravity(Person &p, Circle c){
    float x, y;
    float b;
    x = p.xPos - c.x;
    y = p.yPos - c.y;

    b = atan(y / x);

    if (isGrounded(p, c)){
        p.xVel = 0;
        p.yVel = 0;
    }
    else{
        if (x > 0){
            p.xVel -= gVel * cos(b) / FPS;
            p.yVel -= gVel * sin(b) / FPS;
        }
        else{
            p.xVel += gVel * cos(b) / FPS;
            p.yVel += gVel * sin(b) / FPS;
        }
    }
}

float rotateAngle(Person p, Circle c){
    float x, y;
    float b;
    x = p.xPos - c.x;
    y = p.yPos - c.y;

    b = atan(y / x);

    if (x < 0)
        return b - 3.1415 / 2;
    else return b + 3.1415 / 2;
}

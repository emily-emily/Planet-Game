#include <allegro5/allegro.h>
#include<cmath>
#include"planets.h"

#include<stdio.h>

void togglePause(ALLEGRO_TIMER *timer, bool paused){
    if (paused){
        al_start_timer(timer);
        paused = false;
    }
    else{
        al_stop_timer(timer);
        paused = true;
    }
}

void getNewCoordinates(Sprite &s){
    s.xPos += s.xVel;
    s.yPos += s.yVel;
}

void jump(Sprite &s, Planet a){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (isGrounded(s, a)){
        if (x < 0){
            s.xVel -= jumpVel * cos(b) / FPS;
            s.yVel -= jumpVel * sin(b) / FPS;
        }
        else{
            s.xVel += jumpVel * cos(b) / FPS;
            s.yVel += jumpVel * sin(b) / FPS;
        }
    }


    printf("jump s.xVel is %f\n", jumpVel * cos(b) / FPS);
    printf("jump s.yVel is %f\n", jumpVel * sin(b) / FPS);

}

void shift(Sprite &s, Planet a, Direction spriteD){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (spriteD == LEFT)
        if (x > 0){
            s.xVel += moveSpd * sin(b) / FPS;
            s.yVel -= moveSpd * cos(b) / FPS;
        }
        else{
            s.xVel -= moveSpd * sin(b) / FPS;
            s.yVel += moveSpd * cos(b) / FPS;
        }
    else{
        if (x > 0){
            s.xVel -= moveSpd * sin(b) / FPS;
            s.yVel += moveSpd * cos(b) / FPS;
        }
        else{
            s.xVel += moveSpd * sin(b) / FPS;
            s.yVel -= moveSpd * cos(b) / FPS;
        }
    }
}

bool isGrounded(Sprite s, Planet a){
    float d;
    d = sqrt(pow(s.xPos - a.x, 2) + pow(s.yPos - a.y, 2));

    if (d <= a.r){
        return true;
    }
    else return false;
}

void gravity(Sprite &s, Planet a){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (isGrounded(s, a)){
        s.xVel = 0;
        s.yVel = 0;
    }
    else{
        if (x > 0){
            s.xVel -= gVel * cos(b) / FPS;
            s.yVel -= gVel * sin(b) / FPS;
        }
        else{
            s.xVel += gVel * cos(b) / FPS;
            s.yVel += gVel * sin(b) / FPS;
        }
    }
}

float rotateAngle(Sprite p, Planet a){
    float x, y;
    float b;
    x = p.xPos - a.x;
    y = p.yPos - a.y;

    b = atan(y / x);

    if (x < 0)
        return b - 3.1415 / 2;
    else return b + 3.1415 / 2;
}

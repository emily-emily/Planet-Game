#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include "planets.h"

#include<stdio.h>

void draw(Planet a, Sprite s, ALLEGRO_BITMAP *bitmap, Meteor m[], ALLEGRO_BITMAP *mImage){
    al_clear_to_color(BLACK);

    /**planets -- will change to bitmap**/
    al_draw_circle(a.x, a.y, a.r, WHITE, 2);

    //gravity fields?
    //al_draw_circle(a.x, a.y, a.r + 0.5 * a.r, MAGENTA, 1);

    //draw person
    al_draw_rotated_bitmap(bitmap, al_get_bitmap_width(bitmap) / 2, al_get_bitmap_height(bitmap), s.xPos, s.yPos, rotateAngle(s, a), 0);
    al_draw_line(a.x, a.y, s.xPos, s.yPos, WHITE, 1.0);

    //draw meteors
    for (int i = 0; i < maxMeteors; i++)
        if (!m[i].available)
            al_draw_bitmap(mImage, m[i].xPos, m[i].yPos, 0);
}

void getNewCoordinates(Sprite &s, Meteor m[]){
    s.xPos += s.xVel;
    s.yPos += s.yVel;

    for (int i = 0; i < maxMeteors; i++){
        if (!m[i].available){
            m[i].xPos += m[i].xVel;
            m[i].yPos += m[i].yVel;
        }
    }
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


    //printf("jump s.xVel is %f\n", jumpVel * cos(b) / FPS);
    //printf("jump s.yVel is %f\n", jumpVel * sin(b) / FPS);

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

bool misGrounded(Meteor m[], int i, Planet a){
    float d;
    d = sqrt(pow(m[i].xPos - a.x, 2) + pow(m[i].yPos - a.y, 2));

    if (d <= a.r){
        return true;
    }
    else return false;
}

void gravity(Sprite &s, Meteor m[], Planet a){
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

    for (int i = 0; i < maxMeteors; i++){
        if (!m[i].available){
            x = m[i].xPos - a.x;
            y = m[i].yPos - a.y;

            b = atan(y / x);

            if (misGrounded(m, i, a)){
                //m[i].xVel = 0;
                //m[i].yVel = 0;
                destroyMeteor(m, i);
            }
            else{
                if (x > 0){
                    m[i].xVel -= gVel * cos(b) / FPS / 8;
                    m[i].yVel -= gVel * sin(b) / FPS / 8;
                }
                else{
                    m[i].xVel += gVel * cos(b) / FPS / 8;
                    m[i].yVel += gVel * sin(b) / FPS / 8;
                }
            }
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

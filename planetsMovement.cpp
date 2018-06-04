/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include "planets.h"

#include<stdio.h>

//draws objects
void drawObjects(ALLEGRO_BITMAP *planet, Planet a, Sprite s, ALLEGRO_BITMAP *sprite[], Meteor m[], ALLEGRO_BITMAP *mImage){
    //draws planet at the center of the screen
    al_draw_scaled_bitmap(planet, 0, 0, al_get_bitmap_width(planet), al_get_bitmap_height(planet), SCREEN_W / 2 - a.r, SCREEN_H / 2 - a.r, a.r * 2, a.r * 2, 0);

    /**gravity fields?**/
    al_draw_circle(a.x, a.y, a.r + minMeteorDistance, WHITE, 1);

    //draw sprite
    al_draw_scaled_rotated_bitmap(sprite[s.frame], al_get_bitmap_width(sprite[s.frame]) / 2, al_get_bitmap_height(sprite[s.frame]),
                                  s.xPos, s.yPos, imageScale, imageScale, rotateAngle(s, a), ALLEGRO_FLIP_HORIZONTAL - s.dir);

    //draw meteors
    for (int i = 0; i < maxMeteors; i++)
        if (!m[i].available){
            al_draw_scaled_rotated_bitmap(mImage, m[i].w / 2, m[i].h - (m[i].w / 2), m[i].xPos, m[i].yPos, imageScale, imageScale, 0, 0);
        }
}

//calculate new locations of objects
void getNewCoordinates(Sprite &s, Meteor m[]){
    s.xPos += s.xVel + s.shiftX;
    s.yPos += s.yVel + s.shiftY;

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
}

//left-right movement for sprite (relative to planet)
void shift(Sprite &s, Planet a, Direction spriteD){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (spriteD == LEFT){
        s.dir = 0;
        if (x > 0){
            s.shiftX = moveSpd * sin(b) / FPS;
            s.shiftY = -(moveSpd * cos(b) / FPS);
        }
        else{
            s.shiftX = -(moveSpd * sin(b) / FPS);
            s.shiftY = moveSpd * cos(b) / FPS;
        }
    }
    else if (spriteD == RIGHT){
        s.dir = 1;
        if (x > 0){
            s.shiftX = -(moveSpd * sin(b) / FPS);
            s.shiftY = moveSpd * cos(b) / FPS;
        }
        else{
            s.shiftX = moveSpd * sin(b) / FPS;
            s.shiftY = -(moveSpd * cos(b) / FPS);
        }
    }
    else{
        s.shiftX = 0;
        s.shiftY = 0;
    }
}

//returns true/false for if sprite is on the surface of planet
bool isGrounded(Sprite s, Planet a){
    float d;
    d = sqrt(pow(s.xPos - a.x, 2) + pow(s.yPos - a.y, 2));

    if (d <= a.r){
        return true;
    }
    else return false;
}

//isGrounded function for meteors
bool misGrounded(Meteor m[], int i, Planet a){
    float d;
    d = sqrt(pow(m[i].xPos - a.x, 2) + pow(m[i].yPos - a.y, 2));

    if (d <= a.r){
        return true;
    }
    else return false;
}

//applies gravity to all objects
void gravity(Sprite &s, Meteor m[], Planet a){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (isGrounded(s, a)){
        s.airborne = false;
        s.xVel = 0;
        s.yVel = 0;
    }
    else{
        s.airborne = true;
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

float rotateAngle(Sprite s, Planet a){
    float x, y;
    float b;
    x = s.xPos - a.x;
    y = s.yPos - a.y;

    b = atan(y / x);

    if (x < 0)
        return b - 3.1415 / 2;
    else return b + 3.1415 / 2;
}

bool isCollision(Sprite p, int sw, int sh, Meteor m, int mw, int mh){
    struct Shape{
        float top;
        float bot;
        float left;
        float right;
    };

    Shape sp;
    sp.top = p.yPos;
    sp.bot = p.yPos + sh;
    sp.left = p.xPos;
    sp.right = p.xPos + sw;

    Shape sm;
    sm.top = m.yPos;
    sm.bot = m.yPos + mh;
    sm.left = m.xPos;
    sm.right = m.xPos + mw;

    if (sp.top > sm.bot || sm.top > sp.bot || sp.left > sm.right || sp.right < sm.left){
        return false;
    }
    else return true;
}

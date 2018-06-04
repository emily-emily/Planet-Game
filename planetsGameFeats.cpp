/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "planets.h"

//draws background etc
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *box, Screen scr, ALLEGRO_FONT *f[], float score){
    //background
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, SCREEN_W, SCREEN_H, 0);

    //score
    if (scr == GAME)
        al_draw_textf(f[6], WHITE, SCREEN_W - 150, 0, 0, "SCORE: %d", (int) score);
    else
        al_draw_scaled_bitmap(box, 0, 0, al_get_bitmap_width(box), al_get_bitmap_height(box), 50, 50, SCREEN_W - 100, SCREEN_H - 100, 0);
}

void drawStart(ALLEGRO_FONT *tf, ALLEGRO_FONT *bf, int iFlash){
    al_draw_text(tf, WHITE, (SCREEN_W - al_get_text_width(tf, "GAME TITLE")) / 2, 250, 0, "GAME TITLE");
    if (iFlash > 20)
        al_draw_text(bf, WHITE, SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, "- Press space to continue -");

}

void drawGameOver(ALLEGRO_FONT *f[], float score, Button btn1, Button btn2){
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 270, ALLEGRO_ALIGN_CENTER, "Score: %d", (int)score);
    drawBtn(btn1, f);
    drawBtn(btn2, f);
}

void drawNewHighscore(ALLEGRO_FONT *f[], char name[][maxNameLength], int scores[], int newScore){
    int ranking = 0;
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "NEW HIGHSCORE!");
    al_draw_textf(f[5], WHITE, 400, 270, ALLEGRO_ALIGN_CENTER, "Your score: %d", newScore);

    for (int i = 9; i >= 0; i++)
        if (newScore > scores[i])
            ranking = i + 1;

    al_draw_textf(f[5], WHITE, 450, 270, ALLEGRO_ALIGN_CENTER, "Ranking: %d", newScore);
}

int drawHighscores(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn, char name[][maxNameLength], int score[]){
    if (getHighscores(display, name, score) != 0)
        return -1;

    al_draw_text(f[3], WHITE,SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "HIGHSCORES");

    //draw boxes?
    al_draw_rectangle(100, 180, SCREEN_W / 2 - 30, SCREEN_H - 150, WHITE, 2);
    al_draw_rectangle(SCREEN_W / 2 + 30, 180, SCREEN_W - 100, SCREEN_H - 150, WHITE, 2);

    for (int i = 0; i < 5; i++){
        al_draw_textf(f[5], WHITE, 125, 200 + 72 * i, 0, "%s", name[i]);
        al_draw_textf(f[5], WHITE, 375, 200 + 72 * i, 0, "%d", score[i]);
    }

    for (int i = 5; i < 10; i++){
        al_draw_textf(f[5], WHITE, SCREEN_W / 2 + 55, 200 + 72 * (i - 5), 0, "%s", name[i]);
        al_draw_textf(f[5], WHITE, SCREEN_W / 2 + 305, 200 + 72 * (i - 5), 0, "%d", score[i]);
    }

    drawBtn(btn, f);
    return 0;
}

void findBtnXY(Button &btn, ALLEGRO_FONT *f[], char text[20], int y1){
    btn.x1 = (SCREEN_W - al_get_text_width(f[5], text)) / 2 - 10;
    btn.x2 = (SCREEN_W + al_get_text_width(f[5], text)) / 2 + 10;
    btn.y1 = y1;
    btn.y2 = y1 + 50;
}

bool btnIsClicked(Button btn, int mouseX, int mouseY){
    if (mouseX < btn.x1 || mouseX > btn.x2 || mouseY < btn.y1 || mouseY > btn.y2)
        return false;
    else return true;
}

void drawBtn(Button btn, ALLEGRO_FONT *f[]){
    al_draw_rounded_rectangle(btn.x1, btn.y1, btn.x2, btn.y2, 5, 5, WHITE, 2);
    al_draw_text(f[5], WHITE, btn.x1 + 10, btn.y1 + 10, 0, btn.text);
}

void togglePause(ALLEGRO_TIMER *timer, bool &paused){
    if (paused){
        al_start_timer(timer);
        paused = false;
    }
    else{
        al_stop_timer(timer);
        paused = true;
    }
}

void createMeteor(Meteor m[], ALLEGRO_BITMAP *image){
    int i = 0;
    while (!m[i].available && i < maxMeteors - 1){
        i++;
    }
    if (m[i].available){
        m[i].xPos = rand() % SCREEN_W;
        m[i].yPos = rand() % SCREEN_H;

        //overwrite destroyed meteor info
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].available = false;
    }
}

void destroyMeteor(Meteor m[], int i){
    m[i].available = true;
}

int getHighscores(ALLEGRO_DISPLAY *display, char name[][maxNameLength], int score[]){
    FILE *fptr;
    fptr = fopen("highscores.txt", "r");

    if (!fptr){
        al_show_native_message_box(display, "ERROR", "Error loading file", "\"highscores.txt\" could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    for (int i = 0; i < 10; i++){
        fscanf(fptr, "%s", name[i]);
        fscanf(fptr, "%d", &score[i]);
    }

    fclose(fptr);
    return 0;
}

/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
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

void drawNewHighscore(ALLEGRO_FONT *f[], char name[][maxNameLength], int scores[], int newScore, Button btnSubmit, Button btnNo){
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 175, ALLEGRO_ALIGN_CENTER, "NEW HIGHSCORE!");
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 275, ALLEGRO_ALIGN_CENTER, "Your score: %d", newScore);
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 315, ALLEGRO_ALIGN_CENTER, "Ranking: %d", ranking(scores, newScore));

    al_draw_text(f[5], WHITE, SCREEN_W / 2, 375, ALLEGRO_ALIGN_CENTER, "Enter a name for your highscore:");
    drawBtn(btnSubmit, f);
    drawBtn(btnNo, f);
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

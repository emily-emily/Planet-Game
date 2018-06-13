/*Emily Yu
ICS3U P4
May 2018
Planets Project - ICS3U Summative*/

#include <allegro5/allegro.h>
#include <time.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "planets.h"

#include<stdio.h>

int main(int argc, char *argv[]){
    initializeAllegro();
    srand(time(0));

    //setup
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *q = nullptr;
    ALLEGRO_BITMAP *sprite[8] = {nullptr};
    ALLEGRO_BITMAP *mImage = nullptr;
    ALLEGRO_BITMAP *background = nullptr;
    ALLEGRO_BITMAP *planet = nullptr;
    ALLEGRO_BITMAP *box = nullptr;
    ALLEGRO_MOUSE_STATE mState;
    ALLEGRO_KEYBOARD_STATE kState;
    ALLEGRO_FONT *font[numFonts] = {nullptr};
    ALLEGRO_USTR *text = al_ustr_new("Player1");
    ALLEGRO_SAMPLE *musicTracks[numMusTracks] = {nullptr};
    ALLEGRO_SAMPLE *SFXTracks[numSFXTracks] = {nullptr};

    //create and load
    display = al_create_display(SCREEN_W, SCREEN_H);
    timer = al_create_timer(1.0/FPS);
    q = al_create_event_queue();
    sprite[0] = al_load_bitmap("images/characters/character1.png");
    sprite[1] = al_load_bitmap("images/characters/character2.png");
    sprite[2] = al_load_bitmap("images/characters/character3.png");
    sprite[3] = al_load_bitmap("images/characters/character4.png");
    sprite[4] = al_load_bitmap("images/characters/character5.png");
    sprite[5] = al_load_bitmap("images/characters/character6.png");
    sprite[6] = al_load_bitmap("images/characters/character7.png");
    sprite[7] = al_load_bitmap("images/characters/character8.png");
    mImage = al_load_bitmap("images/meteorV3.png");
    background = al_load_bitmap("images/background.png");
    planet = al_load_bitmap("images/planet.png");
    box = al_load_bitmap("images/box.png");
    for (int i = 0; i < numFonts; i++)
        font[i] = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 80 - 10 * i, 0);
    al_reserve_samples(numMusTracks + numSFXTracks);

    musicTracks[0] = al_load_sample("audio/Dystopic-Factory_Looping.wav");
    musicTracks[1] = al_load_sample("audio/Disco-Ants-Go-Clubbin_Looping.wav");
    musicTracks[2] = al_load_sample("audio/1_person_cheering-Jett_Rifkin-1851518140.wav");

    SFXTracks[0] = al_load_sample("audio/UI_Quirky19.wav");
    SFXTracks[1] = al_load_sample("audio/UI_Quirky31.wav");

    if (checkSetup(display, sprite, mImage, background, planet, box, timer, q, font, musicTracks, SFXTracks) != 0)
        return -1;

    al_set_window_title(display, "Planet Game");

    //register event sources
	al_register_event_source(q, al_get_timer_event_source(timer));
	al_register_event_source(q, al_get_display_event_source(display));
	al_register_event_source(q, al_get_keyboard_event_source());
	al_register_event_source(q, al_get_mouse_event_source());

    //declare objects
    Planet a;
    a.r = 150;
    a.x = SCREEN_W / 2;
    a.y = SCREEN_H / 2;

    Sprite s;
    s.xPos = a.x + a.r;
    s.yPos = a.y - a.r;
    s.xVel = 0.0;
    s.yVel = 0.0;
    s.shiftX = 0.0;
    s.shiftY = 0.0;
    s.dir = 1;
    s.airborne = false;

    Meteor m[maxMeteors];
    for (int i = 0; i < maxMeteors; i++){
        m[i].xPos = 0;
        m[i].yPos = 0;
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].w = al_get_bitmap_width(mImage);
        m[i].h = al_get_bitmap_height(mImage);
        m[i].available = true;
    }

    //buttons
    Button btnBack;
    strcpy(btnBack.text, "Back");
    setupBtn(btnBack, font, SCREEN_H - 125);

    //START
    Button btnSettings;
    strcpy(btnSettings.text, "");
    btnSettings.x1 = SCREEN_W - 150;
    btnSettings.x2 = btnSettings.x1 + 50;
    btnSettings.y1 = 100;
    btnSettings.y2 = btnSettings.y1 + 50;
    Button btnInstructions;
    strcpy(btnInstructions.text, "Instructions");
    setupBtn(btnInstructions, font, SCREEN_H - 370);
    Button btnCredits;
    strcpy(btnCredits.text, "Credits");
    setupBtn(btnCredits, font, SCREEN_H - 230);

    //SETTINGS
    Button btnMusicOn;
    strcpy(btnMusicOn.text, "ON");
    btnMusicOn.x1 = 340;
    btnMusicOn.x2 = btnMusicOn.x1 + al_get_text_width(font[5], btnMusicOn.text) + 20;
    btnMusicOn.y1 = 200;
    btnMusicOn.y2 = btnMusicOn.y1 + 50;

    Button btnMusicOff;
    strcpy(btnMusicOff.text, "OFF");
    btnMusicOff.x1 = btnMusicOn.x1 + 100;
    btnMusicOff.x2 = btnMusicOff.x1 + al_get_text_width(font[5], btnMusicOff.text) + 20;
    btnMusicOff.y1 = btnMusicOn.y1;
    btnMusicOff.y2 = btnMusicOn.y2;

    Button btnSFXOn;
    strcpy(btnSFXOn.text, "ON");
    btnSFXOn.x1 = 300;
    btnSFXOn.x2 = btnSFXOn.x1 + al_get_text_width(font[5], btnSFXOn.text) + 20;
    btnSFXOn.y1 = 380;
    btnSFXOn.y2 = btnSFXOn.y1 + 50;

    Button btnSFXOff;
    strcpy(btnSFXOff.text, "OFF");
    btnSFXOff.x1 = btnSFXOn.x1 + 100;
    btnSFXOff.x2 = btnSFXOff.x1 + al_get_text_width(font[5], btnSFXOff.text) + 20;
    btnSFXOff.y1 = btnSFXOn.y1;
    btnSFXOff.y2 = btnSFXOn.y2;

    //GAMEOVER
    Button btnHighscores;
    strcpy(btnHighscores.text, "Highscores");
    setupBtn(btnHighscores, font, SCREEN_H - 300);
    Button btnToMain;
    strcpy(btnToMain.text, "Main Menu");
    setupBtn(btnToMain, font, SCREEN_H - 230);
    Button btnExit;
    strcpy(btnExit.text, "Exit");
    setupBtn(btnExit, font, SCREEN_H - 160);

    //NEWHIGHSCORE
    Button btnSubmit;
    strcpy(btnSubmit.text, "Submit");
    setupBtn(btnSubmit, font, SCREEN_H - 180);
    Button btnNo;
    strcpy(btnNo.text, "No Thanks");
    setupBtn(btnNo, font, SCREEN_H - 120);

	//additional setup
    al_start_timer(timer);
    bool paused = false;
    int counter = 0; //counter used for meteor spawning, flashing text
    float meteorsPS = 2.0;
    float score = 0.0;
    bool running = true;
    //score
    char name[10][maxNameLength] = {""};
    int highscore[10] = {0};
    //screen
    Screen prevScr = START;
    Screen currentScr = START;

    //settings
    bool musicOn = true;
    bool SFXOn = true;
    int mVolume = 100; //mVolume 0 - 100
    int sVolume = 100;
    int mouseDrag = -1;

    al_play_sample(musicTracks[0], (float) mVolume / 100, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

    //game loop
    while (running){
        ALLEGRO_EVENT ev;

        al_get_mouse_state(&mState);
        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        switch (currentScr){
            //start screen
            case START:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    //draw layout and screen specific items
                    drawLayout(background, box, currentScr, font, score);
                    drawStart(font, btnSettings, btnInstructions, btnHighscores, btnCredits, counter);
                    al_flip_display();
                    counter = (counter + 1) % FPS;
                }
                //button click
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnSettings, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, SETTINGS);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnInstructions, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, INSTRUCTIONS);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnHighscores, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, HIGHSCORES);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnCredits, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, CREDITS);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                }
                //start game
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
                    reset(m, score);
                    counter = 0;
                    switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAME);
                }
                break;

            //instructions screen
            case INSTRUCTIONS:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    if (drawInstructions(display, font, btnBack) != 0)
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, START);
                    else al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, prevScr);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                }
                break;

            case SETTINGS:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawSettings(font, btnMusicOn, btnMusicOff, btnSFXOn, btnSFXOff, btnBack, musicOn, mVolume, SFXOn, sVolume);

                    //sliders
                    if (mouseDrag != -1 && al_mouse_button_down(&mState, 1)){
                        //find mouse position
                        int mX = 0;
                        int mY = 0;
                        al_get_mouse_cursor_position(&mX, &mY);
                        //change appropriate slider based on mouse location
                        switch (mouseDrag){
                            case 0:
                                mVolume = (mX - 550) / 8;
                                if (mVolume > 100)
                                    mVolume = 100;
                                if (mVolume < 0)
                                    mVolume = 0;
                                break;
                            case 1:
                                sVolume = (mX - 550) / 8;
                                if (sVolume > 100)
                                    sVolume = 100;
                                if (sVolume < 0)
                                    sVolume = 0;
                        }
                    }
                    al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnMusicOn, ev.mouse.x, ev.mouse.y)){
                        musicOn = true;
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnMusicOff, ev.mouse.x, ev.mouse.y)){
                        musicOn = false;
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnSFXOn, ev.mouse.x, ev.mouse.y)){
                        SFXOn = true;
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnSFXOff, ev.mouse.x, ev.mouse.y)){
                        SFXOn = false;
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, prevScr);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }

                    mouseDrag = mouseOnSlider(ev.mouse.x, ev.mouse.y, mVolume, sVolume);
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    mouseDrag = -1;
                break;

            //gameplay
            case GAME:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    //spawn a meteor n times per second
                    for (int i = 0; i < meteorsPS - 1; i++)
                        if (counter == (int) ((float)FPS / meteorsPS * i))
                            createMeteor(m, a);
                    //increase difficulty
                    meteorsPS += 0.1 / FPS;

                    //apply gravity
                    gravity(s, m, a);

                    //movement
                    if (al_key_down(&kState, ALLEGRO_KEY_UP))
                        jump(s, a, score);

                    if (al_key_down(&kState, ALLEGRO_KEY_LEFT))
                        shift(s, a, LEFT);
                    else if (al_key_down(&kState, ALLEGRO_KEY_RIGHT))
                        shift(s, a, RIGHT);
                    else shift(s, a, NONE);

                    //check sprite-meteor collision
                    for (int i = 0; i < maxMeteors; i++){
                        if (isCollision(s, al_get_bitmap_width(sprite[0]) * imageScale, al_get_bitmap_height(sprite[0]) * imageScale, m[i],
                                        al_get_bitmap_width(mImage) * imageScale, al_get_bitmap_height(mImage) * imageScale) && !m[i].available){
                            al_stop_samples();
                            //check for high score --> send to submission screen
                            if (getHighscores(display, name, highscore) == 0 && score > highscore[9]){
                                counter = 0;
                                switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, NEWHIGHSCORE);
                            }
                            else switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAMEOVER);
                        }
                    }

                    //update new object locations and draw
                    getNewCoordinates(s, m);
                    drawLayout(background, box, currentScr, font, score);
                    drawObjects(planet, a, s, sprite, counter, m, mImage);
                    al_flip_display();

                    //update variables
                    counter = (counter + 1) % FPS;
                    score += 10.0 / FPS;
                }

                if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if (ev.keyboard.keycode == ALLEGRO_KEY_P){
                        togglePause(timer, paused);
                        al_draw_scaled_bitmap(box, 0, 0, al_get_bitmap_width(box), al_get_bitmap_height(box), 0, 0, SCREEN_W, SCREEN_H, 0);
                        al_draw_text(font[2], WHITE, SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "GAME PAUSED");
                        al_flip_display();
                    }
                }

                if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){
                    paused = false;
                    togglePause(timer, paused);
                }
                break;

            //game over screen
            case GAMEOVER:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawGameOver(font, score, btnHighscores, btnToMain, btnExit);
                    al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnHighscores, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, HIGHSCORES);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    if (btnIsClicked(btnToMain, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, START);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnExit, ev.mouse.x, ev.mouse.y))
                        running = false;
                }
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, START);
                    }
                }
                break;

            //screen for user to enter a new high score
            case NEWHIGHSCORE:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawNewHighscore(font, name, highscore, score, box, text, counter, btnSubmit, btnNo);
                    al_flip_display();
                    counter = (counter + 1) % FPS;
                }
                if (ev.type == ALLEGRO_EVENT_KEY_CHAR){
                    if (ev.keyboard.keycode != ALLEGRO_KEY_ENTER && ev.keyboard.keycode != ALLEGRO_KEY_SPACE && ev.keyboard.keycode != ALLEGRO_KEY_BACKSPACE
                                    && ev.keyboard.keycode != ALLEGRO_KEY_UP && ev.keyboard.keycode != ALLEGRO_KEY_DOWN
                                    && ev.keyboard.keycode != ALLEGRO_KEY_LEFT && ev.keyboard.keycode != ALLEGRO_KEY_RIGHT
                                    && ev.keyboard.keycode != ALLEGRO_KEY_TAB)
                        al_ustr_append_chr(text, ev.keyboard.unichar);
                    //text must be 15 characters or less
                    if (al_ustr_length(text) > maxNameLength)
                        al_ustr_remove_chr(text, maxNameLength);
                    //backspace removes the last character
                    if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                        al_ustr_remove_chr(text, al_ustr_length(text) - 1);

                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        submitScore(name, highscore, al_cstr(text), score, display);
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAMEOVER);
                    }
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnNo, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAMEOVER);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                    if (btnIsClicked(btnSubmit, ev.mouse.x, ev.mouse.y)){
                        submitScore(name, highscore, al_cstr(text), score, display);
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAMEOVER);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                }
                break;

            //displays current highscores
            case HIGHSCORES:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    //if highscores file failed to load
                    if (drawHighscores(display, font, btnBack, name, highscore) != 0)
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, GAMEOVER);
                    else al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, prevScr);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                }
                break;

            case CREDITS:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    if (drawCredits(display, font, btnBack) != 0)
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, prevScr);
                    else al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y)){
                        switchScr(prevScr, currentScr, musicTracks, musicOn, mVolume, prevScr);
                        playSFX(SFXTracks[0], sVolume, SFXOn);
                    }
                }
                break;
        }

        //these things work all the time regardless of screen
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
    }

    //destroy and release data
    for (int i = 0; i < 8; i++)
        al_destroy_bitmap(sprite[i]);
    al_destroy_display(display);
    al_destroy_timer(timer);
    for (int i = 0; i < numFonts; i++)
        al_destroy_font(font[i]);
    for (int i = 0; i < numMusTracks; i++)
    al_destroy_sample(musicTracks[i]);

    return 0;
}

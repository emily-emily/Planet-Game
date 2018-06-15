/*Emily Yu
ICS3U P4
May-June 2018*/

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
//defined values
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define GRAY al_map_rgb(100, 100, 100)
#define RED al_map_rgb(255, 0, 0)

//constant values
const int SCREEN_W = 1200;
const int SCREEN_H = 700;

//gameplay
const int FPS = 60;
const int jumpVel = 800;
const int gVel = 40;
const int moveSpd = 200;
const int maxMeteors = 10;
const int minMeteorDistance = 100;
const float imageScale = 0.1;

//other
const int numFonts = 7;
const int numMusTracks = 3;
//had problems when this was 15; changed to 16
const int maxNameLength = 16;

//structures
struct Planet{
    int x;
    int y;
    int r;
};

struct Sprite{
    float xPos, yPos;
    float xVel, yVel;
    float shiftX, shiftY;
    int dir; //0 left, 1 right
    bool airborne;
};

struct Meteor{
    float xPos, yPos;
    float xVel, yVel;
    int w, h;
    bool available;
};

struct Button{
    int x1, y1;
    int x2, y2;
    char text[20];
};

enum Direction {NONE, LEFT, RIGHT};
enum Screen {START, INSTRUCTIONS, SETTINGS, GAME, GAMEOVER, NEWHIGHSCORE, HIGHSCORES, CREDITS};

//prototypes

//planetsAllegro
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *sprite[], ALLEGRO_BITMAP *mImage, ALLEGRO_BITMAP *background,
               ALLEGRO_BITMAP *planet, ALLEGRO_BITMAP *box, ALLEGRO_BITMAP *icon, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q,
               ALLEGRO_FONT *font[], ALLEGRO_SAMPLE *bgMusic[], ALLEGRO_SAMPLE *SFX);

//planetsMovement: everything movement related
void drawObjects(ALLEGRO_BITMAP *planet, Planet a, Sprite s, ALLEGRO_BITMAP *sprite[], int counter, Meteor m[], ALLEGRO_BITMAP *mImage);
void drawSprite(Planet a, Sprite s, ALLEGRO_BITMAP *sprite[], int frame);
void getNewCoordinates(Sprite &s, Meteor m[]);
void jump(Sprite &s, Planet a, float &score);
void shift(Sprite &s, Planet a, Direction spriteD);
bool isGrounded(Sprite s, Planet a);
bool misGrounded(Meteor m[], int i, Planet a);
void gravity(Sprite &s, Meteor m[], Planet a);
float rotateAngle(Sprite s, Planet a);
bool isCollision(Sprite p, int sw, int sh, Meteor m, int mw, int mh);

//planetsGameFeats: miscellaneous functions for buttons, meteors, highscores...
void setupBtn(Button &btn, ALLEGRO_FONT *f[], int y1);
void drawBtn(Button btn, ALLEGRO_FONT *f[], bool white);
void playSFX(ALLEGRO_SAMPLE *SFX, int volume, bool SFXOn);
int mouseOnSlider(int mX, int mY, int mVolume, int sVolume);
bool btnIsClicked(Button btn, int mouseX, int mouseY);
void togglePause(ALLEGRO_TIMER *timer, bool &paused);
void createMeteor(Meteor m[], Planet a);
void destroyMeteor(Meteor m[], int i);
void reset(Meteor m[], float &score);
int getHighscores(ALLEGRO_DISPLAY *display, char name[][maxNameLength], int score[]);
int ranking(int highscores[], float score);
void submitScore(char name[][maxNameLength], int highscore[], const char newName[], float newScore, ALLEGRO_DISPLAY *display);

//planetsDrawScr: draw screens
void switchScr(Screen &prevScr, Screen &currentScr, ALLEGRO_SAMPLE *tracks[], bool bgMusicOn, int volume, int &track, Screen newScr);
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *box, Screen scr, ALLEGRO_FONT *f[], float score);
void drawStart(ALLEGRO_FONT *f[], Button settings, ALLEGRO_BITMAP *icon, Button btn1, Button btn2, Button btn3, int iFlash);
int drawInstructions(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn);
void drawSettings(ALLEGRO_FONT *f[], Button M1, Button M2, Button S1, Button S2, Button btnBack, bool music, int mVol, bool SFXOn, int sVol);
void drawGameOver(ALLEGRO_FONT *f[], float score, Button btn1, Button btn2, Button btn3);
void drawNewHighscore(ALLEGRO_FONT *f[], char name[][maxNameLength], int scores[], int newScore, ALLEGRO_BITMAP *box,
                ALLEGRO_USTR *text, int counter, Button btnSubmit, Button btnNo);
int drawHighscores(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn, char name[][maxNameLength], int score[]);
int drawCredits(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn);

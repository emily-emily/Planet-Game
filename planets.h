//defined values
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define MAGENTA al_map_rgb(240, 50, 230)

//constant values
const int SCREEN_W = 640;
const int SCREEN_H = 480;

const int FPS = 30;
const int jumpVel = 400;
const int gVel = 20;

//structures
struct Circle{
    int x;
    int y;
    int r;
};

struct Person{
    float xPos, yPos;
    float xVel, yVel;
    char location[30];
};

//prototypes
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *character, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q);
void draw(Circle a, Person p, ALLEGRO_BITMAP *bitmap);
bool isGrounded(Person p, Circle c);
void getNewCoordinates(Person &p);
void jump(Person &p, Circle c);
void gravity(Person &p, Circle c);
float rotateAngle(Person p, Circle c);


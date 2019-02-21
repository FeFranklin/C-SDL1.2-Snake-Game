#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_ttf.h>

//GameSpace specifications
#define GAME_SPACE 460
#define CELL_SIZE 20
#define BORDER 10

//Window specifications
#define WINDOW_WIDTH 480
#define WINDOW_HEIGTH 640

//Snake
#define SNAKE_RADIUS 9
#define SNAKE_VELOCITY 20
    /*Snake segment*/
typedef struct Snake_elem{
    int x; int y;
    int vx; int vy;
    struct Snake_elem *next;
    int dir;
}Snake_elem;

//Apple
#define APPLE_RADIUS 7
typedef struct{
    int x; int y;
}Apple;

//Moving specs
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define UP 4
#define LEVEL_up 25
#define LEVEL 100

//Bottom of Screen specs
#define BTN_HEIGHT 60
#define BTN_WIDTH 180

//Colors
#define BACKGROUND_COLOR 0x8af793FF
#define GRID_COLOR 0x000000FF
#define BLACK 0x000000FF
#define APPLE_COLOR 0xF442D7FF
#define SNAKE_COLOR 0xF44242FF
#define BOTTOM_COLOR 0XFFFF00FF
#define PAUSE_BTN 0x00FFB6FF
#define RESET_BTN 0xF4BC42FF
#define LEVEL_BTN 0xC4C4FFFF
#define SCORE_BTN 0xC4FFFAFF
#define HOF_BTN   0xFFFFFFFF

/*---------------------GAME SECTION------------------------------------------------*/
//Function that draws the field of the game
void draw_field(SDL_Surface *field){
    boxColor(field,0,0, WINDOW_WIDTH, WINDOW_HEIGTH, SNAKE_COLOR);
    //draw the the game field
    boxColor(field, BORDER, 0, WINDOW_WIDTH-BORDER, GAME_SPACE, BACKGROUND_COLOR);
    //draw the lines of the field
    int i;
    for( i = 0; i <= GAME_SPACE; i=i+CELL_SIZE){
        hlineColor(field, BORDER, GAME_SPACE+BORDER, i, GRID_COLOR);
        vlineColor(field, i + BORDER, 0, GAME_SPACE, GRID_COLOR);
    }
}
//Function that draws the bottom section
void draw_bottom(SDL_Surface *field, TTF_Font *font){
    //Text general info
    SDL_Surface *textimg;
    SDL_Color text_img_color = {106, 0, 255};
    SDL_Rect where_to = { 0, 0, 0, 0 };
    //Bigger bottom box
    boxColor(field, BORDER, GAME_SPACE+BORDER, WINDOW_WIDTH-BORDER, WINDOW_HEIGTH-BORDER, BOTTOM_COLOR);
    //Buttons
        /*pause button*/
    boxColor(field, 2*BORDER, GAME_SPACE+2*BORDER, 2*BORDER+BTN_WIDTH, GAME_SPACE+2*BORDER+BTN_HEIGHT+5, PAUSE_BTN);
        //pause btn text
    textimg = TTF_RenderUTF8_Blended(font, "Press '3' to pause", text_img_color);
    where_to.x = 2*BORDER+5;
    where_to.y = GAME_SPACE+2*BORDER;
    SDL_BlitSurface(textimg, NULL, field, &where_to);
    textimg = TTF_RenderUTF8_Blended(font, "Press '3' to resume", text_img_color);
    where_to.y = GAME_SPACE+5*BORDER;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

        /*reset button*/
    boxColor(field, 2*BORDER, GAME_SPACE+4*BORDER+BTN_HEIGHT-5, 2*BORDER+BTN_WIDTH, WINDOW_HEIGTH-2*BORDER, RESET_BTN);
        //reset btn text
    textimg = TTF_RenderUTF8_Blended(font, "'SPACE' to (re)start", text_img_color);
    where_to.x = 2*BORDER+5;
    where_to.y = GAME_SPACE+4*BORDER+BTN_HEIGHT+15;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

        /*level button*/
    boxColor(field, 3*BORDER+BTN_WIDTH, GAME_SPACE+2*BORDER, 3*BORDER+2*BTN_WIDTH, GAME_SPACE+2*BORDER+BTN_HEIGHT+5, LEVEL_BTN);
        //level btn text
    textimg = TTF_RenderUTF8_Blended(font, "Press '2' to levelup", text_img_color);
    where_to.x = 3*BORDER+BTN_WIDTH+5;
    where_to.y = GAME_SPACE+2*BORDER+15;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

        /*hall of fame button*/
    boxColor(field, 3*BORDER+BTN_WIDTH, GAME_SPACE+4*BORDER+BTN_HEIGHT-5, 3*BORDER+2*BTN_WIDTH, WINDOW_HEIGTH-2*BORDER, HOF_BTN);
        //hall of fame text
    textimg = TTF_RenderUTF8_Blended(font, "Press '1' to HoF", text_img_color);
    where_to.x = 3*BORDER+BTN_WIDTH+5;
    where_to.y = GAME_SPACE+4*BORDER+BTN_HEIGHT+15;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

        /*--------------SCORE----------------*/
        /*score button*/
    boxColor(field, 4*BORDER+2*BTN_WIDTH, GAME_SPACE+2*BORDER, WINDOW_WIDTH-2*BORDER, WINDOW_HEIGTH-2*BORDER, SCORE_BTN);
        //score btn text
    textimg = TTF_RenderUTF8_Blended(font, "SCORE", text_img_color);
    where_to.x = 4*BORDER+2*BTN_WIDTH;
    where_to.y = GAME_SPACE+2*BORDER+15;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

    /*Free the textimg sdl object*/
    SDL_FreeSurface(textimg);
}
//Function do update the score, first it erases it and then draw the updated score
void score_Update(SDL_Surface *field, TTF_Font *font, int score){
    //Text general info
    SDL_Surface *textimg;
    SDL_Color text_img_color = {106, 0, 255};
    SDL_Rect where_to = { 0, 0, 0, 0 };

    //Converting from int score to string
    char score_s [10];
    sprintf(score_s, "%d", score);
    //Cleaning score before updating
    boxColor(field, 4*BORDER+2*BTN_WIDTH, GAME_SPACE+8*BORDER, WINDOW_WIDTH-2*BORDER, WINDOW_HEIGTH-2*BORDER, SCORE_BTN);

    //printing the new score
    textimg = TTF_RenderUTF8_Blended(font, score_s, text_img_color);
    where_to.x = 6*BORDER+2*BTN_WIDTH;
    where_to.y = GAME_SPACE+2*BORDER+75;
    SDL_BlitSurface(textimg, NULL, field, &where_to);

    /*Free the textimg sdl object*/
    SDL_FreeSurface(textimg);
}
//this function will be called by mytimer
//generates a user event and places it into the event queue (push)
Uint32 mytimer(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* wait this long till next call */
}
/*----------------------HALL OF FAME SECTION------------------------------------------------*/
//Function to show hall of fame in prompt
void show_Hall_of_Fame(FILE *file){
    //Creates a char data that will receive the string read
    char data[50];
    file = fopen("Hall_of_Fame", "r");
    //show if file could not be opened
    if(file==NULL)
        fprintf(stderr, "The Hall of Fame file could not be shown!\n");
    //Graphic division of what is the hall of fame
    printf("/------------------------------------------------------------------------/\n");
    printf("/-------------------HALL OF FAME-----------------------------------------/\n");
    printf("/------------------------------------------------------------------------/\n");

    //While it reads anything, print the information
     while(fgets(data, 50, file) != NULL)
         printf("%s", data);

    fclose(file);
}
//Function to insert the new score in the Hall of Fame
void insert_Hall_of_fame(FILE *file, int score){
    char name[10];

    file = fopen("Hall_of_Fame", "a");

    gets(name);
    printf("-----------\n");
    fprintf(file, "%s got score of: %d\n", name, score);

    fclose(file);
}
/*----------------------SNAKE SECTION------------------------------------------------------------*/
//Function to add the snake
Snake_elem *add_snake(Snake_elem *head){
    //Dynamic memory is used to create a new segment of the snake
    Snake_elem *newel = (Snake_elem *)malloc(sizeof(Snake_elem));
    //Auxiliary variable p created to traverse the list
    Snake_elem *p = head;
    //goes to the end of the list to add the new segment
    while(p->next != NULL)p = p->next;
    p->next = newel;
    newel->next = NULL;
    return head;
}
//function to change the direction of the snake's head
void set_head_dir(Snake_elem *head, int direction){
    //Set the snake's direction based on the keys pressed
    if(direction == UP && head->dir != UP && head->dir != DOWN){
        head->vx = 0;
        head->vy = -SNAKE_VELOCITY;
        head->dir = UP;
        return;
    }
    if(direction == RIGHT && head->dir != RIGHT && head->dir != LEFT){
        head->vx = SNAKE_VELOCITY;
        head->vy = 0;
        head->dir = RIGHT;
        return;
    }
    if(direction == DOWN && head->dir != DOWN && head->dir != UP){
        head->vx = 0;
        head->vy = SNAKE_VELOCITY;
        head->dir = DOWN;
        return;
    }
    if(direction == LEFT && head->dir != LEFT && head->dir != RIGHT){
        head->vx = -SNAKE_VELOCITY;
        head->vy = 0;
        head->dir = LEFT;
        return;
    }
}
//Function to delete the snake and free the memory when needed
Snake_elem *pop_front(Snake_elem *head){
	if (head != NULL) /* not empty */
	{
		Snake_elem *p = head;
		head = head->next;
		free(p);
	}
	return head;
}
void dispose_list(Snake_elem *head){
	while (head != NULL)
		head = pop_front(head);
}
//Function to verify if the snake has touched the wall
bool touch_wall(Snake_elem *cobra, SDL_TimerID id){
    //auxiliary variable to traverse the list
    Snake_elem *p = cobra;
    //Check if the snake's head touches the wall, if so, the game is lost and reseted to the beginning
    if(p->x < BORDER || p->x > GAME_SPACE+BORDER || p->y < 0 || p->y > GAME_SPACE){
        SDL_RemoveTimer(id);
        SDL_Delay(1000);
        return true;
    }
    return false;
}
//Function to move the snake based on the previous segment 'x' and 'y'
//The function is recursive so it calls until p is in the last segment of snake
//and temp is the previous one, the values from tmp are passed to 'p'
//and in the end, the snake's head position is changed based on the velocity
void Reverse(Snake_elem *p, Snake_elem *tmp){
    // Base case
    if (p == NULL)
       return;

    // print the list after head node
    Reverse(p->next, tmp->next);

    // After everything else is printed, print head
    p->x = tmp->x;
    p->y = tmp->y;
}
void move_snake(SDL_Surface *field, Snake_elem *cobra){
    //First, we erase all the snake without changing its position
    Snake_elem *p = cobra;

    while(p != NULL){
        filledCircleColor(field, p ->x, p ->y, SNAKE_RADIUS, BACKGROUND_COLOR);
        p = p->next;
    }
    //Passes the position of the segment in the front to the segment in behind
    Snake_elem *tmp = cobra;
    p = cobra;
    p = p->next;

    Reverse(p, tmp);

        /*calculates new position of the snake's head*/
    cobra ->x += cobra ->vx;
    cobra ->y += cobra ->vy;
        /*draw snake in new position*/

    p=cobra;
    while(p != NULL){
        if(p->x < BORDER || p->x > GAME_SPACE+BORDER || p->y < 0 || p->y > GAME_SPACE){
            p = p->next;
        } else {
            filledCircleColor(field, p ->x, p ->y, SNAKE_RADIUS, SNAKE_COLOR);
            p = p->next;
        }
    }
}
//Function to initialize the snake allocating memory and setting it to
//the initial position
Snake_elem *initialize(){
    Snake_elem *newel = (Snake_elem *)malloc(sizeof(Snake_elem));
    newel->x = BORDER+10+( 11 * 20 );
    newel->y = 10+( 11 * 20 );
    newel->vx = SNAKE_VELOCITY;
    newel->vy = 0;
    newel->dir = RIGHT;
    newel->next = NULL;
    return newel;
}
//Function called whenever the game is reseted, free the memory from previous
//snake and allocate memory for a new one and set it to starting position
Snake_elem *reset_snake(SDL_Surface *field, Snake_elem *cobra){
    Snake_elem *p = cobra;
    /*clear the snake from previous position*/
    while(p != NULL){
            if(p->x < BORDER || p->x > GAME_SPACE+BORDER || p->y < 0 || p->y > GAME_SPACE){
                p = p->next;
            } else {
                filledCircleColor(field, p ->x, p ->y, SNAKE_RADIUS, BACKGROUND_COLOR);
                p = p->next;
            }
        }
    p = cobra;
    /*Delete only from the 2 segment, the head of the snake is preserved*/
    dispose_list(p);
    /*Set snake to starting position*/
    Snake_elem *q = initialize();
    return q;
}
//Check if the snake touched itself
bool collision_with_snake(Snake_elem *cobra){
    Snake_elem *p = cobra;
    bool collision = false;
    //element p is always the next segment after the head
    //If the head's position is equal to any segment position, the game is lost
    p = cobra->next;

    while(p != NULL){
        if(cobra->x == p->x && cobra->y == p->y){
            collision = true;
        }
        p = p->next;
    }

    return collision;
}
/*---------------------APPLE SECTION------------------------------*/
//Function to place apple in random position
void place_apple(SDL_Surface *field, Apple *maca, Snake_elem *cobra){
    Snake_elem *p = cobra;
    //Generate random position for apple and then place it on the board
    (*maca).x = rand() % 23;
    (*maca).y = rand() % 23;

    //Check if the position of the apple is equal to any segment of the snake
    //if yes, a new position for the apple is placed
    while( p != NULL){
        if((p->x)/20 == (*maca).x+1 && ((p->y)+10)/20 == (*maca).y+1){
        (*maca).x = rand() % 23;
        (*maca).y = rand() % 23;
        }
        p = p->next;
    }
    filledCircleColor(field,((*maca).x * 20)+20, ((*maca).y*20)+10, APPLE_RADIUS, APPLE_COLOR);
}
int main(int argc, char *argv[]){
    #ifdef __WIN32__
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    #endif

    //Creating SDL surface
    SDL_Surface *screen;

    //Initialize the snake
    Snake_elem *cobra_head = initialize();

        /*Level*/
    int level;
    level = LEVEL;

        /*Apple*/
    Apple maca;
    bool apple_check;
    apple_check = false;
    //
    bool game = true;
    //Variable that checks if the snake touched the wall
    bool tWall;
    //Variable that check if the snake touched itself
    bool tSnake = false;
    //Variable to keep the game loop
    bool quit = false;
    //Variable to pause the game
    bool pause = false;
    //Score
    int score = 0;
    //Hall of Fame
    FILE *hof;

    //Creating SDL event and Timer to make the snake move
    SDL_Event event;
    SDL_TimerID id;

    //Bottom information
    TTF_Font *font;
    TTF_Init();
    font = TTF_OpenFont("PatuaOne-Regular.ttf", 19);
    if (!font) {
        fprintf(stderr, "The font could not be opened! %s\n", TTF_GetError());
        exit(1);
    }

    //Initializing SDL and open window
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGTH, 32, SDL_ANYFORMAT);
    if(!screen){
        fprintf(stderr, "The window could not be opened!\n");
        exit(1);
    }
    SDL_WM_SetCaption("Snake2711", "NULL");

    //Draw the field with snake in starting position
    draw_field(screen);
    draw_bottom(screen, font);

    //draw initial score
    score_Update(screen,font,score);

    SDL_Flip(screen);


    while(!quit){
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_USEREVENT:
            //Calls function to place apple
            //if there is an apple, apple_check is true and no other apple is placed
            //Otherwise, the function is called and an apple is positioned
            if(apple_check==false){
                place_apple(screen, &maca, cobra_head);
                apple_check = true;
            }
            //Verify if the next move is possible
            //It it isn't, the timer is removed and the game is paused
            //Calls the function to draw snake
            move_snake(screen, cobra_head);
            //Check if snake touched the wall
            tWall = touch_wall(cobra_head, id);
            //Check if snake touched itself
            tSnake = collision_with_snake(cobra_head);
            if(tWall == true || tSnake == true){
                SDL_RemoveTimer(id);
                //insert_Hall_of_fame(hof, score);
                cobra_head = reset_snake(screen, cobra_head);
                game = false;

            }
            //Now with the new position, it is checked if it has eaten an apple or not
            if( (cobra_head->x)/20 == maca.x+1 && ((cobra_head->y)+10)/20 == (maca.y)+1 ){
                cobra_head = add_snake(cobra_head);
                score = score + 1;
                score_Update(screen,font,score);
                apple_check = false;
            }
            SDL_Flip(screen);break;
        /*---------------------------------------------------------------------------------*/
        /*---------------------------------Keyboard events---------------------------------*/
        /*---------------------------------------------------------------------------------*/
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
            //When ESC button is pressed the program exits
            case SDLK_ESCAPE:
                quit = true;
                SDL_RemoveTimer(id);break;
            //Hall of Fame button
            case SDLK_1:
                if(pause == false){
                    SDL_RemoveTimer(id);
                    show_Hall_of_Fame(hof);
                    pause = true;
                }else{
                    id = SDL_AddTimer(level, mytimer, NULL);
                    pause = false;
                }break;
            //level button
            //Only 4 levels based on the timer
            //When pressed, the game resets with the new level
            case SDLK_2:
                score = 0;
                score_Update(screen, font, score);
                if(level >= 50){
                    level = level - LEVEL_up;
                    cobra_head = reset_snake(screen, cobra_head);
                    SDL_Flip(screen);
                    SDL_RemoveTimer(id);
                    game = true;
                } else {
                    level = LEVEL;
                }break;
            //Pause button
            //When pressed again, the game resumes
            case SDLK_3:
                if(pause == false){
                    SDL_RemoveTimer(id);
                    pause = true;
                }else{
                    id = SDL_AddTimer(level, mytimer, NULL);
                    pause = false;
                }break;
            //Restart button
            case SDLK_SPACE:
                if(game == false){
                    cobra_head = reset_snake(screen, cobra_head);
                    score = 0;
                    score_Update(screen, font, score);
                    game = true;break;
                }
                id = SDL_AddTimer(level, mytimer, NULL);
                game = false;break;
            /*------------------------------*/
            /*-----------MOVE KEYS----------*/
            /*------------------------------*/
            /* w key for going up */
            case SDLK_UP:
                set_head_dir(cobra_head, UP);break;
            /* a key for going left*/
            case SDLK_LEFT:
                set_head_dir(cobra_head, LEFT);break;
            /* s key for going down*/
            case SDLK_DOWN:
                set_head_dir(cobra_head, DOWN);break;
            /* d key for going right*/
            case SDLK_RIGHT:
                set_head_dir(cobra_head, RIGHT);break;
            }
            break;
        }
    }
    //Deletes the timer
    SDL_RemoveTimer(id);
    //Deletes the memory used by the font
    TTF_CloseFont(font);
    //Quits SDL
    SDL_Quit();
    //Free the memory allocated for the snake
    dispose_list(cobra_head);
    return 0;
}

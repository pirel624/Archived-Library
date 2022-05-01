/*
Error:
1. i cant get the rendering with source_rectangle and destination_recatangle quite right
   But if choose to simply use NULL, everything works
Solution:
1. it turned out i need to initialize source_rectangle x and y. bcs apparently default value of int is not 0. So i assign it manually
*/

/*
TO DO:

1. Fix Bullet Class
2. Fix Vector2 Class
3. Why cant my bullet strafe properly ?
*/

/*
Clues:
1. Operation Priority
2. Unintended implicit conversion, reducing accuracy and maybe change the value altogether
3. try using static_cast or floating point literal
4. there is a possibility of sequantiality being importang in AgeBullet() function.
5. floating point truncation !!!!
    0 - (some float number thats less than zero) = -0.787878 >> truncated >> -0 = 0
    This shit will mess your trajectorial movement
    Case in point, AgeBullet() function
*/
/*
Observation:
1. bullet trajectory pivot on 4 orthogonal direction (0, 90, 180, 270)
2. Sometimes, for some reasons, the x and y position of bullet stop at 0
*/


#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <string>


const double PI = 3.14159265;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


struct sprite
{
    SDL_Texture* pTexture = NULL;
    SDL_Rect SourceRectangle;
    SDL_Rect DestinationRectangle;
};

struct keystate
{
    bool Pressed_A = false;
    bool Pressed_S = false;
    bool Pressed_D = false;
    bool Pressed_W = false;
    bool Pressed_ESC = false;

    void reset()
    {
        Pressed_A = false;
        Pressed_S = false;
        Pressed_D = false;
        Pressed_W = false;
        Pressed_ESC = false;
    }
};


class Vector2           // # Built for Precision
{
    private:
    double x = 0;
    double y = 0;
    bool IsNormalized = false;

    public:
    Vector2()
    {
        // do nothing
    }

    void normalize()
    {
        double length;
        length = std::sqrt((x * x) + (y * y));
        x /= length;
        y /= length;
        IsNormalized = true;
    }

    double GetX(){return x;}
    double GetY(){return y;}
    bool IsNormal(){return IsNormalized;}

    void ChangeX(double value)
    {
        x = value;
        IsNormalized = false;
    }

    void ChangeY(double value)
    {
        y = value;
        IsNormalized = false;
    }
};


class bullet
{
    private:
    signed int PosX;
    signed int PosY;
    Vector2 DirectionVector;
    int Speed;                                  // pixel per milisecond
    int Lifespan;                               // in milisecond

    public:

    bullet(int X = 300, int Y = 300, int speed = 10, int lifespan = 3000, int direction_degree = 15)
    {
        PosX = X;
        PosY = Y;
        Speed = speed;
        Lifespan = lifespan;
        DirectionVector.ChangeX(std::cos((static_cast<float>(direction_degree) / 180.0f) * PI));              // USE FLOATING POINT LITERAL!!!
        DirectionVector.ChangeY(-(std::sin((static_cast<float>(direction_degree) / 180.0f) * PI)));
        DirectionVector.normalize();
    }

    void AgeBullet(int time)    /////////////////////////////////// This Function might be broken ////////////
    {
        // Change position according to speed and trajectory and time changes
        PosX = PosX + (DirectionVector.GetX() * static_cast<float>(Speed ) * static_cast<float>(time));
        PosY = PosY + (DirectionVector.GetY() * static_cast<float>(Speed ) * static_cast<float>(time));

        // Reduce Lifespan
        Lifespan -= time;
    }

    int GetX(){return PosX;}
    int GetY(){return PosY;}
    int GetLifespan(){return Lifespan;}
    double GetDirectionVectorX()
    {
        return DirectionVector.GetX();
    }
    double GetDirectionVectorY()
    {
        return DirectionVector.GetY();
    }
};

bool LoadTexture(std::string FileName, std::vector<SDL_Texture*> &TextureCache, SDL_Renderer* renderer)
{
    bool IsLoaded1 = false;
    bool IsLoaded2 = false;
    SDL_Surface* canvas;
    SDL_Texture* texture;

    canvas = SDL_LoadBMP(FileName.c_str());
    if(canvas != NULL){IsLoaded1 = true;}

    texture = SDL_CreateTextureFromSurface(renderer, canvas);
    if(texture != NULL){IsLoaded2 = true;}

    SDL_FreeSurface(canvas);

    if(IsLoaded1 && IsLoaded2)
    {
        TextureCache.push_back(texture);
        return true;
    }
    else{return false;}
}


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* pTemporarySurface = NULL;
SDL_Event EventBuffer;

int main(int argc, char* args[])
{
    // Initializing sdl, windows, and rendering context
    bool IsInitialized = false;
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        IsInitialized = true;
        window = SDL_CreateWindow("Pirel's Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
        if(window == NULL)
        {
            printf("Cannot Initialize Window\n");
            IsInitialized = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
    }

    if(IsInitialized)        // Start The Game
    {
        // Initialize and load game asset
        keystate KeyboardInput;
        const Uint8* KeyboardState = SDL_GetKeyboardState(NULL);

        std::vector<SDL_Texture*> TextureCache;

        while(!LoadTexture("Wastelander.bmp", TextureCache, renderer)){printf("loading texture\n");}

        sprite image;
        image.pTexture = TextureCache[0];        
        SDL_QueryTexture(image.pTexture, NULL, NULL, &image.SourceRectangle.w, &image.SourceRectangle.h);
        image.SourceRectangle.x = 0;
        image.SourceRectangle.y = 0;
        image.DestinationRectangle.h = image.SourceRectangle.h;
        image.DestinationRectangle.w = image.SourceRectangle.w;
        image.DestinationRectangle.x = 0;
        image.DestinationRectangle.y = 0;

        bullet peluru;

        sprite Bullet;
        Bullet.pTexture = TextureCache[0];
        SDL_QueryTexture(Bullet.pTexture, NULL, NULL, &Bullet.SourceRectangle.w, &Bullet.SourceRectangle.h);
        Bullet.SourceRectangle.x = 0;
        Bullet.SourceRectangle.y = 0;
        Bullet.DestinationRectangle.h = image.SourceRectangle.h;
        Bullet.DestinationRectangle.w = image.SourceRectangle.w;
        Bullet.DestinationRectangle.x = 100;
        Bullet.DestinationRectangle.y = 100;

        // game loop
        while(true)
        {
            SDL_PumpEvents();       // Update KeyboardState
            KeyboardInput.reset();  // Reset KeyboardInput data
            if(KeyboardState[SDL_SCANCODE_W]){KeyboardInput.Pressed_W = true;}
            if(KeyboardState[SDL_SCANCODE_A]){KeyboardInput.Pressed_A = true;}
            if(KeyboardState[SDL_SCANCODE_S]){KeyboardInput.Pressed_S = true;}
            if(KeyboardState[SDL_SCANCODE_D]){KeyboardInput.Pressed_D = true;}
            if(KeyboardState[SDL_SCANCODE_ESCAPE]){KeyboardInput.Pressed_ESC = true;}

            if(KeyboardInput.Pressed_W){image.DestinationRectangle.y -= 10;}
            if(KeyboardInput.Pressed_A){image.DestinationRectangle.x -= 10;}
            if(KeyboardInput.Pressed_S){image.DestinationRectangle.y += 10;}
            if(KeyboardInput.Pressed_D){image.DestinationRectangle.x += 10;}
            Bullet.DestinationRectangle.x = peluru.GetX();
            Bullet.DestinationRectangle.y = peluru.GetY();
            peluru.AgeBullet(1);
            std::cout << "X Direction = " << peluru.GetDirectionVectorX() << std::endl;
            std::cout << "Y Direction = " << peluru.GetDirectionVectorY() << std::endl;
            std::cout << "X Position = " << peluru.GetX() << std::endl;
            std::cout << "Y Position = " << peluru.GetY() << std::endl;
            std::cout << std::cos(((3.0f / 2.0f) * PI)) << std::endl;
            

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, image.pTexture, &image.SourceRectangle, &image.DestinationRectangle);
            SDL_RenderCopy(renderer, Bullet.pTexture, &Bullet.SourceRectangle, &Bullet.DestinationRectangle);
            SDL_RenderPresent(renderer);
            SDL_Delay(10);

            if(KeyboardInput.Pressed_ESC){break;}    // when escape is pressed, the main game loop is broken and quit is initialized
        }
    }

    ///////////// Quit the Game //////////

    // Free the RAM from all thing SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

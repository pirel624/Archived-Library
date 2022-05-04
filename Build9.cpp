/*
Error:
1. i cant get the rendering with source_rectangle and destination_recatangle quite right
   But if choose to simply use NULL, everything works
Solution:
1. it turned out i need to initialize source_rectangle x and y. bcs apparently default value of int is not 0. So i assign it manually
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
6. Be careful with the detail !!
    I broke my function because it got passed a copied value, not a reference.......
*/
/*
TO DO:
1. implement hitboxes and collision detection
2. implement multidimensional cache keying
3. refactor everything
    unit test sprite_map and bullet_map class
*/



#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <iterator>


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
    float PosX;      // i use float here so that AgeBullet() can function propwrly. If i were to use int, miniscule change wont be detected because of how floating point truncation and implicit convertion works, it would result to the coordinate stuck at 0,0
    float PosY;
    Vector2 DirectionVector;
    int Speed;                                  // pixel per milisecond

    public:

    bullet(int X = 300, int Y = 300, int speed = 1, int direction_degree = 135)
    {
        PosX = X;
        PosY = Y;
        Speed = speed;
        DirectionVector.ChangeX(std::cos((static_cast<float>(direction_degree) / 180.0f) * PI));              // USE FLOATING POINT LITERAL!!!
        DirectionVector.ChangeY(-(std::sin((static_cast<float>(direction_degree) / 180.0f) * PI)));
        DirectionVector.normalize();
    }

    void AgeBullet(int time)    /////////////////////////////////// This Function might be broken ////////////
    {
        // Change position according to speed and trajectory and time changes
        PosX = PosX + (DirectionVector.GetX() * static_cast<float>(Speed ) * static_cast<float>(time));
        PosY = PosY + (DirectionVector.GetY() * static_cast<float>(Speed ) * static_cast<float>(time));
    }

    int GetX(){return static_cast<int>(PosX);}
    int GetY(){return static_cast<int>(PosY);}
    double GetDirectionVectorX()
    {
        return DirectionVector.GetX();
    }
    double GetDirectionVectorY()
    {
        return DirectionVector.GetY();
    }
};

// Mapping model will be based on one to one correlation, no key will be duplicated
class sprite_map
{
    struct pair
    {
        int Key;
        int Dimension;
        sprite Sprite;
        int lifespan_in_miliseconds = 1000;
    };

    private:
    std::vector<pair> container;

    public:

    // insert a key-value pair, note that overlapping key is not possible
    bool insert(int key, int Dimension, sprite Sprite, int lifespan = 1000)
    {
        bool duplicated = false;
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == key && container[n].Dimension == Dimension){duplicated = true; break;}
        }

        if(!duplicated)
        {
            pair buffer;
            buffer.Key = key;
            buffer.Dimension = Dimension;
            buffer.Sprite = Sprite;
            buffer.lifespan_in_miliseconds = lifespan;
            container.push_back(buffer);
        }

        return !duplicated;
    }

    // Find all sprites that associate with "key" key
    pair* find(int Key, int Dimension)
    {
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == Key && container[n].Dimension == Dimension)
            {
                printf("container returned\n");
                return &container[n];     // return pointer of the associated pair (based on key)
            }
            else
            {
                // do nothing
                printf("Iteration\n");
            }
        }
        // if pair pointer cant be returned, return null pointer
        printf("container not returned\n");
        return nullptr;
    }

    // Return true if erasement succeed, because maybe there is(are) another element that can be erased. Use while control flow to delete all associated element
    bool erase(int Key, int Dimension)
    {
        bool is_successful = false;
        std::vector<pair>::iterator iterator;

        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == Key && container[n].Dimension == Dimension)
            {
                iterator = container.begin();
                for(int m = 0; m < n; m++)
                {
                    iterator++;
                }
                container.erase(iterator);
                is_successful = true;
                break;   // Stop iteration, one erasementis enough
            }
            else
            {
                // do nothing
            }
        }

        return is_successful;
    }

    pair* iterative_access(int n = 0)
    {
        return &container[n];
    }

    int get_size()
    {
        return container.size();
    }

    void age_cache(int time_ms)
    {
        struct int2{int x; int y;};
        std::vector<int2> dimension_key_pair_bucket;

        // reduce lifespan
        for(int n = 0; n < container.size(); n++)
        {
            container[n].lifespan_in_miliseconds -= time_ms;
            if(container[n].lifespan_in_miliseconds <= 0)
            {
                int2 buffer;
                buffer.x = container[n].Dimension;
                buffer.y = container[n].Key;
                dimension_key_pair_bucket.push_back(buffer);
            }
            else
            {
                // do nothing
            }
        }

        // Erase dying sprite (lifespan <= 0)
        for(int n = 0; n < dimension_key_pair_bucket.size(); n++)
        {
            erase(dimension_key_pair_bucket[n].y, dimension_key_pair_bucket[n].x);
        }
    }

    bool load_sprite(SDL_Texture* ptexture, int key, int Dimension, int lifespan = 1000)
    {
        sprite buffer_sprite;
        buffer_sprite.pTexture = ptexture;
        SDL_QueryTexture(ptexture, NULL, NULL, &buffer_sprite.SourceRectangle.w, &buffer_sprite.SourceRectangle.h);
        buffer_sprite.SourceRectangle.x = 0;
        buffer_sprite.SourceRectangle.y = 0;
        buffer_sprite.DestinationRectangle.w = buffer_sprite.SourceRectangle.w;
        buffer_sprite.DestinationRectangle.h = buffer_sprite.SourceRectangle.h;
        buffer_sprite.DestinationRectangle.x = 10;
        buffer_sprite.DestinationRectangle.y = 10;
        insert(key, Dimension, buffer_sprite, lifespan);
        return true;
    }
};

// Mapping model will be based on one to one correlation, no key will be duplicated
class bullet_map
{
   struct pair
    {
        int Key;
        int Dimension;
        bullet Bullet;
        int lifespan_in_miliseconds = 1000;
    };

    private:
    std::vector<pair> container;

    public:

    // insert a key-value pair, note that overlapping key is not possible
    bool insert(int key, int dimension, bullet buffer_bullet, int lifespan = 1000)
    {
        bool duplicated = false;
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == key && container[n].Dimension == dimension){duplicated = true; break;}
        }

        if(!duplicated)
        {
            pair buffer;
            buffer.Key = key;
            buffer.Dimension = dimension;
            buffer.Bullet = buffer_bullet;
            buffer.lifespan_in_miliseconds = lifespan;
            container.push_back(buffer);
        }

        return !duplicated;
    }

    // Find all sprites that associate with "key" key
    pair* find(int Key, int Dimension)
    {
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == Key && container[n].Dimension == Dimension)
            {
                return &container[n];     // return pointer of the associated pair (based on key)
            }
            else
            {
                // do nothing
            }
        }
        // if pair pointer cant be returned, return null pointer
        printf("container not returned\n");
        return nullptr;
    }

    bool erase(int Key, int Dimension)
    {
        bool is_successful = false;
        std::vector<pair>::iterator iterator;

        for(int n = 0; n < container.size(); n++)
        {
            if(container[n].Key == Key && container[n].Dimension == Dimension)
            {
                iterator = container.begin();
                for(int m = 0; m < n; m++)
                {
                    iterator++;
                }
                container.erase(iterator);
                is_successful = true;
                break;   // Stop iteration, one erasementis enough
            }
            else
            {
                // do nothing
            }
        }

        return is_successful;
    }

    pair* iterative_access(int n = 0)
    {
        return &container[n];
    }

    int get_size()
    {
        return container.size();
    }

    void age_cache(int time_ms)
    {
        struct int2{int x; int y;};
        std::vector<int2> dimension_key_pair_bucket;

        // reduce lifespan
        for(int n = 0; n < container.size(); n++)
        {
            container[n].lifespan_in_miliseconds -= time_ms;
            if(container[n].lifespan_in_miliseconds <= 0)
            {
                int2 buffer;
                buffer.x = container[n].Dimension;
                buffer.y = container[n].Key;
                dimension_key_pair_bucket.push_back(buffer);
            }
            else
            {
                // do nothing
            }
        }

        // Erase dying bullet (lifespan <= 0)
        for(int n = 0; n < dimension_key_pair_bucket.size(); n++)
        {
            erase(dimension_key_pair_bucket[n].y, dimension_key_pair_bucket[n].x);
        }
    }

    bool load_bullet(int Key, int Dimension, int X = 100, int Y = 100, int Speed = 1, int Direction = 135, int Lifespan = 1000)
    {
        bullet bullet_buffer(X, Y, Speed, Direction);
        insert(Key, Dimension, bullet_buffer, Lifespan);
        return true;
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

class KeyLabel
{
    public:
    static const int Player1 = 1;
    static const int Player2 = 2;
};

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
        window = SDL_CreateWindow("Pirel's Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
        sprite_map SpriteCache;
        bullet_map BulletCache;

        while(!LoadTexture("Wastelander.bmp", TextureCache, renderer)){printf("loading texture\n");} 

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

            if(KeyboardInput.Pressed_W){}
            if(KeyboardInput.Pressed_A){}
            if(KeyboardInput.Pressed_S){}
            if(KeyboardInput.Pressed_D){}

            SDL_RenderClear(renderer);
            for(int n = 0; n < SpriteCache.get_size(); n++)
            {
                SDL_RenderCopy(renderer,
                SpriteCache.iterative_access(n)->Sprite.pTexture,
                &SpriteCache.iterative_access(n)->Sprite.SourceRectangle,
                &SpriteCache.iterative_access(n)->Sprite.DestinationRectangle);
            }
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

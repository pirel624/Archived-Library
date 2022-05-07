/*
Error:
1. i cant get the rendering with source_rectangle and destination_recatangle quite right
   But if choose to simply use NULL, everything works
2. The current program just crashes
3. sprite_map::load_sprite() doesnt work
4. Calling sprite_map::load_sprite() will always result in crashes, no matter if you set any breakpoint in it
5. hitbox::GetCenterX() and hitbox::GetCenterY() is broken
Solution:
1. it turned out i need to initialize source_rectangle x and y. bcs apparently default value of int is not 0. So i assign it manually
2, 3, 4. it crashes before the sprite loading call were done before the texture loading, so undefined behaviour follows.
5. I rewrote the fucking class
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
1. unit test hitbox cache
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
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 760;



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
    bool Pressed_RSHIFT = false;
    bool Pressed_ESC = false;

    void reset()
    {
        Pressed_A = false;
        Pressed_S = false;
        Pressed_D = false;
        Pressed_W = false;
        Pressed_RSHIFT = false;
        Pressed_ESC = false;
    }
};



class hitbox
{
    private:
    int CenterX;
    int CenterY;
    int Width;
    int Height;
    char Mode;

    public:

    hitbox(int X = 100, int Y = 100, int W = 30, int H = 30, char mode = 'C')
    {
        CenterX = X;
        CenterY = Y;
        Width = W;
        Height = H;
        Mode = mode;
    }

    void SetCenterX(int value){CenterX = value;}
    void SetCenterY(int value){CenterY = value;}
    void SetWidth(int value){Width = value;}
    void SetHeight(int value){Height = value;}
    void SetRectangular(){Mode = 'R';}
    void SetCircular(){Mode = 'C';}

    void MoveX(int value){CenterX += value;}
    void MoveY(int value){CenterY += value;}
    void StretchWidth(int value){Width += value;}
    void StretchHeight(int value){Height += value;}

    int GetCenterX(){return CenterX;}
    int GetCenterY(){return CenterY;}
    int GetWidth(){return Width;}
    int GetHeight(){return Height;}
    char GetMode(){return Mode;}

    SDL_Rect GetSDLRect()
    {
        SDL_Rect buffer;
        buffer.x = CenterX - (Width / 2);
        buffer.y = CenterY - (Height / 2);
        buffer.h = Height;
        buffer.w = Width;
        return buffer;
    }

    int GetSDLRectX(){return CenterX - (Width / 2);}
    int GetSDLRectY(){return CenterY - (Height / 2);}
    int GetSDLRectW(){return Width;}
    int GetSDLRectH(){return Height;}

    void MoldAgainstSprite(sprite Sprite)
    {
        Height = Sprite.DestinationRectangle.h;
        Width = Sprite.DestinationRectangle.w;
        CenterX = Sprite.DestinationRectangle.x + (Width / 2);
        CenterY = Sprite.DestinationRectangle.y + (Height / 2);
    }

    static bool IsColliding(hitbox shape1, hitbox shape2)
    {
        if(shape1.GetMode() == 'C' && shape2.GetMode() == 'C')
        {
            float distance =
            std::sqrt
            (
                (shape1.GetCenterX() - shape2.GetCenterX())*
                (shape1.GetCenterX() - shape2.GetCenterX())+
                (shape1.GetCenterY() - shape2.GetCenterY())*
                (shape1.GetCenterY() - shape2.GetCenterY())
            );

            return (distance <= (shape1.GetWidth() + shape2.GetWidth()));
        }
        else if(shape1.GetMode() == 'C' && shape2.GetMode() == 'R')
        {
            return false;
        }
        else if(shape1.GetMode() == 'R' && shape2.GetMode() == 'C')
        {
            return false;
        }
        else if(shape1.GetMode() == 'R' && shape2.GetMode() == 'R')
        {
            return false;
        }
        else
        {
            return false;
        }
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
    float Speed;                                  // pixel per milisecond

    public:

    bullet(int X = 300, int Y = 300, float speed = 1, int direction_degree = 135)
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
                return &container[n];     // return pointer of the associated pair (based on key)
            }
            else
            {
                // do nothing
            }
        }
        // if pair pointer cant be returned, return null pointer
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
            container[n].Bullet.AgeBullet(time_ms);
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

    bool load_bullet(int Key, int Dimension, int X = 100, int Y = 100, float Speed = 1, int Direction = 135, int Lifespan = 1000)
    {
        bullet bullet_buffer(X, Y, Speed, Direction);
        insert(Key, Dimension, bullet_buffer, Lifespan);
        return true;
    }
};



// Mapping model will be based on one to one correlation, no key will be duplicated
class hitbox_map
{
    struct pair
    {
        int Key;
        int Dimension;
        hitbox Hitbox;
        int lifespan_in_miliseconds = 1000;
    };

    private:
    std::vector<pair> container;

    public:

    // insert a key-value pair, note that overlapping key is not possible
    bool insert(int key, int Dimension, hitbox Hitbox, int lifespan = 1000)
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
            buffer.lifespan_in_miliseconds = lifespan;
            buffer.Hitbox = Hitbox;
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

    bool load_hitbox(int Key, int Dimension, int Lifespan = 1000, int CenterX = 100, int CenterY = 100, int Width = 100, int Height = 100, char Mode = 'R')
    {
        hitbox buffer_hitbox(CenterX, CenterY, Width, Height, Mode);
        insert(Key, Dimension, buffer_hitbox, Lifespan);
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
        sprite_map SpriteCache;
        bullet_map BulletCache;
        hitbox_map HitboxCache;

        while(!LoadTexture("Wastelander.bmp", TextureCache, renderer)){printf("loading texture\n");} 

        SpriteCache.load_sprite(TextureCache[0], 1, 1, 1000000000);
        HitboxCache.load_hitbox(1, 1, 1000000000);
        HitboxCache.find(1, 1)->Hitbox.MoldAgainstSprite(SpriteCache.find(1, 1)->Sprite);

        std::cout << "Sprite X = " << SpriteCache.find(1,1)->Sprite.DestinationRectangle.x << std::endl;
        std::cout << "Sprite Y = " << SpriteCache.find(1,1)->Sprite.DestinationRectangle.y << std::endl;
        std::cout << "Sprite W = " << SpriteCache.find(1,1)->Sprite.DestinationRectangle.w << std::endl;
        std::cout << "Sprite H = " << SpriteCache.find(1,1)->Sprite.DestinationRectangle.h << std::endl;
        std::cout << std::endl;
        std::cout << "Hitbox X = " << HitboxCache.find(1, 1)->Hitbox.GetCenterX() << std::endl;
        std::cout << "Hitbox Y = " << HitboxCache.find(1, 1)->Hitbox.GetCenterY() << std::endl;
        std::cout << "Hitbox W = " << HitboxCache.find(1, 1)->Hitbox.GetWidth() << std::endl;
        std::cout << "Hitbox H = " << HitboxCache.find(1, 1)->Hitbox.GetHeight() << std::endl;
        std::cout << std::endl;
        std::cout << "Hitbox SDL_Rect X = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRect().x << std::endl;
        std::cout << "Hitbox SDL_Rect Y = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRect().y << std::endl;
        std::cout << "Hitbox SDL_Rect W = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRect().w << std::endl;
        std::cout << "Hitbox SDL_Rect H = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRect().h << std::endl;
        std::cout << std::endl;
        std::cout << "Hitbox SDL_Rect X = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRectX() << std::endl;
        std::cout << "Hitbox SDL_Rect Y = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRectY() << std::endl;
        std::cout << "Hitbox SDL_Rect W = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRectW() << std::endl;
        std::cout << "Hitbox SDL_Rect H = " << HitboxCache.find(1, 1)->Hitbox.GetSDLRectH() << std::endl;

        while(true){}

        /*
        // game loop
        while(true)
        {
            SDL_PumpEvents();       // Update KeyboardState
            KeyboardInput.reset();  // Reset KeyboardInput data
            if(KeyboardState[SDL_SCANCODE_W]){KeyboardInput.Pressed_W = true;}
            if(KeyboardState[SDL_SCANCODE_A]){KeyboardInput.Pressed_A = true;}
            if(KeyboardState[SDL_SCANCODE_S]){KeyboardInput.Pressed_S = true;}
            if(KeyboardState[SDL_SCANCODE_D]){KeyboardInput.Pressed_D = true;}
            if(KeyboardState[SDL_SCANCODE_RSHIFT]){KeyboardInput.Pressed_RSHIFT = true;}
            if(KeyboardState[SDL_SCANCODE_ESCAPE]){KeyboardInput.Pressed_ESC = true;}

            BulletCache.age_cache(10);
            SpriteCache.age_cache(10);
            if(KeyboardInput.Pressed_W){SpriteCache.find(2, 1)->Sprite.DestinationRectangle.y -= 10;}
            if(KeyboardInput.Pressed_A){SpriteCache.find(2, 1)->Sprite.DestinationRectangle.x -= 10;}
            if(KeyboardInput.Pressed_S){SpriteCache.find(2, 1)->Sprite.DestinationRectangle.y += 10;}
            if(KeyboardInput.Pressed_D){SpriteCache.find(2, 1)->Sprite.DestinationRectangle.x += 10;}

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
        */
    }

    ///////////// Quit the Game //////////

    // Free the RAM from all thing SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

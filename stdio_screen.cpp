#include <iostream>

class matrix
{
    public:
    static const int X_SIZE = 100;
    static const int Y_SIZE = 50;

    private:
    char character_map[X_SIZE][Y_SIZE];

    public:

    matrix()
    {
        for(int y = 0; y < Y_SIZE; y++)
        {
            for(int x = 0; x < X_SIZE; x++)
            {
                character_map[x][y] = ' ';
            }
        }
    }

    void clean()
    {
        for(int y = 0; y < Y_SIZE; y++)
        {
            for(int x = 0; x < X_SIZE; x++)
            {
                character_map[x][y] = ' ';
            }
        }
    }

    void display()
    {
        //Upper Border
        for(int up = 0; up < X_SIZE + 2; up++)
        {
            std::cout << '#';
        }
        std::cout << '\n';

        //Body
        for(int y = 0; y < Y_SIZE; y++)
        {
            std::cout << '#';                       //Left Border
            for(int x = 0; x < X_SIZE; x++)
            {
				std::cout << character_map[x][y];
			}
            std::cout << '#';                       //Right Border
			std::cout << '\n';                      //New Line
		}

        //Lower Border
        for(int down = 0; down < X_SIZE + 2; down++)
        {
            std::cout << '#';
        }
        std::cout << '\n';
    }

    bool edit(int pos_x, int pos_y, char character)
    {
        if(pos_x <= X_SIZE && pos_y <= Y_SIZE && pos_x >= 1 && pos_y >= 1)
        {
            character_map[pos_x - 1][pos_y - 1] = character;
            return true;
        }else{return false;}
    }

    static void window_clear_screen()
    {
        system("cls");
    }

};


int main()
{
	matrix screen;
	screen.edit(2, 3, '@');
	while(true)
	{
		matrix::window_clear_screen();
		//screen.clean();
		screen.display();
	}

	return 0;
}





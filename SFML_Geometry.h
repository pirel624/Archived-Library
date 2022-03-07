struct Point//////////////////////////////////////////////////////////////////////////////////////////////////// APPROVED ////////////////////////////////////////////////////////////
{
	private:
		bool Cartesian = true;
	public:	
		int ScreenHeight = 600;
		int PosX = 50;
		int PosY = 50;
		void Convert()  // Convert Cartesian Coordinate to Screen Coordinate, and Vice Versa
		{
			if(Cartesian)
			{
				PosY = (ScreenHeight - PosY);    
				Cartesian = !Cartesian;
			}
			else if(!Cartesian)
			{
				PosY = (ScreenHeight - PosY);
				Cartesian = !Cartesian;
			}
		}
		
		bool isCartesian()
		{
			return Cartesian;
		}
		bool isScreen()
		{
			return !Cartesian;
		}
		
		void display_value()
		{
			std::cout << "Coordinate   " << std::to_string(PosX) << "   " << std::to_string(PosY) << std::endl;
			std::cout << "ScreenHeight   " << ScreenHeight << std::endl;
			std::cout << "Is Cartesian ?   " << isCartesian() << std::endl;
			std::cout << std::endl;
		}
		
		
};


class Rectangle ///////////////////////////////////////////////////////////////////////////////////////////////////////////////// APPROVED //////////////////////////////////////////////////////////////////////////
{
	private:
	Point A, B, C, D;
	int length, width; // Length = Horizontal, Width = Vertical
	
	void RectangleSync()      // Variabel Bebas (A, length, width)          Variabel Terikat (B, C, D)
	{
		int PosX = A.PosX;
		int PosY = A.PosY;
		
		if(A.isCartesian())
		{
			if(B.isCartesian())
			{
				B.PosX = PosX + length; B.PosY = PosY;
			}
			else
			{
				B.Convert();
				B.PosX = PosX + length; B.PosY = PosY;
			}
			
			if(C.isCartesian())
			{
				C.PosX = PosX + length; C.PosY = PosY + width;
			}
			else
			{
				C.Convert();
				C.PosX = PosX + length; C.PosY = PosY + width;
			}
			
			if(D.isCartesian())
			{
				D.PosX = PosX; D.PosY = PosY + width;
			}
			else
			{
				D.Convert();
				D.PosX = PosX; D.PosY = PosY + width;
			}
		}
		
		else
		{
			if(B.isCartesian())
			{
				B.Convert();
				B.PosX = PosX + length; B.PosY = PosY;
			}
			else
			{
				B.PosX = PosX + length; B.PosY = PosY;
			}
			
			if(C.isCartesian())
			{
				C.Convert();
				C.PosX = PosX + length; C.PosY = PosY + width;
			}
			else
			{
				C.PosX = PosX + length; C.PosY = PosY + width;
			}
			
			if(D.isCartesian())
			{
				D.Convert();
				D.PosX = PosX; D.PosY = PosY + width;
			}
			else
			{
				D.PosX = PosX; D.PosY = PosY + width;
			}
		}
	}
	
	void RectangleReset(int PosX = 50, int PosY = 50, int Length = 50, int Width = 50)
	{
		A.PosX = PosX; A.PosY = PosY;
		B.PosX = PosX + Length; B.PosY = PosY;
		C.PosX = PosX + Length; C.PosY = PosY + Width;
		D.PosX = PosX; D.PosY = PosY + Width;
		if(A.isScreen())	{A.Convert();}
		if(B.isScreen())	{B.Convert();}
		if(C.isScreen())	{C.Convert();}
		if(D.isScreen())	{D.Convert();}
	}
	
	public:
	
	Rectangle(int PosX = 50, int PosY = 50, int Length = 50, int Width = 50, int ScreenHeight = 600)
	{
		A.PosX = PosX; A.PosY = PosY;
		B.PosX = PosX + Length; B.PosY = PosY;
		C.PosX = PosX + Length; C.PosY = PosY + Width;
		D.PosX = PosX; D.PosY = PosY + Width;
		
		A.ScreenHeight = ScreenHeight; B.ScreenHeight = ScreenHeight;
		C.ScreenHeight = ScreenHeight; D.ScreenHeight = ScreenHeight;
	}
	
	void SetLength(int Length)
	{
		length = Length;
		Rectangle(A.PosX, A.PosY, length, width);
		RectangleSync();
		
	}
	
	void SetWidth(int Width)
	{
		width = Width;
		Rectangle(A.PosX, A.PosY, length, width);
		RectangleSync();
	}
	
	void SetScreenHeight(int Height)
	{
		A.ScreenHeight = Height; B.ScreenHeight = Height;
		C.ScreenHeight = Height; D.ScreenHeight = Height;
		RectangleSync();
	}
	
	void SetPosX(int x)
	{
		A.PosX = x;
		RectangleSync();
	}
	
	void SetPosY(int y)
	{
		A.PosY = y;
		RectangleSync();
	}
	
	Point GetAnchorCartesian()//////////////////////////////////////// APPROVED ///////////////////////////////////////////
	{
		if(A.isCartesian()){return A;}
		else if(A.isScreen())
		{
			Point ReturnValue;
			A.Convert();
			ReturnValue = A;
			A.Convert();
			return ReturnValue;
		}
		else{}
	}
	
	Point GetAnchorScreen()//////////////////////////////////////// APPROVED ///////////////////////////////////////////
	{
		if(A.isScreen()){return A;}
		else if(A.isCartesian())
		{
			Point ReturnValue;
			A.Convert();
			ReturnValue = A;
			A.Convert();
			return ReturnValue;
		}
		else{}
	}
	
	int GetLength(){return length;}
	
	int GetWidth(){return width;}
	
	Point GetPointA(){return A;}
	Point GetPointB(){return B;}
	Point GetPointC(){return C;}
	Point GetPointD(){return D;}
	
	
	bool IsPointInside(Point titik) ////// WORKING ON THIS PART (DETERMINE WHETJER A CERTAIN POINT IS INSIDE OF THIS RECTANGLE) ///////////
	{
		if(((titik.PosX - A.PosX) <= length) && ((titik.PosX - A.PosX) >= 0) && ((titik.PosY - A.PosY) <= width) &&  ((titik.PosY - A.PosY) >= 0)){return true;}
		else{return false;}
	}
};

#define SFML_Geometry

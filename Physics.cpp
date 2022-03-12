#include <iostream>
#include <string>

/*
TO DO LIST :
	
*/	   

/*
ERROR:
	1. Something wrong with multipilcation
	
*/

class fractional
{
	private://////////////////////////////////////////////////PRIVATE//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	long int Nominator;          // its a typo, but a convenient typo, so i keep it
	long int Denominator;
	
	void setFractionValue(long int nom, long int denom)										// Actually creating the fraction
	{
		Nominator = nom;
		Denominator = denom;
	}
	
	public://///////////////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	fractional(long int Numerator = 1, long int Denumerator = 1)
	{
		Nominator = Numerator;
		Denominator = Denumerator;
	}
	
	template<typename T>    															// Get absolute of any primitive number types (short, int, long, float, .....)
	static T AbsoluteNumber(T number){if(number < 0){return -number;} else{return number;}}
	
	static long int GreatestCommonDivisor(long int A, long int B) 							// Applying euclidian algorithm
	{
		A = AbsoluteNumber<long int>(A);
		B = AbsoluteNumber<long int>(B);
		
		while(true)
		{
			A = AbsoluteNumber<long int>(A - B);    
			if(A == B){break;}
			B = AbsoluteNumber<long int>(A - B);
			if(A == B){break;}
		}
		
		return A;
	}
	
	void Simplify()
	{
		long int GCD = GreatestCommonDivisor(Nominator, Denominator);     	// Find GreatestCommonDivisor
		if(GCD == 0)
		{
			/*if so, it only means that the numerator and denumerator are valued 0, which result a zero division error; so, do nothing*/
			/*it also means that both the numerator and denumerator are valued 0, an undefined mathematical concept, so let it be like that, (0/0)*/
		}
		else
		{
			Denominator = Denominator / GCD;
			Nominator = Nominator / GCD;									  	// Use it to divide both the numerator and denumerator
		}
	}
	
	void setNominator(long int x){Nominator = x;}											// Creating the fraction value manually
	void setDenominator(long int x){Denominator = x;}
	
	long int getNumerator()
	{
		Simplify();
		return Nominator;
			
	}
	long int getDenumerator()
	{
		Simplify();
		return Denominator;
			
	}
	
	static fractional addition(fractional& A, fractional& B)
	{
		//Simplification before doing actual arithmetics
		A.Simplify();
		B.Simplify();
		
		// Addition Algorithm
		long int Denumerator = A.getDenumerator() * B.getDenumerator();
		long int Numerator   =  A.getNumerator() * B.getDenumerator() + B.getNumerator() * A.getDenumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
	}
	
	static fractional substraction(fractional& A, fractional& B)
	{
		//Simplification before doing actual arithmetics
		A.Simplify();
		B.Simplify();
		
		// Addition Algorithm
		long int Denumerator = A.getDenumerator() * B.getDenumerator();
		long int Numerator   =  A.getNumerator() * B.getDenumerator() - B.getNumerator() * A.getDenumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
	}
	
	static fractional multiplication(fractional& A, fractional& B)
	{
		//Simplification before doing actual arithmetics
		A.Simplify();
		B.Simplify();
		
		// Addition Algorithm
		long int Denumerator = A.getDenumerator() * B.getDenumerator();
		long int Numerator   =  A.getNumerator() * B.getNumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
	}
	
	static fractional division(fractional& A, fractional& B)
	{
		//Simplification before doing actual arithmetics
		A.Simplify();
		B.Simplify();
		
		// Addition Algorithm
		long int Denumerator = A.getDenumerator() * B.getNumerator();
		long int Numerator   = A.getNumerator() * B.getDenumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
	}
	
	static bool isequal(fractional& A, fractional& B)
	{
		long int a, b, c, d;
		a = A.getNumerator();
		b = A.getDenumerator();
		c = B.getNumerator();
		d = B.getDenumerator();
		
		if((a == c) && (b == d)){return true;}
		else{return false;}
	}
	
	fractional operator+(fractional& A)
	{
		return addition(*this, A);
	}
	fractional operator-(fractional& A)
	{
		return substraction(*this, A);
	}
	fractional operator*(fractional& A)
	{
		return multiplication(*this, A);
	}
	fractional operator/(fractional& A)
	{
		return division(*this, A);
	}
	bool operator==(fractional& A)
	{
		return isequal(*this, A);
	}
};

int main()
{
	while(true) // Yes, its not efficient coding, but its readable coding, so fuck it
	{
		fractional Pecahan1; 
		fractional Pecahan2; 
		fractional Pecahan3;
		std::string operation; 	
		
		std::cout << "Pembilang pecahan 1 					: ";{int a; std::cin >> a; Pecahan1.setNominator(a);}
		std::cout << "Penyebut pecahan 1  					: ";{int a; std::cin >> a; Pecahan1.setDenominator(a);}
		std::cout << "Pembilang pecahan 1 					: ";{int a; std::cin >> a; Pecahan2.setNominator(a);}
		std::cout << "Penyebut pecahan 1  					: ";{int a; std::cin >> a; Pecahan1.setDenominator(a);}
		std::cout << "Masukkan operator (case sensitive)   	: ";std::cin >> operation;
		
		if(operation == "penjumlahan"){Pecahan3 = Pecahan1 + Pecahan2;}
		else if(operation == "pengurangan"){Pecahan3 = Pecahan1 - Pecahan2;}
		else if(operation == "perkalian"){Pecahan3 = Pecahan1 * Pecahan2;}
		else if(operation == "pembagian"){Pecahan3 = Pecahan1 / Pecahan2;}
		else{Pecahan3.setNominator(0), Pecahan3.setDenominator(0);}
		
		std::cout << "Hasil dari operasi                   : " << Pecahan3.getNumerator() << "/" << Pecahan3.getDenumerator() << std::endl;
		std::cout << "Apakah pecahan 1 dan 2 nilainya sama : " << (Pecahan1 == Pecahan2) << std::endl << std::endl;
		
	}
	
	return 0;
}

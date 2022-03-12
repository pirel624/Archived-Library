#include <iostream>

/*
TO DO LIST :
	1. Implement euclidian algorithm
	2. Find a way to simplify fractional

ASSUMED AXIOM:
	1. If there are two fractional, a and b, and they have been reduced to its simplest form, then if
	   a and b are different in form (regarding nominator and denominator), a and b are not equal. So too apply the vice versa

*/	   

/*
ERROR:
	1. Fractional division is still wrong

/*
FPB dari 128 dan 64 adalah 64
	1. 128 - 64 = 64 >> 64
FPB dari 256 dan 16 adalah 16
	1. 256 - 16 = 240
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
	
	void setNominator(long int x){Nominator = x;}											// Creating the fraction value manually
	void setDenominator(long int x){Denominator = x;}
	
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
		long int GCD = GreatestCommonDivisor(Nominator, Denominator);     // Find GreatestCommonDivisor
		Nominator = Nominator / GCD;												// Use it to divide both the numerator and denumerator
		Denominator = Denominator / GCD;
	}
	
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
		// Addition Algorithm
		long int Denumerator = A.getNumerator() * B.getDenumerator();
		long int Numerator   = B.getNumerator() * A.getDenumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
	}
};

int main()
{
	
	fractional Pecahan1 = fractional(45, 5); std::cout << "1 Done" << std::endl;
	fractional Pecahan2 = fractional(15, 5);std::cout << "2 Done" << std::endl;
	fractional Pecahan3 = fractional::division(Pecahan1, Pecahan2);std::cout << "3 Done" << std::endl;
	
	std::cout << Pecahan3.getNumerator() << "/" << Pecahan3.getDenumerator() << std::endl; std::cout << "4 Done" << std::endl;
	//std::cout << Pecahan1.getNumerator() << "/" << Pecahan1.getDenumerator() << std::endl; std::cout << "4 Done" << std::endl;
	
	
	while(true){}
	
	
	/*int number1;
	int number2;
	
	while(true)
	{
		std::cout << " Bilangan 1 : "; std::cin >> number1; std::cout << std::endl;
		std::cout << " Bilangan 2 : "; std::cin >> number2; std::cout << std::endl;
		std::cout << "Faktor Pembagian Terbesar : " << fractional::GreatestCommonDivisor(number1, number2) << std::endl;
	}*/
	
	return 0;
}

#include <iostream>


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
		long int Denumerator = A.getDenumerator() * B.getNumerator();
		long int Numerator   = A.getNumerator() * B.getDenumerator();
		fractional return_value = fractional(Numerator, Denumerator);
		
		// Simplification Algorithm
		return_value.Simplify();
		
		// Return it
		return return_value;
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
	
	fractional Pecahan1 = fractional(45, 5); 
	fractional Pecahan2 = fractional(15, 5);
	fractional Pecahan3 = Pecahan1 + Pecahan2;
	
	std::cout << Pecahan3.getNumerator() << "/" << Pecahan3.getDenumerator() << std::endl;
	
	while(true){}
	
	return 0;
}

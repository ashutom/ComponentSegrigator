#pragma once 


#include<vector>
#include<string>



inline int getnumfromdigit(char inc){ 	return (int) ( inc - '0' ); }
inline char getcharofdigit(int digit){ 	return (char) ( digit + '0' ); }

class LargeUnsignedInts{

	//Private APIs
	void ConvertFromULLToOurFormat(unsigned long long);
	void ConvertFromStringToOurFormat(std::string );
	void CorrectFormatAfterOperation();	//removes zeros from front of string
	void multiplyOneDigit(std::string& Num, int n);
	int  borrowandmodify(unsigned long long thiscounter);
	int  findQuitiontForOne(const std::vector<std::string>& TableofDividend, const std::string& divisor);
	void DivisionOfStrings(const std::vector<std::string>& TableofDividend, const std::string& divisor, std::string& qut);

	//Data Member
	std::string mNumber;

	public:
		LargeUnsignedInts(unsigned long long num=0) { ConvertFromULLToOurFormat(num); }
		LargeUnsignedInts(std::string num) { ConvertFromStringToOurFormat(num); }
		void Add(const LargeUnsignedInts& that);       //result will be the number to which 'that' is added
		int substract(const LargeUnsignedInts& that);  //result will be the number from which 'that' is substracted
		void multiply(const LargeUnsignedInts& that);  //result will be the number to which 'that' is multiplied
		void Divide(const LargeUnsignedInts& divisor); //Quotient will be stored in the Dividend and Remainder will be dropped
		std::string getNumber() const	{ return mNumber; }
		unsigned long long length() const { return mNumber.length(); }
		char operator[](unsigned long long int id) const;
		bool operator<(const LargeUnsignedInts& other)	{ return mNumber < other.getNumber();	}
		bool operator==(const LargeUnsignedInts& other)	{ return mNumber == other.getNumber();	}
		bool operator>(const LargeUnsignedInts& other)	{ return mNumber > other.getNumber();	}
		LargeUnsignedInts& operator=(const LargeUnsignedInts& other)	{ mNumber.clear(); mNumber = other.getNumber();	return *this; }
		bool operator<(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this < tmp;  }
		bool operator==(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this == tmp;  }
		bool operator>(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this > tmp; }

};




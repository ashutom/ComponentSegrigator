
#include <stdexcept>
#include <iostream>
#include "LargeUnsignedInts.h"

char LargeUnsignedInts::operator[](unsigned long long int id) const {
	if( id>=0 && id<mNumber.length() ) {
		return mNumber[id];
	} else {
		throw std::runtime_error(" Index out of bouds for operator [] for class LargeUnsignedInts ");
	}
}

void LargeUnsignedInts::ConvertFromULLToOurFormat(unsigned long long Other){
	std::string s;
	while(Other){
		int digit=Other%10;
		char ch = '0'+ digit;
		s= ch + s;
		Other=Other/10;
	}
	mNumber=s;
}

void LargeUnsignedInts::ConvertFromStringToOurFormat(std::string other){

	if(0 == other.length()){
		mNumber.clear();
		mNumber=mNumber + '0';
		return;
	}

	for(unsigned long long i=0;i<other.length();i++){
		if( other[i]<'0' || other[i]>'9'){
			std::cerr<<" Fatal!!, conversion of ["<<other<<"] To LargeUnsignedInts Failed because of unwanted char in the string" ;
			throw std::runtime_error(" Found a char which is not in range 0 - 9 ");
		}
	}
	mNumber=other;
}

void LargeUnsignedInts::Add(const LargeUnsignedInts& that) {
	std::string s;
	std::string toadd=that.getNumber();
	unsigned long long thiscounter=mNumber.length()-1;
	unsigned long long thatcounter=toadd.length()-1;
	int left, right, thiscarry, nextcarry;
	left=right=thiscarry=nextcarry=0;
	while(thatcounter>=0 && thiscounter>=0) {
		left = 0 + (int) (mNumber[thiscounter]-'0');
		right = 0 + (int) (toadd[thatcounter]-'0');
		if(left+right + thiscarry > 9 ) { nextcarry = 1; }
		else { nextcarry = 0 ; };
		left = left + right + thiscarry;
		left = left%10;
		thiscarry=nextcarry;
		char ch = '0' + left;
		s= ch + s;
		if(thiscounter==0 || thatcounter==0) break; //we need to adapt to this because the type of counter is ULL so, it doesnt gets negative
		thatcounter--;
		thiscounter--;
	}
	while(thatcounter>=0){
		right = 0 + (int) (toadd[thatcounter]-'0');
		if(right+thiscarry > 9 ) { nextcarry = 1; }
		else { nextcarry = 0 ; };
		right = right + thiscarry;
		right = right%10;
		thiscarry=nextcarry;
		char ch = '0' + right;
		s= ch + s;
		if(thatcounter==0) break; //we need to adapt to this because the type of counter is ULL so, it doesnt gets negative
		thatcounter--;
	}
	while(thiscounter>=0){
		left = 0 + (int) (mNumber[thiscounter]-'0');
		if(left+thiscarry > 9 ) { nextcarry = 1; }
		else { nextcarry = 0 ; };
		left = left + thiscarry;
		left = left%10;
		thiscarry=nextcarry;
		char ch = '0' + left;
		s= ch + s;
		if(thiscounter==0) break; //we need to adapt to this because the type of counter is ULL so, it doesnt gets negative
		thiscounter--;
	}
	mNumber=s;
}

void LargeUnsignedInts::CorrectFormatAfterOperation(){
	unsigned long long counter=0;
	for(;counter<mNumber.length();counter++){
		if(mNumber[counter] != '0'){
			break;
		}
	}
	if(counter>=mNumber.length()){
		mNumber.clear();
		mNumber=mNumber + '0';
	}else if(counter>0){
		mNumber = mNumber.substr(counter,mNumber.length()-counter);
	}
}

int LargeUnsignedInts::borrowandmodify(unsigned long long thiscounter){
	unsigned long long tempc=thiscounter-1;
        while(tempc>=0){
		int num=getnumfromdigit(mNumber[tempc]);
		if(num>0){
			mNumber[tempc]=getcharofdigit(num-1);
			for(unsigned long long i=tempc+1;i<thiscounter;i++){
				mNumber[i]='9';
			}
			return 0;
		}else if(num==0 && tempc==0){
			return -1; //should never come here
		}
		if(tempc==0) break; //we need to adapt to this because the type of counter is ULL so, it doesnt gets negative
		tempc--;
	}
	return -1; //should never come here
}

int LargeUnsignedInts::substract(const LargeUnsignedInts& that) {  //result will be the number from which 'that' is substracted
	int RetVal=-1;
	if(that.length()>mNumber.length()) return RetVal;
	if((that.length()==mNumber.length()) && (mNumber[0]<that[0])) return RetVal;

	std::string s=mNumber;
	unsigned long long thatcounter=that.length()-1;
	unsigned long long thiscounter=mNumber.length()-1;

	while(thatcounter>=0){
		int l,r;
		l=getnumfromdigit(mNumber[thiscounter]);
		r=getnumfromdigit(that[thatcounter]);
		if(l-r>=0){
			mNumber[thiscounter]=getcharofdigit(l-r);
		}else{
			if(thiscounter==0) return RetVal;
			if(-1 == borrowandmodify(thiscounter)) return RetVal;
			mNumber[thiscounter]=getcharofdigit(10+l-r);
		}
		if(thiscounter==0 || thatcounter==0) break; //we need to adapt to this because the type of counter is ULL so, it doesnt gets negative
		thiscounter--;
		thatcounter--;
	}
	CorrectFormatAfterOperation();
	return RetVal=0;
}

void LargeUnsignedInts::multiplyOneDigit(std::string& Num, int n) {

	int carry=0;
	for(int i=Num.length()-1;i>=0;i--){
		int mul=((int)(Num[i]-'0')*n)+carry;
		carry=mul/10;
		int numchar=mul%10;
		Num[i]=(char) numchar +'0';
	}
	std::string carry_string;

	while(carry){
		int numchar=carry%10;
		carry=carry/10;
		char char_digit = (char)numchar + '0';
		carry_string = char_digit + carry_string;
	}
	Num = carry_string + Num;
}

void LargeUnsignedInts::multiply(const LargeUnsignedInts& that) {   //result will be the number to which 'that' is multiplied

	LargeUnsignedInts TempInt;
	for(int i=that.length()-1;i>=0;i--){
		std::string ThisMultiplyer=this->getNumber();
		multiplyOneDigit(ThisMultiplyer,(int)(that[i]-'0'));
		for(int j=i-that.length()+1;j>0;j--){
			ThisMultiplyer=ThisMultiplyer + '0';
		}
		TempInt.Add(LargeUnsignedInts(ThisMultiplyer));
	}
	this->mNumber=TempInt.getNumber();

}

int LargeUnsignedInts::findQuitiontForOne(const std::vector<std::string>& TableofDividend, const std::string& divisor)  {
	int li, ri; li=1; ri=9;
	if(divisor<TableofDividend[li]) return li-1;
	if(divisor == TableofDividend[li]) return li;
	if(divisor>=TableofDividend[ri]) return ri;

	while(ri-li>0){
		if(divisor == TableofDividend[ri] ) return ri;
		if(divisor <  TableofDividend[ri] ) ri--;
		else if( divisor > TableofDividend[ri] ) return ri;
	}
	return li;
}

void LargeUnsignedInts::DivisionOfStrings(const std::vector<std::string>& TableofDividend, const std::string& divisor, std::string& qut)  {

	if(TableofDividend[1]>divisor){
		qut=std::string("0");
		return;
	}

	unsigned int minlen=TableofDividend[0].length();
	unsigned int endi=minlen-1;
	std::string curstr=divisor.substr(0,minlen);
	qut.clear();
	while(endi<divisor.length()) {
		int qt=findQuitiontForOne(TableofDividend,curstr);
		qut=qut + (char)( 0 + qt );
		endi++;
		if(endi>=divisor.length()){	break;	}
		if(0 == qt){
			curstr=curstr+divisor[endi];
			continue;
		}else{
			LargeUnsignedInts temp(curstr);
			temp.substract(TableofDividend[qt]);
			curstr = temp.getNumber();
			curstr = curstr + divisor[endi];
		}
	}
}

void LargeUnsignedInts::Divide(const LargeUnsignedInts& divisor) {   //Quotient will be stored in the Dividend and Remainder will be dropped

	LargeUnsignedInts temp = *this;
	std::vector<std::string> TableofDividend(11);
	TableofDividend[0]=std::string("0");
	TableofDividend[1]=temp.getNumber();
	for(int i=2;i<=10;i++){
		temp.Add(LargeUnsignedInts(TableofDividend[i-1]));
		TableofDividend[i]=temp.getNumber();
		temp= this->getNumber();
	}
}


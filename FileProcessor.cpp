#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<tuple>
#include<cstdlib>
#include<map>
#include<unordered_map>
#include<algorithm>
//#include<ranges> //only in c++ 20

const int MATCH_PERCENTAGE=69;
const std::string DEBUGENV_VAR("AMF_DEBUG");
const std::string DEBUGENV_VAL("1");
static bool IS_DEBUG_ON=false;


#define CHECK_RETURN_ON_FAIL_All(X,Y)   do{     \
                                                if(!X.is_open()) {      \
                                                        std::cerr<<" Could not open "<< Y<<std::endl;   \
                                                        f1.close(); f2.close(); f3.close(); Of.close(); \
                                                        return; \
                                                }       \
                                        }while(0);

#define CHECK_RETURN_ON_FAIL(X,Y)       do{     \
                                                if(!X.is_open()) {      \
                                                        std::cerr<<" Could not open "<< Y<<std::endl;   \
                                                        f1.close(); f2.close(); Of.close();     \
                                                        return; \
                                                }       \
                                        }while(0);


#define CHECK_RETURN_ON_FAIL_THIS(X,Y)       do{     \
                                                if(!X.is_open()) {      \
                                                        std::cerr<<" Could not open "<< Y<<std::endl;   \
                                                        X.close();   \
                                                        return; \
                                                }       \
                                        }while(0);






template <typename ...Args>
void DebugPrintImpl(int line, const char* funName, Args&& ...args)
{
    std::ostringstream stream;
    //stream << funName << "(" << line << ") : ";
    (stream << ... << std::forward<Args>(args)) << '\n';

    std::cout<<stream.str().c_str()<<std::endl;
}

#define DEBUG_PRINT(...)        do{     \
                                        if(IS_DEBUG_ON) { \
                                                DebugPrintImpl(__LINE__,__PRETTY_FUNCTION__,__VA_ARGS__);       \
                                        }       \
                                } while(0);


inline int getnumfromdigit(char inc){ 	return (int) ( inc - '0' ); }
inline char getcharofdigit(int digit){ 	return (char) ( digit + '0' ); }

class LargeUnsignedInts{

	void ConvertFromULLToOurFormat(unsigned long long);
	void ConvertFromStringToOurFormat(std::string );
	void CorrectFormatAfterOperation();	//removes zeros from front of string
	//void borrowandmodify(unsigned long long thiscounter);
        int  borrowandmodify(unsigned long long thiscounter);
	std::string mNumber;
	public:
		LargeUnsignedInts(unsigned long long num=0) { ConvertFromULLToOurFormat(num); }
		LargeUnsignedInts(std::string num) { ConvertFromStringToOurFormat(num); }
		void Add(const LargeUnsignedInts& that);       //result will be the number to which 'that' is added
		int substract(const LargeUnsignedInts& that); //result will be the number from which 'that' is substracted 
		//void multiply(const LargeUnsignedInts& that);  //result will be the number to which 'that' is multiplied
		std::string getNumber() const	{ return mNumber; }
		unsigned long long length() const { return mNumber.length(); } 
		char operator[](unsigned long long int id) const;
		bool operator<(const LargeUnsignedInts& other);
		bool operator==(const LargeUnsignedInts& other);
		bool operator>(const LargeUnsignedInts& other) { return !(*this<other ||  *this==other); }
		bool operator<(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this<tmp;  }
		bool operator==(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this==tmp;  }
		bool operator>(const unsigned long long& other) { LargeUnsignedInts tmp(other); return *this>tmp; }

};


char LargeUnsignedInts::operator[](unsigned long long int id) const {
	if( id>=0 && id<mNumber.length() ) {
		return mNumber[id];
	} else {
		throw std::runtime_error(" Index out of bouds for operator [] for class LargeUnsignedInts ");
	}
}

bool LargeUnsignedInts::operator==(const LargeUnsignedInts& other){

	if( other.length() != mNumber.length()) return false;
	for(unsigned long long i=0;i<mNumber.length();i++){
		if(mNumber[i] != other[i]){
			return false;
		}
	}
	return true;
}


bool LargeUnsignedInts::operator<(const LargeUnsignedInts& other){

	if(other.length()>mNumber.length()) return true;
	if(other.length()<mNumber.length()) return false;
	if(other.length()==mNumber.length()){
		LargeUnsignedInts num(mNumber);
		if(-1 == num.substract(other)) return true;
	}
	return false;
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


std::string ExtractLastColFromline(const std::string& line){
        std::string retval;
        int i=line.length()-1;
        if(line.size()<=0) return retval;
        while(line[i]!=',' && i>=0) {    i--;   }
        if(i<0){
                std::cerr<<" Reached the end before processing "<<std::endl;
                return retval;
        }
        i++;//getting one step away from ','
        if(line[i]!='\0') {
                retval=line.substr(i);
        }
        return retval;
}

std::string ExtractFirstColFromline(const std::string& line){
        int i=0;std::string retval;
        if(line.size()<=0) return retval;

        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                retval=line.substr(0,i);
        }
        return  retval;
}

void ExtractIDs(std::string& id, std::string& id2, const std::string& line){
        if(line.size()<=0) return;
        int i=0;
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                id=line.substr(0,i);
        }
        i++;
        if(line[i]!='\0') {
                id2=line.substr(i);
        }

}

std::vector<std::string> SplitWords(const std::string& line, char delim){
	std::vector<std::string> Retval;
	unsigned int laststart=0;unsigned int i=0;
	for(;i<line.length();i++){
		if(line[i]==delim){
			if(i-laststart>0){
				Retval.push_back(line.substr(laststart,i-laststart));
			}
			laststart=i+1;
		}
	}
	if(i-laststart>0){
		Retval.push_back(line.substr(laststart,i-laststart));
	}
	return Retval;	
}

bool DoesWordsMatch(const std::string& w1,const std::string& w2){
	unsigned int c,m;m=c=0;
	for(;c<w1.length() && c<w2.length();c++){
		if(w1[c]==w2[c]){
			m++;
		}
	}
	bool Retval=false;
	if( ((m*100)/c) >=MATCH_PERCENTAGE){ Retval=true; }
	return Retval;
	
}

bool DoesKernelMatch(const std::string& k1, const std::string& k2){

	std::vector<std::string> Fwrods, Swords;
	std::vector<std::string> FwrodsEle2, SwordsEle2;
	std::string L("legacy");std::string M("modern");
	Fwrods=SplitWords(k1,' ');
	Swords=SplitWords(k2,' ');
	//bool fmatch=DoesWordsMatch(Fwrods[0],Swords[0]); //return values or single name kernerl
	if(Fwrods.size()>=2 && Swords.size()>=2){
		FwrodsEle2=SplitWords(Fwrods[1],':');
		SwordsEle2=SplitWords(Swords[1],':');
		//return DoesWordsMatch(FwrodsEle2[FwrodsEle2.size()-1],SwordsEle2[SwordsEle2.size()-1]);
		unsigned int counter=0; bool latmatch=true;
		for(;counter<FwrodsEle2.size() && counter<SwordsEle2.size() && latmatch && counter<5;counter++){ //matches only 4 elements
			if((FwrodsEle2[counter]==L && SwordsEle2[counter]==M) || (FwrodsEle2[counter]==M && SwordsEle2[counter]==L) ){ //Special case of legacy / modern 
				continue;	
			}
			latmatch=latmatch && DoesWordsMatch(FwrodsEle2[counter],SwordsEle2[counter]);
		}
		return latmatch;
	}else{ // kernel is a single line 
		FwrodsEle2=SplitWords(Fwrods[0],',');
		SwordsEle2=SplitWords(Swords[0],',');
		return DoesWordsMatch(FwrodsEle2[0],SwordsEle2[0]);		
	}
	return false;
}

void WriteCommonKernelTimesToFirstFile(std::string& FirstI, std::string& Second, std::string& Third, std::string& Output){
//First file contains last col as the execution time
//Second file contains last col as execution time of the other
//Third file shall contain IDs in comma seprated

        std::ifstream f1(FirstI.c_str()),  f2(Second.c_str());
        std::ifstream f3(Third.c_str());
        std::ofstream Of(Output.c_str());

        CHECK_RETURN_ON_FAIL_All(f1,FirstI);
        CHECK_RETURN_ON_FAIL_All(f2,Second);
        CHECK_RETURN_ON_FAIL_All(f3,Third);
        CHECK_RETURN_ON_FAIL_All(Of,Output);


        if(f1.eof()) {
                std::cerr<<" Empty file " << FirstI<<std::endl;
                return;
        }
        std::string Lin;
        getline(f1,Lin);
        //add the last col name to this
        Lin+=std::string(",SecondBuildExecutionTime,DiffOfFirstFromSecond");
        Of<<Lin;
        getline(f2,Lin);//just eat the top header
        while( !f1.eof()  && !f2.eof() && !f2.eof()){
                std::string line1, line2, line3;
                getline(f3,line3);
                if(line3.size()<=0){
                        std::cout<<" File ended :"<<Third<<std::endl;
                        break;
                }
                std::string id1,id2;
                int Intid1=0,Intid2=0;
                ExtractIDs(id1,id2,line3);
                Intid1=std::atoi(id1.c_str());
                do{
                        getline(f1,line1);
                        if(line1.size()<=0){
                                std::cout<<" File ended :"<<FirstI<<std::endl;
                                break;
                        }
                        Intid2=std::atoi(ExtractFirstColFromline(line1).c_str());
                }while(Intid2<Intid1);
                if(Intid2==Intid1){
                        Intid1=std::atoi(id2.c_str());
                        do{
                                getline(f2,line2);
                                if(line2.size()<=0){
                                        std::cout<<" File ended :"<<Second<<std::endl;
                                        break;
                                }
                                Intid2=std::atoi(ExtractFirstColFromline(line2).c_str());
                        }while(Intid2<Intid1);
                        if(Intid2==Intid1){
                                std::string ExeTimeFirst=ExtractLastColFromline(line1);
                                std::string ExeTimeSecond=ExtractLastColFromline(line2);
                                long diff=  (long)(std::stol(ExeTimeSecond)) - (long)(std::stol(ExeTimeFirst));
                                line1=  line1 + std::string(",") +
                                        ExtractLastColFromline(line2) + std::string(",") +
                                        std::to_string(diff);
                                        Of<<line1<<std::endl;
                        }
                }
        }
        f1.close(); f2.close(); f3.close(); Of.close();
}


void parseIdAndKernel(std::string& id,std::string& kernel, std::string line){
        if(line.size()<=0) return;

        int i=0;
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                id=line.substr(0,i);
        }
        i++;
        if(line[i]!='\0') {
                kernel=line.substr(i);
        }
}


void parseExecutionTimeAndKernelV2(long long& ExecutionTime,std::string& kernel, const std::string& line){
	//assumint the line is of the format:
	//ExecutionTime,Index,KernelName,gpu-id,queue-id,queue-index.....
        if(line.size()<=0) return;

        int i=0;int secondstart=0;
	
	//Get to the first Comma
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                ExecutionTime=std::stol(line.substr(0,i)); //Get the first col now
        }
        i++;
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
	i++;i++; // skipping , & "
	secondstart=i; // second is id ... so let it go

        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                kernel=line.substr(secondstart); // Get the third as that is Kernel
        }
}


void parseExecutionTimeAndKernelV3(long long& ExecutionTime,std::string& kernel, const std::string& line){ //This shall be used for stats mainly

	//assumint the line is of the format:
	//ExecutionTime,Index,KernelName,gpu-id,queue-id,queue-index.....
        if(line.size()<=0) return;

        int i=0;int secondstart=0;
	
	//Get to the first Comma
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
        if(line[i]!='\0') {
                ExecutionTime=std::stol(line.substr(0,i)); //Get the first col now
        }
        i++;
        while(line[i]!=',' && line[i]!='\0') {   i++;   }
	i++;i++; // skipping , & "
	secondstart=i; // second is id ... so let it go

	//search for 18 commas from last
	i=line.size()-1;int numofcommas=0;
        while(i>secondstart && numofcommas<18 ) {   if(line[i]==','){ numofcommas++;} i--;   }
	
	if(numofcommas>=18 && i-secondstart>0){
                kernel=line.substr(secondstart,i-secondstart); // Get the third as that is Kernel
	}
}

void GetDiffBasedOnKernel(long long& Diff,const std::string& line1, const std::string& line2){
        if(line1.size()<=0 || line2.size()<=0) return;

        std::string k1,k2;
        long long firsnum,secnum;firsnum=secnum=0;
	Diff=0;
	parseExecutionTimeAndKernelV2(firsnum,k1,line1);
        parseExecutionTimeAndKernelV2(secnum,k2,line2);
	//std::cout<<" K1 = "<<k1<<" and K2 = " <<k2<<std::endl;
	//if(k1==k2){
	if(DoesKernelMatch(k1,k2)){
		Diff=secnum-firsnum;
	}
}

void WriteCommonKernelIDsToFile(std::string& FirstI, std::string& Second, std::string& Output){

        std::ifstream f1(FirstI.c_str()),  f2(Second.c_str());
        std::ofstream Of(Output.c_str());


        CHECK_RETURN_ON_FAIL(f1,FirstI);
        CHECK_RETURN_ON_FAIL(f2,Second);
        CHECK_RETURN_ON_FAIL(Of,Output);

        Of<<" FID "<<"," <<"SID"<<std::endl;
        std::string line1, line2;
        while( !f1.eof()  && !f2.eof()){
                getline(f1,line1);
                getline(f2,line2);
                if(line1.size()<=0 || line2.size()<=0) {
                        std::cout<<" Reached End of the file for one file " <<std::endl;
                        break;
                }
                std::string fid,fk, sid, sk;
                parseIdAndKernel(fid,fk,line1);
                parseIdAndKernel(sid,sk,line2);
                //std::cout<<" fid == "<<fid<<" fk = "<<fk<<" sid=="<<sid<<"  sk= "<<sk<<std::endl;
                if(DoesKernelMatch(fk,sk)){
                        Of<<fid<<","<<sid<<std::endl;
                }
        }
        f1.close(); f2.close(); Of.close();
}

void WriteDiffExecutionTimeToSameFile(std::string& FirstI, std::string& Second){

	std::string TempF=std::string("With_Diff_Time_")+FirstI;
        std::string line1, line2;
        std::ifstream f1(FirstI.c_str()),  f2(Second.c_str());
        std::ofstream Of(TempF.c_str());


        CHECK_RETURN_ON_FAIL(f1,FirstI);
        CHECK_RETURN_ON_FAIL(f2,Second);
        CHECK_RETURN_ON_FAIL(Of,TempF);

        getline(f1,line1);
        getline(f2,line2);
        Of<<"DiffExecutionTime,"<<line1<<std::endl;
        while( !f1.eof()  && !f2.eof()){
                getline(f1,line1);
                getline(f2,line2);
                if(line1.size()<=0 || line2.size()<=0) {
                        std::cout<<" Reached End of the file for one file " <<std::endl;
                        break;
                }
		long long diff=0;
		GetDiffBasedOnKernel(diff,line1,line2);
                //std::cout<<" diff == "<<diff<<std::endl;
		//Of<<std::to_string(diff)<<","<<line1<<std::endl;
		Of<<diff<<","<<line1<<std::endl;
        }
        f1.close(); f2.close(); Of.close();
}

void GetStatsData(std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>>& StatsData, std::ifstream& InputFile){

	std::string firstline;
	getline(InputFile,firstline); // eat firstline as that is meta data
	while(!InputFile.eof()){
		std::string line, kernel;
		long long Exetime=0;
		getline(InputFile,line);
		parseExecutionTimeAndKernelV3(Exetime,kernel,line);
		DEBUG_PRINT("line= " ,line);
		DEBUG_PRINT("kernel= " ,kernel);
		DEBUG_PRINT("Exetime= " ,Exetime);
		if(kernel.length()<2) continue;
		auto found=StatsData.find(kernel);
		if(found ==StatsData.end()){
			StatsData.insert({kernel,std::make_tuple<unsigned long long,unsigned long>(Exetime,1) });
			//std::tuple<unsigned long long,unsigned long> tmptuple =StatsData[kernel];
			//std::cout<<" Inserting New Kernel :=>> [ "<<kernel<<" ] & times = "<<std::get<1>(tmptuple)<<"]<<=="<<std::endl;
		}else{
			std::tuple<unsigned long long,unsigned long> tmptuple =StatsData[kernel];
			unsigned long times=std::get<1>(tmptuple);
			unsigned long long currentaverage = std::get<0>(tmptuple);
			StatsData[kernel]= std::make_tuple<unsigned long long,unsigned long>(
					((currentaverage*times) + Exetime)/(times+1),times+1
				);
			//std::cout<<" Incrementing Kernel  :=>> [ "<<kernel<<" ] occurance to ["<<times+1<< "]<<=="<<std::endl;
		}
	}
}

void PutStatsData(const std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>>& StatsData, std::ofstream& OutputFile){
	//lets sort the data first
	std::map<unsigned long long, std::tuple<std::string,unsigned long long,unsigned long>> sortmap;
	for( const auto& [kernel, data] : StatsData) {
		unsigned long long avgtime=std::get<0>(data);
		unsigned long nota=std::get<1>(data);
		sortmap.insert({ avgtime*nota, { kernel, avgtime, nota}});
	}
	int counter=0;
	OutputFile<<" Index, Kernel, AverageExecutionTime, NumberOfTimeAppearenceInExecution, TotalTimeOfExecution "<<std::endl;
	counter++;
	std::string comma(",");std::string codes("\"");
	for(auto iter = sortmap.rbegin(); iter != sortmap.rend(); ++iter)// since we need the largest ones at the top hence reverse the it
	{
		OutputFile<<counter++<<comma<<
			codes<<std::get<0>(iter->second)<<codes<<comma
			<<std::get<1>(iter->second)<<comma
			<<std::get<2>(iter->second)<<comma
			<<iter->first
			<<std::endl;
	}
}

inline bool IsvalidInt(std::string& num){
	for(char& ch : num){
		if(ch<'0' || ch>'9') return false;
	}
	return true;
}


unsigned long long parseExecutionTimeAndKernelProfV2(std::string& kernel, const std::string& line){ //This shall be used for stats mainly

	//assumint the line is of the format:
	//..........,KernelName,start time, end time, correlationid
        //std::cout<<" In parseExecutionTimeAndKernelProfV2 : Processing line \n line = ["<<line<<"]"<<std::endl;
        unsigned long long ExecutionTime=0;
        if(line.size()<=0) return ExecutionTime;
        int i=line.size()-1;

	//Get to the first Comma from the last
        while(line[i]!=',' && i>=0) {   i--;   }
        int start_of_first_int = --i;
        int end_of_first_int = start_of_first_int;
        int start_of_second_int = start_of_first_int;
        int end_of_second_int = start_of_first_int;
        int start_of_kernel = start_of_first_int;
        int end_of_kernel = start_of_first_int;

        std::string EndTime, StartTime;
        if(i>=0) {
                while(i>=0 && line[i]!=',') {   i--;   }
                if(i>=0 && line[i] == ','){
			end_of_first_int = i+1;
                        start_of_second_int = --i;
                }else{
                   // should not reach here
                   std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" Problem with Endtime parsing " << std::endl;
		   return ExecutionTime;
                }
                while(i>=0 && line[i]!=',') {   i--;   }
                if(i>=0 && line[i] == ','){
                        end_of_second_int=i+1;
                }else{
                   // should not reach here
                   std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" Problem with Start time parsing " << std::endl;
		   return ExecutionTime;
                }
		//kernel in the file is in codes(  " " ) hence the delimeter is different now
		--i;
		if(i>=0 && line[i]=='"'){
			start_of_kernel= --i;
			while(i>=0 && line[i]!='"') {   i--;   }
				if(i>=0 && line[i] == '"'){
					end_of_kernel=i+1;
				}else{
					// should not reach here
					std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" Problem with Kernel name parsing " << std::endl;
					return ExecutionTime; //found error case in the file with this line ==> "1274,15,1,134\",14,6,0"
				}
		}
		EndTime=line.substr(end_of_first_int,start_of_first_int-end_of_first_int+1);
		StartTime=line.substr(end_of_second_int,start_of_second_int-end_of_second_int+1);
		if(!IsvalidInt(EndTime) || !IsvalidInt(StartTime)){
			 std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" Problem with Parsing Time data " << std::endl;
			 return ExecutionTime;
		}
                kernel=line.substr(end_of_kernel,start_of_kernel-end_of_kernel+1);

		LargeUnsignedInts LnumStartTime(StartTime);
		LargeUnsignedInts LnumEndTime(EndTime);
		if(LnumEndTime.substract(LnumStartTime)<0){
			std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" LargeUnsignedInts == Problem with Time calulations endtime is less than start time  " << std::endl;
			kernel.clear();
			return ExecutionTime;
		}
		
		if(LnumEndTime.getNumber().length()>19){ //ULL is 18 digit in decimal & 64 bits in binary
			std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<" Execution time is ["<<LnumEndTime.getNumber()<<"] ## BEYOND ULL SIZE  " << std::endl;
			kernel.clear();
			return ExecutionTime;
		}

                ExecutionTime=stoull(LnumEndTime.getNumber());

		//Lets print for debugging
		//std::cout<<" StartTime == ["<<StartTime<<"]"<<std::endl
		//	<<" EndTime == ["<<EndTime<<"]"<<std::endl
		//	<<" ExecutionTime == ["<<ExecutionTime<<"]"<<std::endl
		//	<<" ExecutionTime == ["<<LnumEndTime.getNumber()<<"]  ####  from LargeUnsignedInts  "<<std::endl
		//	<<" kernel == ["<<kernel<<"]"<<std::endl;
		//more debug
		//LargeUnsignedInts convertednum(ExecutionTime);
		//if(convertednum == LnumEndTime){
		//	std::cout<<" convertednum == LnumEndTime <============"<<std::endl;
		//}else{
		//	std::cout<<" @@@@@@@@@@@@@@@@@   PROBLEM   @@@@@@@@@@@@@   Class value is not same as Execution value @@@@@@@@@@@@@@@  "<<std::endl;
		//}
        }else{
        	// should not reach here
                std::cout<<" FATAL Error in calculating the data at "<<__LINE__<<std::endl;
        }
	return ExecutionTime;
}
void GetStatsDataRocProfV2(std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>>& StatsData, std::ifstream& InputFile){

	std::string firstline;
	unsigned long long skippedrows=0;
	getline(InputFile,firstline); // eat firstline as that is meta data
	while(!InputFile.eof()){
		std::string line, kernel;
		getline(InputFile,line);
		unsigned long long Exetime=parseExecutionTimeAndKernelProfV2(kernel,line);
		DEBUG_PRINT("line= " ,line);
		DEBUG_PRINT("kernel= " ,kernel);
		DEBUG_PRINT("Exetime= " ,Exetime);
		if(kernel.length()<2){
			std::cout<<" Skipping this row = [ "<<line<<" ]"<<std::endl;
			skippedrows++;
			continue; //Skipping this kernel as this has wrong computation
		}
		auto found=StatsData.find(kernel);
		if(found ==StatsData.end()){
			StatsData.insert({kernel,std::make_tuple<unsigned long long,unsigned long>(std::forward<unsigned long long&&>(Exetime),1) });
			//std::tuple<unsigned long long,unsigned long> tmptuple =StatsData[kernel];
			//std::cout<<" Inserting New Kernel :=>> [ "<<kernel<<" ] & times = "<<std::get<1>(tmptuple)<<"]<<=="<<std::endl;
		}else{
			std::tuple<unsigned long long,unsigned long> tmptuple =StatsData[kernel];
			unsigned long times=std::get<1>(tmptuple);
			unsigned long long currentaverage = std::get<0>(tmptuple);
			StatsData[kernel]= std::make_tuple<unsigned long long,unsigned long>(
					((currentaverage*times) + Exetime)/(times+1),times+1
				);
			//std::cout<<" Incrementing Kernel  :=>> [ "<<kernel<<" ] occurance to ["<<times+1<< "]<<=="<<std::endl;
		}
	}
	std::cout<<"\n\n\n Processed Whole Input File. Total rows skipped == [ "<<skippedrows<<" ]\n\n\n"<<std::endl;
}

void CalculateStats(std::string& FirstI, std::string& Second){

	std::vector<std::string> InputArry={ FirstI, Second };
	unsigned int counter=0;
	while(counter<InputArry.size()){

		std::cout<<"\n\n\n Processing file [ "<<InputArry[counter]<< " ] "<<std::endl;
		std::ifstream InputFile(InputArry[counter].c_str());
		CHECK_RETURN_ON_FAIL_THIS(InputFile,InputArry[counter]);
		
		std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>> StatsData;
		GetStatsData(StatsData,InputFile);
		std::cout<<" Processed File ["<<InputArry[counter]<<"]"<<std::endl
		<<" Found : "<<StatsData.size() << " Kernels "<<std::endl;
		InputFile.close();
		
		std::string outputfile=std::string("OUT_")+InputArry[counter];
		std::ofstream OutputFile(outputfile.c_str());
		CHECK_RETURN_ON_FAIL_THIS(OutputFile,InputArry[counter]);
		
		std::cout<<" Writing Stats Data to File ["<<outputfile<<"]"<<std::endl;
		PutStatsData(StatsData,OutputFile);
		std::cout<<" Finished processing of File ["<<outputfile<<"]"<<std::endl;
		OutputFile.close();

		counter++;
	}
}


void ConvertV2DataToRequisiteFormat(std::string InputfileName, std::string OutputfileName){
	//Algorithm
		// for each line get kernel, start, end , and execution time 
			// Insert in to set with count

	std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>> StatsData;

	std::cout<<"\n\n\n Processing file [ "<<InputfileName<< " ] "<<std::endl;
	std::ifstream InputFile(InputfileName.c_str());
	std::ofstream OutputFile(OutputfileName.c_str());
	if(!InputFile.is_open() || !OutputFile.is_open()) {
		std::cerr<<" Could not open "<< InputfileName <<" or "<<OutputfileName<< std::endl;
	}else{
		GetStatsDataRocProfV2(StatsData,InputFile);
		std::cout<<" Writing Stats Data to File ["<<OutputfileName<<"]"<<std::endl;
		PutStatsData(StatsData,OutputFile);
		std::cout<<" Finished processing of File ["<<OutputfileName<<"]"<<std::endl;
		OutputFile.close();
		InputFile.close();
	}
}


int main( int argc, char* argv[] ){

	if(argc < 3) {
            std::cerr<< " Insufficient arguments " << std::endl;
            exit(1);
        }

        if(std::getenv(DEBUGENV_VAR.c_str())){
                if(DEBUGENV_VAL == std::getenv(DEBUGENV_VAR.c_str())){
                        IS_DEBUG_ON=true;
                }
        }

	switch( std::stol(std::string(argv[1]))) {
		case 1: { 
				if(argc < 5) {
			            std::cerr<< " Insufficient arguments " << std::endl;
			            exit(1);
        			}
				std::string If1(argv[2]);
				std::string If2(argv[3]);
				std::string Op(argv[4]);
				WriteCommonKernelIDsToFile(If1,If2,Op);
			}
			break;
		case 2: {
				if(argc < 6) {
			            std::cerr<< " Insufficient arguments " << std::endl;
			            exit(1);
        			}
				std::string If1(argv[2]);
				std::string If2(argv[3]);
				std::string If3(argv[4]);
				std::string Op(argv[5]);
				WriteCommonKernelTimesToFirstFile(If1,If2,If3,Op);
			}
			break;
		case 3: {
				if(argc < 4) {
			            std::cerr<< " Insufficient arguments " << std::endl;
			            exit(1);
        			}
				std::string If1(argv[2]);
				std::string If2(argv[3]);
				WriteDiffExecutionTimeToSameFile(If1,If2);
			}
			break;
		case 4: {
				if(argc < 4) {
			            std::cerr<< " Insufficient arguments " << std::endl;
			            exit(1);
        			}
				std::string If1(argv[2]);
				std::string If2(argv[3]);
				CalculateStats(If1,If2);
			}
			break;
		case 5: {
				if(argc < 4) {
					std::cerr<< " Insufficient arguments " << std::endl;
					exit(1);
				}
				std::string If1(argv[2]);
				std::string If2(argv[3]);
				ConvertV2DataToRequisiteFormat(If1,If2);
			}
			break;
		default : std::cerr<< " Wrong arguments " << std::endl; exit(1);
	}

	return 0;
}



#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<tuple>
#include<unordered_map>


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


const int MATCH_PERCENTAGE=69;

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
                                long diff=  (long)(std::stoi(ExeTimeSecond)) - (long)(std::stoi(ExeTimeFirst));
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
                ExecutionTime=std::stoi(line.substr(0,i)); //Get the first col now
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
                ExecutionTime=std::stoi(line.substr(0,i)); //Get the first col now
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
		getline(InputFile,line);
		long long Exetime;
		parseExecutionTimeAndKernelV3(Exetime,kernel,line);
		auto found=StatsData.find(kernel);
		if(found ==StatsData.end()){
			//std::cout<<" Inserting New Kernel :=>> [ "<<kernel<<" ] <<=="<<std::endl;
			StatsData.insert({kernel,std::make_tuple<unsigned long long,unsigned long>(Exetime,1) });
		}else{
			std::tuple<unsigned long long,unsigned long> tmptuple =StatsData[kernel];
			StatsData[kernel]= std::make_tuple<unsigned long long,unsigned long>(
					(std::get<0>(tmptuple)*std::get<1>(tmptuple) + Exetime)/(std::get<1>(tmptuple)+1),
					std::get<1>(tmptuple)+1
				);
		}
	}
	std::cout<<" File processing  Over "<<std::endl
                 <<" Found : "<<StatsData.size() << " Kernels "<<std::endl;
}

void PutStatsData(const std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>>& StatsData, std::ofstream& OutputFile){
	OutputFile<<" Kernel, AverageExecutionTime, NumberOfTimeAppearenceInExecution"<<std::endl;
	std::string comma(",");
	for( const auto& [kernel, value] : StatsData ) {
		OutputFile<<kernel<<comma<<std::get<0>(value)<<comma<<std::get<1>(value)+1<<std::endl;
	}
}

void CalculateStats(std::string& FirstI, std::string& Second){

	std::vector<std::string> InputArry={ FirstI, Second };
	unsigned int counter=0;
	while(counter<InputArry.size()){

		std::ifstream InputFile(InputArry[counter].c_str());
		CHECK_RETURN_ON_FAIL_THIS(InputFile,InputArry[counter]);
		
		std::unordered_map<std::string,std::tuple<unsigned long long,unsigned long>> StatsData;
		GetStatsData(StatsData,InputFile);
		InputFile.close();
		
		std::string outputfile=std::string("STATS_")+InputArry[counter];
		std::ofstream OutputFile(outputfile.c_str());
		CHECK_RETURN_ON_FAIL_THIS(OutputFile,InputArry[counter]);
		
		PutStatsData(StatsData,OutputFile);
		OutputFile.close();

		counter++;
	}
}
int main( int argc, char* argv[] ){

	if(argc < 3) {
            std::cerr<< " Insufficient arguments " << std::endl;
            exit(1);
        }

	switch( std::stoi(std::string(argv[1]))) {
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
		default : std::cerr<< " Wrong arguments " << std::endl; exit(1);
	}

	return 0;
}



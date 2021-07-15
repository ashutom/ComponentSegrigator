#include<fstream>
#include<iostream>
#include<string>


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
                std:std::string fid,fk, sid, sk;
                parseIdAndKernel(fid,fk,line1);
                parseIdAndKernel(sid,sk,line2);
                //std::cout<<" fid == "<<fid<<" fk = "<<fk<<" sid=="<<sid<<"  sk= "<<sk<<std::endl;
                if(fk==sk){
                        Of<<fid<<","<<sid<<std::endl;
                }
        }
        f1.close(); f2.close(); Of.close();
}

int main( int argc, char* argv[] ){

        std::cout<<" Options : \n\n\n"
                <<" 1. Make the Common IDs File \n"
                <<" 2. Make the Final File using the common IDs file \n\n"
                <<" Choice : (1/2) : ";
        std::string choice;
        std::getline(std::cin,choice);
        //std::cin>>choice;
        if(std::to_string(1)==choice){
                std::string If1, If2, Op;
                std::getline(std::cin,If1);
                std::getline(std::cin,If2);
                std::getline(std::cin,Op);
                WriteCommonKernelIDsToFile(If1,If2,Op);
        }else if(std::to_string(2)==choice){
                std::string If1, If2, If3, Op;
                std::getline(std::cin,If1);
                std::getline(std::cin,If2);
                std::getline(std::cin,If3);
                std::getline(std::cin,Op);
                WriteCommonKernelTimesToFirstFile(If1,If2,If3,Op);

        }else{
                std::cerr<<" Not given expected arguments "<<std::endl;
        }


        return 0;
}



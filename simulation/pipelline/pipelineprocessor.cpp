#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <iterator>
using namespace std;

//I have used stall 1 here for the data hazards and stall 2 for the structural hazards and stall 3 for the branching hazards
//stall 3 is used for jump jr and jal instrucions as well  //stall 4 is for that point when beq bne etc. me bhi vo register aa jaaye jo khud load ho rha ho

int register_address(string s)
{
  if (s.compare("$zero")==0) {return 0;}
  else if(s.compare("$at")==0) {return 1;}
  else if(s.compare("$v0")==0) {return 2;}
  else if(s.compare("$v1")==0) {return 3;}
  else if(s.compare("$a0")==0) {return 4;}
  else if(s.compare("$a1")==0) {return 5;}
  else if(s.compare("$a2")==0) {return 6;}
  else if(s.compare("$a3")==0) {return 7;}
  else if(s.compare("$t0")==0) {return 8;}
  else if(s.compare("$t1")==0) {return 9;}
  else if(s.compare("$t2")==0) {return 10;}
  else if(s.compare("$t3")==0) {return 11;}
  else if(s.compare("$t4")==0) {return 12;}
  else if(s.compare("$t5")==0) {return 13;}
  else if(s.compare("$t6")==0) {return 14;}
  else if(s.compare("$t7")==0) {return 15;}
  else if(s.compare("$s0")==0) {return 16;}
  else if(s.compare("$s1")==0) {return 17;}
  else if(s.compare("$s2")==0) {return 18;}
  else if(s.compare("$s3")==0) {return 19;}
  else if(s.compare("$s4")==0) {return 20;}
  else if(s.compare("$s5")==0) {return 21;}
  else if(s.compare("$s6")==0) {return 22;}
  else if(s.compare("$s7")==0) {return 23;}
  else if(s.compare("$t8")==0) {return 24;}
  else if(s.compare("$t9")==0) {return 25;}
  else if(s.compare("$k0")==0) {return 26;}
  else if(s.compare("$k1")==0) {return 27;}
  else if(s.compare("$gp")==0) {return 28;}
  else if(s.compare("$sp")==0) {return 29;}
  else if(s.compare("$fp")==0) {return 30;}
  else if(s.compare("$ra")==0) {return 31;}
  else {return -1;}
}

typedef struct InstrFetch{
   string instr;
   int pc;
}IFstruct;

typedef struct InstrDecode{
   string instr;
   int pc;
   string type_instruction;
   int reg1_index;
   int reg2_index;
   int dest_index;
   int offset;
   int branchtarget; //hum isi me check kr lenge ki ky krna hai
   string label; //for beq, bne , blez, bgtz purpose
}IDstruct;

typedef struct InstrExecute{
  string instr;
  int pc;
  string type_instruction;
  int aluresult;  //isme add subtarct ke results hai, aur all arl ke bhi multiplied value shai
  int swdata; //isme register ki vo value hai jo last me memory me likhni hai
  int branchtarget;  //ye basically program counter ki vo nayi value hai
  int dest_memory;    //add krna h offset ke saath ..matlab isme sirf vo memory ka index mila hai, jisme read ya write krna hai
                   //ye memory ka vo index bhi ho skta hai jis sey read kr rhe ya ya jisme write kr rhe hai
  int dest_register; //ye vo register hai jisme write krna hai
}EXstruct;

typedef struct ReadMemory{
  string instr;
  int pc;
  string type_instruction;
  int dest_register; //memory ka address, register ka index
  int dest_memory;
  int writedata; //vo cheez bhi aayegi jo memory read kri hai, isme aluresult , isme branchtarget bhi aayega, aur jo memory me store krni hai vo bhi
}MEMstruct;

typedef struct WriteBack{
  string instr;
  int pc;
  string type_instruction;
  int dest_register; //memory ka address, register ka index
  int dest_memory;
  int writedata; //vo cheez bhi aayegi jo memory read kri hai, isme aluresult , isme branchtarget bhi aayega
}WBstruct;

string regIndexToString(int a)
{
  switch(a)
  {
    case 0 : return "$zero";
    case 1 : return "$at";
    case 2 : return "$v0";
    case 3 : return "$v1";
    case 4 : return "$a0";
    case 5 : return "$a1";
    case 6 : return "$a2";
    case 7 : return "$a3";
    case 8 : return "$t0";
    case 9 : return "$t1";
    case 10 : return "$t2";
    case 11 : return "$t3";
    case 12 : return "$t4";
    case 13: return "$t5";
    case 14: return "$t6";
    case 15: return "$t7";
    case 16: return "$s0";
    case 17: return "$s1";
    case 18: return "$s2";
    case 19: return "$s3";
    case 20: return "$s4";
    case 21: return "$s5";
    case 22: return "$s6";
    case 23: return "$s7";
    case 24: return "$t8";
    case 25: return "$t9";
    case 26: return "$k0";
    case 27: return "$k1";
    case 28: return "$gp";
    case 29: return "$sp";
    case 30: return "$fb";
    case 31: return "$ra";
    default : return "noop";
  }
}

void printregisters(int registers[]){
  for(int i=0; i<32; i++)
    cout<<regIndexToString(i)<<" : "<<registers[i]<<"\t";
  cout<<endl<<endl;
}
void printmemory(string memory[] , int ins){
  for(int i=ins; i<4000; i++)
  {
    if(memory[i] != "")
      cout<<"memory["<<i<<"] : "<<memory[i]<<endl;
  }
}
int main(){
        int registers[32] = {0};
      string memory[4000];
     // FOR Test Case 1
    // registers[register_address("$t1")] = 1;
    // registers[register_address("$t2")] = 2;
    // registers[register_address("$t3")] = 3;
    // registers[register_address("$t4")] = 4;
    // registers[register_address("$t5")] = 5;
    // registers[register_address("$ra")] = 50; 
    
    // For Test Case 2
    // registers[3] = 4; //v1
    // registers[4] = 7; //a0
    // registers[5] = 1; //a1
    // registers[7] = 3; //a3
    // registers[9] = 5; //t1
    // registers[10] = 3; //t2
    // registers[12] = 2; //t4
    // registers[13] = 2; //t5
    // registers[14] = 13; //t6
    //      memory[28] = "73";

     // FOR Test case 3
     registers[register_address("$a0")] = 7; //index pof fibonacci number
     registers[register_address("$t1")] = 3;
     registers[register_address("$t3")] = 2;
     registers[register_address("$t4")] = 1;
   registers[register_address("$sp")] = 3900;
        int program_counter = 0;
        map<string,int> labels;
    int cycles = 0;
    int totalins = 0;
    ofstream outfile;
    outfile.open("eachCycleStageReport.txt");
    ofstream outfile2;
    outfile2.open("eachCycleRegisterReport.txt");
     ifstream infile;
      infile.open("instructions.txt"); //opening txt file from where we want to take inputs
      string s;
      int currentinstruction = 0;

    while(getline(infile, s))    //s is the current line which is going to be read
    {
        if(s[s.length()-1] ==':') //it is a label
        {
        labels[s.substr(0, s.length()-1)] = currentinstruction;
        //isme current intruction ko nhi increase kiya
        }
        else if(s.length() != 0)
        {
        memory[currentinstruction++] = s;
        //vo label nhi hai to use memory me daal diya hai
        }
    }
    infile.close();
    
    IFstruct IF = {"NOOP 0 0 0" , 0};
    IDstruct ID = {"NOOP 0 0 0" , -1, "NOOP" , -1, -1 , -1, -1 , -1 , "blank"};
    EXstruct EX = {"NOOP 0 0 0" , 0, "NOOP", -1, -1, -1 ,-1, -1};
    MEMstruct MEM = {"NOOP 0 0 0" , 0, "NOOP", -1, -1, -1};
    WBstruct WB = {"NOOP 0 0 0" , 0, "NOOP", -1, -1, -1 };
    int totalcycles=0;
    while(1){
      // string str;
      // cin>>str;
          outfile<<"IF : "<<IF.pc<<"\tID : "<<ID.instr<<"\tEX : "<<EX.instr<<"\tMEM : "<<MEM.instr<<"\tWB : "<<WB.instr<<endl;
          for(int i=0; i<32; i++)
        outfile2<<regIndexToString(i)<<" : "<<registers[i]<<"\t";
      outfile2<<endl;


      if(MEM.type_instruction == "HALT"){
        cout<<"\ntotal cycles : "<<totalcycles<<endl;
          cout << "\nreg: " ;
                printregisters(registers);
                cout << "mem: " ;
                printmemory(memory, currentinstruction);
            exit(0);
        }
        else{

             //WRITE BACK KA START
            totalcycles++;
                WB.instr = MEM.instr ;
                WB.pc = MEM.pc ;
                WB.type_instruction = MEM.type_instruction ;
            
                if(WB.type_instruction == "NOOP"){
                }
                else if (WB.type_instruction == "add"){
                   // cout << "this is add in wb\n";
                    WB.writedata = MEM.writedata ;
                    WB.dest_register = MEM.dest_register ;
                    registers[WB.dest_register] = WB.writedata ;
                }
                else if(WB.type_instruction == "sub"){
                   // cout << "this is sub in wb\n";
                    WB.writedata = MEM.writedata ;
                    WB.dest_register = MEM.dest_register ;
                    registers[WB.dest_register] = WB.writedata ;
                }
                else if(WB.type_instruction == "sll"){
                   // cout << "this is sll in wb\n";
                    WB.writedata = MEM.writedata ;
                    WB.dest_register = MEM.dest_register ;
                    registers[WB.dest_register] = WB.writedata ;
                }
                else if(WB.type_instruction == "srl"){
                   // cout << "this is srl in wb\n";
                    WB.writedata = MEM.writedata ;
                   WB.dest_register = MEM.dest_register ;
                   registers[WB.dest_register] = WB.writedata ;
                }
                else if(WB.type_instruction == "lw"){
                   // cout << "this is lw in wb\n";
                    WB.writedata = MEM.writedata ;
                    WB.dest_register = MEM.dest_register ;
                    registers[WB.dest_register] = WB.writedata ;
                }
                else if(WB.type_instruction == "sw"){
                   // cout << "this is sw in wb\n";
                    WB.writedata = MEM.writedata ;
                    WB.dest_memory = MEM.dest_memory;
                
                    // stringstream str1;
                    // str1 << WB.writedata ;
                    // string s = str1.str();
                  
                    // memory[WB.dest_memory] = s;
                }
            else if(WB.type_instruction=="STALL1"||WB.type_instruction=="STALL2"||WB.type_instruction=="STALL3"||
                   WB.type_instruction=="STALL4_bne"||WB.type_instruction=="STALL4_beq"||WB.type_instruction=="STALL4_bgtz"||WB.type_instruction=="STALL4_blez"||
                    WB.type_instruction=="beq"||WB.type_instruction=="bne"||WB.type_instruction=="bgtz"||WB.type_instruction=="blez"||
                    WB.type_instruction=="j" || WB.type_instruction=="jr" || WB.type_instruction=="jal")
            {
                //kuch mat kro kahin bhi
                //cout << "this is nothing in wb\n";
            }

 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //READ MEMORY KA START
            
                  MEM.instr = EX.instr;
                  MEM.pc = EX.pc;
                  MEM.type_instruction = EX.type_instruction;
      
                   if(MEM.type_instruction == "NOOP")
                   {
                       //cout << "this is noop in mem\n";
                   }
                   else if (MEM.type_instruction == "add"){
                       MEM.dest_register = EX.dest_register ;
                       MEM.writedata = EX.aluresult ;
                      // cout << "this is add in mem\n";
                   }
                   else if(MEM.type_instruction == "sub"){
                       MEM.dest_register = EX.dest_register ;
                       MEM.writedata = EX.aluresult ;
                       //cout << "this is sub in mem\n";
                   }
                   else if(MEM.type_instruction == "sll"){
                      MEM.dest_register = EX.dest_register ;
                      MEM.writedata = EX.aluresult ;
                       //cout << "this is sll in mem\n";
                   }
                   else if(MEM.type_instruction == "srl"){
                     MEM.dest_register = EX.dest_register ;
                     MEM.writedata = EX.aluresult ;
                       //cout << "this is srl in mem\n";
                   }
                   else if(MEM.type_instruction == "lw"){
                       MEM.dest_register = EX.dest_register ;
                       MEM.dest_memory = EX.dest_memory ;
                       stringstream str(memory[MEM.dest_memory]);
                       int v;
                       str >> v;
                       MEM.writedata = v ;
                      // cout << "this is lw in mem\n";
                   }
                   else if(MEM.type_instruction == "sw"){
                       MEM.dest_memory = EX.dest_memory ;
                       MEM.writedata = EX.swdata ;

                    stringstream str1;
                    str1 << MEM.writedata ;
                    string s = str1.str();
                  
                    memory[MEM.dest_memory] = s;
                      // cout << "this is sw in mem\n";
                   }
      
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //INSTRUCTION EXECUTE KA START
        
            if(EX.type_instruction == "NOOP"||
               EX.type_instruction=="STALL3"||
               EX.type_instruction=="STALL4_beq"||EX.type_instruction=="STALL4_bne"||EX.type_instruction=="STALL4_bgtz"||EX.type_instruction=="STALL4_blez"||
               EX.type_instruction=="beq"||EX.type_instruction=="bne"||EX.type_instruction=="bgtz"||EX.type_instruction=="blez"||
               EX.type_instruction=="j"||EX.type_instruction=="jr"||EX.type_instruction=="jal"){
                
                    EX.instr = ID.instr;
                    EX.pc = ID.pc;
                    EX.type_instruction = ID.type_instruction;
                
                    if(EX.type_instruction == "add"){
                        EX.dest_register = ID.dest_index;
                        EX.aluresult = registers[ID.reg1_index] + registers[ID.reg2_index] ;
                    }
                    else if(EX.type_instruction == "sub"){
                        EX.dest_register = ID.dest_index;
                        EX.aluresult = registers[ID.reg1_index] - registers[ID.reg2_index] ;
                    }
                    else if(EX.type_instruction == "sll"){
                        EX.dest_register = ID.dest_index;
                        int value = registers[ID.reg1_index];
                        int c = ID.offset;
                        for (int k = 0 ; k < c ; k++ ){
                        value = value * 2;
                        }
                        EX.aluresult = value;
                    }
                    else if(EX.type_instruction == "srl"){
                        EX.dest_register = ID.dest_index;
                        int value = registers[ID.reg1_index];
                        int c = ID.offset;
                            int two = 1;
                            for (int h = 0 ; h < c ; h++) {
                            two = two * 2;
                            }
                            value = (int) (value/two);
                            EX.aluresult = value ;
                    }
                    else if(EX.type_instruction == "lw"){
                        EX.dest_register = ID.dest_index;
                        EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                    }
                    else if(EX.type_instruction == "sw"){
                        EX.swdata = registers[ID.dest_index];
                        EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                    }
                    
            }
             else if (EX.type_instruction == "add" || EX.type_instruction == "sub" || EX.type_instruction == "sll" || EX.type_instruction == "srl"){
                    EX.instr = ID.instr;
                    EX.pc = ID.pc;
                    EX.type_instruction = ID.type_instruction;
                   
                    if (EX.type_instruction == "add"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = EX.aluresult + registers[ID.reg2_index] ;
                        }
                        else if(EX.dest_register == ID.reg2_index){
                             EX.dest_register = ID.dest_index;
                             EX.aluresult = EX.aluresult + registers[ID.reg1_index] ;
                        }
                        else{
                               EX.dest_register = ID.dest_index;
                               EX.aluresult = registers[ID.reg1_index] + registers[ID.reg2_index] ;
                        }
                     //   cout << "this is add in ex\n";
                    }
                    else if(EX.type_instruction == "sub"){
                       if(EX.dest_register == ID.reg1_index){
                           EX.dest_register = ID.dest_index;
                           EX.aluresult = EX.aluresult - registers[ID.reg2_index] ;
                       }
                       else if(EX.dest_register == ID.reg2_index){
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = registers[ID.reg1_index] - EX.aluresult ;
                       }
                       else{
                              EX.dest_register = ID.dest_index;
                              EX.aluresult = registers[ID.reg1_index] - registers[ID.reg2_index] ;
                       }
                      //  cout << "this is sub in sub\n";
                    }
                    else if(EX.type_instruction == "sll"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                            int value = EX.aluresult;
                            int c = ID.offset;
                            for (int k = 0 ; k < c ; k++ ){
                            value = value * 2;
                            }
                            EX.aluresult = value;
                        }
                        else{
                            EX.dest_register = ID.dest_index;
                            int value = registers[ID.reg1_index];
                            int c = ID.offset;
                            for (int k = 0 ; k < c ; k++ ){
                            value = value * 2;
                            }
                            EX.aluresult = value;
                        }
                      //  cout << "this is sll in ex\n";
                    }
                    else if(EX.type_instruction == "srl"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                             int value = EX.aluresult;
                            int c = ID.offset;
                            int two = 1;
                            for (int h = 0 ; h < c ; h++) {
                            two = two * 2;
                            }
                            value = (int) (value/two);
                            EX.aluresult = value ;
                        }
                        else{
                            EX.dest_register = ID.dest_index;
                            int value = registers[ID.reg1_index];
                            int c = ID.offset;
                                int two = 1;
                                for (int h = 0 ; h < c ; h++) {
                                two = two * 2;
                                }
                                value = (int) (value/two);
                                EX.aluresult = value ;
                        }
                      //  cout << "this is srl in ex\n";
                    }
                    else if(EX.type_instruction == "lw"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                            EX.dest_memory  =  (ID.offset + EX.aluresult) ;
                        }
                        else{
                            EX.dest_register = ID.dest_index;
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                        }
                       // cout << "this is lw in ex\n";
                    }
                    else if(EX.type_instruction == "sw"){
                        if(EX.dest_register == ID.dest_index){
                            EX.swdata  =  EX.aluresult;
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                        }
                        else if(EX.dest_register == ID.reg1_index){
                            EX.swdata = registers[ID.dest_index];
                            EX.dest_memory = (ID.offset  +  EX.aluresult);
                        }
                        else{
                            EX.swdata = registers[ID.dest_index];
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                        }
                       // cout << "this is sw in ex\n";
                    }
                    
            }
            else if(EX.type_instruction == "sw"){
                
                         if(ID.type_instruction == "NOOP"||
                            ID.type_instruction=="STALL1"||ID.type_instruction=="STALL2"||ID.type_instruction=="STALL3"||
                    ID.type_instruction=="STALL4_bne"||ID.type_instruction=="STALL4_beq"||ID.type_instruction=="STALL4_bgtz"||ID.type_instruction=="STALL4_blez"||
                             ID.type_instruction=="beq"||ID.type_instruction=="bne"||ID.type_instruction=="bgtz"||ID.type_instruction=="blez"||
                             ID.type_instruction=="j" || ID.type_instruction=="jr" || ID.type_instruction=="jal")
                         {
                             EX.instr = ID.instr;
                             EX.pc = ID.pc;
                             EX.type_instruction = ID.type_instruction;
                         }
                        else if (ID.type_instruction == "add"){
                            EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = registers[ID.reg1_index] + registers[ID.reg2_index] ;
                        }
                        else if (ID.type_instruction == "sub"){
                            EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = registers[ID.reg1_index] - registers[ID.reg2_index] ;
                        }
                        else if (ID.type_instruction == "sll"){
                            EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.dest_register = ID.dest_index;
                            int value = registers[ID.reg1_index];
                            int c = ID.offset;
                            for (int k = 0 ; k < c ; k++ ){
                            value = value * 2;
                            }
                            EX.aluresult = value;
                        }
                        else if (ID.type_instruction == "srl"){
                            EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.dest_register = ID.dest_index;
                            int value = registers[ID.reg1_index];
                            int c = ID.offset;
                                int two = 1;
                                for (int h = 0 ; h < c ; h++) {
                                two = two * 2;
                                }
                                value = (int) (value/two);
                                EX.aluresult = value ;
                        }
                        else if (ID.type_instruction == "lw"){// structural hazard will be there, memory will be assesed all at the same time
                             EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.dest_register = ID.dest_index;
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]);
                    
                            //IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                            //ye hum next me nhi use nhi kr skte kyonki saari states ek saath chage hoti hai
                            // EX.type_instruction = "STALL2"; //stall 2 is used for structural hazard
                            // continue;
                            //continue se wapas jha se while ki starting hoti hai wahi se shuru hota hai
                        }
                        else if (ID.type_instruction == "sw"){
                            EX.instr = ID.instr;
                            EX.pc = ID.pc;
                            EX.type_instruction = ID.type_instruction;
                            EX.swdata = registers[ID.dest_index];
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                        }
                        
            }
                    
            else if(EX.type_instruction == "lw"){
                
                            if(ID.type_instruction == "NOOP"||
                               ID.type_instruction=="STALL1"||ID.type_instruction=="STALL2"||ID.type_instruction=="STALL3"||
                    ID.type_instruction=="STALL4_bne"||ID.type_instruction=="STALL4_beq"||ID.type_instruction=="STALL4_bgtz"||ID.type_instruction=="STALL4_blez"||
                               ID.type_instruction=="beq"||ID.type_instruction=="bne"||ID.type_instruction=="bgtz"||ID.type_instruction=="blez"||
                               ID.type_instruction=="j" || ID.type_instruction=="jr" || ID.type_instruction=="jal" || ID.type_instruction == "HALT")
                            {
                                EX.instr = ID.instr;
                                EX.pc = ID.pc;
                                EX.type_instruction = ID.type_instruction;
                            }
                            else if (ID.type_instruction == "add"){
                                if(EX.dest_register == ID.reg1_index){
                                   // IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1"; //stall 1 just ye represent krti hai ki humne ek stall ka wait kiya hai
                                    continue;
                                }
                                else if(EX.dest_register == ID.reg2_index){
                                  //  IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{ //ye bina hazard ke hai
                                          EX.instr = ID.instr;
                                          EX.pc = ID.pc;
                                          EX.type_instruction = ID.type_instruction;
                                    EX.dest_register = ID.dest_index;
                                    EX.aluresult = registers[ID.reg1_index] + registers[ID.reg2_index] ;
                                }
                            }
                            else if (ID.type_instruction == "sub"){
                                if(EX.dest_register == ID.reg1_index){
                                   // IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else if(EX.dest_register == ID.reg2_index){
                                  //  IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{ //ye bina hazard ke hai
                                    EX.instr = ID.instr;
                                    EX.pc = ID.pc;
                                    EX.type_instruction = ID.type_instruction;
                                    EX.dest_register = ID.dest_index;
                                    EX.aluresult = registers[ID.reg1_index] - registers[ID.reg2_index] ;
                                }
                            }
                            else if (ID.type_instruction == "sll"){
                                if(EX.dest_register == ID.reg1_index){
                                   // IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{ //ye bina hazard ke hai
                                    EX.instr = ID.instr;
                                    EX.pc = ID.pc;
                                    EX.type_instruction = ID.type_instruction;

                                    EX.dest_register = ID.dest_index;
                                    int value = registers[ID.reg1_index];
                                    int c = ID.offset;
                                    for (int k = 0 ; k < c ; k++ ){
                                        value = value * 2;
                                    }
                                    EX.aluresult = value;
                                }
                            }
                            else if (ID.type_instruction == "srl"){
                                if(EX.dest_register == ID.reg1_index){
                                    // IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                     EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{ //ye bina hazard ke hai
                                        EX.instr = ID.instr;
                                        EX.pc = ID.pc;
                                        EX.type_instruction = ID.type_instruction;

                                        EX.dest_register = ID.dest_index;
                                        int value = registers[ID.reg1_index];
                                        int c = ID.offset;
                                        int two = 1;
                                        for (int h = 0 ; h < c ; h++) {
                                            two = two * 2;
                                        }
                                        value = (int) (value/two);
                                        EX.aluresult = value ;
                                }
                            }
                            else if (ID.type_instruction == "sw"){
                                if(EX.dest_register == ID.dest_index){
                                   // IF.pc = IF.pc - 1;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else if(EX.dest_register == ID.reg1_index){
                                  //  IF.pc = IF.pc - 1;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{
                                    EX.instr = ID.instr;
                                    EX.pc = ID.pc;
                                    EX.type_instruction = ID.type_instruction;
                                    
                                    EX.swdata = registers[ID.dest_index];
                                    EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                                }
                            }
                            else if (ID.type_instruction == "lw"){
                                if(EX.dest_register == ID.reg1_index){
                                   // IF.pc = IF.pc - 1;  //instruction fetch na hone do;
                                    EX.type_instruction = "STALL1";
                                    continue;
                                }
                                else{
                                    EX.instr = ID.instr;
                                    EX.pc = ID.pc;
                                    EX.type_instruction = ID.type_instruction;
                                    
                                    EX.dest_register = ID.dest_index;
                                    EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                                }
                            }
                            
            }
                
            else if (EX.type_instruction == "STALL1"){
                    EX.instr = ID.instr;
                    EX.pc = ID.pc;
                    EX.type_instruction = ID.type_instruction;
                    
                    if(EX.type_instruction == "add"){
                       if(EX.dest_register == ID.reg1_index){
                             EX.dest_register = ID.dest_index;
                             EX.aluresult = MEM.writedata + registers[ID.reg2_index] ;
                       }
                       else if(EX.dest_register == ID.reg2_index){
                             EX.dest_register = ID.dest_index;
                             EX.aluresult = MEM.writedata + registers[ID.reg1_index] ;
                       }
                    }
                    else if(EX.type_instruction == "sub"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = MEM.writedata - registers[ID.reg2_index] ;
                        }
                        else if(EX.dest_register == ID.reg2_index){
                            EX.dest_register = ID.dest_index;
                            EX.aluresult = registers[ID.reg1_index] - MEM.writedata;
                        }
                    }
                    else if(EX.type_instruction == "sll"){
                        if(EX.dest_register == ID.reg1_index){
                              EX.dest_register = ID.dest_index;
                              int value = MEM.writedata;
                              int c = ID.offset;
                              for (int k = 0 ; k < c ; k++ ){
                                  value = value * 2;
                              }
                              EX.aluresult = value;
                        }
                    }
                    else if(EX.type_instruction == "srl"){
                        if(EX.dest_register == ID.reg1_index){
                              EX.dest_register = ID.dest_index;
                              int value = MEM.writedata;
                              int c = ID.offset;
                              int two = 1;
                              for (int h = 0 ; h < c ; h++) {
                                  two = two * 2;
                              }
                              value = (int) (value/two);
                              EX.aluresult = value ;
                        }
                    }
                    else if(EX.type_instruction == "lw"){
                        if(EX.dest_register == ID.reg1_index){
                            EX.dest_register = ID.dest_index;
                            EX.dest_memory  =  (ID.offset + MEM.writedata) ;
                        }
                    }
                    else if(EX.type_instruction == "sw"){
                        if(EX.dest_register == ID.dest_index){
                            EX.swdata = MEM.writedata;
                            EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                        }
                        else if(EX.dest_register == ID.reg1_index){
                          EX.swdata = registers[ID.dest_index];
                          EX.dest_memory  =  ( ID.offset + MEM.writedata ) ;
                        }
                    }

                }
                
            else if (EX.type_instruction == "STALL2"){
                EX.instr = ID.instr;
                EX.pc = ID.pc;
                EX.type_instruction = ID.type_instruction;
                
                if(EX.type_instruction == "lw"){
                    EX.dest_register = ID.dest_index;
                    EX.dest_memory  =  (ID.offset + registers[ID.reg1_index]) ;
                }
            }
            
        
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //INSTRUCTION DECODE KA START

              ID.instr = IF.instr;
              ID.pc = IF.pc;
              stringstream sep(ID.instr);
              sep >> ID.type_instruction;
              if(ID.type_instruction == "NOOP"){
                  
              }
              else if (ID.type_instruction == "add"){
                  string destreg;
                  string reg1;
                  string reg2;
                  sep >> destreg ;
                  sep >> reg1 ;
                  sep >> reg2 ;
                  ID.dest_index  =  register_address(destreg);
                  ID.reg1_index = register_address(reg1);
                  ID.reg2_index = register_address(reg2);
                }
                else if(ID.type_instruction == "sub"){
                   string destreg;
                   string reg1;
                   string reg2;
                   sep >> destreg ;
                   sep >> reg1 ;
                   sep >> reg2 ;
                   ID.dest_index  =  register_address(destreg);
                   ID.reg1_index = register_address(reg1);
                   ID.reg2_index = register_address(reg2);
                }
                else if(ID.type_instruction == "sll"){
                   string destreg;
                   sep >> destreg;
                   ID.dest_index  =  register_address(destreg);
                    string reg1;
                    sep >> reg1;
                    ID.reg1_index = register_address(reg1);
                  string offsetstr;
                  sep >> offsetstr;
                  stringstream b1(offsetstr);
                  int c;
                  b1>>c;
                  ID.offset = c;
                }
                else if(ID.type_instruction == "srl"){
                 string destreg;
                   sep >> destreg;
                   ID.dest_index  =  register_address(destreg);
                    string reg1;
                    sep >> reg1;
                    ID.reg1_index = register_address(reg1);
                  string offsetstr;
                  sep >> offsetstr;
                  stringstream b1(offsetstr);
                  int c;
                  b1>>c;
                  ID.offset = c;
                }
                else if(ID.type_instruction == "lw"){
                    string destreg;
                    sep >> destreg;
                    ID.dest_index  =  register_address(destreg);
                    
                    string offsetreg_str;
                    sep >> offsetreg_str;
                    int len = offsetreg_str.size();
                    int h=0;
                    for (int k=0; k<len ; k++){
                        while( offsetreg_str[h] != '(' ){
                            h++;
                        }
                        break;
                    }
                    string offs = "";
                    for (int g=0; g<h ; g++){
                        offs += offsetreg_str[g];
                    }
                    stringstream b1(offs);
                    int b;
                    b1 >> b;
                    
                    string reg = "";
                    for (int g=0 ; g< (len-h-2) ; g++){
                        reg += offsetreg_str[g+h+1];
                    }
                    
                    ID.offset = b;
                    ID.reg1_index =  register_address(reg);
                    
                }
                else if(ID.type_instruction == "sw"){
                    string destreg;
                    sep >> destreg;
                    ID.dest_index  =  register_address(destreg);
                    
                    string offsetreg_str;
                    sep >> offsetreg_str;
                    int len = offsetreg_str.size();
                    int h=0;
                    for (int k=0; k<len ; k++){
                        while( offsetreg_str[h] != '(' ){
                            h++;
                        }
                        break;
                    }
                    string offs = "";
                    for (int g=0; g<h ; g++){
                        offs += offsetreg_str[g];
                    }
                    stringstream b1(offs);
                    int b;
                    b1 >> b;
                    
                    string reg = "";
                    for (int g=0 ; g< (len-h-2) ; g++){
                        reg += offsetreg_str[g+h+1];
                    }
                    
                    ID.offset = b;
                    ID.reg1_index =  register_address(reg);
                }
                 else if(ID.type_instruction == "beq"){
                     string reg1;
                     string reg2;
                     sep >> reg1 >> reg2;
                     ID.reg1_index = register_address(reg1);
                     ID.reg2_index = register_address(reg2);
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
  //abhi yha jo me ex aur mem use kr rhi hu, agar asli hardware hota to yhi value id aur ex ki hoti to bas hum to uske similar model bana rhe hai to no matter
                if(EX.type_instruction == "NOOP"||
                   EX.type_instruction == "sw"||
                   EX.type_instruction == "beq"||EX.type_instruction == "bne"||EX.type_instruction == "blez"||EX.type_instruction == "bgtz"||
                   EX.type_instruction=="STALL1"||EX.type_instruction=="STALL2"||EX.type_instruction=="STALL3"||
                   EX.type_instruction=="STALL4_beq"||EX.type_instruction=="STALL4_bne"||EX.type_instruction=="STALL4_bgtz"||EX.type_instruction=="STALL4_blez"||
                   EX.type_instruction=="j"||EX.type_instruction=="jr"||EX.type_instruction=="jal")
                {
                       if(MEM.type_instruction == "NOOP"||
                          MEM.type_instruction == "sw"||
                          MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                          MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                          MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                       {
                          //kuch na kro
                       }
                       else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                               ||MEM.type_instruction == "lw"){
                                   if(MEM.dest_register == ID.reg1_index){
                                       if(MEM.writedata == registers[ID.reg2_index]){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                                   else if(MEM.dest_register == ID.reg2_index){
                                       if(MEM.writedata == registers[ID.reg1_index]){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                                   else{
                                       if(registers[ID.reg1_index] == registers[ID.reg2_index]){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                       }
                }
                else if(EX.type_instruction == "add"||EX.type_instruction == "sub"||EX.type_instruction == "sll"||EX.type_instruction == "srl"){
                         if(MEM.type_instruction == "NOOP"||
                            MEM.type_instruction == "sw"||
                            MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                            MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                            MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                         {
                                 if(EX.dest_register == ID.reg1_index){
                                     if(registers[ID.reg2_index] == EX.aluresult){ //equal hai
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                                 else if(EX.dest_register == ID.reg2_index){
                                     if(registers[ID.reg1_index] == EX.aluresult){
                                        IF.pc = new_pc;
                                        IF.instr = "STALL3 0 0 0";
                                        continue;
                                     }
                                 }
                                 else{
                                     if(registers[ID.reg1_index] == registers[ID.reg2_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                         }
                         else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                 ||MEM.type_instruction == "lw"){
                             if(EX.dest_register == ID.reg1_index){
                                 if(MEM.dest_register == ID.reg2_index){
                                     if(EX.aluresult == MEM.writedata){
                                          IF.pc = new_pc;
                                          IF.instr = "STALL3 0 0 0";
                                          continue;
                                     }
                                 }
                                 else{
                                     if(EX.aluresult == registers[ID.reg2_index]){
                                          IF.pc = new_pc;
                                          IF.instr = "STALL3 0 0 0";
                                          continue;
                                     }
                                 }
                             }
                             else if(EX.dest_register == ID.reg2_index){
                                 if(MEM.dest_register == ID.reg1_index){
                                        if(EX.aluresult == MEM.writedata){
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                 }
                                 else{
                                     if(EX.aluresult == registers[ID.reg1_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                             }
                             else{
                                 if(MEM.dest_register == ID.reg1_index){
                                     if(MEM.writedata == registers[ID.reg2_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                                 else if(MEM.dest_register == ID.reg2_index){
                                     if(MEM.writedata == registers[ID.reg1_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                                 else{
                                     if(registers[ID.reg1_index] == registers[ID.reg2_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                             }
                         }
                    
                     }
                   else if(EX.type_instruction == "lw"){
                            if(MEM.type_instruction == "NOOP"||
                               MEM.type_instruction == "sw"||
                               MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                               MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                               MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                            {
                                    if(EX.dest_register == ID.reg1_index){
                                        //ab isme EX.dest_register == reg1 hai par uski value to humko memory read krne wale step me
                                        //that is MEM me hi milegi to fir yha ek aur loop chalana hai
                                        IF.instr = "STALL4_beq 0 0 0";
                                        continue;
                                    }
                                    else if(EX.dest_register == ID.reg2_index){
                                        IF.instr = "STALL4_beq 0 0 0";
                                        continue;
                                    }
                                    else{
                                        if(registers[ID.reg1_index] == registers[ID.reg2_index]){
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                    }
                            }
                            else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                    ||MEM.type_instruction == "lw"){
                                if(EX.dest_register == ID.reg1_index){
                                    IF.instr = "STALL4_beq 0 0 0";
                                    continue;
                                }
                                else if(EX.dest_register == ID.reg2_index){
                                    IF.instr = "STALL4_beq 0 0 0";
                                    continue;
                                }
                                else{
                                    if(MEM.dest_register == ID.reg1_index){
                                        if(MEM.writedata == registers[ID.reg2_index]){
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                    }
                                    else if(MEM.dest_register == ID.reg2_index){
                                        if(MEM.writedata == registers[ID.reg1_index]){
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                    }
                                    else{
                                        if(registers[ID.reg1_index] == registers[ID.reg2_index]){
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                    }
                                }
                            }
                     }
                     
                 }
                else if(ID.type_instruction == "bne")
                {
                     string reg1;
                     string reg2;
                     sep >> reg1 >> reg2;
                     ID.reg1_index = register_address(reg1);
                     ID.reg2_index = register_address(reg2);
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
                    //abhi yha jo me ex aur mem use kr rhi hu, agar asli hardware hota to yhi value id aur ex ki hoti to bas hum to uske similar model bana rhe hai to no matter
                     if(EX.type_instruction == "NOOP"||
                        EX.type_instruction == "sw"||
                        EX.type_instruction == "beq"||EX.type_instruction == "bne"||EX.type_instruction == "blez"||EX.type_instruction == "bgtz"||
                        EX.type_instruction=="STALL1"||EX.type_instruction=="STALL2"||EX.type_instruction=="STALL3"||
                   EX.type_instruction=="STALL4_beq"||EX.type_instruction=="STALL4_bne"||EX.type_instruction=="STALL4_bgtz"||EX.type_instruction=="STALL4_blez"||
                        EX.type_instruction=="j"||EX.type_instruction=="jr"||EX.type_instruction=="jal")
                        {
                            if(MEM.type_instruction == "NOOP"||
                               MEM.type_instruction == "sw"||
                               MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                               MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                               MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                            {
                                            //kuch na kro
                            }
                            else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                    ||MEM.type_instruction == "lw")
                            {
                                 if(MEM.dest_register == ID.reg1_index)
                                 {
                                     if(MEM.writedata != registers[ID.reg2_index]){
                                             IF.pc = new_pc;
                                             IF.instr = "STALL3 0 0 0";
                                             continue;
                                     }
                                 }
                                 else if(MEM.dest_register == ID.reg2_index){
                                        if(MEM.writedata != registers[ID.reg1_index])
                                        {
                                            IF.pc = new_pc;
                                            IF.instr = "STALL3 0 0 0";
                                            continue;
                                        }
                                 }
                                 else{
                                        if(registers[ID.reg1_index] != registers[ID.reg2_index]){
                                                IF.pc = new_pc;
                                                IF.instr = "STALL3 0 0 0";
                                                continue;
                                        }
                                     }
                              }
                        }
                        
                        else if(EX.type_instruction == "add"||EX.type_instruction == "sub"||EX.type_instruction == "sll"||EX.type_instruction == "srl")
                        {
                             if(MEM.type_instruction == "NOOP"||
                                MEM.type_instruction == "sw"||
                                MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                                MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                                MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                                {
                                    if(EX.dest_register == ID.reg1_index)
                                    {
                                       if(registers[ID.reg2_index] != EX.aluresult){ //equal hai
                                                IF.pc = new_pc;
                                                IF.instr = "STALL3 0 0 0";
                                                continue;
                                        }
                                    }
                                    else if(EX.dest_register == ID.reg2_index)
                                    {
                                        if(registers[ID.reg1_index] != EX.aluresult){
                                                 IF.pc = new_pc;
                                                 IF.instr = "STALL3 0 0 0";
                                                 continue;
                                        }
                                    }
                                    else
                                    {
                                         if(registers[ID.reg1_index] != registers[ID.reg2_index]){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                         }
                                     }
                                }
                                else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                        ||MEM.type_instruction == "lw")
                                {
                                        if(EX.dest_register == ID.reg1_index)
                                        {
                                                if(MEM.dest_register == ID.reg2_index){
                                                         if(EX.aluresult != MEM.writedata){
                                                              IF.pc = new_pc;
                                                              IF.instr = "STALL3 0 0 0";
                                                              continue;
                                                         }
                                                }
                                                else{
                                                     if(EX.aluresult != registers[ID.reg2_index]){
                                                            IF.pc = new_pc;
                                                            IF.instr = "STALL3 0 0 0";
                                                            continue;
                                                       }
                                                   }
                                         }
                                         else if(EX.dest_register == ID.reg2_index){
                                                   if(MEM.dest_register == ID.reg1_index){
                                                          if(EX.aluresult != MEM.writedata){
                                                              IF.pc = new_pc;
                                                              IF.instr = "STALL3 0 0 0";
                                                              continue;
                                                          }
                                                   }
                                                   else{
                                                       if(EX.aluresult != registers[ID.reg1_index]){
                                                           IF.pc = new_pc;
                                                           IF.instr = "STALL3 0 0 0";
                                                           continue;
                                                       }
                                                   }
                                         }
                                         else{
                                                   if(MEM.dest_register == ID.reg1_index){
                                                       if(MEM.writedata != registers[ID.reg2_index]){
                                                           IF.pc = new_pc;
                                                           IF.instr = "STALL3 0 0 0";
                                                           continue;
                                                       }
                                                   }
                                                   else if(MEM.dest_register == ID.reg2_index){
                                                       if(MEM.writedata != registers[ID.reg1_index]){
                                                           IF.pc = new_pc;
                                                           IF.instr = "STALL3 0 0 0";
                                                           continue;
                                                       }
                                                   }
                                                   else{
                                                       if(registers[ID.reg1_index] != registers[ID.reg2_index]){
                                                           IF.pc = new_pc;
                                                           IF.instr = "STALL3 0 0 0";
                                                           continue;
                                                       }
                                                   }
                                               }
                                           }
                                      
                        }
                        else if(EX.type_instruction == "lw")
                        {
                                  if(MEM.type_instruction == "NOOP"||
                                     MEM.type_instruction == "sw"||
                                     MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                                     MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                                     MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                                 {
                                        if(EX.dest_register == ID.reg1_index){
                                                          //ab isme EX.dest_register == reg1 hai par uski value to humko memory read krne wale step me
                                                          //that is MEM me hi milegi to fir yha ek aur loop chalana hai
                                         IF.instr = "STALL4_bne 0 0 0";
                                         continue;
                                        }
                                        else if(EX.dest_register == ID.reg2_index){
                                            IF.instr = "STALL4_bne 0 0 0";
                                            continue;
                                         }
                                        else{
                                            if(registers[ID.reg1_index] != registers[ID.reg2_index]){
                                                IF.pc = new_pc;
                                                IF.instr = "STALL3 0 0 0";
                                                continue;
                                         }
                                                      }
                                 }
                                else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                        ||MEM.type_instruction == "lw")
                                {
                                        if(EX.dest_register == ID.reg1_index){
                                            IF.instr = "STALL4_bne 0 0 0";
                                            continue;
                                        }
                                        else if(EX.dest_register == ID.reg2_index){
                                            IF.instr = "STALL4_bne 0 0 0";
                                            continue;
                                        }
                                        else{
                                            if(MEM.dest_register == ID.reg1_index){
                                                if(MEM.writedata != registers[ID.reg2_index]){
                                                        IF.pc = new_pc;
                                                        IF.instr = "STALL3 0 0 0";
                                                        continue;
                                                }
                                            }
                                            else if(MEM.dest_register == ID.reg2_index){
                                                if(MEM.writedata != registers[ID.reg1_index]){
                                                        IF.pc = new_pc;
                                                        IF.instr = "STALL3 0 0 0";
                                                        continue;
                                                }
                                            }
                                            else{
                                                if(registers[ID.reg1_index] != registers[ID.reg2_index]){
                                                        IF.pc = new_pc;
                                                        IF.instr = "STALL3 0 0 0";
                                                        continue;
                                                }
                                            }
                                         }
                                    }
                    }
                }
                 
                 else if(ID.type_instruction == "bgtz"){
                     string reg1;
                     sep >> reg1;
                     ID.reg1_index = register_address(reg1);
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
                     
                     if(EX.type_instruction == "NOOP"||
                        EX.type_instruction == "sw"||
                        EX.type_instruction == "beq"||EX.type_instruction == "bne"||EX.type_instruction == "blez"||EX.type_instruction == "bgtz"||
                        EX.type_instruction=="STALL1"||EX.type_instruction=="STALL2"||EX.type_instruction=="STALL3"||
                    EX.type_instruction=="STALL4_beq"||EX.type_instruction=="STALL4_bne"||EX.type_instruction=="STALL4_bgtz"||EX.type_instruction=="STALL4_blez"||
                        EX.type_instruction=="j"||EX.type_instruction=="jr"||EX.type_instruction=="jal")
                     {
                         if(MEM.type_instruction == "NOOP"||
                            MEM.type_instruction == "sw"||
                            MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                            MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                            MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                          {
                             //kuch na kro
                          }
                          else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                 ||MEM.type_instruction == "lw")
                          {
                              if(MEM.dest_register == ID.reg1_index){
                                  if(MEM.writedata > 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                              else{
                                  if(registers[ID.reg1_index] > 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                          }
                     }
                     else if(EX.type_instruction == "add"||EX.type_instruction == "sub"||EX.type_instruction == "sll"||EX.type_instruction == "srl")
                     {
                          if(MEM.type_instruction == "NOOP"||
                             MEM.type_instruction == "sw"||
                             MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                             MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                             MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                          {
                              if(EX.dest_register == ID.reg1_index){
                                  if(EX.aluresult > 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                              else{
                                  if(registers[ID.reg1_index] > 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                          }
                          else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                 ||MEM.type_instruction == "lw")
                          {
                              if(EX.dest_register == ID.reg1_index){
                                  if(EX.aluresult > 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                              else{
                                  if(MEM.dest_register == ID.reg1_index){
                                      if(MEM.writedata > 0){
                                          IF.pc = new_pc;
                                          IF.instr = "STALL3 0 0 0";
                                          continue;
                                      }
                                  }
                                  else{
                                      if(registers[ID.reg1_index] > 0){
                                          IF.pc = new_pc;
                                          IF.instr = "STALL3 0 0 0";
                                          continue;
                                      }
                                  }
                              }
                          }
                     }
                     else if(EX.type_instruction == "lw")
                     {
                         if(MEM.type_instruction == "NOOP"||
                            MEM.type_instruction == "sw"||
                            MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                            MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                            MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                         {
                             if(EX.dest_register == ID.reg1_index){
                                  IF.instr = "STALL4_bgtz 0 0 0";
                                  continue;
                             }
                             else{
                                 if(registers[ID.reg1_index] > 0){
                                     IF.pc = new_pc;
                                     IF.instr = "STALL3 0 0 0";
                                     continue;
                                 }
                             }
                         }
                         else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                ||MEM.type_instruction == "lw")
                         {
                             if(EX.dest_register == ID.reg1_index){
                                  IF.instr = "STALL4_bgtz 0 0 0";
                                  continue;
                             }
                             else{
                                 if(MEM.dest_register == ID.reg1_index){
                                     if(MEM.writedata > 0){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                                 else{
                                     if(registers[ID.reg1_index] > 0){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                             }
                         }
                     }
                 }
                     
                 else if(ID.type_instruction == "blez"){
                     string reg1;
                     sep >> reg1;
                     ID.reg1_index = register_address(reg1);
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
                     
                     if(EX.type_instruction == "NOOP"||
                        EX.type_instruction == "sw"||
                        EX.type_instruction == "beq"||EX.type_instruction == "bne"||EX.type_instruction == "blez"||EX.type_instruction == "bgtz"||
                        EX.type_instruction=="STALL1"||EX.type_instruction=="STALL2"||EX.type_instruction=="STALL3"||
                    EX.type_instruction=="STALL4_beq"||EX.type_instruction=="STALL4_bne"||EX.type_instruction=="STALL4_bgtz"||EX.type_instruction=="STALL4_blez"||
                        EX.type_instruction=="j"||EX.type_instruction=="jr"||EX.type_instruction=="jal")
                     {
                             if(MEM.type_instruction == "NOOP"||
                                MEM.type_instruction == "sw"||
                                MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                                MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                                MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                             {
                                //kuch na kro
                             }
                             else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                    ||MEM.type_instruction == "lw")
                             {
                                 if(MEM.dest_register == ID.reg1_index){
                                     if(MEM.writedata <= 0){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }
                                 else{
                                     if(registers[ID.reg1_index] <= 0){
                                         IF.pc = new_pc;
                                         IF.instr = "STALL3 0 0 0";
                                         continue;
                                     }
                                 }

                             }
                     }
                     else if(EX.type_instruction == "add"||EX.type_instruction == "sub"||EX.type_instruction == "sll"||EX.type_instruction == "srl")
                     {
                               if(MEM.type_instruction == "NOOP"||
                                  MEM.type_instruction == "sw"||
                                  MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                                  MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                                  MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                               {
                                   if(EX.dest_register == ID.reg1_index){
                                       if(EX.aluresult <= 0){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                                   else{
                                       if(registers[ID.reg1_index] <= 0){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                               }
                               else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                      ||MEM.type_instruction == "lw")
                               {
                                       if(EX.dest_register == ID.reg1_index){
                                           if(EX.aluresult <= 0){
                                               IF.pc = new_pc;
                                               IF.instr = "STALL3 0 0 0";
                                               continue;
                                           }
                                       }
                                       else{
                                           if(MEM.dest_register == ID.reg1_index){
                                               if(MEM.writedata <= 0){
                                                   IF.pc = new_pc;
                                                   IF.instr = "STALL3 0 0 0";
                                                   continue;
                                               }
                                           }
                                           else{
                                               if(registers[ID.reg1_index] <= 0){
                                                   IF.pc = new_pc;
                                                   IF.instr = "STALL3 0 0 0";
                                                   continue;
                                               }
                                           }
                                       }
                               }
                     }
                     else if(EX.type_instruction == "lw")
                     {
                           if(MEM.type_instruction == "NOOP"||
                              MEM.type_instruction == "sw"||
                              MEM.type_instruction == "STALL1"||MEM.type_instruction == "STALL2"||MEM.type_instruction=="STALL3"||
                MEM.type_instruction=="STALL4_beq"||MEM.type_instruction=="STALL4_bne"||MEM.type_instruction=="STALL4_bgtz"||MEM.type_instruction=="STALL4_blez"||
                              MEM.type_instruction=="beq"||MEM.type_instruction=="bne"||MEM.type_instruction=="blez"||MEM.type_instruction=="bgtz"||
                              MEM.type_instruction=="j"||MEM.type_instruction=="jr"||MEM.type_instruction=="jal")
                           {
                              if(EX.dest_register == ID.reg1_index){
                                   IF.instr = "STALL4_blez 0 0 0";
                                   continue;
                              }
                              else{
                                  if(registers[ID.reg1_index] <= 0){
                                      IF.pc = new_pc;
                                      IF.instr = "STALL3 0 0 0";
                                      continue;
                                  }
                              }
                           }
                           else if(MEM.type_instruction == "add"||MEM.type_instruction == "sub"||MEM.type_instruction == "sll"||MEM.type_instruction == "srl"
                                  ||MEM.type_instruction == "lw")
                           {
                               if(EX.dest_register == ID.reg1_index){
                                    IF.instr = "STALL4_blez 0 0 0";
                                    continue;
                               }
                               else{
                                   if(MEM.dest_register == ID.reg1_index){
                                       if(MEM.writedata <= 0){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                                   else{
                                       if(registers[ID.reg1_index] <= 0){
                                           IF.pc = new_pc;
                                           IF.instr = "STALL3 0 0 0";
                                           continue;
                                       }
                                   }
                               }

                           }
                     }

                 }
                 else if(ID.type_instruction == "STALL3"){
                     
                 }
                 else if(ID.type_instruction == "STALL4_beq"){
                     int new_pc = labels[ID.label];
                     if(MEM.dest_register == ID.reg1_index){
                         if(MEM.writedata == registers[ID.reg2_index]){
                             IF.pc = new_pc;
                             IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                     else if(MEM.dest_register == ID.reg2_index){
                         if(MEM.writedata == registers[ID.reg1_index]){
                              IF.pc = new_pc;
                              IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                 }
                 else if(ID.type_instruction == "STALL4_bne"){
                     int new_pc = labels[ID.label];
                     if(MEM.dest_register == ID.reg1_index){
                         if(MEM.writedata != registers[ID.reg2_index]){
                              IF.pc = new_pc;
                              IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                     else if(MEM.dest_register == ID.reg2_index){
                         if(MEM.writedata != registers[ID.reg1_index]){
                             IF.pc = new_pc;
                             IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                 }
                 else if(ID.type_instruction == "STALL4_bgtz"){
                     int new_pc = labels[ID.label];
                     if(MEM.dest_register == ID.reg1_index){
                         if(MEM.writedata > 0){
                             IF.pc = new_pc;
                             IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                 }
                 else if(ID.type_instruction == "STALL4_blez"){
                     int new_pc = labels[ID.label];
                     if(MEM.dest_register == ID.reg1_index){
                         if (MEM.writedata <= 0 ) {
                             IF.pc = new_pc;
                             IF.instr = "STALL3 0 0 0";
                             continue;
                         }
                     }
                 }
                 else if(ID.type_instruction == "j"){  //jump ke liye humne stall3 hi use kar liya
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
                     IF.pc = new_pc;
                     IF.instr = "STALL3 0 0 0";
                     continue;
                 }
                 else if(ID.type_instruction == "jr"){ //jr me bhi humne stall 3 hi use kar liya
                     string reg;
                     sep >> reg;
                     int regindex = register_address(reg) ;
                     int new_pc = registers[regindex];
                     IF.pc = new_pc;
                     IF.instr = "STALL3 0 0 0";
                     continue;
                 }
                 else if(ID.type_instruction == "jal"){ //jal ke liye bhi stall 3 hi use kar liya hai mene
                     string lab;
                     sep >> lab;
                     ID.label = lab;
                     int new_pc = labels[ID.label];
                     registers[31] = ID.pc ;
                     IF.pc = new_pc;
                     IF.instr = "STALL3 0 0 0";
                     continue;
                 }
 
 
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //INSTRUCTION FETCH KA START
                     
            IF.instr = memory[IF.pc] ;
            IF.pc = IF.pc + 1 ;

        }
    }
}

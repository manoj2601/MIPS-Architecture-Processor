#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <iterator>
using namespace std;

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

void printregisters(int registers[]){
  for(int i=0; i<32; i++)
    cout<<regIndexToString(i)<<" : "<<registers[i]<<"\t";
  cout<<endl<<endl;
}
void printmemory(string memory[], int total){
  for(int i=total; i<5000; i++)
  {
    if(memory[i] != "")
      cout<<"memory["<<i<<"] : "<<memory[i]<<endl;
  }
}

int main()
{
    int registers[32] = {0};
    string memory[5000];
    int program_counter = 0;
    map<string,int> labels; //it is like a hashtable in with key = label(string) and value(uske just aage wala instruction ka index)
//matlab humne labels me labels along with their instruction number daale hain
    map<string,int> time;
    int totaltime=0;
    int totalins = 0;

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
ifstream infile2;
infile2.open("delays.txt");

string s1;
    while(getline(infile2, s1))
    {
      stringstream sep(s1);
      string one;
      sep>>one;
      int a;
      sep>>a;
      time[one] = a;
    }
infile2.close();
infile.close();

while(memory[program_counter] != "")	//ith instruction
{
	cout<<"instruction : "<<memory[program_counter]<<endl;
	stringstream sep(memory[program_counter]);
	string one;
	sep >> one;

    if(one == "add")
    {
          totaltime += time["add"];
          totalins++;
          string reg1, reg2, reg3;
          sep>>reg1;
          sep>>reg2;
          sep>>reg3;
        int a = register_address(reg1);
        int b = register_address(reg2);
        int c = register_address(reg3);

        registers[a] = registers[b] + registers[c] ;
    }

    else if(one == "sub")
    {
      totaltime += time["sub"];
      totalins++;
      //same as above
      string reg1, reg2, reg3;
      sep>>reg1;
      sep>>reg2;
      sep>>reg3;

//execute your add instruction here
// cin>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;

      int a = register_address(reg1);
      int b = register_address(reg2);
      int c = register_address(reg3);

      registers[a] = registers[b] - registers[c] ;
   }

    else if (one == "lw")
    {
        totaltime += time["lw"];
        totalins++;
        // cin>>instruction_3[0]>>instruction_3[1]>>instruction_3[2]
        string instruction_3[3];
        instruction_3[0] = "lw";
        sep>>instruction_3[1];
        sep>>instruction_3[2];
        int a = register_address(instruction_3[1]);

        int memory_len = instruction_3[2].size();
        int h = 0;
        for (int k = 0 ; k < memory_len ; k++){
        if(instruction_3[2][k] != '(')
        {
        h++;
        }
        break;
        }
        string addr = "";
        // char addr[h];
        for (int g = 0 ; g < h ; g++) {
        addr+= instruction_3[2][g];
        }
        string reg = "";
        // char reg[memory_len - h - 2];
        for (int g = 0 ; g< (memory_len - h - 2); g++)
        {
        reg += instruction_3[2][g+h+1];
        }

        // int b = boost::lexical_cast(addr);
        stringstream b1(addr);
        int b;
        b1>>b;

        int k = register_address(reg);
        int c = registers[k];
        
        stringstream str(memory[b+c]);
        int v ;
        str>>v;
        registers[a] = v;
    }

    else if(one == "sw")
    {
        totaltime += time["sw"];
        totalins++;
        // cin>>instruction_3[0]>>instruction_3[1]>>instruction_3[2]
        string instruction_3[3];
        instruction_3[0] = "sw";
        sep>>instruction_3[1];
        sep>>instruction_3[2];
        int a = register_address(instruction_3[1]);

        int memory_len = instruction_3[2].size();
        int h = 0;
        for (int k = 0 ; k < memory_len ; k++){
            if(instruction_3[2][k] != '(')
            {
                h++;
            }
            break;
        }
        string addr = "";
        // char addr[h];
        for (int g = 0 ; g < h ; g++){
        addr+= instruction_3[2][g];
        }
        string reg = "";
        // char reg[memory_len - h - 2];
        for (int g = 0 ; g< (memory_len - h - 2); g++)
        {
        reg += instruction_3[2][g+h+1];
        }
        stringstream b1(addr);
        int b;
        b1>>b;
        int k = register_address(reg);
        int c = registers[k];
         
        stringstream str1;
        str1 << registers[a];
        string s = str1.str();

        memory[b+c] = s ;

    }

    else if(one == "sll")
    {
        totaltime += time["sll"];
        totalins++;
        string instruction_4[4];
        // cin>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;
        instruction_4[0] = "sll";
        sep>>instruction_4[1];
        sep>>instruction_4[2];
        sep>>instruction_4[3];
        int a = register_address(instruction_4[1]);
        int b = register_address(instruction_4[2]);
        // int c = boost::lexical_cast(instruction_4[3]);
        stringstream b1(instruction_4[3]);
        int c;
        b1>>c;
        int value = registers[b];
        for (int k = 0 ; k < c ; k++ ){
        value = value * 2;
    }

    registers[a] = value;
    }

    else if (one == "srl")
    {
        totaltime += time["srl"];
        totalins++;
        //cin>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;

        string instruction_4[4];
        instruction_4[0] = "srl";
        sep>>instruction_4[1];
        sep>>instruction_4[2];
        sep>>instruction_4[3];
        int a = register_address(instruction_4[1]);
        int b = register_address(instruction_4[2]);
        // int c = boost::lexical_cast(instruction_4[3]);
        stringstream b1(instruction_4[3]);
        int c;
        b1>>c;
        int two = 1;
        for (int h = 0 ; h < c ; h++) {
        two = two * 2;
        }
        int value = (int)(registers[b]/two);
        registers[a] = value;
    }

    else if ( one == "bne")
    {
        totaltime += time["bne"];
        totalins++;
        // cin>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;
        string instruction_4[4];
        instruction_4[0] = "bne";
        sep>>instruction_4[1];
        sep>>instruction_4[2];
        sep>>instruction_4[3];
        int a = register_address(instruction_4[1]);
        int b = register_address(instruction_4[2]);

        if (registers[a] != registers[b])
        {
        program_counter = labels[instruction_4[3]];
        program_counter--;
    //mere according yha program_counter jo ki label me store kiya hai wahi hona chahiye
    //kyonki labels me jab jab label ka instruction number store kiya tha tab humne
    //instruction number ko increase nhi kiya tha !
    //aur instruction number wahi tha jo memory array me usko store krne wali block ka number ho
    //ye upar batayi gyi baate sahi hai par humne end me program counter ko +1 kiya hai to wha apne aap
//  ye value aa hi jaaegi jo humko chahiye
        }
    }

    else if (one == "beq")
    {
        totaltime += time["beq"];
        totalins++;
        // cin>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;
        // in>>instruction_4[0]>>instruction_4[1]>>instruction_4[2]>>instruction_4[3] ;
        string instruction_4[4];
        instruction_4[0] = "beq";
        sep>>instruction_4[1];
        sep>>instruction_4[2];
        sep>>instruction_4[3];
        int a = register_address(instruction_4[1]);
        int b = register_address(instruction_4[2]);

        if (registers[a] == registers[b])
        {
        program_counter = labels[instruction_4[3]];
        program_counter--;;
        }
    }

    else if (one == "blez")
    {
        totaltime += time["blez"];
        totalins++;
        // cin>>instruction_3[0]>>instruction_3[1]>>instruction_3[2] ;
        string instruction_3[3];
        instruction_3[0] = "blez";
        sep>>instruction_3[1];
        sep>>instruction_3[2];
        int a = register_address(instruction_3[1]);

        if (registers[a] <= 0)
        {
        program_counter = labels[instruction_3[2]];
        program_counter--;
        }
    }

    else if (one == "bgtz")
    {
        totaltime += time["bgtz"];
        totalins++;
        // cin>>instruction_3[0]>>instruction_3[1]>>instruction_3[2] ;
        string instruction_3[3];
        instruction_3[0] = "bgtz";
        sep>>instruction_3[1];
        sep>>instruction_3[2];
        int a = register_address(instruction_3[1]);

        if (registers[a] > 0)
        {
        program_counter = labels[instruction_3[2]];
        program_counter--;
        }
    }

//jump akela use hota hai par jump and link ke baad humesh jis bhi label pe jump kar rha hai us label
//ke end me jump register lagana padega jisme jis line se humne jump kiya tha uske next wala address
//store kiya hoga humne

    else if (one == "j")
    {
        totaltime += time["j"];
        totalins++;
        // cin>>instruction_2[0]>>instruction_2[1]
        string instruction_2[2];
        instruction_2[0] = "j";
        sep>>instruction_2[1];
        program_counter = labels[instruction_2[1]];
        program_counter--;
    }

    else if (one == "jr")
    {
        totaltime += time["jr"];
        totalins++;
        // cin>>instruction_2[0]>>instruction_2[1] ;
        string instruction_2[2];
        instruction_2[0] = "jr";
        sep>>instruction_2[1];
        int a = register_address(instruction_2[1]);
      //  program_counter = a;
      //yha a se to vo register ka index aaya hai ab us index wale register ke andar ki value nikalni
      //hain,  to most probably program counter uss register ke andar wali value hogi !!
      program_counter  =  registers[a];
        program_counter--;
    }

    else if (one == "jal")
    {
        totaltime += time["jal"];
        totalins++;
        // cin>>instruction_2[0]>>instruction_2[1]
        string instruction_2[2];
        instruction_2[0] = "jal";
        sep>>instruction_2[1];
        registers[31] = program_counter+1;
        program_counter = labels[instruction_2[1]];
        program_counter--;
    }

 	program_counter++;
 	printregisters(registers);
	printmemory(memory, currentinstruction);
}

int clock_cycle_count = totaltime ;
double avg_instruction_per_cycle = (totalins+0.0)/totaltime;
//your code goes here
cout << "Total number of clock cycle counts: " << clock_cycle_count << "\n";
cout << "Average instructions per cycle (IPC): "  << avg_instruction_per_cycle << "\n";

return 0;
}

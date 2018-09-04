#include<iostream>
#include<string.h>
#include<fstream>
#include<stdlib.h>
#include <stdio.h>
#include <sstream>
using namespace std;

//Representing Code Memory
struct Code_Line {
int file_line_number;  //: integer
int address;            //: integer (multiple of 4)
string instruction_string;  //: string
};

 //array of Code_Line - filled in by reading from code file.
struct Code_Memory {
Code_Line codemem[100];
} codeobj;

//Representing Data Memory
struct Data_Memory {
int base_address;  //: integer (where this starts)
int data_array[4000];//integers, indexed by offset of word address from base add
}data;


//Representing Register File
struct Register{
int address;
int value;
bool valid;
};

//: array of struct Registers
struct Register_File {
 Register R[16];
}reg_obj;


//Representing Pipeline Stage

struct Instruction_info {

string instruction_string;

string source_registers_add;
int source_registers_value;
int source_registers_index;

string source2_registers_add;
int source2_registers_value;
int source2_registers_index;

string dest_register_add;
int dest_register_index;
int dest_register_value;

string  opcode;
string literal_string;

int literal_value;
int pc_value;
int mem_buff_reg;
bool stall = false;
int flag = 0;

};

struct Bus
{
    int bus1;
    int bus2;
    int bus3=0;
    int bus4;
    int bus5;
    int bus6;
    int bus7;
    int bus8;
    int bus9;
}bus_pass;


struct stage {
Instruction_info *Input_Instruction;
Instruction_info *Output_Instruction;

bool initialize = false;
} fetch,dec_pass,exe_pass,mul_1_pass, mul_2_pass,mem_pass,wrt_pass,memtemp_pass;

//Flags:
struct Flags{
bool zero;  // : (true or false)
bool carry;  // : (true or false)
bool Negative;  // : (true or false)
};


//Collecting Stats:
struct Stats{
int cycle; //: integer (initialize to zero)
};

bool exe_flag;
bool mul_1_flag;
bool mul_2_flag;
bool fetch_flag;
bool decode_flag;
bool mul2_mem_flag;


bool decode_stall = false;
bool execute_stall = false;
bool memory_stall = false;
bool check_stall = false;



int convert( string s)
{

	// object from the class stringstream
	stringstream p(s);
	int x = 0;
	p >> x;
	return x;
}


int initialize()
{
        // setting the value data array to 0
        Data_Memory datamem;
        int j;
        for (j=0;j<=4000;j++)
        {
            datamem.data_array[j] =0;
        }
        int i;
        for (i=0;i<=16;i++)
        {
            reg_obj.R[i].value = 0;
            reg_obj.R[i].valid = true;
        }

            exe_flag = true ;
            mul_1_flag = true;
            mul_2_flag = true ;
            decode_flag =false;
            fetch_flag = false;
            mul2_mem_flag = false;


        cout<<"initialized all values"<<endl<<endl;
}


void Fetch(stage* fet, stage* dec)
{
    cout<<endl;
       cout<<"................... fetch ................"<< endl;

    fet->Output_Instruction->instruction_string = fet->Input_Instruction->instruction_string;
    fet->Output_Instruction->pc_value = fet->Input_Instruction->pc_value;

    cout<<"input inst"<<fet->Input_Instruction->instruction_string<<endl;
    cout <<"output inst"<<fet->Output_Instruction->instruction_string<<endl;

    dec->Input_Instruction->pc_value = fet->Output_Instruction->pc_value;
    dec->Input_Instruction->instruction_string = fet->Output_Instruction->instruction_string;


    cout<<endl;
	//dec->initialize = true;
}

void Decode( stage* fet,stage* dec,stage* exe,stage* mul_1,Register_File* reg, Bus* bus)  //Register_File* r
{
	//if (dec->initialize)
	{
       // if (!dec->Output_Instruction->stall)
        {
    	dec->Output_Instruction->instruction_string = dec->Input_Instruction->instruction_string;
        dec->Output_Instruction->pc_value = dec->Input_Instruction->pc_value;
     	cout<<"................... decode ................"<< endl;
    	string full_inst2;
        full_inst2 = dec->Output_Instruction->instruction_string;

    	cout<<full_inst2<<endl;
        int space_position = full_inst2.find (' ');
        // cout << space_position<<endl;
        string opcode;
        opcode = full_inst2.substr(0,space_position);
        dec->Output_Instruction->opcode = opcode;
        cout <<"opcode - "<< dec->Output_Instruction->opcode<<endl; //getting opcode
        string des_reg,
        lit,
        src_reg,
        src_reg2,
        leftover,
        x,a,c,
        leftover2;

        int coma_position,
        coma_position2,
        hash_position,
        space_position2,
        space_position3,
        y,b,d;
        //lit_value;


      if (opcode == "MUL")
      {

          leftover = full_inst2.substr(space_position+1,100);
          //cout<<leftover<<endl;
          coma_position = leftover.find(",");
          des_reg = leftover.substr(0,coma_position);
          dec->Output_Instruction->dest_register_add = des_reg;
          cout<<"dest reg add - "<<dec->Output_Instruction->dest_register_add<<endl;
          x = dec->Output_Instruction->dest_register_add;
          x.erase (x.begin());
          y = convert(x);
          dec->Output_Instruction->dest_register_index = y;
          cout<<"dest reg is LALA index int value - "<<dec->Output_Instruction->dest_register_index<<endl;

          space_position2 = leftover.find (" ");
          leftover2 = leftover.substr(space_position2+1,30);
          //cout<<leftover2<<endl;
          space_position3 = leftover2.find(" ");
          src_reg = leftover2.substr(0,space_position3-1);
          dec->Output_Instruction->source_registers_add = src_reg;
          cout<<"scr reg add - "<<dec->Output_Instruction->source_registers_add <<endl;
          a = dec->Output_Instruction->source_registers_add;
          a.erase(a.begin());
          b = convert(a);
          dec->Output_Instruction->source_registers_index = b;
          cout<<"src reg is index int value - "<<dec->Output_Instruction->source_registers_index<<endl;


          src_reg2 = leftover2.substr(space_position3+1);
          dec->Output_Instruction->source2_registers_add = src_reg2;
          cout<<"src2 reg add - "<<dec->Output_Instruction->source2_registers_add <<endl;
          c = dec->Output_Instruction->source2_registers_add;
          c.erase(c.begin());
          d  = convert(c);
          dec->Output_Instruction->source2_registers_index = d;
          cout<<"src2 reg is index int value "<<dec->Output_Instruction->source2_registers_index<<endl;

         /* if (reg->R[dec->Output_Instruction->dest_register_index].valid == true)
          {
              cout<<" dest reg is valid  , no stall "<<endl;
              decode_stall = false;
          }
          else
          {
              decode_stall= true;
              cout<< " stall due to output dependency"<<endl;
              return;
          } */


          //............. reading src 1 reg values..........

           if ((reg->R[dec->Output_Instruction->source_registers_index].valid == true ))

                {

                  dec->Output_Instruction->source_registers_value = reg->R[dec->Output_Instruction->source_registers_index].value;
                  cout<< " src1 is read "<<endl;

                  cout<<"no stall "<<endl;

                }

                else if ( dec->Output_Instruction->source_registers_index == bus->bus1)
                         {
                             dec->Output_Instruction->source_registers_value = bus->bus2;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                else if    ( dec->Output_Instruction->source_registers_index == bus->bus4)
                         {
                            dec->Output_Instruction->source_registers_value = bus->bus5;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                         else
                            {
                                return;
                            }
        //.................Reading src 2...............................
            if ((reg->R[dec->Output_Instruction->source2_registers_index].valid == true ))

                {

                  dec->Output_Instruction->source2_registers_value = reg->R[dec->Output_Instruction->source2_registers_index].value;
                  cout<< " src2 is read "<<endl;


                  /*reg->R[dec->Output_Instruction->dest_register_index].valid = false;
                  cout<<" Dest Reg R"<<dec->Output_Instruction->dest_register_index<<"is set invalid as "
                  <<reg->R[dec->Output_Instruction->dest_register_index].valid<<endl; */

                  //decode_stall = false;
                  cout<<"no stall "<<endl;

                }

                else if ( dec->Output_Instruction->source2_registers_index == bus->bus1)
                         {
                             dec->Output_Instruction->source2_registers_value = bus->bus2;
                             cout<<" data written in src 2 as  " <<dec->Output_Instruction->source2_registers_value<<endl;
                         }
                else if    ( dec->Output_Instruction->source2_registers_index == bus->bus4)
                         {
                            dec->Output_Instruction->source2_registers_value = bus->bus5;
                             cout<<" data written in src 2 as  " <<dec->Output_Instruction->source2_registers_value<<endl;
                         }
                         else
                            {
                                cout<< "just returning"<<endl;
                                return;
                            }



          mul_1_flag = false;
          exe_flag = true;

          cout<<endl;
            //if (decode_stall == false)
            //{



                cout<<"src 1 R"<<dec->Output_Instruction->source_registers_index << " stauts is "
                << reg->R[dec->Output_Instruction->source_registers_index].valid<<endl;

                cout<<"src 2 R"<<dec->Output_Instruction->source2_registers_index<<" status is"<<
                 reg->R[dec->Output_Instruction->source2_registers_index].valid<< endl;

                cout<<"des R"<<dec->Output_Instruction->dest_register_index<<" status is "<<
                reg->R[dec->Output_Instruction->dest_register_index].valid<<endl;

                 reg->R[dec->Output_Instruction->dest_register_index].valid = false;
                  cout<<" Dest Reg R"<<dec->Output_Instruction->dest_register_index<<"is set invalid as "
                  <<reg->R[dec->Output_Instruction->dest_register_index].valid<<endl;

            mul_1->Input_Instruction->opcode =                  dec->Output_Instruction->opcode;
            mul_1->Input_Instruction->dest_register_add =       dec->Output_Instruction->dest_register_add;
            mul_1->Input_Instruction->source_registers_add =    dec->Output_Instruction->source_registers_add;
            mul_1->Input_Instruction->source2_registers_add =   dec->Output_Instruction->source2_registers_add;
            mul_1->Input_Instruction->literal_string =          dec->Output_Instruction->literal_string;
            mul_1->Input_Instruction->literal_value =           dec->Output_Instruction->literal_value;
            mul_1->Input_Instruction->dest_register_index =     dec->Output_Instruction->dest_register_index;
            mul_1->Input_Instruction->source_registers_index=   dec->Output_Instruction->source_registers_index;
            mul_1->Input_Instruction->source2_registers_index=  dec->Output_Instruction->source2_registers_index;
            mul_1->Input_Instruction->instruction_string =      dec->Output_Instruction->instruction_string;
            mul_1->Input_Instruction->pc_value =                dec->Output_Instruction->pc_value;
            mul_1->Input_Instruction->dest_register_value=      dec->Output_Instruction->dest_register_value;
            mul_1->Input_Instruction->source_registers_value=   dec->Output_Instruction->source_registers_value;
            mul_1->Input_Instruction->source2_registers_value=  dec->Output_Instruction->source2_registers_value;
            //}

      }

      else
      {
             exe_flag = false;
          	 mul_1_flag = true;

      		if (opcode == "MOVC" || opcode == "JUMP" )
      		{

         	 leftover = full_inst2.substr(space_position+1,100);
         	 coma_position = leftover.find(",");
         	 des_reg = leftover.substr(0,coma_position);
         	 dec->Output_Instruction->dest_register_add = des_reg;
          	 cout<<"dest_reg addresss - "<<dec->Output_Instruction->dest_register_add<<endl;
         	 x = dec->Output_Instruction->dest_register_add;
          	 x.erase (x.begin());
          	 y = convert(x);
          	 dec->Output_Instruction->dest_register_index = y;
          	 cout<<"dest reg GAGA is index int value - "<<dec->Output_Instruction->dest_register_index<<endl;


             hash_position = full_inst2.find("#");
           	 lit = full_inst2.substr(hash_position+1);
         	 dec->Output_Instruction->literal_string = lit;
         	 cout<< "literal string is -  "<<dec->Output_Instruction->literal_string<<endl;
          	 dec->Output_Instruction->literal_value = convert(dec->Output_Instruction->literal_string);
          	 cout<<"literal int value is "<<dec->Output_Instruction->literal_value<<endl;
      		}



                         	 //........................READING VALUES..............
               /*   dec->Output_Instruction->dest_register_value = reg->R[dec->Output_Instruction->dest_register_index].value;

                   if (reg->R[dec->Output_Instruction->dest_register_index].valid == true)
          {
              cout<<" dest reg is valid  , no stall "<<endl;
              decode_stall = false;
          }
          else
          {
              decode_stall= true;
              cout<< " stall due to output dependency"<<endl;
              return;
          }
      		} */





       	else if(opcode == "ADD" || opcode ==  "SUB" || opcode ==  "OR" ||opcode ==  "EXOR" || opcode == "AND")
        {

             leftover = full_inst2.substr(space_position+1,100);
         	 //cout<<leftover<<endl;
         	 coma_position = leftover.find(",");
         	 des_reg = leftover.substr(0,coma_position);
         	 dec->Output_Instruction->dest_register_add = des_reg;
         	 cout<<"dest reg add - "<<dec->Output_Instruction->dest_register_add<<endl;
             x = dec->Output_Instruction->dest_register_add;
             x.erase (x.begin());
             y = convert(x);
           	 dec->Output_Instruction->dest_register_index = y;
             cout<<"dest reg is LALA index int value - "<<dec->Output_Instruction->dest_register_index<<endl;



          	space_position2 = leftover.find (" ");
          	leftover2 = leftover.substr(space_position2+1,30);
            //cout<<leftover2<<endl;
            space_position3 = leftover2.find(" ");
            src_reg = leftover2.substr(0,space_position3-1);
          	dec->Output_Instruction->source_registers_add = src_reg;
          	cout<<"scr1 reg add - "<<dec->Output_Instruction->source_registers_add <<endl;
          	a = dec->Output_Instruction->source_registers_add;
            a.erase(a.begin());
            b = convert(a);
            dec->Output_Instruction->source_registers_index = b;
          	cout<<"src1 reg is index int value - "<<dec->Output_Instruction->source_registers_index<<endl;


       	  	 src_reg2 = leftover2.substr(space_position3+1);
         	 dec->Output_Instruction->source2_registers_add = src_reg2;
         	 cout<<"src2 reg add - "<<dec->Output_Instruction->source2_registers_add <<endl;
          	 c = dec->Output_Instruction->source2_registers_add;
         	 c.erase(c.begin());
          	 d  = convert(c);
          	 dec->Output_Instruction->source2_registers_index = d;
          	 cout<<"src2 reg is index int value "<<dec->Output_Instruction->source2_registers_index<<endl;

         	 cout<<endl;

             /*    if (reg->R[dec->Output_Instruction->dest_register_index].valid == true)
          {
              cout<<" dest reg is valid  , no stall "<<endl;
              decode_stall = false;
              //fetch_flag = false;
          }
          else
          {
              decode_stall= true;
              //fetch_flag = true;
              cout<< " stall due to output dependency and returning"<<endl;
              return;
          } */


         	 //........................READING VALUES..............
                  dec->Output_Instruction->dest_register_value = reg->R[dec->Output_Instruction->dest_register_index].value;




                //............. reading src 1 reg values.................

           if ((reg->R[dec->Output_Instruction->source_registers_index].valid == true ))

                {

                  dec->Output_Instruction->source_registers_value = reg->R[dec->Output_Instruction->source_registers_index].value;
                  cout<< " src1 is read "<<endl;
                  cout<<"no stall "<<endl;

                }

                else if ( dec->Output_Instruction->source_registers_index == bus->bus1)
                         {
                             dec->Output_Instruction->source_registers_value = bus->bus2;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                else if    ( dec->Output_Instruction->source_registers_index == bus->bus4)
                         {
                            dec->Output_Instruction->source_registers_value = bus->bus5;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                         else
                            {
                        cout<<"bus 1 "<<bus->bus1<<endl;
                        cout<<"bus 2 "<<bus->bus2<<endl;
                        //cout<<"bus 3 "<<bus->bus3<<endl;
                        cout<<"bus 4 "<<bus->bus4<<endl;
                        cout<<"bus 5 "<<bus->bus5<<endl;
                        //cout<<"bus 6 "<<bus->bus6<<endl;
                                cout<<"i m returning "<<endl;
                                return;
                            }
        //.................Reading src 2...............................
         if ((reg->R[dec->Output_Instruction->source2_registers_index].valid == true ))

                {

                  dec->Output_Instruction->source2_registers_value = reg->R[dec->Output_Instruction->source2_registers_index].value;
                  cout<< " src2 is read "<<endl;
                  cout<<"no stall "<<endl;

                }

                else if ( dec->Output_Instruction->source2_registers_index == bus->bus1)
                         {
                             dec->Output_Instruction->source2_registers_value = bus->bus2;
                             cout<<" data written in src 2 as  " <<dec->Output_Instruction->source2_registers_value<<endl;
                         }
                else if    ( dec->Output_Instruction->source2_registers_index == bus->bus4)
                         {
                            dec->Output_Instruction->source2_registers_value = bus->bus5;
                             cout<<" data written in src 2 as  " <<dec->Output_Instruction->source2_registers_value<<endl;
                         }
                         else
                            {
                        cout<<"bus 1 "<<bus->bus1<<endl;
                        cout<<"bus 2 "<<bus->bus2<<endl;
                        //cout<<"bus 3 "<<bus->bus3<<endl;
                        cout<<"bus 4 "<<bus->bus4<<endl;
                        cout<<"bus 5 "<<bus->bus5<<endl;
                        //cout<<"bus 6 "<<bus->bus6<<endl;
                                cout<< "just returning"<<endl;
                                return;

                            }
        }

      else if(opcode == "STORE" || opcode ==  "LOAD" || opcode ==  "ADDL"  )
      {


          leftover = full_inst2.substr(space_position+1,100);
          //cout<<leftover<<endl;
          coma_position = leftover.find(",");
          des_reg = leftover.substr(0,coma_position);
          dec->Output_Instruction->dest_register_add = des_reg;
          cout<<" dest reg add - "<<dec->Output_Instruction->dest_register_add<<endl;
          x = dec->Output_Instruction->dest_register_add;
          x.erase (x.begin());
           y = convert(x);
           //cout<< " "
          dec->Output_Instruction->dest_register_index = y;
          cout<<"dest reg CHAHA is index int value - "<<dec->Output_Instruction->dest_register_index<<endl;

          space_position2 = leftover.find (" ");
          leftover2 = leftover.substr(space_position2+1,100);
          cout<<leftover2<<endl;
          space_position3 = leftover2.find(" ");
          src_reg = leftover2.substr(0,space_position3-1);
          dec->Output_Instruction->source_registers_add = src_reg;
          cout<<"src reg add - "<<dec->Output_Instruction->source_registers_add <<endl;
          a = dec->Output_Instruction->source_registers_add;
          a.erase(a.begin());
          b = convert(a);
          dec->Output_Instruction->source_registers_index = b;
          cout<<"src reg is index int value - "<<dec->Output_Instruction->source_registers_index<<endl;


          hash_position = full_inst2.find("#");
          lit = full_inst2.substr(hash_position+1);
          dec->Output_Instruction->literal_string = lit;
          cout<< "literal string is "<<dec->Output_Instruction->literal_string<<endl;
          dec->Output_Instruction->literal_value = convert(dec->Output_Instruction->literal_string);
          cout<<"literal int value is "<<dec->Output_Instruction->literal_value<<endl;

         /*  if (reg->R[dec->Output_Instruction->dest_register_index].valid == true)
          {
              cout<<" dest reg is valid  , no stall "<<endl;
              decode_stall = false;
          }
          else
          {
              decode_stall= true;
              cout<< " stall due to output dependency "<<endl;
              return;
          } */

                 	 //........................READING VALUES..............
                  dec->Output_Instruction->dest_register_value = reg->R[dec->Output_Instruction->dest_register_index].value;


                //............. reading src 1 reg values.................

           if ((reg->R[dec->Output_Instruction->source_registers_index].valid == true ))

                {

                  dec->Output_Instruction->source_registers_value = reg->R[dec->Output_Instruction->source_registers_index].value;
                  cout<< " src1 is read "<<endl;
                  cout<<"no stall "<<endl;

                }

                else if ( dec->Output_Instruction->source_registers_index  == bus->bus1)
                         {
                             dec->Output_Instruction->source_registers_value = bus->bus2;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                else if    ( dec->Output_Instruction->source_registers_index == bus->bus4)
                         {
                            dec->Output_Instruction->source_registers_value = bus->bus5;
                             cout<<" data written in src 1 as  " <<dec->Output_Instruction->source_registers_value<<endl;
                         }
                         else
                            {
                                cout<< "just returning"<<endl;
                                return;

                            }
      }


      else if ( opcode == "BZ" || opcode == "BNZ")
      {
          hash_position = full_inst2.find("#");
          lit = full_inst2.substr(hash_position+1);
          dec->Output_Instruction->literal_string = lit;
          cout<< "literal string is "<<dec->Output_Instruction->literal_string<<endl;
          dec->Output_Instruction->literal_value = convert(dec->Output_Instruction->literal_string);
          cout<<"literal int value is "<<dec->Output_Instruction->literal_value<<endl;

      }



		cout<<endl;
	   //exe->initialize = true;
	}


                    cout<<"src 1 R"<<dec->Output_Instruction->source_registers_index << " stauts is "
                << reg->R[dec->Output_Instruction->source_registers_index].valid<<endl;

                cout<<"src 2 R"<<dec->Output_Instruction->source2_registers_index<<" status is"<<
                 reg->R[dec->Output_Instruction->source2_registers_index].valid<< endl;

                cout<<"des R"<<dec->Output_Instruction->dest_register_index<<" status is "<<
                reg->R[dec->Output_Instruction->dest_register_index].valid<<endl;




                    reg->R[dec->Output_Instruction->dest_register_index].valid = false;
                  cout<<" Dest Reg R"<<dec->Output_Instruction->dest_register_index<<"is set invalid as "
                  <<reg->R[dec->Output_Instruction->dest_register_index].valid<<endl<<endl;

                          if (decode_stall == false )
                          {
                             fetch_flag = false;
                        exe->Input_Instruction->opcode =                    dec->Output_Instruction->opcode;
                        exe->Input_Instruction->dest_register_add =         dec->Output_Instruction->dest_register_add;
                        exe->Input_Instruction->source_registers_add =      dec->Output_Instruction->source_registers_add;
                        exe->Input_Instruction->source2_registers_add =     dec->Output_Instruction->source2_registers_add;
                        exe->Input_Instruction->literal_string =            dec->Output_Instruction->literal_string;
                        exe->Input_Instruction->literal_value =             dec->Output_Instruction->literal_value;
                        exe->Input_Instruction->dest_register_index =       dec->Output_Instruction->dest_register_index;
                        exe->Input_Instruction->source_registers_index=     dec->Output_Instruction->source_registers_index;
                        exe->Input_Instruction->source2_registers_index=    dec->Output_Instruction->source2_registers_index;
                        exe->Input_Instruction->instruction_string =        dec->Output_Instruction->instruction_string;
                        exe->Input_Instruction->pc_value =                  dec->Output_Instruction->pc_value;
                        exe->Input_Instruction->dest_register_value=        dec->Output_Instruction->dest_register_value;
                        exe->Input_Instruction->source_registers_value=     dec->Output_Instruction->source_registers_value;
                        exe->Input_Instruction->source2_registers_value=    dec->Output_Instruction->source2_registers_value;
                          }



                        cout<<"bus 1 "<<bus->bus1<<endl;
                        cout<<"bus 2 "<<bus->bus2<<endl;
                        //cout<<"bus 3 "<<bus->bus3<<endl;
                        cout<<"bus 4 "<<bus->bus4<<endl;
                        cout<<"bus 5 "<<bus->bus5<<endl;
                       // cout<<"bus 6 "<<bus->bus6<<endl;


	}


    }
}



void MUL_1(stage* mul_1,stage* mul_2)
{


      cout<<"................... MUL_1 ................"<< endl;
        mul_1->Output_Instruction->instruction_string =       mul_1->Input_Instruction->instruction_string;
    	mul_1->Output_Instruction->pc_value =                 mul_1->Input_Instruction->pc_value;
        mul_1->Output_Instruction->opcode =                   mul_1->Input_Instruction->opcode;
    	mul_1->Output_Instruction->dest_register_add  =       mul_1->Input_Instruction->dest_register_add;
    	mul_1->Output_Instruction->source_registers_add =     mul_1->Input_Instruction->source_registers_add;
    	mul_1->Output_Instruction->source2_registers_add =    mul_1->Input_Instruction->source2_registers_add;
    	mul_1->Output_Instruction->literal_string=            mul_1->Input_Instruction->literal_string;
    	mul_1->Output_Instruction->literal_value =            mul_1->Input_Instruction->literal_value;
    	mul_1->Output_Instruction->dest_register_index =      mul_1->Input_Instruction->dest_register_index;
    	mul_1->Output_Instruction->source_registers_index =   mul_1->Input_Instruction->source_registers_index;
   	    mul_1->Output_Instruction->source2_registers_index =  mul_1->Input_Instruction->source2_registers_index;

   	    mul_1->Output_Instruction->dest_register_value=       mul_1->Input_Instruction->dest_register_value;
   	    mul_1->Output_Instruction->source2_registers_value=   mul_1->Input_Instruction->source2_registers_value;
   	    mul_1->Output_Instruction->source_registers_value=    mul_1->Input_Instruction->source_registers_value;

        if (mul_1->Output_Instruction->opcode == "MUL")
        {
            mul_2_flag = false ;
            cout<<"  PASSING MUL 1 with instruction "<< mul_1->Output_Instruction->instruction_string<<endl;
        }


        mul_2->Input_Instruction->instruction_string =       mul_1->Output_Instruction->instruction_string;
        mul_2->Input_Instruction->pc_value =                 mul_1->Output_Instruction->pc_value;
        mul_2->Input_Instruction->opcode =                   mul_1->Output_Instruction->opcode;
        mul_2->Input_Instruction->dest_register_add  =       mul_1->Output_Instruction->dest_register_add;
        mul_2->Input_Instruction->source_registers_add =     mul_1->Output_Instruction->source_registers_add;
        mul_2->Input_Instruction->source2_registers_add =    mul_1->Output_Instruction->source2_registers_add;
        mul_2->Input_Instruction->literal_string=            mul_1->Output_Instruction->literal_string;
        mul_2->Input_Instruction->literal_value =            mul_1->Output_Instruction->literal_value;
        mul_2->Input_Instruction->dest_register_index =      mul_1->Output_Instruction->dest_register_index;
        mul_2->Input_Instruction->source_registers_index =   mul_1->Output_Instruction->source_registers_index;
        mul_2->Input_Instruction->source2_registers_index =  mul_1->Output_Instruction->source2_registers_index;
        mul_2->Input_Instruction->mem_buff_reg =             mul_1->Output_Instruction->mem_buff_reg;

        mul_2->Input_Instruction->dest_register_value=       mul_1->Output_Instruction->dest_register_value;
   	    mul_2->Input_Instruction->source2_registers_value=   mul_1->Output_Instruction->source2_registers_value;
   	    mul_2->Input_Instruction->source_registers_value=    mul_1->Output_Instruction->source_registers_value;
        cout <<endl;

	}

void MUL_2(stage* mul_2, stage* mem,Register_File* reg, Data_Memory* da, Bus* bus)
{


        mul_2->Output_Instruction->instruction_string=        mul_2->Input_Instruction->instruction_string;
        mul_2->Output_Instruction->instruction_string =       mul_2->Input_Instruction->instruction_string;
    	mul_2->Output_Instruction->pc_value =                 mul_2->Input_Instruction->pc_value;
        mul_2->Output_Instruction->opcode =                   mul_2->Input_Instruction->opcode;
    	mul_2->Output_Instruction->dest_register_add  =       mul_2->Input_Instruction->dest_register_add;
    	mul_2->Output_Instruction->source_registers_add =     mul_2->Input_Instruction->source_registers_add;
    	mul_2->Output_Instruction->source2_registers_add =    mul_2->Input_Instruction->source2_registers_add;
    	mul_2->Output_Instruction->literal_string=            mul_2->Input_Instruction->literal_string;
    	mul_2->Output_Instruction->literal_value =            mul_2->Input_Instruction->literal_value;
    	mul_2->Output_Instruction->dest_register_index =      mul_2->Input_Instruction->dest_register_index;
    	mul_2->Output_Instruction->source_registers_index =   mul_2->Input_Instruction->source_registers_index;
   	    mul_2->Output_Instruction->source2_registers_index =  mul_2->Input_Instruction->source2_registers_index;

        mul_2->Output_Instruction->dest_register_value=       mul_2->Input_Instruction->dest_register_value;
   	    mul_2->Output_Instruction->source2_registers_value=   mul_2->Input_Instruction->source2_registers_value;
   	    mul_2->Output_Instruction->source_registers_value=    mul_2->Input_Instruction->source_registers_value;



        cout<<".................... MUL 2 ..................."<<endl;
        string opcode;
        opcode = mul_2->Output_Instruction->opcode;
        cout<<"opcode is "<<opcode<<endl;
        int val,lit;
        int srcval;
        int srcval2;  double result;

        if (mul_2->Input_Instruction->opcode == "MUL")
        {
        srcval = mul_2->Output_Instruction->source_registers_index;
        srcval2 = mul_2->Output_Instruction->source2_registers_index;
        cout<<"executing registes R"<<srcval<<"and register R"<<srcval2 <<endl;
        //mul_2->Output_Instruction->mem_buff_reg = reg->R[srcval].value * reg->R[srcval2].value;
        mul_2->Output_Instruction->mem_buff_reg  =
        mul_2->Output_Instruction->source_registers_value * mul_2->Output_Instruction->source2_registers_value;

        cout<<"scource value is "<<mul_2->Output_Instruction->source_registers_value<<endl;
        cout<<"scource2 value is "<< mul_2->Output_Instruction->source2_registers_value<< endl;

        cout<<"data stored in buffer is "<< mul_2->Output_Instruction->mem_buff_reg<<endl<<endl;
        }


        bus->bus4 = mul_2->Output_Instruction->dest_register_index;
        bus->bus5 = mul_2->Output_Instruction->mem_buff_reg;

        cout<< "bus 4 "<<bus->bus4<<endl;
        cout<<"bus 5 "<<bus->bus5<<endl;






        mem->Input_Instruction->instruction_string =       mul_2->Output_Instruction->instruction_string;
        mem->Input_Instruction->pc_value =                 mul_2->Output_Instruction->pc_value;
        mem->Input_Instruction->opcode =                   mul_2->Output_Instruction->opcode;
        mem->Input_Instruction->dest_register_add  =       mul_2->Output_Instruction->dest_register_add;
        mem->Input_Instruction->source_registers_add =     mul_2->Output_Instruction->source_registers_add;
        mem->Input_Instruction->source2_registers_add =    mul_2->Output_Instruction->source2_registers_add;
        mem->Input_Instruction->literal_string=            mul_2->Output_Instruction->literal_string;
        mem->Input_Instruction->literal_value =            mul_2->Output_Instruction->literal_value;
        mem->Input_Instruction->dest_register_index =      mul_2->Output_Instruction->dest_register_index;
        mem->Input_Instruction->source_registers_index =   mul_2->Output_Instruction->source_registers_index;
        mem->Input_Instruction->source2_registers_index =  mul_2->Output_Instruction->source2_registers_index;
        mem->Input_Instruction->mem_buff_reg =             mul_2->Output_Instruction->mem_buff_reg;

        mem->Input_Instruction->dest_register_value=       mul_2->Output_Instruction->dest_register_value;
   	    mem->Input_Instruction->source2_registers_value=   mul_2->Output_Instruction->source2_registers_value;
   	    mem->Input_Instruction->source_registers_value=    mul_2->Output_Instruction->source_registers_value;
        cout <<endl;
        mul2_mem_flag = true;

}



void Execute( stage* dec, stage* mem,stage*  exe, stage* mul_1, stage* mul_2, Register_File* r, Data_Memory* da,Bus* bus)
{
	//if (exe->initialize)
	{
        cout <<endl;
        cout<<"................... Execute ................"<< endl;
        string opcode;
        opcode = dec->Output_Instruction->opcode;
        cout<<"opcode is "<<opcode<<endl;
        int val,lit;
        int srcval;
        int srcval2;  double result;

        exe->Output_Instruction->instruction_string =       exe->Input_Instruction->instruction_string;
    	exe->Output_Instruction->pc_value =                 exe->Input_Instruction->pc_value;
        exe->Output_Instruction->opcode =                   exe->Input_Instruction->opcode;
    	exe->Output_Instruction->dest_register_add  =       exe->Input_Instruction->dest_register_add;
    	exe->Output_Instruction->source_registers_add =     exe->Input_Instruction->source_registers_add;
    	exe->Output_Instruction->source2_registers_add =    exe->Input_Instruction->source2_registers_add;
    	exe->Output_Instruction->literal_string=            exe->Input_Instruction->literal_string;
    	exe->Output_Instruction->literal_value =            exe->Input_Instruction->literal_value;
    	exe->Output_Instruction->dest_register_index =      exe->Input_Instruction->dest_register_index;
    	exe->Output_Instruction->source_registers_index =   exe->Input_Instruction->source_registers_index;
        exe->Output_Instruction->source2_registers_index =  exe->Input_Instruction->source2_registers_index;
        exe->Output_Instruction->dest_register_value=       exe->Input_Instruction->dest_register_value;
   	    exe->Output_Instruction->source2_registers_value=   exe->Input_Instruction->source2_registers_value;
   	    exe->Output_Instruction->source_registers_value=    exe->Input_Instruction->source_registers_value;



         if (opcode == "MOVC")
         {

            exe->Output_Instruction->mem_buff_reg = exe->Output_Instruction->literal_value;
            cout<< "data stores in buffer is   "<<exe->Output_Instruction->mem_buff_reg<<endl;

         }
            else if (opcode == "ADD")
            {


                cout<<"i m in if of add" <<endl;
                srcval = exe->Output_Instruction->source_registers_index;
                srcval2 = exe->Output_Instruction->source2_registers_index;
                cout<<"executing registes R"<<srcval<<"and register R"<<srcval2 <<endl;
                exe->Output_Instruction->mem_buff_reg =
                exe->Output_Instruction->source_registers_value + exe->Output_Instruction->source2_registers_value;

                //exe->Output_Instruction->mem_buff_reg = r->R[srcval].value + r->R[srcval2].value;
                 cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;

            }


            else if (opcode == "ADDL")
            {

                lit = exe->Output_Instruction->literal_value;
                srcval = exe->Output_Instruction->source_registers_index;
                 exe->Output_Instruction->mem_buff_reg = lit + exe->Output_Instruction->source_registers_value;

               // exe->Output_Instruction->mem_buff_reg = lit + r->R[srcval].value;
               cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;

            }

            else if (opcode == "SUB")
            {
                    srcval = exe->Output_Instruction->source_registers_index;
                    srcval2 = exe->Output_Instruction->source2_registers_index;
                    cout<<"executing registes R"<<srcval<<" and register R"<<srcval2 <<endl;
                      exe->Output_Instruction->mem_buff_reg =
                    exe->Output_Instruction->source_registers_value - exe->Output_Instruction->source2_registers_value;

                    //exe->Output_Instruction->mem_buff_reg = r->R[srcval].value - r->R[srcval2].value;
                    cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;
            }

            else if(opcode == "STORE")
            {
                    lit = exe->Output_Instruction->literal_value;
                    srcval = exe->Output_Instruction->source_registers_index;
                    cout<< "adding "<<lit<<" and value of R"<<srcval<<endl;
                     exe->Output_Instruction->mem_buff_reg = lit + exe->Output_Instruction->source_registers_value;
                   // exe->Output_Instruction->mem_buff_reg = lit + r->R[srcval].value;
                    cout<< "computed data memory loc as " << exe->Output_Instruction->mem_buff_reg<<endl;
            }
            else if (opcode == "LOAD")
            {

                    lit = exe->Output_Instruction->literal_value;
                    srcval = exe->Output_Instruction->source_registers_index;
                    cout<< "adding "<<lit<<"and value of R"<<srcval<<endl;
                    int memloc;
                    memloc = lit + exe->Output_Instruction->source_registers_value;
                    //memloc = lit + r->R[srcval].value;
                    cout <<"computed mem loc is "<<memloc<<endl;
                    exe->Output_Instruction->mem_buff_reg = da->data_array[memloc];
                    cout<< "data reterived from mem loc is  " << exe->Output_Instruction->mem_buff_reg<<endl;
            }

            else if (opcode == "OR")
            {
                    srcval = exe->Output_Instruction->source_registers_index;
                    srcval2 = exe->Output_Instruction->source2_registers_index;
                    cout<<"executing registes R"<<srcval<<"and register R"<<srcval2 <<endl;
                    result = exe->Output_Instruction->source_registers_value | exe->Output_Instruction->source2_registers_value;
                    //result = r->R[srcval].value | r->R[srcval2].value;
                    exe->Output_Instruction->mem_buff_reg = result;
                   cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;

            }
            else if (opcode == "EXOR")
            {
                    srcval = exe->Output_Instruction->source_registers_index;
                    srcval2 = exe->Output_Instruction->source2_registers_index;
                    cout<<"executing registes R"<<srcval<<"and register R"<<srcval2 <<endl;
                    exe->Output_Instruction->mem_buff_reg = exe->Output_Instruction->source_registers_value ^ exe->Output_Instruction->source2_registers_value;

                   // exe->Output_Instruction->mem_buff_reg = r->R[srcval].value ^ r->R[srcval2].value;
                     cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;
            }
            else if (opcode == "AND")
            {

                    srcval = exe->Output_Instruction->source_registers_index;
                    srcval2 = exe->Output_Instruction->source2_registers_index;
                    cout<<"executing registes R"<< srcval<<"and register R"<<srcval2 <<endl;
                    exe->Output_Instruction->mem_buff_reg = exe->Output_Instruction->source_registers_value & exe->Output_Instruction->source2_registers_value;
                    //exe->Output_Instruction->mem_buff_reg = r->R[srcval].value & r->R[srcval2].value;
                     cout<<"scource value is "<<exe->Output_Instruction->source_registers_value<<endl;
                    cout<<"scource2 value is "<< exe->Output_Instruction->source2_registers_value<< endl;
                    cout<<"data stored in buffer is  "<< exe->Output_Instruction->mem_buff_reg<<endl;
            }

            bus->bus1 = exe->Output_Instruction->dest_register_index;
            bus->bus2 = exe->Output_Instruction->mem_buff_reg;

            //cout<< "this should store bus 2 is  "<<exe->Output_Instruction->mem_buff_reg<<endl;

            cout<<"bus 1 "<<bus->bus1<<endl;
            cout<<"bus 2 "<<bus->bus2<<endl;




            if ( (exe_flag == 0) && (mul_2_flag == 0) )
            {
                fetch_flag = true;
                decode_flag = true;
                exe_flag =   true;

                cout <<"stalling at exe stage to let mul_2 to to mem"<<endl;

            }
            else
            {

                mem->Input_Instruction->instruction_string =       exe->Output_Instruction->instruction_string;
                mem->Input_Instruction->pc_value =                 exe->Output_Instruction->pc_value;
                mem->Input_Instruction->opcode =                   exe->Output_Instruction->opcode;
                mem->Input_Instruction->dest_register_add  =       exe->Output_Instruction->dest_register_add;
                mem->Input_Instruction->source_registers_add =     exe->Output_Instruction->source_registers_add;
                mem->Input_Instruction->source2_registers_add =    exe->Output_Instruction->source2_registers_add;
                mem->Input_Instruction->literal_string=            exe->Output_Instruction->literal_string;
                mem->Input_Instruction->literal_value =            exe->Output_Instruction->literal_value;
                mem->Input_Instruction->dest_register_index =      exe->Output_Instruction->dest_register_index;
                mem->Input_Instruction->source_registers_index =   exe->Output_Instruction->source_registers_index;
                mem->Input_Instruction->source2_registers_index =  exe->Output_Instruction->source2_registers_index;
                mem->Input_Instruction->mem_buff_reg =              exe->Output_Instruction->mem_buff_reg;

                mem->Input_Instruction->dest_register_value=       exe->Output_Instruction->dest_register_value;
                mem->Input_Instruction->source2_registers_value=   exe->Output_Instruction->source2_registers_value;
                mem->Input_Instruction->source_registers_value=    exe->Output_Instruction->source_registers_value;

            }


    cout <<endl;
   // mem->initialize = true;
    }
}






void Memory(stage* mem, stage* wri,Register_File* r,Data_Memory* da)
{
	//if(mem->initialize)
	{

    mem->Output_Instruction->instruction_string =       mem->Input_Instruction->instruction_string;
    mem->Output_Instruction->pc_value =                 mem->Input_Instruction->pc_value;
    mem->Output_Instruction->opcode =                   mem->Input_Instruction->opcode;
    mem->Output_Instruction->dest_register_add  =       mem->Input_Instruction->dest_register_add;
    mem->Output_Instruction->source_registers_add =     mem->Input_Instruction->source_registers_add;
    mem->Output_Instruction->source2_registers_add =    mem->Input_Instruction->source2_registers_add;
    mem->Output_Instruction->literal_string=            mem->Input_Instruction->literal_string;
    mem->Output_Instruction->literal_value =            mem->Input_Instruction->literal_value;
    mem->Output_Instruction->dest_register_index =      mem->Input_Instruction->dest_register_index;
    mem->Output_Instruction->source_registers_index =   mem->Input_Instruction->source_registers_index;
    mem->Output_Instruction->mem_buff_reg =             mem->Input_Instruction->mem_buff_reg;
    mem->Output_Instruction->dest_register_value=       mem->Input_Instruction->dest_register_value;
    mem->Output_Instruction->source2_registers_value=   mem->Input_Instruction->source2_registers_value;
    mem->Output_Instruction->source_registers_value=    mem->Input_Instruction->source_registers_value;




       cout<<"................... Memory ................"<< endl;
    string opcode;
    opcode = mem->Output_Instruction->opcode;
    cout<<"opcode is "<<opcode<<endl;
    int destreg;

     if (opcode == "STORE")
    {
       int h = mem->Output_Instruction->mem_buff_reg;

        int src = mem->Output_Instruction->dest_register_index;
        da->data_array[h] = r->R[src].value;
        cout << "The value of R"<<mem->Output_Instruction->dest_register_index<<" is ";
        cout<<r->R[src].value<<endl;
        cout<< "Mem add in data mem to writen is " <<h<<endl;
        cout<< "storing values in data mem add " <<h<< "is "<< da->data_array[h]<<endl;

    }
    else if (opcode =="LOAD")
    {
        int des = mem->Output_Instruction->dest_register_index;
        r->R[des].value = mem->Output_Instruction->mem_buff_reg;
        cout<<"value loaded in R"<<des<<"is "<<r->R[des].value;
    }

        wri->Input_Instruction->instruction_string =       mem->Output_Instruction->instruction_string;
        wri->Input_Instruction->pc_value =                 mem->Output_Instruction->pc_value;
        wri->Input_Instruction->opcode =                   mem->Output_Instruction->opcode;
        wri->Input_Instruction->dest_register_add  =       mem->Output_Instruction->dest_register_add;
        wri->Input_Instruction->source_registers_add =     mem->Output_Instruction->source_registers_add;
        wri->Input_Instruction->source2_registers_add =    mem->Input_Instruction->source2_registers_add;
        wri->Input_Instruction->literal_string=            mem->Output_Instruction->literal_string;
        wri->Input_Instruction->literal_value =            mem->Output_Instruction->literal_value;
        wri->Input_Instruction->dest_register_index =      mem->Output_Instruction->dest_register_index;
        wri->Input_Instruction->source_registers_index =   mem->Output_Instruction->source_registers_index;
        wri->Input_Instruction->source2_registers_index =  mem->Output_Instruction->source2_registers_index;
        wri->Input_Instruction->mem_buff_reg =             mem->Output_Instruction->mem_buff_reg;
        wri->Input_Instruction->dest_register_value=       mem->Output_Instruction->dest_register_value;
        wri->Input_Instruction->source2_registers_value=   mem->Output_Instruction->source2_registers_value;
        wri->Input_Instruction->source_registers_value=    mem->Output_Instruction->source_registers_value;


    cout<<endl;
    cout<<endl;
	//wri->initialize = true;
	}

}

void WriteBack(stage* wri, Register_File* r)
{
	//if (wri->initialize)
	if ( mul2_mem_flag == true )
    {
                mul_2_flag = true;
                exe_flag = false;
                fetch_flag = false;
                decode_flag = false;
               //  cout<<"setting everything to false again "<<endl;
    }

    {
       cout<<"................... write back ................"<< endl;

    wri->Output_Instruction->instruction_string =       wri->Input_Instruction->instruction_string;
    wri->Output_Instruction->pc_value =                 wri->Input_Instruction->pc_value;

    wri->Output_Instruction->opcode =                   wri->Input_Instruction->opcode;
    wri->Output_Instruction->dest_register_add  =       wri->Input_Instruction->dest_register_add;
    wri->Output_Instruction->source_registers_add =     wri->Input_Instruction->source_registers_add;
    wri->Output_Instruction->source2_registers_add =    wri->Input_Instruction->source2_registers_add;
    wri->Output_Instruction->literal_string=            wri->Input_Instruction->literal_string;
    wri->Output_Instruction->literal_value =            wri->Input_Instruction->literal_value;
    wri->Output_Instruction->dest_register_index =      wri->Input_Instruction->dest_register_index;
    wri->Output_Instruction->source_registers_index =   wri->Input_Instruction->source_registers_index;
    wri->Output_Instruction->source2_registers_index =  wri->Input_Instruction->source2_registers_index;
    wri->Output_Instruction->mem_buff_reg =              wri->Input_Instruction->mem_buff_reg;
    wri->Output_Instruction->dest_register_value=       wri->Input_Instruction->dest_register_value;
    wri->Output_Instruction->source2_registers_value=   wri->Input_Instruction->source2_registers_value;
    wri->Output_Instruction->source_registers_value=    wri->Input_Instruction->source_registers_value;



    string opcode;
    opcode = wri->Output_Instruction->opcode;
    cout<<"opcode is "<<opcode<<endl;

    if(opcode == "ADD" || opcode == "SUB" || opcode == "MUL" || opcode == "ADDL" || opcode == "LOAD"
       || opcode == "OR" ||opcode == "EXOR" || opcode == "AND" || opcode == "MOVC")
    {
        int d =  wri->Output_Instruction->dest_register_index;
        r->R[d].value = wri->Output_Instruction->mem_buff_reg;
        cout << "written  R"<<d<<" value as " <<r->R[d].value<<endl;
    }

    if (( wrt_pass.Output_Instruction->opcode != "STORE") && (wrt_pass.Output_Instruction->opcode != "nop" ))
              {
                  //.......... set dest as valid  .........
                  reg_obj.R[wri->Output_Instruction->dest_register_index].valid = true;
                 int d;

                 ////decode_stall = false;
                 dec_pass.Output_Instruction->stall = false;
                 d = wrt_pass.Output_Instruction->dest_register_index;
                  cout<<"status of dest reg R"<<d<<"is set valid as"<<reg_obj.R[d].valid<<endl;

              }
        cout<<endl;

        cout<<endl;

}
//stall_flag = false;




}
  void display()
     {
     int la;
        for (la=0; la<=16; la++)
        {
            cout<<"the value of the reg R"<<la<<" is "<< reg_obj.R[la].value<<endl;
            cout<<"the valid of reg R"<<la<<"is "<<reg_obj.R[la].valid<<endl<<endl;
        }

        int ga;
        for (ga =0; ga<=100;ga++)
        {
            if(data.data_array[ga] != 0 ){
            cout<< "value of memnory loation "<< ga<< "is "<<data.data_array[ga]<<endl;
            }
        }
     }


     int simulate ()
     {

          fetch.Input_Instruction = new Instruction_info();
          fetch.Output_Instruction = new Instruction_info();

          dec_pass.Input_Instruction = new Instruction_info();
          dec_pass.Output_Instruction = new Instruction_info();

          exe_pass.Input_Instruction = new Instruction_info();
          exe_pass.Output_Instruction = new Instruction_info();

          mem_pass.Input_Instruction = new Instruction_info();
          mem_pass.Output_Instruction = new Instruction_info();

          wrt_pass.Input_Instruction = new Instruction_info();
          wrt_pass.Output_Instruction = new Instruction_info();


          mul_1_pass.Input_Instruction = new Instruction_info();
          mul_1_pass.Output_Instruction = new Instruction_info();

          mul_2_pass.Input_Instruction = new Instruction_info();
          mul_2_pass.Output_Instruction = new Instruction_info();


        int pcv =4000;

        // reading instruction to CodeMemory
        ifstream my_file;
        Code_Memory codeobj;
        int number_of_lines = 0;
        my_file.open("input.txt");
    if( my_file.is_open() )
    {
          string s; int k=1;
          while( getline( my_file, s ) )
          {
                ++number_of_lines;
                codeobj.codemem[k].instruction_string = s;
                cout <<"instruct in codemem with file line " << k << "\t" << s << endl;
                k++;
          }
          my_file.close();
          cout<<"firdt line is "<<codeobj.codemem[1].instruction_string<<endl;
          cout <<"number of lines is "<<number_of_lines<<endl;

}
else
      cout<< "Error opening file "<< endl;

      cout<<"enter the value of clock cycle "<<endl;
      int c_cyc;
      cin>>c_cyc;
      fetch.initialize = true;
      fetch.Input_Instruction->instruction_string = codeobj.codemem[1].instruction_string;
      fetch.Input_Instruction->pc_value = pcv;

      cout << "my first inst is "<< fetch.Input_Instruction->instruction_string << endl;

      reg_obj.R[1].value = 1;
      reg_obj.R[6].value = 6;


  bool stalled = false;
      int c_Lit;
  for (c_Lit = 1;c_Lit <= c_cyc;c_Lit++)
  {

            WriteBack(&wrt_pass,&reg_obj);


            Memory(&mem_pass,&wrt_pass,&reg_obj,&data);

            if (mul_2_flag == false )
                {
                 MUL_2( &mul_2_pass, &mem_pass,&reg_obj, &data, &bus_pass);
                }

            if (mul_1_flag == false)
                {
                 MUL_1(&mul_1_pass,&mul_2_pass);
                }
                    mul_1_flag = true;

            if (exe_flag == false )
                {

                Execute(&dec_pass,&mem_pass,&exe_pass,&mul_1_pass,&mul_2_pass,&reg_obj,&data,&bus_pass);

                }



            if (decode_flag == false)
                {

                Decode(&fetch,&dec_pass,&exe_pass,&mul_1_pass,&reg_obj,&bus_pass);

                }
            if (fetch_flag == false)
            {

                  Fetch(&fetch,&dec_pass);

            }


            /*  if (( wrt_pass.Output_Instruction->opcode != "STORE") && (wrt_pass.Output_Instruction->opcode != "nop" ))
              {
                  //.......... set dest as valid  .........
                  reg_obj.R[wrt_pass.Output_Instruction->dest_register_index].valid = true;
                 int d;

                 decode_stall = false;
                 dec_pass.Output_Instruction->stall = false;
                 d = wrt_pass.Output_Instruction->dest_register_index;
                  cout<<"status of dest reg R"<<d<<"is set valid as"<<reg_obj.R[d].valid<<endl;

              } */

           if ( wrt_pass.Input_Instruction->instruction_string == "nop" )
           {
             break;
           }
            if(fetch.Input_Instruction->instruction_string == "HALT")
            {
                break;
            }
            if ( c_Lit <= number_of_lines)
            {
                  fetch.Input_Instruction->instruction_string = codeobj.codemem[c_Lit + 1].instruction_string;
                   pcv = pcv+4;
                  fetch.Input_Instruction->pc_value = pcv;
                  cout<<"pc counter value increaments to "<<fetch.Input_Instruction->pc_value<<endl;

            }
            else
            {
                fetch.Input_Instruction->instruction_string = "nop";
            }

      // ............allined output........................

            if (fetch.Output_Instruction->instruction_string == " " && fetch.Output_Instruction->instruction_string =="nop" )
            {
                cout<<"Fetch : "<<fetch.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Fetch : I("<< (fetch.Output_Instruction->pc_value-4000)/4 <<") "<<fetch.Output_Instruction->instruction_string<<endl;
            }

             if (dec_pass.Output_Instruction->instruction_string == " " && dec_pass.Output_Instruction->instruction_string =="nop" )
            {
                cout<<"Decode : "<<dec_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Decode  : I("<< (dec_pass.Output_Instruction->pc_value-4000)/4 <<") "<<dec_pass.Output_Instruction->instruction_string<<endl;
            }

             if (mul_1_pass.Output_Instruction->instruction_string == " " && mul_1_pass.Output_Instruction->instruction_string =="nop" )
            {
                 cout<<"Mul1  : "<<mul_1_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Mul1  : I("<< (mul_1_pass.Output_Instruction->pc_value-4000)/4 <<") "<<mul_1_pass.Output_Instruction->instruction_string<<endl;
            }

              if (mul_2_pass.Output_Instruction->instruction_string == " " && mul_2_pass.Output_Instruction->instruction_string =="nop" )
            {
                 cout<<"Mul2  : "<<mul_2_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Mul2  : I("<< (mul_2_pass.Output_Instruction->pc_value-4000)/4 <<") "<<mul_2_pass.Output_Instruction->instruction_string<<endl;
            }

            if (exe_pass.Output_Instruction->instruction_string == " " && exe_pass.Output_Instruction->instruction_string =="nop" )
            {
                cout<<"Execute : "<<exe_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Execute  : I("<< (exe_pass.Output_Instruction->pc_value-4000)/4 <<") "<<exe_pass.Output_Instruction->instruction_string<<endl;
            }

             if (mem_pass.Output_Instruction->instruction_string == " " && mem_pass.Output_Instruction->instruction_string =="nop" )
            {
                cout<<"Memory : "<<mem_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Memory  : I("<< (mem_pass.Output_Instruction->pc_value-4000)/4 <<") "<<mem_pass.Output_Instruction->instruction_string<<endl;
            }

             if (wrt_pass.Output_Instruction->instruction_string == " " && wrt_pass.Output_Instruction->instruction_string =="nop" )
            {
                cout<<"Write : "<<wrt_pass.Output_Instruction->instruction_string<<endl;
            }
            else
            {
                cout<<"Write  : I("<< (wrt_pass.Output_Instruction->pc_value-4000)/4 <<") "<<wrt_pass.Output_Instruction->instruction_string<<endl;
            }

    cout<<"///////////////////////////////cycle complete ////////////////////////"<<endl;

  }
}

int main ()
{
    int exe_flag, mul_1_flag, mul_2_flag;

    cout<<"enter your selection"<<endl;
    cout <<"1.initliaze"<<endl;
    cout<<"2.simulate " <<endl;
    cout<<"3.display "<<endl;
    cout<<"4.Do All "<<endl;
    int choice;

    cin>>choice;

    switch(choice) {
           case 1 :initialize();
           break;
           case 2 :simulate();
           break;
           case 3 :display();
           break;
           case 4 : initialize(); simulate(); display();
           break;

      default :
         cout << "Invalid choice " << endl;
   }


}













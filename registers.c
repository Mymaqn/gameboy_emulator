#include <stdio.h>
#include <stdbool.h>

//bit position of the different flags in the f register
const unsigned char ZERO_FLAG = 7;
const unsigned char SUBTRACT_FLAG = 6;
const unsigned char HALF_CARRY_FLAG = 5;
const unsigned char CARRY_FLAG = 4;

typedef struct Registers {
  unsigned char a;
  unsigned char b;
  unsigned char c;
  unsigned char d;
  unsigned char e;
  unsigned char f;
  unsigned char h;
  unsigned char l;

}Reg;

//Extra flags register for easy manipulation later
typedef struct s_FlagsRegister{
  bool zero;
  bool subtract;
  bool half_carry;
  bool carry;
}FlagsRegister;

//Gets the virtual register bc. Takes a ptr to Reg as input. Returns unsigned short
unsigned short registers_getbc(Reg* reg){
  unsigned short bc;
  unsigned short b;
  unsigned short c;

  b = (unsigned short)reg->b;
  c = (unsigned short)reg->c;

  bc= b << 8 | c;

  return bc;

}

//Sets the virtual register bc to a specified value. Modifies the register directly. Returns nothing
void registers_setbc(Reg* reg, unsigned short value){
  reg->b = (unsigned char)((value & 0xFF00)>>8);
  reg->c = (unsigned char)((value & 0xFF));

  return;
}

//Gets the virtual register af. Takes a ptr to Reg as input. Returns unsigned short
unsigned short registers_getaf(Reg* reg){
  unsigned short af;
  unsigned short a;
  unsigned short f;

  a = (unsigned short)reg->a;
  f = (unsigned short)reg->f;

  af = a << 8 | f;

  return af;

}

//Sets the virtual register af to a specified value. Modifies the register directly. Returns nothing
void registers_setaf(Reg* reg, unsigned short value){
  reg->a = (unsigned char)((value & 0xFF00)>>8);
  reg->f = (unsigned char)((value & 0xFF));
  return;

}

//Gets the virtual register de. Takes a ptr to Reg as input. Returns unsigned short
unsigned short registers_getde(Reg* reg){
  unsigned short de;
  unsigned short d;
  unsigned short e;

  d = (unsigned short)reg->d;
  e = (unsigned short)reg->e;

  de = d << 8 | e;

  return de;

}

//Sets the virtual register de to a specified value. Modifies the register directly. Returns nothing
void registers_setde(Reg* reg, unsigned short value){
  reg->d = (unsigned char)((value & 0xFF00)>>8);
  reg->e = (unsigned char)((value & 0xFF));
  return;

}
//Gets the virtual register hl. Takes a ptr to Reg as input. Returns unsigned short
unsigned short registers_gethl(Reg* reg){
  unsigned short hl;
  unsigned short h;
  unsigned short l;

  h = (unsigned short)reg->h;
  l = (unsigned short)reg->l;

  hl = h << 8 | l;

  return hl;

}


//Sets the virtual register hl to a specified value. Modifies the register directly. Returns nothing
void registers_sethl(Reg* reg, unsigned short value){
  reg->h = (unsigned char)((value & 0xFF00)>>8);
  reg->l = (unsigned char)((value & 0xFF));
  return;

}

//Converts a flagsregister struct to a an f register (an unsigned short value) returns unsigned short
unsigned short registers_FlagsRegisterToRegf(FlagsRegister fr){
  unsigned short f = 0;
  if(fr.zero == true){
    f+=fr.zero << ZERO_FLAG;
  }
  if(fr.subtract == true){
    f += fr.subtract << SUBTRACT_FLAG;
  }
  if(fr.half_carry == true){
    f+= fr.half_carry << HALF_CARRY_FLAG;
  }
  if(fr.carry == true){
    f+= fr.carry << CARRY_FLAG;
  }
  return f;
}


//Converts an unsigned short (f register) to a FlagsRegister. Returns a FlagsRegister.
FlagsRegister registers_RegfToFlagsRegister(unsigned char f){
  FlagsRegister FlagReg;

  unsigned char zero = (f & (unsigned char)0x80) >> ZERO_FLAG;
  unsigned char subtract = (f & (unsigned char)0x40) >> SUBTRACT_FLAG;
  unsigned char half_carry = (f & (unsigned char)0x20) >> HALF_CARRY_FLAG;
  unsigned char carry = (f & (unsigned char)0x10) >> CARRY_FLAG;

  FlagReg.zero = (bool)zero;
  FlagReg.subtract = (bool)subtract;
  FlagReg.half_carry = (bool)half_carry;
  FlagReg.carry = (bool)carry;

  return FlagReg;

}


//Testing
int main(void){
  Reg r1;
  FlagsRegister flagReg;

  r1.a = 10;
  r1.f = 20;
  r1.b = 10;
  r1.c = 20;
  r1.d = 10;
  r1.e = 20;
  r1.h = 10;
  r1.l = 20;

  printf("\nTesting all registers with values 10 and 20 expecting output of 2580\n");
  printf("af: %d\n",registers_getaf(&r1));
  printf("bc: %d\n",registers_getbc(&r1));
  printf("de: %d\n",registers_getde(&r1));
  printf("hl: %d\n",registers_gethl(&r1));

  printf("Emptying all registers...\n\n");
  r1.a = 0;
  r1.f = 0;
  r1.b = 0;
  r1.c = 0;
  r1.d = 0;
  r1.e = 0;
  r1.h = 0;
  r1.l = 0;

  printf("Filling all virtual registers with value of 2580 expecting output of 10 and 20\n");
  registers_setaf(&r1, 2580);
  registers_setbc(&r1, 2580);
  registers_setde(&r1, 2580);
  registers_sethl(&r1, 2580);

  printf("a: %d, f: %d\n",r1.a,r1.f);
  printf("b: %d, c: %d\n",r1.b,r1.c);
  printf("d: %d, e: %d\n",r1.d,r1.e);
  printf("h: %d, l: %d\n",r1.h,r1.l);


  printf("\nChecking FlagsRegister...\n");
  flagReg.zero = true;
  flagReg.subtract = true;
  flagReg.half_carry = false;
  flagReg.carry = false;

  printf("Flag register set to:\n");
  printf("Zero_flag: %d\n",flagReg.zero);
  printf("Subtract_flag: %d\n",flagReg.subtract);
  printf("Half_carry_flag: %d\n",flagReg.half_carry);
  printf("Carry_flag: %d\n",flagReg.carry);

  printf("\nExpecting value: %d\n",0xF0);
  unsigned char f = registers_FlagsRegisterToRegf(flagReg);
  printf("Flag Value: %d\n",f);

  printf("\nZeroing out flagRegister...\n");
  flagReg.zero = false;
  flagReg.subtract = false;
  flagReg.half_carry = false;
  flagReg.carry = false;

  printf("Trying to set the flags with function.. Expecting all flags to be set\n");
  flagReg = registers_RegfToFlagsRegister(f);

  printf("Flag register set to:\n");
  printf("Zero_flag: %d\n",flagReg.zero);
  printf("Subtract_flag: %d\n",flagReg.subtract);
  printf("Half_carry_flag: %d\n",flagReg.half_carry);
  printf("Carry_flag: %d\n",flagReg.carry);


  printf("Testing complete.\n");
}

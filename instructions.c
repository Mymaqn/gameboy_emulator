#include <stdio.h>
#include "registers.c"
#include <limits.h>
#include <stdbool.h>


enum ArithmeticTarget{A = 0,B,C,D,E,H,L};
//Utility function to avoid code repetition
unsigned char instructions_getTargetValue(Reg* reg, enum ArithmeticTarget target){
  unsigned char value;
  switch (target) {
    case 0:
      value = reg->a;
      break;

    case 1:
      value = reg->b;
      break;

    case 2:
      value = reg->c;
      break;

    case 3:
      value = reg->d;
      break;

    case 4:
      value = reg->e;
      break;

    case 5:
      value = reg->h;
      break;

    case 6:
      value = reg->l;
      break;

    default:
      printf("Invalid register, number passed was: %d",target);
      return 0;
  }
  return value;
}

//Adding on a gameboy works by giving it a target register and adding it to the a register.
void instructions_add(Reg* reg,enum ArithmeticTarget target){

  unsigned char value = instructions_getTargetValue(reg,target);

  //Grab the current flag register
  FlagsRegister flags = registers_RegfToFlagsRegister(reg->f);

  //Check for carry/overflow. This needs to be done before adding.
  if (value > (UCHAR_MAX - reg->a)) {
    flags.carry = true;
  }
  else{
    flags.carry = false;
  }
  //Check for half-carry/half-overflow
  if( (reg->a & 0xF) + (value&0xF) > 0xF){
    flags.half_carry = true;
  }
  else{
    flags.half_carry = false;
  }

  reg->a = reg->a+value;

  //Check if the output is 0
  if(reg->a == 0){
    flags.zero = true;
  }
  else{
    flags.zero = false;
  }

  flags.subtract = false;

  //Set the f register
  reg->f = registers_FlagsRegisterToRegf(flags);

  return;
}

void instructions_adc(Reg* reg, enum ArithmeticTarget target){
  unsigned char value = instructions_getTargetValue(reg,target);

  //Grab the current flag register
  FlagsRegister flags = registers_RegfToFlagsRegister(reg->f);

  bool carrySet = flags.carry;

  //Check for carry/overflow. This needs to be done before adding.
  if (value > (UCHAR_MAX - (reg->a+carrySet))) {
    flags.carry = true;
  }
  else{
    flags.carry = false;
  }
  //Check for half-carry/half-overflow
  if( ((reg->a & 0xF) + (value & 0xF))+carrySet > 0xF){
    flags.half_carry = true;
  }
  else{
    flags.half_carry = false;
  }

  reg->a = reg->a+value + carrySet;

  //Check if the output is 0
  if(reg->a == 0){
    flags.zero = true;
  }
  else{
    flags.zero = false;
  }

  flags.subtract = false;

  //Set the f register
  reg->f = registers_FlagsRegisterToRegf(flags);

  return;

}




int main(void){

  Reg reg1;
  reg1.a = 40;
  reg1.c = 220;

  instructions_add(&reg1,C);

  printf("Expecting f to be 48 and a to be 4: \nf:%d\na:%d",reg1.f,reg1.a);

  instructions_adc(&reg1,C);
  reg1.c = 220;

  printf("\nExpecting f to be 32 and a to be 225\nf:%d\na:%d",reg1.f,reg1.a);
  FlagsRegister flags = registers_RegfToFlagsRegister(32);
  printf("Flags: %d, %d, %d, %d",flags.zero,flags.carry,flags.half_carry,flags.subtract);


}

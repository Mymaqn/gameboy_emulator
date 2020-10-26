#include <stdio.h>
#include "registers.c"
#include <limits.h>


enum ArithmeticTarget{A = 0,B,C,D,E,H,L};
//Adding on a gameboy works by giving it a target register and adding it to the a register.
void instructions_add(Reg* reg,enum ArithmeticTarget target){

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
      return;
  }

  //Grab the current flag register
  FlagsRegister flags = registers_RegfToFlagsRegister(reg->f);

  //Check for carry/overflow. This needs to be done before adding.
  if (value > (UCHAR_MAX - reg->a)) {
    flags.carry = true;
  }
  //Check for half-carry/half-overflow
  if( (reg->a & 0xF) + (value&0xF) > 0xF){
    flags.half_carry = true;
  }

  reg->a = reg->a+value;

  //Check if the output is 0
  if(reg->a == 0){
    flags.zero = true;
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

  printf("%d\n",reg1.a);
  printf("%d",reg1.f);
}

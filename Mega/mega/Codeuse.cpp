#include "Codeuse.h"

int32_t ticksTab[2]={0,0};
uint8_t pins[2][2]={{0,0},{0,0}};
uint8_t Codeuse::nextIdToGive=0;

void Codeuse::actuate(float dt)
{
  ticks=ticksTab[ID];
  dPos=(ticks-oldTicks)*tickToPos/dt;
  pos+=dPos*dt;
  oldTicks=ticks;
}

void handler0() 
 {
  if (digitalRead(pins[0][0]) == HIGH) 
  {
    if (digitalRead(pins[0][1]) == HIGH)
      ticksTab[0] = ticksTab[0] + 1;
    else
      ticksTab[0] = ticksTab[0] - 1;
  }
  else 
  {
    if (digitalRead(pins[0][1]) == LOW)
      ticksTab[0] = ticksTab[0] + 1;
    else
      ticksTab[0] = ticksTab[0] - 1;
  }
}

void handler1() 
 {
  if (digitalRead(pins[1][0]) == HIGH) 
  {
    if (digitalRead(pins[1][1]) == HIGH)
      ticksTab[1] = ticksTab[1] + 1;
    else
      ticksTab[1] = ticksTab[1] - 1;
  }
  else 
  {
    if (digitalRead(pins[1][1]) == LOW)
      ticksTab[1] = ticksTab[1] + 1;
    else
      ticksTab[1] = ticksTab[1] - 1;
  }
}

Codeuse::Codeuse(uint8_t pin1,uint8_t pin2,float tickToPos)
{ 
  this->pin1=pin1;
  this->pin2=pin2;
  this->ID=nextIdToGive;
  nextIdToGive++;
  pins[ID][0]=pin1;
  pins[ID][1]=pin2;
  if (ID==0)
  {
    attachInterrupt(digitalPinToInterrupt(pin1),handler0,CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin2),handler0,CHANGE);
  }
  else
  {
    attachInterrupt(digitalPinToInterrupt(pin1),handler1,CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin2),handler1,CHANGE);
  }
  ticks=0;
  oldTicks=0;
  pos=0.0;
  dPos=0.0;
  this->tickToPos=tickToPos;
}

Codeuse::Codeuse()
{  
}

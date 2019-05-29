#include "Barillet.h"
#include "Arduino.h"
#include "Mega.h"
float normalizeBarillet(float theta)
{
    float out;
    out=theta-(2*PI)*((int)(theta/(2*PI)));
    if (out>PI)
        return (out-2*PI);
    else if (out<=-PI)
        return (out+2*PI);
    return out;
}

//Cellule::Cellule(PaletE bas, PaletE millieu, PaletE haut, PaletE highPriority, PaletE midPriority, PaletE lowPriority, float angleDroite, float angleGauche)
//{
//    stock[BAS]=bas;
//    stock[MILLIEU]=millieu;
//    stock[HAUT]=haut;
//    priorityIn[LOWPRIORITY]=lowPriority;
//    priorityIn[MIDPRIORITY]=midPriority;
//    priorityIn[HIGHPRIORITY]=highPriority;
//    nbPaletIn=(stock[BAS]==VIDE)?(0):(1) + (stock[MILLIEU]==VIDE)?(0):(1) + (stock[HAUT]==VIDE)?(0):(1);
//    this->angleDroite=angleDroite;
//    this->angleGauche=angleGauche;
//}
//
//Cellule::Cellule()
//{
//
//}

//void Cellule::add(PaletE palet)
//{
//    nbPaletIn++;
//    if (stock[BAS]==VIDE)
//        stock[BAS]=palet;
//    else if(stock[MILLIEU]==VIDE)
//        stock[MILLIEU]=palet;
//    else if(stock[HAUT]==VIDE)
//        stock[HAUT]=palet;
//    else
//        Serial.print("Remplir une cellule pleine ? For real mate ?");
//}

//void Cellule::take()
//{
//    nbPaletIn--;
//    if (stock[HAUT]!=VIDE)
//        stock[HAUT]=VIDE;
//    else if(stock[MILLIEU]!=VIDE)
//        stock[MILLIEU]=VIDE;
//    else if(stock[BAS]!=VIDE)
//        stock[BAS]=VIDE;
//    else
//        Serial.print("Vider une cellule vide ? For real mate ?");
//}


Barillet::Barillet(uint8_t pinContacteur,uint8_t pin1Codeuse,uint8_t pin2Codeuse,float tickToPos,uint8_t pinMoteurPwr,uint8_t pinMoteurSens,uint8_t pinMoteurBrake,bool coteviolet,Mega* ptrMega)
{
  pidBarillet=new PID(true,350.0,0,50.0,100,0);
  moteurBarillet=new Motor(pinMoteurPwr,pinMoteurSens,pinMoteurBrake);
  codeuseBarillet=new Codeuse(true,pin1Codeuse,pin2Codeuse,tickToPos);
  contacteurBarillet=new Contacteur(pinContacteur);
  this->coteviolet=coteviolet;
  this->ptrMega=ptrMega;
  color=new Color(true);
  aim=0.0;
  dAim=0.0;
}

void Barillet::actuate(float dt)
{
  color->actuate();
  //color->raw();
  //Serial.print("Color (R,B,V,Vide) ");Serial.println(color->getPaletCouleur());
  //Serial.print("pos barillet ");Serial.println(codeuseBarillet->pos);
  Serial.print("Aim ");Serial.print(aim);Serial.print("\t target ");Serial.print(target);Serial.print("\t pos ");Serial.println(codeuseBarillet->pos);
  if (millis()/1000.0<tInversion)
  {
    dAim+=ACCBARILLET*dt;
    aim=normalizeBarillet(aim+dAim*dt);
  }
  else if(millis()/1000.0<tFinGoto)
  {
      dAim-=ACCBARILLET*dt;
      aim=normalizeBarillet(aim+dAim*dt);
  }
  else
  {
      dAim=0.0;
      aim=target;
  }
  codeuseBarillet->actuate(dt);
  moteurBarillet->order=pidBarillet->compute(dt,aim,codeuseBarillet->pos,codeuseBarillet->dPos)*0.85;
  moteurBarillet->actuate();

}

Barillet::Barillet()
{

}

bool Barillet::init()
{
  float t=millis()/1000.0;
  float tIni=t;
  
  while (!contacteurBarillet->isPressed() and t-tIni<20)
  {
    t=millis()/1000.0;
    moteurBarillet->order=20;   //On fait tourner le barillet doucement
    moteurBarillet->actuate();
  }

  moteurBarillet->order=0;// On arrete l'elevateur
  moteurBarillet->actuate();
  codeuseBarillet->reset(); // On initialise la position
  pidBarillet->reset();


  // definition des positions du barillet par rapport à l'origine 
  Poscellule1 = 0;
  Poscellule2 = normalizeBarillet(BARILLET_AngleToNext);
  Poscellule3 = normalizeBarillet(2*BARILLET_AngleToNext);
  Poscellule4 = normalizeBarillet(3*BARILLET_AngleToNext);
  Poscellule5 = normalizeBarillet(4*BARILLET_AngleToNext);
  Poscellule6 = normalizeBarillet(5*BARILLET_AngleToNext);
  
  if (t-tIni>=20)return false;
  else
  {
    goTo(BARILLET_AngleToInit);
    return true;
  }
}

//void Barillet::addPalet(int cellId, PaletE palet)
//{
//	cellule[cellId]->add(palet);
//}
//
void Barillet::goTo(float angle)
{
    this->aim=codeuseBarillet->pos;
    this->dAim=0.0;
    tStartGoto=millis()/1000.0;
    tFinGoto=tStartGoto + sqrt(4.0*(angle-codeuseBarillet->pos)/ACCBARILLET);
    tInversion=(tFinGoto+tStartGoto)/2.0;
    target=angle;
}

void Barillet::goToDelta(float angle)
{
  goTo(codeuseBarillet->pos+angle);
}



bool Barillet::goodenough()
{
    return (abs(normalizeBarillet(target - codeuseBarillet->pos))<0.01);
}



bool Barillet::RedefinitionPosBleuium(){ 
  
  int i=0;
  while ((color->getPaletCouleur() != Bleu) and (i<6))// on fait tourner le barillet jusqu'à trouver le palet bleu
    {
      Serial.println("je suis dans la boucle pour trouver la position");
      goTo(Poscellule1 + i*BARILLET_AngleToNext);
      i++;
      ptrMega->actuate();
      
      }
      Serial.println("j'ai trouvé la position");
      if (coteviolet){ //si on est du coté violet on met le bleu à droite
      while(!goodenough()){
     goTo(Poscellule1 + (i-3)*BARILLET_AngleToNext);
     ptrMega->actuate();
      }
  }
  else {
   while(!goodenough()){// si on est coté jaune on met le bleu à gauche
   goTo(Poscellule1 + (i-2)*BARILLET_AngleToNext);
   ptrMega->actuate();
      }    
    }
    codeuseBarillet->reset(); // maitenant le palet est du bon coté et à la bonne position
  }

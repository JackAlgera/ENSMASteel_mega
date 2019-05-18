#include "Mega.h"

void Mega::actuate(float dt)
{
	//barillet.Actuate(dT);
	pinces.actuate();
	//elevator.actuate(dt);
}


void Mega::init()
{
	//elevator.init();
  brasGauche=MegaServo(BRAS_GAUCHE_PIN,BRAS_GAUCHE_RETRACTED,BRAS_GAUCHE_HALF_RETRACTED,BRAS_GAUCHE_HALF_EXTENDED,BRAS_GAUCHE_EXTENDED);
  brasDroit=MegaServo(BRAS_DROIT_PIN,BRAS_DROIT_RETRACTED,BRAS_DROIT_HALF_RETRACTED,BRAS_DROIT_HALF_EXTENDED,BRAS_DROIT_EXTENDED);
  doigtGauche=MegaServo(DOIGT_GAUCHE_PIN,DOIGT_GAUCHE_RETRACTED,DOIGT_GAUCHE_HALF_RETRACTED,DOIGT_GAUCHE_HALF_EXTENDED,DOIGT_GAUCHE_EXTENDED);
  doigtDroit=MegaServo(DOIGT_DROIT_PIN,DOIGT_DROIT_RETRACTED,DOIGT_DROIT_HALF_RETRACTED,DOIGT_DROIT_HALF_EXTENDED,DOIGT_DROIT_EXTENDED);
  pinces=Pinces(true);
  pompeG=Pompe(POMPE_GAUCHE_PIN_MOTEUR_PWR,POMPE_GAUCHE_PIN_MOTEUR_SENS,POMPE_GAUCHE_PIN_MOTEUR_BRAKE,POMPE_GAUCHE_PIN_AMP);
  pompeD=Pompe(POMPE_DROITE_PIN_MOTEUR_PWR,POMPE_DROITE_PIN_MOTEUR_SENS,POMPE_DROITE_PIN_MOTEUR_BRAKE,POMPE_DROITE_PIN_AMP);
}

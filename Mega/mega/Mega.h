#ifndef MEGA_INCLUDED
#define MEGA_INCLUDED
//#include "Barillet.h"
#include "Pompe.h"
//#include "Elevator.h"
#include "MegaServo.h"
#include "1_CONSTANTS.h"
#include "Pinces.h"
#include "Sharp.h"
#include "Contacteur.h"
#include "Elevator.h"
#include "Barillet.h"
#include "Comm.h"
enum actionCouranteE {Idle,Chaos,Distrib,RecupGoldonium,DeposeGoldonium,PoussePaletBleu,DeposePaletSolA};
enum etapeChaosE {PrepChaos,DescentSouffletSol,RemontePalet,DeposeOneFloor,DeposeRemonte,TourneBarillet};
enum etapeRecupGoldoniumE {Safety,Recup};
enum etapePaletBleuE {SafetyPal,Pousse};
enum etapeDeposePaletSolE {SafetyEtapeDeposePaletSol,AttrapePaletBarillet,RemontePaletPourDeposeSol,TourneBarilletPourDeposeSol};
class Mega
{
public:
    Barillet barillet;
    Pompe pompeG, pompeD;
    Elevator elevator;
    MegaServo brasGauche,brasDroit,doigtGauche,doigtDroit;
    Pinces pinces;
    Sharp sharpAVG,sharpAVD,sharpARG,sharpARD,sharpPaletG,sharpPaletD,AmperemetrePompeGauche,AmperemetrePompeDroit;
    Contacteur tirette;
    Comm comm;
    actionCouranteE actionCourante;
    etapeChaosE etapeChaos;
    etapeRecupGoldoniumE etapeRecupGoldonium;
    etapePaletBleuE etapePaletBleu;
    int iPosBarillet=1;
    uint32_t millisInit,millisActu;

    void actuate();
    void init();

    bool coteviolet = false ;

private:
    bool evitting;


};

#endif // !MEGA_INCLUDED

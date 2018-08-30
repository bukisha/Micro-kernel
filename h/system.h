#ifndef _system_h_
#define _system_h_


#include "PCB.h"
#include "Thread.h"
#include "IThread.h"
#include "Lista.h"
#include "dos.h"


typedef void (interrupt *IR)();


class  System {
friend class PCB;
friend class Thread;
friend class IThread;

public:
static volatile int startFlag;//flag koji sluzi da se pokupi prilikom prvog prekida ss i sp za povratak na main funkciju
static volatile int expDisp;
static volatile PCB* running; //pokazivac na pcb niti koja se izvrsava trenutno
static IR setINT();// menja mesta mojoj i rutini iz ulaza u ivtp tabeli
static void restoreINT();  // vraca staru rutinu timer na njeno mesto
static Thread* starting;     // pocetna nit;
static IThread* Idle;     // idle thread
static void dispatch();
static Lista* sleepList;   //lista za pcbove niti koje se uspavljuju;
static IR oldRutine; //pokazivac na staru rutinu koji sluzi da bi jam ogo da pozivam staru rutinu unutar svoje nove
  //cuvam stare seg i off da bi mogo kad zavrsim da vratim stari timer na svoje mesto

static void interrupt myTimer();  // moja intterrupt ruutina timer
static void wrapper();  // omotacka funkcija ista za sve threadove koja ce da poziva funkciju run() 
						//za odredjeni thread i da obavlja jos neke radnje nakon zavrsetka niti 

static void initiallize();   // funkcija koja ce da inicijalizuje system
static void ShutDown();   // funkcija koja ce da gasi sistem


};   //end system
extern void tick();

#endif
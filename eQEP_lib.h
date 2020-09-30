


typedef struct
{
   volatile struct EQEP_REGS *eQEPRegHandle;
   float32 pos[2]; // pos(0) - pozycja poprzednia pos(1) - pozycja aktualna
   float32 theta;
   float32 theta_rad;
   float32 omega_fast;
   float32 omega_slow;
   int16 dir;
   void (*init)();
   void (*calc)();
}E_QEP;


typedef E_QEP *E_QEP_h;

#define E_QEP_default {&EQep1Regs,{0,0},0,0,0,0,0,(void (*)(long))E_QEP_Init,(void (*)(long))E_QEP_calc}

void E_QEP_Init(void);
void E_QEP_calc(E_QEP_h);
extern E_QEP EncoderObj;


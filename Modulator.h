
#ifndef _INCL_Modulator
#define _INCL_Modulator
/*
 * Modulator.h
 *
 *  Created on: 27 maj 2015
 *      Author: tosiek
 */


typedef struct{
    float32 Mag;
    float32 theta;
    float32 Alpha;
    float32 Beta;
    float32 freq;
    void (*init)();
    void (*calc)();

}VECTOR;
typedef VECTOR *VECTOR_h;


#define VECTOR_default {0,0,0,0,0,(void(*)(long))VECTOR_init, (void(*)(long))VECTOR_calc}
void VECTOR_init(VECTOR *Vector,float Mag);
void VECTOR_calc(VECTOR_h);



typedef struct{
	float32 sectors[6][2][2];
	Uint16 ActualSector;
	Uint16 PWMperiodValue;
	float32 freqOfSaw;
	float32 sawOutput;
	VECTOR *InputVoltage;
	COMPARATORE_TIME *timeTab;
	void (*init)();
	void (*saw)();
	void (*calc)();
	void (*sector)();
}SPVM;
typedef SPVM *SPVM_h;

#define InvertedArray { {{ 1.5,-0.866 },  { 0,  1.732 }},  {{ -1.5,0.866 }, { 1.5, 0.866 }}, {{ 0, 1.732 }, { -1.5,-0.866 }}, {{ 0,-1.732 },  { -1.5, 0.866 }},  {{ -1.5,-0.866 }, { 1.5,-0.866 }}, {{ 1.5,0.866 },  { 0,-1.732 }} }
#define SPVM_default {InvertedArray,1,1250,0,0,0,0,(void(*)(long))SPVM_Init, (void(*)(long))SPVM_Saw, (void(*)(long))SPVM_Calc, (void(*)(long))SPVM_Sector}
void SPVM_Init(SPVM_h ,VECTOR_h);
void SPVM_Saw(SPVM_h);
void SPVM_Calc(SPVM_h);
void SPVM_Sector(SPVM_h);

extern SPVM pila1;
extern SPVM pila2;

#else

#endif




typedef struct{
	EPWM_INFO *epwm1_info;
	EPWM_INFO *epwm2_info;
	EPWM_INFO *epwm3_info;
	ADC_INFO *adc_info;
	RAMP *ramp;
	SPVM *spvm;
	float32 buf1[1000];
	float32 buf2[1000];
	float32 buf3[1000];
	float32 *buf1h, *buf2h, *buf3h;
	Uint32 bufCount;

}CONTROLER;

typedef CONTROLER *CONTROLER_h;
extern CONTROLER ControlerObj;
extern float32 KI1, KP1, KC1;
extern float32 KI2, KP2, KC2;
extern float32 u_p1,  u_p2;
extern float32 u_i1[2], u_i2[2];
extern float32 v1, v2;
extern float32 Max1, Max2, Min1, Min2;

extern float32 Id_zad, Iq_zad;
#define CONTROLER_default {0,0,0,0,0,0,0,0,0,0,0,0}

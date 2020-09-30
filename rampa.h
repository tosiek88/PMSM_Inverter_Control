

typedef struct{
	float32 upperSat;
	float32 lowerSat;

	float32 ratio;

	float32 Zad;
	float32 Wyj[2]; // TODO przeliczone na Hz musi byc!

	Uint16 k;

	void (*init)();
	void (*update)();
}RAMP;

typedef RAMP *RAMP_h;
						//1/t_sample (freq of PWM interrupt)
#define RAMP_default {(0.0001), (-0.0001), 1, 0, {0,0}, 1,(void (*)(long))RAMP_Init, (void (*)(long))RAMP_Update};
void* RAMP_Init(void);
void RAMP_Update(RAMP_h, float32);

extern RAMP ramp_d, ramp_q;
extern RAMP_h RampHalender;

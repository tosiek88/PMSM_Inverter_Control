#include "DSP28x_Project.h"



void* RAMP_Init(void)
{
	RAMP ramp=RAMP_default;
	RAMP_h RampHalender;
	RampHalender=&ramp;

	return RampHalender;

}

void RAMP_Update(RAMP *ramp_ptr, float32 Zad){
	float32 e_;
	e_=Zad-(ramp_ptr->Wyj[0]);

	if(e_>ramp_ptr->upperSat){
		e_=ramp_ptr->upperSat;
	}
	else if(e_<ramp_ptr->lowerSat){
		e_=ramp_ptr->lowerSat;
	}
	else{
		e_=e_;
	}
	//omega(k)=e+omega(k-1)
	ramp_ptr->Wyj[1]=e_+ramp_ptr->Wyj[0];
	ramp_ptr->Wyj[0]=ramp_ptr->Wyj[1]; //poprzedni staje siê aktualnym

}




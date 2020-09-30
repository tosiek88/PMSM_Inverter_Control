/*
 * PID.c
 *
 *  Created on: 5 cze 2015
 *      Author: tosiek
 */
#include "DSP28x_Project.h"
#include "PID.h"

void vLimieter_calc(VLIMITER *vLim, float32 max){
	float32 M, fi; //Magnituide
	M=(vLim->inX)*(vLim->inX)+(vLim->inY)*(vLim->inY);

	M=(float32)sqrt((double)M);
	fi=atan2(vLim->inY, vLim->inX);


	if(M>(max)){//*0.577
		vLim->outX=max*cos(fi);
		vLim->outY=max*sin(fi);
	}
	else{
		vLim->outX=vLim->inX;
		vLim->outY=vLim->inY;
	}

}


void PID_calc(PID *pid){ // PAMIETAJ uc jest zmienn¹ nieograniczon¹ przez limiter!!

	pid->up=pid->e*pid->Kp; //obliczanie wartoci steruj¹cej w torze proporcjonalnym
	pid->ui=pid->e*pid->Ki+(pid->u-pid->uc)*pid->Kc;
	pid->integral=pid->ui+pid->integral;

	pid->uc=pid->up+pid->integral;

}

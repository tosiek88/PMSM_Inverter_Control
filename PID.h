/*
 * PID.h
 *
 *  Created on: 5 cze 2015
 *      Author: tosiek
 */

#ifndef PID_H_
#define PID_H_


typedef struct{
	float32 inX;
	float32 inY;

	float32 outX;
	float32 outY;

	float32 Max;
	void (*calc)();

}VLIMITER; //Vektor Limiter
typedef VLIMITER *VLIMITER_h;

#define VLIMITER_default {0,0,0,0,0, (void (*)(long))vLimieter_calc}

void vLimieter_calc(VLIMITER_h, float32);

typedef struct{
	float32 integral;
	float32 Kp;
	float32 Ki;
	float32 Kc;
	float32 up; //sygna³ w ga³ezi proporcjonalnej
	float32 ui; //sygna³ w ga³êzi ca³kuj¹cej
	float32 uc; //wyjscie regulatora
	float32 u;  //wyjscie ograniczone przez limiter
	float32 e;  //uchyb - wejscie regulatora
	void (*calc)();
}PID;

typedef PID *PID_h;
#define PID_default {0,2,1.5,1,0,0,0,0,0, (void (*)(long))PID_calc}

void PID_calc(PID_h);

#endif /* PID_H_ */

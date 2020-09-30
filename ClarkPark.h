/*
 * ClarkPark.h
 *
 *  Created on: 1 cze 2015
 *      Author: tosiek
 */

#ifndef CLARKPARK_H_
#define CLARKPARK_H_


typedef struct{
	float32 Xa, Xb;
	float32 Xalfa, Xbeta;
	void (*calc)();


}CLARK;
typedef CLARK *CLARK_h;




#define CLARK_default {0, 0, 0, 0, (void(*)(long))CLARK_calc}
void CLARK_calc(CLARK_h);

typedef struct{
	float32 Xalfa, Xbeta;
	float32 theta;
	float32 d,q;
	void (*calc)();
	void (*inv)();
}PARK;
typedef PARK *PARK_h;
#define PARK_default {0, 0, 0, 0,0, (void(*)(long))PARK_calc,(void(*)(long))PARK_inv}
void PARK_calc(PARK_h);
void PARK_inv(PARK_h);








#endif /* CLARKPARK_H_ */

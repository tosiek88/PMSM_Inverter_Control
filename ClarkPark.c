/*
 * ClarkPark.c
 *
 *  Created on: 1 cze 2015
 *      Author: tosiek
 */
#include "DSP28x_Project.h"


void CLARK_calc(CLARK *Clark){

	Clark->Xalfa=Clark->Xa;
	Clark->Xbeta=(1.154*Clark->Xb+0.577*Clark->Xa);
}


void PARK_calc(PARK *Park){
	Park->d=Park->Xalfa*cos(Park->theta)+Park->Xbeta*sin(Park->theta);
	Park->q=Park->Xbeta*cos(Park->theta)-Park->Xalfa*sin(Park->theta);
}


void PARK_inv(PARK *Park){
	Park->Xalfa=Park->d*cos(Park->theta)-Park->q*sin(Park->theta);
	Park->Xbeta=Park->d*sin(Park->theta)+Park->q*cos(Park->theta);
}





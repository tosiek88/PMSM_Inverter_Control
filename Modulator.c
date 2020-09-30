#include "DSP28x_Project.h"

/*
 * Modulator.c
 *
 *  Created on: 27 maj 2015
 *      Author: tosiek
 */



void VECTOR_init(VECTOR *Vector,float Mag){

	 Vector->Mag=Mag;
	 Vector->theta=0;
	 Vector->calc(Vector);
}

void VECTOR_calc(VECTOR_h Vector){
	   Vector->Alpha=Vector->Mag*cos( (double)Vector->theta);
	    Vector->Beta=Vector->Mag*sin( (double)Vector->theta);
}


void SPVM_Init(SPVM *spvm_ptr, VECTOR *ptrVec)
{
	spvm_ptr->InputVoltage=ptrVec;


}

void SPVM_Saw(SPVM *spvm_ptr)
{
	float32 delta_y;

	 delta_y=(0.000628)*spvm_ptr->freqOfSaw; // z proporcji : delta_y/delta_t=2*PI/1 szukamy delta_y
	 spvm_ptr->sawOutput=(spvm_ptr->sawOutput+delta_y);
	        //generator pi³y
	 if (spvm_ptr->sawOutput>PI) spvm_ptr->sawOutput=-PI;
	 if (spvm_ptr->sawOutput<(-PI)) spvm_ptr->sawOutput=PI;
}



void SPVM_Calc(SPVM *spvm_ptr){
	float32 taon, tbon, tcon;




	spvm_ptr->timeTab->T1=spvm_ptr->InputVoltage->Alpha*spvm_ptr->sectors[spvm_ptr->ActualSector][0][0]+spvm_ptr->InputVoltage->Beta*spvm_ptr->sectors[spvm_ptr->ActualSector][0][1];
	spvm_ptr->timeTab->T2=spvm_ptr->InputVoltage->Alpha*spvm_ptr->sectors[spvm_ptr->ActualSector][1][0]+spvm_ptr->InputVoltage->Beta*spvm_ptr->sectors[spvm_ptr->ActualSector][1][1];

	taon=(1-spvm_ptr->timeTab->T1-spvm_ptr->timeTab->T2)*0.5;
	tbon=taon+spvm_ptr->timeTab->T1;
	tcon=tbon+spvm_ptr->timeTab->T2;



	switch((spvm_ptr->ActualSector)){
	      case 0: spvm_ptr->timeTab->compTa = taon;  spvm_ptr->timeTab->compTb = tbon; spvm_ptr->timeTab->compTc = tcon; break; // sektor 1
	      case 1: spvm_ptr->timeTab->compTa = tbon;  spvm_ptr->timeTab->compTb = taon; spvm_ptr->timeTab->compTc = tcon; break; // sektor 2
	      case 2: spvm_ptr->timeTab->compTa = tcon;  spvm_ptr->timeTab->compTb = taon; spvm_ptr->timeTab->compTc = tbon; break; // sektor 3
	      case 3: spvm_ptr->timeTab->compTa = tcon;  spvm_ptr->timeTab->compTb = tbon; spvm_ptr->timeTab->compTc = taon; break; // sektor 4
	      case 4: spvm_ptr->timeTab->compTa = tbon;  spvm_ptr->timeTab->compTb = tcon; spvm_ptr->timeTab->compTc = taon; break; // sektor 5
	      case 5: spvm_ptr->timeTab->compTa = taon;  spvm_ptr->timeTab->compTb = tcon; spvm_ptr->timeTab->compTc = tbon; break; // sektor 6
	}

	spvm_ptr->timeTab->compTa*=1250;
	spvm_ptr->timeTab->compTb*=1250;
	spvm_ptr->timeTab->compTc*=1250;


}



void  SPVM_Sector(SPVM *spvm_ptr){

	 	if(spvm_ptr->InputVoltage->theta>=0 && spvm_ptr->InputVoltage->theta<=1.047) spvm_ptr->ActualSector=1;//60*pi/180
	    if(spvm_ptr->InputVoltage->theta>=1.047 && spvm_ptr->InputVoltage->theta<=2.094) spvm_ptr->ActualSector=2;
	    if(spvm_ptr->InputVoltage->theta>=2.094 && spvm_ptr->InputVoltage->theta<=PI)  spvm_ptr->ActualSector=3;
	    if(spvm_ptr->InputVoltage->theta>=-PI && spvm_ptr->InputVoltage->theta<=-2.094)  spvm_ptr->ActualSector=4;
	    if(spvm_ptr->InputVoltage->theta>=-2.094 && spvm_ptr->InputVoltage->theta<=-1.047)  spvm_ptr->ActualSector=5;
	    if(spvm_ptr->InputVoltage->theta>=-1.047 && spvm_ptr->InputVoltage->theta<=0)  spvm_ptr->ActualSector=6;

	    spvm_ptr->ActualSector-=1;
}






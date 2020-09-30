#include "DSP28x_Project.h"

Uint16 count=0;

void E_QEP_Init(void){

    #if (CPU_FRQ_150MHZ)
	  EQep1Regs.QUPRD=15000;			// Unit Timer for 100Hz at 150 MHz SYSCLKOUT jest to zegar dla modu³u eQEP
	#endif
    #if (CPU_FRQ_100MHZ)
	  EQep1Regs.QUPRD=1000000;			// Unit Timer for 100Hz at 100 MHz SYSCLKOUT
	#endif

	EQep1Regs.QDECCTL.bit.QSRC=00;		// QEP quadrature count mode page 15 eQEP

	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=0x00;		// PCRM=00 mode - QPOSCNT reset on index event
	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
	EQep1Regs.QPOSMAX=2000;
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// QEP enable

	EQep1Regs.QCAPCTL.bit.UPPS=0x3;   	// 1/8 co 8 puls bêdzie
	EQep1Regs.QCAPCTL.bit.CCPS=7;		// 1/128 for CAP clock
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable


}

void  E_QEP_calc(E_QEP *eqep_ptr)
{

	float32 diff=0;

	//eqep_ptr->theta=(2*PI*eqep_ptr->eQEPRegHandle->QPOSCNT)/2000;
	//przerwanie co 10ms -> 100Hz
	if(eqep_ptr->eQEPRegHandle->QFLG.bit.UTO==1){

		eqep_ptr->dir = EQep1Regs.QEPSTS.bit.QDF;    // Motor direction: 0=CCW/reverse, 1=CW/forward

		eqep_ptr->pos[1]=EQep1Regs.QPOSLAT;
		//eqep_ptr->pos[1]/=2000.0;
		//obliczanie ró¿nicy

		eqep_ptr->theta=(float32)eqep_ptr->pos[1]*0.0031; //jeden obrót to 4*500; jeden obrót to równie¿ 2*PI
		if((float32)eqep_ptr->pos[1]<=1000){
			eqep_ptr->theta_rad=(float32)eqep_ptr->pos[1]*0.0031;

		}
		else if((float32)eqep_ptr->pos[1]>1000){
			eqep_ptr->theta_rad=(-6.28+(float32)eqep_ptr->pos[1]*0.0031);
		}

		//eqep_ptr->theta=(eqep_ptr->pos[1]/2000)*PI;
		// roznica pomiedzy wartoscia aktualna [1] a poprzednia [0] (w pierwszej iteracji wartosc poprzednia jest równa 0);

		if(count==100){
			if(eqep_ptr->dir==0){
						if((eqep_ptr->pos[1])>(eqep_ptr->pos[0])){
							diff=-(2000-eqep_ptr->pos[1]+eqep_ptr->pos[0]); //1 new 0 old
						}
						else{
							diff=eqep_ptr->pos[1]-eqep_ptr->pos[0]; //1 new 0 old
						}
					}
					else if(eqep_ptr->dir==1){
						if((eqep_ptr->pos[1])<(eqep_ptr->pos[0])){

							diff=2000+eqep_ptr->pos[1]-eqep_ptr->pos[0]; //1 new 0 old
						}
						else{
							diff=eqep_ptr->pos[1]-eqep_ptr->pos[0];  //1 new 0 old
						}
					}

			eqep_ptr->omega_fast=diff*3;
			eqep_ptr->pos[0]=eqep_ptr->pos[1];
			count=0;
		}
		else{
			count++;
		}

		//estymacja prêdksoci theta(k)-theta(k-1)/delta_T=d(theta)/deta_T
		 //przerwanie jest co 0,1ms zatem DELTA T równa 0.1ms;
		//przypisanie do aktualna próbke jako poprzednia

		eqep_ptr->eQEPRegHandle->QCLR.bit.UTO=1;
	}


	if((eqep_ptr->eQEPRegHandle->QEPSTS.bit.UPEVNT)==1)//wyst¹piapilo zdarzenie zliczeniu 8 pulsów
	{
		//GpioDataRegs.GPATOGGLE.bit.GPIO11=0x1;

		//150000000/128=1171875[Hz] => 1/1171875=8.533e-07  CCPS=128 -> 1/128 for CAP clock maksymalny czas jaki
		//mo¿na zliczyæ to 8.533e-07*2^16=0.05

		eqep_ptr->omega_slow=(float32)(281260.98675729520684/((float32)eqep_ptr->eQEPRegHandle->QCPRDLAT)); //delta t
		if(eqep_ptr->dir==0){
			eqep_ptr->omega_slow*=(-1);
		}
		eqep_ptr->eQEPRegHandle->QEPSTS.all=0x88; //wyczysc falge zdarzenia
	}
}

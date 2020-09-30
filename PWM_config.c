#include "DSP28x_Project.h"
#include "PID.h"

COMPARATORE_TIME tabTime;
EPWM_INFO epwm1_info, epwm2_info,epwm3_info;

Uint16 blk=0; //blokada
unsigned char key_lock=0;
float32 _zadId=0;
float32 _zadIq=0;
float32 Id_zad=0, Iq_zad=0;
float32 Id=0, Iq=0;

float32 _zadUd=0;
float32 _zadUq=0;

float32 nb_p=2;
float32 temp;

float32 Udc=0;
float32 Ud_zad=0, Uq_zad=0;

PID Pid_d=PID_default;
PID Pid_q=PID_default;

float32 _zadOmega;

float32 count2=0;

VLIMITER vLim=VLIMITER_default;
CLARK Iabc_Clark=CLARK_default;
PARK Iab_dq=PARK_default;
PARK Uab_dq_inv=PARK_default;
VECTOR IalphaBeta;


void initGPIO(){
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13=0x0;
	GpioCtrlRegs.GPADIR.bit.GPIO13=0x1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO12=0x0;
	GpioCtrlRegs.GPADIR.bit.GPIO12=0x1;

	GpioCtrlRegs.GPAMUX2.bit.GPIO27=0x0; //skofigurowany jako input do stopu uk≥adu
	GpioCtrlRegs.GPADIR.bit.GPIO27=0x0;

	GpioCtrlRegs.GPAQSEL2.bit.GPIO27=0x10;
	GpioCtrlRegs.GPACTRL.bit.QUALPRD3=0xFF;

	EDIS;
}


interrupt void epwm1_isr(void)
{
	GpioDataRegs.GPATOGGLE.bit.GPIO13=0x1;
	// pomocnicznie przerwanie - synchronizacja dla analizatora logicznego

	EncoderObj.calc(&EncoderObj);
	ControlerObj.spvm->saw(ControlerObj.spvm);  //WYGENEROWANIE PI£Y -3.14 +3.14

	Iabc_Clark.Xa=ControlerObj.adc_info->Ia; //Pomiar prπdu
	Iabc_Clark.Xb=ControlerObj.adc_info->Ib;
	Iabc_Clark.calc(&Iabc_Clark); //Transformacja w przÛd alfa beta



	Udc=ControlerObj.adc_info->Udc;
	if(Udc>560){
		Udc=560;
	}
	else if(Udc<10){
		blk=0;
	}

	if(blk==0){
		 EALLOW;
		 ControlerObj.epwm1_info->EPwmRegHandle->TZFRC.bit.OST = 1;
		 ControlerObj.epwm2_info->EPwmRegHandle->TZFRC.bit.OST = 1;
		 ControlerObj.epwm3_info->EPwmRegHandle->TZFRC.bit.OST = 1;
		 EDIS;

		 Pid_d.u=0;
		 Pid_q.u=0;

		 Pid_d.uc=0;
		 Pid_q.uc=0;

		 Pid_d.integral=0;
		 Pid_q.integral=0;

		 Pid_d.e=0;
		 Pid_q.e=0;


		 count2=0;

	}
	if(blk==1){
		 EALLOW;
		 ControlerObj.epwm1_info->EPwmRegHandle->TZCLR.bit.OST = 1;
		 ControlerObj.epwm2_info->EPwmRegHandle->TZCLR.bit.OST = 1;
		 ControlerObj.epwm3_info->EPwmRegHandle->TZCLR.bit.OST = 1;
		 EDIS;

		 if(count2<5000){
			 ControlerObj.epwm1_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)500;//625;//325;
			 ControlerObj.epwm2_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)750;//625;//325;
			 ControlerObj.epwm3_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)750;//625;//325;
			 count2++;
			 if(count2==5000){
				 EncoderObj.eQEPRegHandle->QPOSLAT=0;
			 }
		 }else{

		Iab_dq.Xalfa=Iabc_Clark.Xalfa;
		Iab_dq.Xbeta=Iabc_Clark.Xbeta;
			 	//TUTAJ WKELJ SYGNA£ Z ENKODERA
		Iab_dq.theta=nb_p*EncoderObj.theta;//ControlerObj.spvm->sawOutput;//
		Iab_dq.calc(&Iab_dq); //Transformacja w przÛd dq


		 ramp_d.update(&ramp_d, _zadId); //URUCHOMIENIE RAMPY
		  //WYJåCIE Z RAMPY przypisane do Id_zad - zmiennej wchodzπcej na odwrotnπ transformator PARKA



		 ramp_q.update(&ramp_q, _zadIq); //URUCHOMIENIE RAMPY
		 //WYJåCIE Z RAMPY przypisane do Id_zad - zmiennej wchodzπcej na odwrotnπ transformator PARKA

		 Pid_d.e=ramp_d.Wyj[0]-Iab_dq.d;//obliczanie uchybu dla sk≥adowej d
		 Pid_q.e=(ramp_q.Wyj[0]-Iab_dq.q);//obliczanie uchybu dla sk≥adowej q

		 //temp=(float32)cos(nb_p*EncoderObj.theta_rad); // uwzglÍdniam ilo par biegunÛw;

		 //EncoderObj.omega_fast;
		 Pid_d.calc(&Pid_d); // obliczenie wartoÊi sterujπcej nieograniczonej
		 Pid_q.calc(&Pid_q);	// obliczenie wartoÊi sterujπcej nieograniczonej

		 vLim.inX=Pid_d.uc; //Pod≥πczenie siÍ do LIMITERA
		 vLim.inY=Pid_q.uc;

		 vLim.calc(&vLim, Udc); //Obliczenie

		 Pid_d.u=vLim.outX;  //Wyjscie z limitera (wartosci ograniczone)
		 Pid_q.u=vLim.outY;

		 Uab_dq_inv.d=(60/Udc)*0.511; //Pod≥πczenie Id_zad do bloku odwrotnej transformaty
		 Uab_dq_inv.q=(0/Udc)*0.511; //Pod≥πczenie Iq_zad do bloku odwrotnej transformaty

		 //TUTAJ WKELJ SYGNA£ Z ENKODERA !!!
		 Uab_dq_inv.theta= ControlerObj.spvm->sawOutput;//nb_p*EncoderObj.theta;//ControlerObj.spvm->sawOutput;// 2*(EncoderObj.theta*PI/180)  //Wyjcie z generatora PI£Y

		 Uab_dq_inv.inv(&Uab_dq_inv); //obliczenie sk≥adowych wyjsciowych odwrotnej transformaty
		 //temp=cos(nb_p*ControlerObj.spvm->sawOutput);

		 ControlerObj.spvm->InputVoltage->Alpha=Uab_dq_inv.Xalfa;
		 ControlerObj.spvm->InputVoltage->Beta=Uab_dq_inv.Xbeta;
		 ControlerObj.spvm->InputVoltage->theta=atan2(Uab_dq_inv.Xbeta, Uab_dq_inv.Xalfa);

		 ControlerObj.spvm->sector(ControlerObj.spvm);
		 ControlerObj.spvm->calc(ControlerObj.spvm);

		 ControlerObj.epwm1_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)ControlerObj.spvm->timeTab->compTa;//625;//325;
		 ControlerObj.epwm2_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)ControlerObj.spvm->timeTab->compTb;//625;//325;
		 ControlerObj.epwm3_info->EPwmRegHandle->CMPA.half.CMPA=(Uint16)ControlerObj.spvm->timeTab->compTc;//625;//325;

	}
	}

 //BUFFORY

	ControlerObj.buf1[ControlerObj.bufCount]=(float32)(*ControlerObj.buf1h);
	ControlerObj.buf2[ControlerObj.bufCount]=(float32)(*ControlerObj.buf2h);
	ControlerObj.buf3[ControlerObj.bufCount]=(float32)(*ControlerObj.buf3h);

	ControlerObj.bufCount++;

	if(ControlerObj.bufCount>=999){
		ControlerObj.bufCount=0;
	}

//KONIEC PRZERWANIA
   EPwm1Regs.ETCLR.bit.INT = 1;
   // Acknowledge this interrupt to receive more interrupts from group 3
   // potwierdü to przerwanie øeby odebrac inne przerwania z tej grupy
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
   PieCtrlRegs.PIEIER3.bit.INTx1 = 0;

}

void InitPWM()
{

   // Setup TBCLK
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV6;   // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

   // Setup shadow register load on ZERO
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   // Set Compare values
   EPwm1Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;    // Set compare A value
   EPwm1Regs.CMPB = EPWM1_MIN_CMPB;              // Set Compare B value

   // Set actionsAQ_SET
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;            // Set PWM1A on Zero //zmien
   EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;          // Clear PWM1A on event A, up count

   //EPwm1Regs.AQCTLB.bit.CAU =  AQ_SET;           // Set PWM1B on Zero
   //EPwm1Regs.AQCTLB.bit.CAD =  AQ_CLEAR;        // Clear PWM1B on event B, up count

   // Interrupt where we will change the Compare Values
   EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
   EPwm1Regs.ETSEL.bit.SOCASEL = 1;       // Select SOC when TBCTR=0x0000
   EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event


   EPwm1Regs.ETSEL.bit.INTSEL = 1;     // Select INT on Zero event
   EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;

   //Dead Time strona 52
   EPwm1Regs.DBCTL.bit.POLSEL=0x02; //0x01
   EPwm1Regs.DBCTL.bit.OUT_MODE=0x03;
   EPwm1Regs.DBFED=50;
   EPwm1Regs.DBRED=50;

   EALLOW;
   EPwm1Regs.TZCTL.bit.TZA = 2;
   EPwm1Regs.TZCTL.bit.TZB = 2;
   EPwm1Regs.TZFRC.bit.OST = 1;
   EDIS;


   // Information this example uses to keep trac
   // of the direction the CMPA/CMPB values are
   // moving, the min and max allowed values and
   // a pointer to the correct ePWM registers
   epwm1_info.EPwm_CMPA_Direction = EPWM_CMP_UP; // Start by increasing CMPA & CMPB
   epwm1_info.EPwm_CMPB_Direction = EPWM_CMP_UP;
   epwm1_info.EPwmTimerIntCount = 0;             // Zero the interrupt counter
   epwm1_info.EPwmRegHandle = &EPwm1Regs;        // Set the pointer to the ePWM module
   epwm1_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;      // Setup min/max CMPA/CMPB values
   epwm1_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
   epwm1_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
   epwm1_info.EPwmMinCMPB = EPWM1_MIN_CMPB;

   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm2Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV6;   // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

     // Setup shadow register load on ZERO
   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

     // Set Compare values
   EPwm2Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;       // Set compare A value
   EPwm2Regs.CMPB = EPWM1_MIN_CMPB;                 // Set Compare B value

   EPwm2Regs.DBCTL.bit.POLSEL=0x02;
   EPwm2Regs.DBCTL.bit.OUT_MODE=0x03;
   EPwm2Regs.DBFED=50;
   EPwm2Regs.DBRED=50;

     // Set actions
   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;             // Clear PWM2A on Period
   EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;               // Set PWM2A on event A, up count

               // Set PWM2B on event B, up count

     // Interrupt where we will change the Compare Values
   EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;        // Select INT on Zero event
   EPwm2Regs.ETSEL.bit.INTEN = 0;                   // Enable INT
   EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;              // Generate INT on 3rd event

   EALLOW;
   EPwm2Regs.TZCTL.bit.TZA = 2;
   EPwm2Regs.TZCTL.bit.TZB = 2;
   EPwm2Regs.TZFRC.bit.OST = 1;
   EDIS;

     // Information this example uses to keep track
     // of the direction the CMPA/CMPB values are
     // moving, the min and max allowed values and
     // a pointer to the correct ePWM registers
   epwm2_info.EPwm_CMPA_Direction = EPWM_CMP_UP;    // Start by increasing CMPA
   epwm2_info.EPwm_CMPB_Direction = EPWM_CMP_UP;  // and decreasing CMPB
   epwm2_info.EPwmTimerIntCount = 0;                // Zero the interrupt counter
   epwm2_info.EPwmRegHandle = &EPwm2Regs;           // Set the pointer to the ePWM module
   epwm2_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;         // Setup min/max CMPA/CMPB values
   epwm2_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
   epwm2_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
   epwm2_info.EPwmMinCMPB = EPWM1_MIN_CMPB;

   EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm3Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period
   EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
   EPwm3Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
   EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
   EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV6;   // Clock ratio to SYSCLKOUT
   EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;

        // Setup shadow register load on ZERO
   EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   EPwm3Regs.CMPA.half.CMPA = EPWM1_MIN_CMPA;       // Set compare A value
   EPwm3Regs.CMPB = EPWM1_MIN_CMPB;                 // Set Compare B value

          // Set actions
   EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;             // Clear PWM2A on Period
   EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;               // Set PWM2A on event A, up count



   EPwm3Regs.DBCTL.bit.POLSEL=0x02;
   EPwm3Regs.DBCTL.bit.OUT_MODE=0x03;
   EPwm3Regs.DBFED=50;
   EPwm3Regs.DBRED=50;

          // Interrupt where we will change the Compare Values
   EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;        // Select INT on Zero event
   EPwm3Regs.ETSEL.bit.INTEN = 0;                   // Enable INT
   EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;              // Generate INT on 3rd event

   EALLOW;
   EPwm3Regs.TZCTL.bit.TZA = 2;
   EPwm3Regs.TZCTL.bit.TZB = 2;
   EPwm3Regs.TZFRC.bit.OST = 1;
   EDIS;
        // Start by increasing the compare A and decreasing compare B
   epwm3_info.EPwm_CMPA_Direction = EPWM_CMP_UP;
   epwm3_info.EPwm_CMPB_Direction = EPWM_CMP_UP;
        // Start the cout at 0
   epwm3_info.EPwmTimerIntCount = 0;
   epwm3_info.EPwmRegHandle = &EPwm3Regs;
   epwm3_info.EPwmMaxCMPA = EPWM1_MAX_CMPA;
   epwm3_info.EPwmMinCMPA = EPWM1_MIN_CMPA;
   epwm3_info.EPwmMaxCMPB = EPWM1_MAX_CMPB;
   epwm3_info.EPwmMinCMPB = EPWM1_MIN_CMPB;



}





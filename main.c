#include "DSP28x_Project.h"

// Device Headerfile and Examples Include File


//SPACE VECTOR Z URUCHOMIONYM PWM
// Deklaracja Obiekt�w GLOBALNYCH (WSKA�NIK�W DO OBIEKT�W)

ADC_INFO AdcInfoObj=ADC_INFO_default;
SPVM SpvmObj=SPVM_default;
CONTROLER ControlerObj=CONTROLER_default;
E_QEP EncoderObj=E_QEP_default;
RAMP ramp_d=RAMP_default
RAMP ramp_q=RAMP_default
extern Uint16 blk;
extern unsigned char key_lock;

void main(void)
{
   InitSysCtrl();
   InitEPwm1Gpio();
   InitEQep1Gpio();

   EALLOW;  // zezwolenie na zapis rejestru chronionego
   	   //SysyCrtrl Reg obejmuje wszystkie rejestry dotyczace PLL, ZEGARA, Watchdoga strona 34 Reference Guid Tabela 14
   		SysCtrlRegs.HISPCP.all = ADC_MODCLK; // daje zegar dla przetwornika ADC
   EDIS;

   DINT; //wylaczenie przerwan
   InitPieCtrl();
   IER = 0x0000; // wy�aczenie przerwan maskowalnych Interrupt Enable Register
   IFR = 0x0000; // Interrupt Flag Register
   InitPieVectTable();
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.ADCINT = &adc_isr;
   PieVectTable.EPWM1_INT = &epwm1_isr;  //przemapowanie przerwania du miejsca w pami�ci
   EDIS;    // This is needed to disable write to EALLOW protected registers
   InitAdc();
   ADC_Init();
    // For this example, init the ADC
   initGPIO();



   //Realizacja algorytmu synchronizacji zegar�w dla PWM
   //Reference Guide strona 35

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0; // wylacz na czas inicjalizacji
   EDIS;

   InitPWM();// ustawienie preskalera , TBCLOCKa strona 35 System and clock

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

   // Zegary zosta�y synchronizowane


// W��czenie zg�aszania przerwa� dla Grupy 3
// Ref Guide strona 124
   IER |= M_INT1;
   IER |= M_INT3;


// W�acznie odpowiedniego przerwania w Grupie 3
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // wlaczenie przerwania dla ADC
   //PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // wlaczenie przerwania dla EPWM 1 //nie jest w��czony teraz b�dzie w��czony dopiero


   EINT;   // Wlaczenie globalnych przerwan INTM, Interrupt global mask bit
   ERTM;   // Wlaczenie globalnych przerwani real time DBGM (dzieki temu debugger mo�e zatrzyma� program)




   ControlerObj.epwm1_info=&epwm1_info;
   ControlerObj.epwm2_info=&epwm2_info;
   ControlerObj.epwm3_info=&epwm3_info;

   ControlerObj.spvm=&SpvmObj;
   VECTOR Vec=VECTOR_default;
   ControlerObj.spvm->InputVoltage=&Vec;
   COMPARATORE_TIME Comp={0,0,0,0,0,0};
   ControlerObj.spvm->timeTab=&Comp;
   ControlerObj.adc_info=&AdcInfoObj;
   EncoderObj.init(&EncoderObj);

   ControlerObj.spvm->freqOfSaw=0.5;

   //ControlerObj.spvm->sawOutput;


   for(;;){

	   if(!key_lock && !GpioDataRegs.GPADAT.bit.GPIO27){
	   			key_lock=1;
	   			blk^=1;

	   		}else if (key_lock && GpioDataRegs.GPADAT.bit.GPIO27)key_lock++;

   }

}






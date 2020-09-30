#include "DSP28x_Project.h"

void ADC_Init(){

	   AdcRegs.ADCTRL3.bit.SMODE_SEL=0x1;
	   AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1;       // Bedzie 2
	   AdcRegs.ADCMAXCONV.bit.MAX_CONV2=1;

	   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA3 as 1st SEQ1 conv.
	   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;

	   AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
	   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
	   AdcRegs.ADCTRL3.bit.ADCCLKPS=0;
	   AdcRegs.ADCTRL1.bit.CPS=0;
	   AdcRegs.ADCTRL1.bit.ACQ_PS=8;
	   //AdcRegs.ADCREFSEL.all=0x01;  referencyjne zewnetrzne

}

void ADC_Convert(ADC_INFO *AdcInfo){
	GpioDataRegs.GPATOGGLE.bit.GPIO12=0x1;

	AdcInfo->Ia=AdcInfo->AdcRegHandle->ADCRESULT0>>4;
	AdcInfo->Ib=AdcInfo->AdcRegHandle->ADCRESULT1>>4;
	AdcInfo->Udc=AdcInfo->AdcRegHandle->ADCRESULT2>>4;
	AdcInfo->Pusty=AdcInfo->AdcRegHandle->ADCRESULT3>>4;

	AdcInfo->Ia=-0.0060380859375*AdcInfo->Ia+(12.371771);
	AdcInfo->Ib=-0.0060380859375*AdcInfo->Ib+(12.371771);
	AdcInfo->Udc=-0.3214599609375*AdcInfo->Udc+653.8358;
	//GpioDataRegs.GPATOGGLE.bit.GPIO12=0x1;
}

interrupt void adc_isr(){

	AdcInfoObj.convert(&AdcInfoObj);


	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
}


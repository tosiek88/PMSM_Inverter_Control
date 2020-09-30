

typedef struct{
	volatile struct ADC_REGS *AdcRegHandle;
	float32 Ia,Ib,Udc,Pusty;

	void (*init)();
	void (*convert)();
}ADC_INFO;

typedef ADC_INFO *ADC_INFO_h;
#define ADC_INFO_default {&AdcRegs, 0,0,0,0, (void(*)(long))ADC_Init, (void(*)(long))ADC_Convert }
extern ADC_INFO AdcInfoObj;
#define ADC_MODCLK 0x3;
void ADC_Init();
void ADC_Convert(ADC_INFO_h);
interrupt void adc_isr();

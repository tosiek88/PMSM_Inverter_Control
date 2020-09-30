
#ifndef _INCL_PWM




#define EPWM1_TIMER_TBPRD 1250
#define TB_DIV6 0x3;
#define TB_DIV10 0x5;
#define TB_DIV12 0x6;

#define EPWM1_MAX_CMPA     1230
#define EPWM1_MIN_CMPA       5
#define EPWM1_MAX_CMPB     1230
#define EPWM1_MIN_CMPB       5

#define EPWM_CMP_UP   1
#define EPWM_CMP_DOWN 0

typedef struct
{
   volatile struct EPWM_REGS *EPwmRegHandle;
   Uint16 EPwm_CMPA_Direction;
   Uint16 EPwm_CMPB_Direction;
   Uint16 EPwmTimerIntCount;
   Uint16 EPwmMaxCMPA;
   Uint16 EPwmMinCMPA;
   Uint16 EPwmMaxCMPB;
   Uint16 EPwmMinCMPB;
}EPWM_INFO;

typedef struct{
    float32 T1;
    float32 T2;
    float32 T0_0;
    float32 compTa;
    float32 compTb;
    float32 compTc;
}COMPARATORE_TIME;

typedef COMPARATORE_TIME *COMPARATORE_TIME_h;
extern EPWM_INFO epwm1_info,epwm2_info,epwm3_info;

extern COMPARATORE_TIME tabTime;
void initGPIO();
void InitPWM(void);
interrupt void epwm1_isr(void);
#define _INCL_PWM

#else


#endif

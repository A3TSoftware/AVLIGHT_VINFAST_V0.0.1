
/*
PROJECT NAME: FLASHER
CUSTOMER: AMPERE - GREAVES MOBILITY
TESTED WITH: NEXUS LITE and RYD
DATE UPDATED: 17/10/2026
OPERATING VOLTAGE: 14V
OPERATING VOLTAGE RANGE: 10V - 18V
*/

#define MAIN_C

#include "adcx.h"
#include "delay.h"
#include "led.h"

/* -------------------------------------------------
   GLOBAL VARIABLES
   ------------------------------------------------- */

float Voltage;
float InputVoltage;
volatile int Value_adc = 0;

u16 delayTime = 350;

void LOAD_OUTPUT_ENABLE(void);
int ReadAndStoreADCValue(void);
int getDelayFromFormula(int voltage, int adcValue);
void ReadBatteryADCValue(void);
void getDelayValue(void);

u8 ADC_buff = 0;
u8 ADC_buff1 = 0;

volatile bool check_flag = false;
volatile u8 i = 0;

/* -------------------------------------------------
   VOLTAGE CUTOFF THRESHOLDS

   Based on your measured ADC values:
   16.0V: Load ON  = 3295
          Load OFF = 3400

   15.9V: Load ON  = 3280
          Load OFF = 3380
   ------------------------------------------------- */

#define VOLTAGE_OFF_ADC    3390
#define VOLTAGE_ON_ADC     3500

/* -------------------------------------------------
   STATE MACHINE
   ------------------------------------------------- */

typedef enum
{
    STATE_NORMAL,
    STATE_VOLTAGE_OFF
} VoltageState;

VoltageState voltageState = STATE_NORMAL;

/* -------------------------------------------------
   EXISTING ADC TABLES
   ------------------------------------------------- */

const uint16_t VoltageADCTable[7] =
{
    2000,   // 10V
    2200,   // 11V
    2400,   // 12V
    2600,   // 13V
    2800,   // 14V
    3000,   // 15V
    3310    // 16V - existing calibration
};

const uint16_t ThresholdTable[7] =
{
     220,   // 10V
     650,   // 11V
    1300,   // 12V
    2000,   // 13V
    2500,   // 14V
    2800,   // 15V 3300
    4000    // 16V 3800
};

/* -------------------------------------------------
   MAIN
   ------------------------------------------------- */

s32 main(void)
{
    DELAY_Init();

    ADC1BasicConfigWithParameter();
    ADC1PinConfigWithParameter();
    ADC1ChannelConfigWithParameter();

    OUTPUT_Init();
    INPUT_Init();

    Buzzer_low();

    ADC1SingleChannelInit();

    DELAY_Ms(200);

    ReadAndStoreADCValue();

    delayTime = 350;

    while (1)
    {
        LOAD_OUTPUT_ENABLE();
    }
}

/* -------------------------------------------------
   BATTERY ADC ONLY
   Reads Channel 5 without changing delayTime or
   Value_adc, which belong to the original flasher.
   ------------------------------------------------- */

void ReadBatteryADCValue(void)
{
    ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_5);

    DELAY_Ms(1);

    InputVoltage = GetAdcAverage1(ADC1, 5);

    Voltage = (((float)InputVoltage) / 4095.0f) * 5.0f;
}

/* -------------------------------------------------
   VOLTAGE STATE MACHINE + EXISTING FLASHER
   ------------------------------------------------- */

void LOAD_OUTPUT_ENABLE(void)
{
    /* Read battery voltage before deciding the state */
ReadBatteryADCValue();
//		ReadAndStoreADCValue();
    switch (voltageState)
    {
        /* -----------------------------------------
           NORMAL OPERATION
           ----------------------------------------- */

        case STATE_NORMAL:
					
    
				
            /*
             * Battery reached the OFF threshold
             * while the load is operating.
             */
            if (InputVoltage >= VOLTAGE_OFF_ADC)
            {
                voltageState = STATE_VOLTAGE_OFF;

                delayTime = 0;
                check_flag = false;

                Enable_Low();
                LEFT_OUTPUT_LOW();
                Buzzer_low();

                break;
            }

            /*
             * Voltage is below the cutoff threshold.
             * Run the original flasher logic.
             */

           
//ReadAndStoreADCValue(); 
            if ((Value_adc < 1) && (InputVoltage <= 3299))
            {
							 Enable_High();
            Select_Low();
//                delayTime = 350;

                LEFT_OUTPUT_HIGH();

                ReadAndStoreADCValue();

                check_flag = true;
            }
            else
            {
                if (check_flag == true)
                {
										 Enable_High();
                    check_flag = false;
                    delayTime = 350;

                    LEFT_OUTPUT_HIGH();

                    Buzzer_high();
                    DELAY_Ms(delayTime);

                    Buzzer_low();

                    LEFT_OUTPUT_LOW();
                    DELAY_Ms(delayTime);
                }

                /* Original 350 ms timing */
                if (delayTime == 350)
                {
										 Enable_High();
                    ADC_buff1 = 0;

                    LEFT_OUTPUT_HIGH();
                    Buzzer_high();

                    DELAY_Ms(80);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    DELAY_Ms(130);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    DELAY_Ms(80);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    Buzzer_low();

                    LEFT_OUTPUT_LOW();

                    DELAY_Ms(350);
										


                    if ((ADC_buff1 < 2) &&
                        (ADC_buff1 > 0))
                    {
                        ADC_buff1 = 0;
                        delayTime = 165;
                    }
                }

                /* Original 165 ms timing */
                if (delayTime == 165)
                {
										 Enable_High();
                    ADC_buff1 = 0;

                    LEFT_OUTPUT_HIGH();
                    Buzzer_high();

                    DELAY_Ms(30);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    DELAY_Ms(50);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    DELAY_Ms(25);

                    ADC_buff = ReadAndStoreADCValue();
                    ADC_buff1 = ADC_buff + ADC_buff1;

                    Buzzer_low();

                    LEFT_OUTPUT_LOW();

                    DELAY_Ms(165);
										


                    if (ADC_buff1 > 2)
                    {
                        delayTime = 350;
                        ADC_buff1 = 0;
                    }
                }
            }

            break;

        /* -----------------------------------------
           VOLTAGE CUTOFF
           Keep the load OFF until the restart
           threshold is reached.
           ----------------------------------------- */

        case STATE_VOLTAGE_OFF:

            Enable_Low();
            LEFT_OUTPUT_LOW();
            Buzzer_low();

            /*
             * At 16.0V, load OFF ADC is about 3400.
             * Remain OFF until it falls to 3380
             * or below (approximately 15.9V).
             */
            if (InputVoltage <= VOLTAGE_ON_ADC)
            {
                voltageState = STATE_NORMAL;

                delayTime = 350;
                check_flag = true;

                /*
                 * Do not turn the output on here.
                 * The NORMAL state will resume the
                 * existing flasher logic next loop.
                 */
            }

            break;

        /* -----------------------------------------
           SAFETY DEFAULT
           ----------------------------------------- */

        default:

            voltageState = STATE_VOLTAGE_OFF;

            Enable_Low();
            LEFT_OUTPUT_LOW();
            Buzzer_low();

            break;
    }
}

/* -------------------------------------------------
   EXISTING ADC READ + LOAD DETECTION
   ------------------------------------------------- */

int ReadAndStoreADCValue(void)
{
    ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_5);

    DELAY_Ms(10);

    InputVoltage = GetAdcAverage1(ADC1, 5);

    Voltage = (((float)InputVoltage) / 4095.0f) * 5.0f;

    ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_7);

    DELAY_Ms(10);

    Value_adc = GetAdcAverage1(ADC1, 5);

    delayTime = getDelayFromFormula((int)InputVoltage,
                                    Value_adc);

    if (Value_adc == 0)
    {
        delayTime = 350;
    }

    if (delayTime == 350 )
    {
        return 1;
    }

    if ((delayTime == 165) && (Value_adc != 0))
    {
        return 0;
    }

//    return 1;
}

/* -------------------------------------------------
   EXISTING LOAD DETECTION FORMULA
   ------------------------------------------------- */

int getDelayFromFormula(int batteryADC, int loadADC)
{
    uint8_t i;
    uint16_t threshold;

    /* Clamp below 10V */
    if (batteryADC <= VoltageADCTable[0])
    {
        threshold = ThresholdTable[0];
    }
    /* Clamp above 16V */
    else if (batteryADC >= VoltageADCTable[6])
    {
        threshold = ThresholdTable[6];
    }
    else
    {
        /* Find voltage interval */
        for (i = 0; i < 6; i++)
        {
            if ((batteryADC >= VoltageADCTable[i]) &&
                (batteryADC <= VoltageADCTable[i + 1]))
            {
                break;
            }
        }

        /* Linear interpolation */
        threshold =
            ThresholdTable[i] +
            ((uint32_t)(batteryADC - VoltageADCTable[i]) *
            (ThresholdTable[i + 1] - ThresholdTable[i])) /
            (VoltageADCTable[i + 1] - VoltageADCTable[i]);
    }

    if (loadADC >= threshold)
    {
        return 350;    /* Two lamps connected */
    }
    else
    {
        return 165;    /* One lamp / lamp failure */
    }
}

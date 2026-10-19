////////////////////////////////////////////////////////////////////////////////
/// @file    adcx.c
/// @author  AE TEAM
/// @brief   Output received data.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////
// Define to prevent recursive inclusion
#define _ADCX_C_

// Files includes
#include "delay.h"
#include "adcx.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADCX
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ADC_Exported_Functions
/// @{

u16 gAdcNum;
float fValue;

////////////////////////////////////////////////////////////////////////////////
/// @brief  Setup the ADC port clock
/// @param  ADCx : Select ADC port.
/// @param  NewState : Enable or disable the ADC clock.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC_RCC_ClockSet(ADC_TypeDef* ADCx, FunctionalState NewState)
{
    if(ADCx == ADC1) {
        RCC_APB1PeriphClockCmd(RCC_APB1ENR_ADC1, ENABLE);                    //Enable ADC clock
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC pin Configuration.
/// @param  GPIOx: select the GPIO peripheral.
/// @param  pin: select pin.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxAssignPin(GPIO_TypeDef* GPIOx, u16 pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    if(GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    }
    else if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
    }
    GPIO_InitStructure.GPIO_Pin  =  pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set ADCn sample time.
/// @param  ADCn: where n can be 1, 2 to select the ADC peripheral.
/// @param  sampleTime: the ADC Channel n Sample time to configure.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void ADCxSampleTimeConfig(ADC_TypeDef* ADCn, ADCSAM_TypeDef sampleTime)
{
    ADCn->CFGR &= ~ADC_CFGR_SAMCTL;
    ADCn->CFGR |= sampleTime;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 pin configuration.
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1PinConfigWithParameter(void)
{
    //customer can change below config based Pin assign
    //sample No.1
//    ADCxAssignPin(GPIOA, GPIO_Pin_15);
	  ADCxAssignPin(GPIOA, GPIO_Pin_2);
		ADCxAssignPin(GPIOA, GPIO_Pin_7);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Enable the selected ADC channel
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1ChannelConfigWithParameter(void)
{
    ADC_TypeDef* ADCn;
    ADCn = ADC1;
	
    ADC_ANY_Cmd(ADCn, DISABLE);                                                 //Enable and use ANYChan to control ADC sample
    ADC_ANY_NUM_Config(ADCn, 0);                                                //set Single(one) Channel as Analog iuput
//    ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_6);                                  //assign ADC channel 5 to RANK 0;
    ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_5);                                  
	  ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_7);
		ADC_ANY_Cmd(ADCn, ENABLE);                                                  //Enable and use ANYChan to control ADC sample
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 basic configuration with parameter.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1BasicConfigWithParameter(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_TypeDef* ADCn;
    ADCn = ADC1;
    ADC_StructInit(&ADC_InitStructure);

    ADC_RCC_ClockSet(ADCn, ENABLE);                                             //Enable ADC clock
    ADC1PinConfigWithParameter();

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;                     //ADC prescale factor
    ADC_InitStructure.ADC_Mode = ADC_Mode_Imm;                             //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //AD data right-justified
//    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

    ADC_Init(ADCn, &ADC_InitStructure);
    ADCxSampleTimeConfig(ADCn, ADC_Samctl_13_5);
    ADC_Cmd(ADCn, ENABLE);                                                      //Enable AD conversion
    ADC_ANY_Cmd(ADCn, DISABLE);                                                 //Enable and use ANYChan to control ADC sample
    ADC_ANY_NUM_Config(ADCn, 0);                                                //set Single(one) Channel as Analog iuput
    ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_6);                                  //assign ADC channel 5 to RANK 0;
    ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_5);
		ADC_ANY_CH_Config(ADCn, 0, ADC_Channel_7);
		ADC_ANY_Cmd(ADCn, ENABLE);                                                  //Enable and use ANYChan to control ADC sample

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC Configure.
/// @param  NVIC Channel, Priority or SubPriority.
/// @arg    ch: IRQChannel
/// @arg    pri: Priority, Use only Cortex-M3
/// @arg    sub: SubPriority
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
static void NVIC_Configure(u8 ch, u8 pri, u8 sub)
{

    exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ch;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = pri;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = sub;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    exNVIC_Init(&NVIC_InitStruct);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Get_ADC_IRQ_Flag function.
/// @param  ADCx: where n can be 1, 2 to select the ADC peripheral.
/// @retval UART IRQ index
////////////////////////////////////////////////////////////////////////////////
u8 Get_ADC_IRQ_Flag(ADC_TypeDef* ADCx)
{
    if      (ADCx == ADC1)      return ADC_COMP_IRQn;//ADC1_IRQn;

    return ADC_COMP_IRQn;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC NVIC initialization
/// @param  ADCx: where n can be 1, 2 to select the ADC peripheral.
/// @arg    pri: Priority, Use only Cortex-M3
/// @arg    sub: SubPriority
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADCxNvicInit(ADC_TypeDef* ADCx, u8 pri, u8 sub)
{
    NVIC_Configure(Get_ADC_IRQ_Flag(ADCx), pri, sub);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 NVIC initialization with paramters.
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1NvicInitWithParameters(void)
{
    ADCxNvicInit(ADC1, 0, 1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1_IRQHandler function.
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1_IRQHandler(void)
{
    if(RESET != ADC_GetITStatus(ADC1, ADC_IT_EOC)) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);//ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
        gAdcValue = 0xFFF & ADC_GetConversionValue(ADC1);
        gAdcFlag = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get single channel value.
/// @param  None
/// @retval single channel value.
////////////////////////////////////////////////////////////////////////////////
//u16 GetSingleChannelValue(void)
//{
//    u16 uiADData;
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                     //Software start conversion
//    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
//    gAdcFlag = 0;
//    while(1) {
//        if(1 == gAdcFlag) {
//            uiADData = (u16)gAdcValue;
//            gAdcFlag = 0;
//            break;
//        }
//    }
//    return uiADData;
//}



////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is getting the average of ADC
/// @note   None.
/// @param  times: Sampling frequency.
/// @retval average.
////////////////////////////////////////////////////////////////////////////////
//u16 GetAdcAverage(u8 times)
//{
//    u32 temp_val = 0;
//    u8 t;
//    u8 delay;
//    for(t = 0; t < times; t++) {
//        temp_val += GetSingleChannelValue();
//        for(delay = 0; delay < 100; delay++);
//    }
//    return temp_val / times;
//}

////////////////////////////////////////////////////////////////////////////////
/// @brief  ADC1 single channel initialization.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void ADC1SingleChannelInit(void)
{
//    printf("ADC1_CH5 test\r\n");
    ADC1BasicConfigWithParameter();
    ADC_Cmd(ADC1, ENABLE);                                                      //Enable AD conversion();
    ADC1NvicInitWithParameters();
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}



///////////////
u16 GetSingleChannelValue(ADC_TypeDef* ADCx)
{
    u16 uiADData;
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    gAdcFlag = 0;

    while (1) {
        if (1 == gAdcFlag) {
            uiADData = (u16)gAdcValue;
            gAdcFlag = 0;
            break;
        }
    }

    return uiADData;
}

u16 GetAdcAverage1(ADC_TypeDef* ADCx, u8 times)
{
    u32 temp_val = 0;
    u8 t;
    u8 delay;

    for (t = 0; t < times; t++) {
        temp_val += GetSingleChannelValue(ADCx);
        for (delay = 0; delay < 100; delay++);
    }

    return temp_val / times;
}
//////////////
/// @}


/// @}

/// @}



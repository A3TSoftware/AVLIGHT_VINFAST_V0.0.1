////////////////////////////////////////////////////////////////////////////////
/// @file    led.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
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
#ifndef __LED_H
#define __LED_H

// Files includes
#include <string.h>

#include "mm32_device.h"
#include "hal_conf.h"




////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Example_Layer
/// @brief MM32 Example Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_RESOURCE
/// @brief MM32 Examples resource modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Constants
/// @{

#define Right_output_pin GPIO_Pin_4
#define Left_output_pin GPIO_Pin_10
#define Enable_Pin GPIO_Pin_9
#define Select_Pin GPIO_Pin_8
#define Buzzer GPIO_Pin_15


//#define LH_SENSE_ON_HIGH() GPIO_SetBits(GPIOA,GPIO_Pin_5) ;
//#define LH_SENSE_ON_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//#define LH_SENSE_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5))?(GPIO_ResetBits(GPIOA,GPIO_Pin_5)):(GPIO_SetBits(GPIOA,GPIO_Pin_5))

#define Enable_High() GPIO_SetBits(GPIOA, Enable_Pin);
#define Enable_Low() GPIO_ResetBits(GPIOA, Enable_Pin);


#define Select_High() GPIO_SetBits(GPIOA, Select_Pin);
#define Select_Low() GPIO_ResetBits(GPIOA, Select_Pin);

#define Select_High1() GPIO_SetBits(GPIOA, GPIO_Pin_5);
#define Select_Low1() GPIO_ResetBits(GPIOA, GPIO_Pin_5);

#define Select_High2() GPIO_SetBits(GPIOA, GPIO_Pin_6);
#define Select_Low2() GPIO_ResetBits(GPIOA, GPIO_Pin_6);

#define Buzzer_high() GPIO_SetBits(GPIOA,Buzzer);
#define Buzzer_low() GPIO_ResetBits(GPIOA,Buzzer);

#define LEFT_OUTPUT_HIGH() GPIO_SetBits(GPIOA, Left_output_pin);
#define LEFT_OUTPUT_LOW() GPIO_ResetBits(GPIOA, Left_output_pin);
#define LEFT_OUTPUT_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,Left_output_pin))?(GPIO_ResetBits(GPIOA,Left_output_pin)):(GPIO_SetBits(GPIOA,Left_output_pin))

//#define RH_SENSE_ON_HIGH() GPIO_SetBits(GPIOA,GPIO_Pin_6) ;
//#define RH_SENSE_ON_LOW() GPIO_SetBits(GPIOA,GPIO_Pin_6) ;
//#define RH_SENSE_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))?(GPIO_ResetBits(GPIOA,GPIO_Pin_6)):(GPIO_SetBits(GPIOA,GPIO_Pin_6))

#define RIGHT_OUTPUT_HIGH() GPIO_SetBits(GPIOA, Right_output_pin);
#define RIGHT_OUTPUT_LOW() GPIO_ResetBits(GPIOA, Right_output_pin);
#define RIGHT_OUTPUT_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,Right_output_pin))?(GPIO_ResetBits(GPIOA,Right_output_pin)):(GPIO_SetBits(GPIOA,Right_output_pin))


#define IGNITION_HIGH() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == Bit_SET
#define IGNITION_LOW()  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == Bit_RESET

#define LH_HIGH() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_SET
#define LH_LOW()  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET

#define RH_HIGH() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_SET
#define RH_LOW()  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET

#define HAZARD_HIGH() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_SET
#define HAZARD_LOW()  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_RESET

//#define GPIO_10_HIGH()  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == Bit_SET
//#define GPIO_10_LOW()   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == Bit_RESET





void INPUT_Init(void);
void OUTPUT_Init(void);

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Enumeration
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief XXXX enumerate definition.
/// @anchor XXXX
////////////////////////////////////////////////////////////////////////////////

typedef enum {
    LED1,
} Led_TypeDef;

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Variables
/// @{
#ifdef _LED_C_
#define GLOBAL







#else
#define GLOBAL extern







#endif





#undef GLOBAL

/// @}


////////////////////////////////////////////////////////////////////////////////
/// @defgroup MM32_Exported_Functions
/// @{




void LED_Init(void);

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

/**
  ******************************************************************************
  * @file    comm.c
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "comm.h"
#include "eeprom.h"
#include "tim.h"

/* Configuration -------------------------------------------------------------*/

// Virtual address vector for the eeprom emulation
uint16_t VirtAddVarTab[2] = //NB_OF_VAR] = 
{
  0x0,
};

// --------------------------------------------------------------------------------------
// ADDRESS      0           DEVICE COMMOUNICATION PROTOCOL IDENTIFIER           READ ONLY

uint32_t device_comm_protocol_id = 0xFFFF0000; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      1           DEVICE COMMOUNICATION DRIVER IDENTIFIER             READ ONLY

uint32_t device_comm_driver_id = 0xFFFF0000; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      2           PWM FRQ MULTIPLER                                   WRITEABLE

uint32_t pwm_frq_multipler = 10; 

static void pwm_frq_multipler_init()
{
    #ifdef COMM_SET_INIT

      EE_WriteVariable(0, 1);
  
    #endif

    uint16_t temp;
    EE_ReadVariable(0, &temp);
    pwm_frq_multipler = temp;
    __HAL_TIM_SET_AUTORELOAD(&htim4, htim4.Init.Period / temp);   
}

static void pwm_frq_multiper_after(COMM_EventType_TypeDef EventType, void * n)
{
    if (EventType == COMM_WRITE)
    {
        EE_WriteVariable(0, pwm_frq_multipler);
        __HAL_TIM_SET_AUTORELOAD(&htim4, 65535 / pwm_frq_multipler);
    }
}

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      3           PWM COMPARE VALUE                                   WRITEABLE

int32_t pwm_compare_val = 0;

static void pwm_compare_val_after(COMM_EventType_TypeDef EventType, void * n)
{
    if (EventType == COMM_WRITE)
    {
        if (pwm_compare_val >= 0)
        {
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm_compare_val / pwm_frq_multipler);
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
        }
        else
        {
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, -1 * pwm_compare_val / pwm_frq_multipler);
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
        }
    }
}

// --------------------------------------------------------------------------------------

// Communication structure buildup

COMM_Registers_TypeDef COMM_DEF_Reg =
{
  { COMM_READ_ONLY, COMM_UINT32, &device_comm_protocol_id,      NULL,                           NULL},                          
  { COMM_READ_ONLY, COMM_UINT32, &device_comm_driver_id,        NULL,                           NULL},
  { COMM_WRITEABLE, COMM_UINT32, &pwm_frq_multipler,            NULL,                           pwm_frq_multiper_after},
  { COMM_WRITEABLE, COMM_INT32,  &pwm_compare_val,              NULL,                           pwm_compare_val_after}   
};

void COMM_Data_Init()
{   
  //  2  PWM FRQ MULTIPLER
  pwm_frq_multipler_init();
}
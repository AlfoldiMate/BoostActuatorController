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

// --------------------------------------------------------------------------------------
// ADDRESS      0           DEVICE COMMOUNICATION PROTOCOL IDENTIFIER           READ ONLY

uint32_t device_comm_protocol_id = 0xFFFF0000; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      1           DEVICE COMMOUNICATION DRIVER IDENTIFIER             READ ONLY

uint32_t device_comm_driver_id = 0xFFFF0000; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      2           EEPROM EMULATOR TEST                                WRITEABLE

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x0};

uint32_t eeprom_emulator_test_data = 100; 

static void eeprom_emulator_before(COMM_EventType_TypeDef EventType, void * n)
{
    if (EventType == COMM_READ)
    {
        uint16_t data;
        EE_ReadVariable(0, &data);
        eeprom_emulator_test_data = data;
    }
}

static void eeprom_emulator_after(COMM_EventType_TypeDef EventType, void * n)
{
    if (EventType == COMM_WRITE)
    {
        EE_WriteVariable(0, eeprom_emulator_test_data);
    }
}

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      3           UINT TEST DATA                                    WRITEABLE

uint32_t inmemory_uint_test_data = 0; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      4           INT TEST DATA                                    WRITEABLE

int32_t inmemory_int_test_data = 0; 

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
// ADDRESS      5           UINT TEST DATA                                    WRITEABLE

float inmemory_float_test_data = 0; 

// --------------------------------------------------------------------------------------


// Communication structure buildup

COMM_Registers_TypeDef COMM_DEF_Reg =
{
  { COMM_READ_ONLY, COMM_UINT32, &device_comm_protocol_id,      NULL,                           NULL},                          
  { COMM_READ_ONLY, COMM_UINT32, &device_comm_driver_id,        NULL,                           NULL},
  { COMM_WRITEABLE, COMM_UINT32, &eeprom_emulator_test_data,    eeprom_emulator_before,         eeprom_emulator_after},   
  { COMM_WRITEABLE, COMM_UINT32, &inmemory_uint_test_data,      NULL,                           NULL},
  { COMM_WRITEABLE, COMM_INT32,  &inmemory_int_test_data,       NULL,                           NULL},
  { COMM_WRITEABLE, COMM_FLOAT,  &inmemory_float_test_data,     NULL,                           NULL}
};
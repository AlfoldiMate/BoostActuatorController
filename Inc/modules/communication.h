/**
  ******************************************************************************
  * @file    communication.h
  * @brief   This file contains all the function prototypes for
  *          the communication.c file
  ******************************************************************************
  */
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "stm32f1xx_hal.h"

typedef enum
{

  COMM_READ_ONLY,
  COMM_WRITEABLE

} COMM_RegisterWriteFlag_TypeDef;

typedef enum
{

  COMM_READ = 0,
  COMM_WRITE = 1

} COMM_EventType_TypeDef;

typedef enum
{

  COMM_SHORT = 0,
  COMM_LONG = 1

} COMM_IdType_TypeDef;

typedef enum
{

  COMM_UINT32,
  COMM_INT32,
  COMM_FLOAT

} COMM_DataType_TypeDef;

typedef struct
{

  COMM_RegisterWriteFlag_TypeDef writeable;
  COMM_DataType_TypeDef type;
  void * ptr;
  void (*before)(COMM_EventType_TypeDef eventType, void * commRegister);
  void (*after)(COMM_EventType_TypeDef eventType, void * commRegister);

} COMM_Register_TypeDef;

typedef COMM_Register_TypeDef COMM_Registers_TypeDef[];

void COMM_Init(COMM_Registers_TypeDef* RegisterRegistry);//, uint16_t RegistryLength);
void COMM_Handle(uint8_t* Buf, uint32_t Len, uint8_t (*Transmit)(uint8_t* Buf, uint16_t Len));

#endif
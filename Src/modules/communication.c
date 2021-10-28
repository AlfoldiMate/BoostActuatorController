/**
  ******************************************************************************
  * @file    communication.c
  * @brief   This file contains all the function prototypes for
  *          the communication.c file
  ******************************************************************************
  */

#include "communication.h"

COMM_Registers_TypeDef*    RegisterRegistry;
//uint16_t                  RegistryLength;

void COMM_Init(COMM_Registers_TypeDef* commRegisterRegistry)//, uint16_t RegistrySizeof)
{
  RegisterRegistry  = commRegisterRegistry;
  //RegistryLength    = RegistrySizeof / sizeof(COMM_Register_TypeDef);
}

void COMM_Handle(uint8_t* Buf, uint32_t Len, uint8_t (*Transmit)(uint8_t* Buf, uint16_t Len))
{

  COMM_EventType_TypeDef eventType = (Buf[0] >> 6) & 1; 

  uint16_t address = Buf[0] & 0b00111111;
  COMM_IdType_TypeDef idType = ((Buf[0] >> 7) & 1);

  if (idType == COMM_LONG)
  {
    // ID length is long .. May be wrong.. never tested LOL
    address += (Buf[1] << 6); 
  }

  //if (address > RegistryLength)
  //{
    // If invalid id is requested then reply withe error code 3 and stop the current instance of communication handling
    //Buf[0] = 2;
    //Transmit(Buf, 1);
    //return;
  //}
  // ITT VOLT NAGY MOD HA BAJ LENNE
  // Get the com register definition according to the address
  COMM_Register_TypeDef actualRegister = (*RegisterRegistry)[address];

  // Check if before function pointer definied then call it
  if (actualRegister.before != NULL)
    actualRegister.before(eventType, &actualRegister);

  if (eventType == COMM_READ)
  {
    // READ

    if (actualRegister.ptr != NULL)
    {
      Transmit(actualRegister.ptr, 4);
    }
    else
    {
      Buf[0] = 0;
      Transmit(Buf, 1);
    }
  }
  else
  {
    // WRITE

    if (actualRegister.writeable == COMM_READ_ONLY)
    {
      // error code 1.. variable not writeable
      Buf[0] = 1;
      Transmit(Buf, 1);
    }
    else
    {
      // WHEN POSSIBLE TO WRITE

      if (actualRegister.ptr != NULL)
      {
        // AND THERE IS POINTER TO WIRTE

        if (actualRegister.type == COMM_UINT32)
        {
          *(uint32_t*)(actualRegister.ptr) = (uint32_t)((Buf[idType + 4] << 24) + (Buf[idType + 3] << 16) + (Buf[idType + 2] << 8) + Buf[idType + 1]);
        }
        else if (actualRegister.type == COMM_INT32)
        {
          *(int32_t*)(actualRegister.ptr) = (int32_t)((Buf[idType + 4] << 24) + (Buf[idType + 3] << 16) + (Buf[idType + 2] << 8) + Buf[idType + 1]);
        }
        else if (actualRegister.type == COMM_FLOAT)
        {
          //*(float*)(actualRegister.ptr) = (float)((Buf[idType + 1] << 24) + (Buf[idType + 2] << 16) + (Buf[idType + 3] << 8) + Buf[idType + 4]);

          uint8_t * ptr = actualRegister.ptr;

          ptr[3] = Buf[idType + 4];
          ptr[2] = Buf[idType + 3];
          ptr[1] = Buf[idType + 2];
          ptr[0] = Buf[idType + 1];

        }

        // status message ok
        Buf[0] = 0;
        Transmit(Buf, 1);
      }
      else
      {
        Buf[0] = 1;
        Transmit(Buf, 1);
        // Nothing to write
      }
    }
  }

  // Check if before function pointer definied then call it
  if (actualRegister.after != NULL)
    actualRegister.after(eventType, &actualRegister);

}



/*--------------------------------------------------------------------
@file            : p7rm8t.h
@brief           : The full name of [p7rm8t] is Parameter management.
                    Used to manage data centrally - this data can be set externally to the MCU,
                    simplifying the triviality of encoding and decoding 
                    when transmitting data from the MCU.
----------------------------------------------------------------------
@author          : zhangpeiwei02@gmail.com
 Release Version : 0.0.1
 Release Date    : 2024/1/29
----------------------------------------------------------------------
@attention       :
Copyright [2024] [copyright holder]     
     
Licensed under the Apache License, Version 2.0 (the "License");     
you may not use this file except in compliance with the License.     
You may obtain a copy of the License at     
  http://www.apache.org/licenses/LICENSE-2.0     
Unless required by applicable law or agreed to in writing, software     
distributed under the License is distributed on an "AS IS" BASIS,     
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     
See the License for the specific language governing permissions and     
limitations under the License.     

--------------------------------------------------------------------*/

#ifndef __P7RM8T_H__
#define __P7RM8T_H__

#include "stdint.h"
#include "string.h"


// DEFINE --------------------------------------------------------------------
// 0: both malloc and static cache
// 1: only static cache
// 2: only malloc
#ifndef P7RM8T_CACHE_MODEL
#define P7RM8T_CACHE_MODEL      0
#endif

#ifndef P7RM8T_NAME_LENGTH
#define P7RM8T_NAME_LENGTH      4
#endif

#ifndef P7RM8T_PROPERTY_LENGTH
#define P7RM8T_PROPERTY_LENGTH      (P7RM8T_NAME_LENGTH + 5)
#endif

#define P7RM8T_ERR_NOMATCHID     1
#define P7RM8T_ERR_NOADDR        2
#define P7RM8T_ERR_OVERSIZE      3
#define P7RM8T_ERR_ONLYREAD      4


// TYPE ----------------------------------------------------------------------

typedef enum{
    P7RRM8T_Authority_Normal    = 0x00,
    P7RRM8T_Authority_ReadWrite = 0x00,
    P7RRM8T_Authority_CanSave   = 0x00,
    P7RRM8T_Authority_Show      = 0x00,
    P7RRM8T_Authority_Valid     = 0x00,
    P7RRM8T_Authority_OnlyRead  = 0x01,
    P7RRM8T_Authority_NotSave   = 0x02,
    P7RRM8T_Authority_Hide      = 0x04,
    P7RRM8T_Authority_Invalid   = 0x08,  //无效
}p7rm8t_authority_t;

typedef struct {
   uint32_t OnlyRead:1;
   uint32_t NotSvae:1;
   uint32_t Hide:1;
}p7rm8t_authhandle_t;

typedef struct {
    uint8_t Name[P7RM8T_NAME_LENGTH];
    uint16_t ID;
    uint16_t Size;
    uint32_t Addr;
    p7rm8t_authhandle_t Authority;
}p7rm8t_data_t;

typedef struct {
  uint32_t Count;
  p7rm8t_data_t *Data;
}p7rm8t_list_t;

typedef struct {
    uint16_t ID;
    uint16_t ErrCode;
}p7rm8t_err_t;

// VLAUE ---------------------------------------------------------------------


// FUNCTION ------------------------------------------------------------------
void p7rm8t_deInit(void);
uint32_t p7rm8t_init(uint32_t _size, uint8_t* _cache);
uint16_t p7rm8t_reg(void* _data, uint16_t _size, char _name[P7RM8T_NAME_LENGTH]);
int p7rm8t_setProperty(uint16_t _id, p7rm8t_authority_t _auth);
int p7rm8t_set(uint16_t _id, uint8_t* _value, uint16_t _size);
void* p7rm8t_get(uint16_t _id);
const p7rm8t_list_t* p7rm8t_getAll(void);
int p7rm8t_property2Array(uint16_t _id,uint8_t *_buf,uint8_t _len);
p7rm8t_data_t p7rm8t_getProperty(uint16_t _id);
uint32_t p7rm8t_packAllProperty(uint8_t *_buf,uint32_t _len,uint8_t _includehid);
uint32_t p7rm8t_packSomeProperty(uint8_t* _buf, uint32_t _len, uint16_t *_idlist,uint16_t _count, uint8_t _includehid);
uint32_t p7rm8t_packAllValue(uint8_t *_buf,uint32_t _len,uint8_t _includehid);
uint32_t p7rm8t_packSomeValue(uint8_t *_buf,uint32_t _len, uint16_t *_idlist,uint16_t _count,uint8_t _includehid);
int p7rm8t_unpackPropertyByArray(p7rm8t_data_t *_valbuf,uint32_t _count, uint8_t* _data, uint32_t _len);
int p7rm8t_unpackValueByArray(p7rm8t_data_t *_valbuf,uint32_t _count,uint8_t* _data, uint32_t _len);
int p7rm8t_updateValueByArray(uint8_t* _data, uint32_t _len,p7rm8t_err_t *_errlist,uint32_t _count);





#endif // __P7RM8T_H__
/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/

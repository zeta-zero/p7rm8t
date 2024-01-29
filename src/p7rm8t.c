/*--------------------------------------------------------------------
@file            : p7rm8t.c
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
#include "p7rm8t.h"
#include "malloc.h"

// DEFINE --------------------------------------------------------------------
#define P7RM8T_MALLOC_STEP  10

// TYPE ----------------------------------------------------------------------



typedef struct {
    uint16_t MaxCount;
    p7rm8t_list_t List;
    struct {
        uint8_t CacheModel : 2;
    }Sta;
        
}p7rm8t_params_t;

// FUNCTION ------------------------------------------------------------------


// VLAUE ---------------------------------------------------------------------


// Internal global variable
static p7rm8t_params_t p7rm8t_Params;
static p7rm8t_params_t* const base = &p7rm8t_Params;


/**-------------------------------------------------------------------
 * @fn     : p7rm8t_deInit
 * @brief  : Reset the initialization content
 * @param  : none
 * @return : res
 */
void p7rm8t_deInit(void)
{
    if (base->Sta.CacheModel == 0) {
        goto end;
    }
    else if (base->Sta.CacheModel == 1) {
        
    }
    else if (base->Sta.CacheModel == 2) {
#if P7RM8T_CACHE_MODEL != 1
        if (base->MaxCount != 0) {
            free(base->List.Data);
        }
#endif
    }
    base->MaxCount = 0;
    base->List.Count = 0;
    base->List.Data = 0;
    base->Sta.CacheModel = 0;

end:
    return;
}

 /**-------------------------------------------------------------------
 * @fn     : p7rm8t_init
 * @brief  : Initialization function
 * @param  : none
 * @return : res
 */
uint32_t p7rm8t_init(uint32_t _size,uint8_t *_cache)
{
    uint32_t res = 0;
    if (base->Sta.CacheModel != 0) {
        goto end;
    }
    if (_cache == NULL && _size == 0) {
#if P7RM8T_CACHE_MODEL != 1
        base->List.Data = (p7rm8t_data_t*)calloc(P7RM8T_MALLOC_STEP, sizeof(p7rm8t_data_t));
        if (base->List.Data == NULL) {
            goto end;
        }
        base->Sta.CacheModel = 2;
        base->MaxCount = P7RM8T_MALLOC_STEP;
        base->List.Count = 0;
#endif
    }
    else if (_cache != 0 && _size != 0) {
#if P7RM8T_CACHE_MODEL != 2
        base->Sta.CacheModel = 1;
        base->MaxCount = _size / sizeof(p7rm8t_data_t);
        base->List.Data = (p7rm8t_data_t*)_cache;
        base->List.Count = 0;
#endif
    }
    res = base->MaxCount;
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_reg
 * @brief  : Register a parameter
 * @param  : none
 * @return : res   A valid ID is greater than 0, starting from 1.
 */
uint16_t p7rm8t_reg(void *_data,uint16_t _size,char _name[P7RM8T_NAME_LENGTH])
{
    uint16_t res = 0;
    p7rm8t_data_t* buf = NULL;
    if (base->Sta.CacheModel == 0 || _data == 0 || _size == 0) {
        goto end;
    }
    else if (base->Sta.CacheModel == 1) {
#if P7RM8T_CACHE_MODEL != 2
        if (base->List.Count >= base->MaxCount) {
            goto end;
        }
#endif
    }
    else if (base->Sta.CacheModel == 2) {
#if P7RM8T_CACHE_MODEL != 1
        if (base->List.Count >= base->MaxCount) {
            base->MaxCount += P7RM8T_MALLOC_STEP;
            buf = (p7rm8t_data_t*)realloc(base->List.Data, base->MaxCount * sizeof(p7rm8t_data_t));
            if (buf == NULL) {
                base->MaxCount -= P7RM8T_MALLOC_STEP;
                goto end;
            }
            base->List.Data = buf;
        }
#endif
    }
    
    memcpy(base->List.Data[base->List.Count].Name, _name, P7RM8T_NAME_LENGTH);
    base->List.Data[base->List.Count].Addr = (uint32_t)_data;
    base->List.Data[base->List.Count].Size = _size;
    res = base->List.Data[base->List.Count].ID = base->List.Count + 1;
    base->List.Count++;

end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_checkID
 * @brief  : Check whether the ID is valid
 * @param  : none
 * @return : res
 */
int p7rm8t_checkID(uint16_t _id)
{
    int res = 0;
    if (base->Sta.CacheModel == 0) {
        res = -1;
    }
    else if (_id == 0) {
        res = -2;
    }
    else if (_id > base->List.Count) {
        res = -3;
    }
    
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_setAuthority
 * @brief  : Set permissions by ID
 * @param  : none
 * @return : res
 */
int p7rm8t_setProperty(uint16_t _id,p7rm8t_authority_t _auth)
{
    int res = 0;
    if (p7rm8t_checkID(_id) != 0) {
        res = -1;
    }
    else {
        base->List.Data[_id-1].Authority.OnlyRead = ((_auth & P7RRM8T_Authority_OnlyRead) == P7RRM8T_Authority_OnlyRead);
        base->List.Data[_id-1].Authority.NotSvae = ((_auth & P7RRM8T_Authority_NotSave) == P7RRM8T_Authority_NotSave);
        base->List.Data[_id-1].Authority.Hide = ((_auth & P7RRM8T_Authority_Hide) == P7RRM8T_Authority_Hide);
    }
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_set
 * @brief  : Set the value by ID
 * @param  : none
 * @return : res
 */
int p7rm8t_set(uint16_t _id,uint8_t *_value,uint16_t _size)
{
    int res = 0;
    if (p7rm8t_checkID(_id) != 0) {
        res = -1;
    }
    else if (base->List.Data[_id - 1].Addr == 0) {
        res = -2;
    }
    else if (base->List.Data[_id - 1].Authority.OnlyRead == 1) {
        res = -3;
    }
    else if (base->List.Data[_id - 1].Size < _size) {
        res = -4;
    }
    else {
        memcpy((void *)base->List.Data[_id-1].Addr,_value,_size);
    }
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_get
 * @brief  : Get a value by ID
 * @param  : none
 * @return : res
 */
void* p7rm8t_get(uint16_t _id)
{
    void* res = 0;
    if (p7rm8t_checkID(_id) != 0) {
        res = 0;
    }
    else if (base->List.Data[_id - 1].Addr == 0) {
        res = 0;
    }
    else if (base->List.Data[_id - 1].Authority.Hide == 1) {
        res = 0;
    }
    else {
        res = (void*)base->List.Data[_id - 1].Addr;
    }
    
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_getAll
 * @brief  : Get all the information of parameter
 * @param  : none
 * @return : res
 */
const p7rm8t_list_t* p7rm8t_getAll(void)
{
    return (const p7rm8t_list_t*)&base->List;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_property2Array
 * @brief  : Turn the information into an array by ID:[ID:2][Name:4][Size:2][Authority:1]
 * @param  : none
 * @return : res
 */
int p7rm8t_property2Array(uint16_t _id,uint8_t *_buf,uint8_t _len)
{
    int res = 0;
    uint8_t auth = 0;
    if (p7rm8t_checkID(_id) != 0) {
        res = -1;
    }
    else if (_buf == NULL || _len < P7RM8T_PROPERTY_LENGTH) {
        res = -2;
    }
    else {
        _buf[0] = (_id >> 8) & 0xFF;
        _buf[1] = (_id >> 0) & 0xFF;
        memcpy(&_buf[2], base->List.Data[_id - 1].Name, P7RM8T_NAME_LENGTH);
        _buf[6] = (base->List.Data[_id - 1].Size >> 8) & 0xFF;
        _buf[7] = (base->List.Data[_id - 1].Size >> 0) & 0xFF;

        auth |= base->List.Data[_id - 1].Authority.OnlyRead == 1;
        auth <<= 1;
        auth |= base->List.Data[_id - 1].Authority.NotSvae == 1;
        auth <<= 1;
        auth |= base->List.Data[_id - 1].Authority.Hide == 1;
        _buf[8] = auth;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_getProperty
 * @brief  : Get the attribute by ID
 * @param  : none
 * @return : res
 */
p7rm8t_data_t p7rm8t_getProperty(uint16_t _id)
{
    p7rm8t_data_t ret = {0};
    if(p7rm8t_checkID(_id) == 0){
        ret = base->List.Data[_id-1];
    }
error:
    return ret;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_packAllProperty
 * @brief  : To convert all data information into an array:[ID:2][Name:4][Size:2][Authority:1]
 * @param  : _buf           cache address
 *           _len           cache length
 *           _includehide   include hide data or not
 * @return : res
 */
uint32_t p7rm8t_packAllProperty(uint8_t *_buf,uint32_t _len,uint8_t _includehid)
{
    uint32_t res = 0;
    uint32_t pos = 0;
    uint8_t auth = 0;
    uint16_t count = 0;
    if (_buf == NULL || _len < P7RM8T_PROPERTY_LENGTH) {
        res = 0;
    }
    else {
        pos = 2;
        for (int i = 0;i < base->List.Count && _len - pos >= P7RM8T_PROPERTY_LENGTH;i++) {
            if (_includehid == 0 && base->List.Data[i].Authority.Hide == 1) {
                continue;
            }
            _buf[pos++] = (base->List.Data[i].ID >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[i].ID >> 0) & 0xFF;
            memcpy(&_buf[pos], base->List.Data[i].Name, P7RM8T_NAME_LENGTH);
            pos += P7RM8T_NAME_LENGTH;
            _buf[pos++] = (base->List.Data[i].Size >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[i].Size >> 0) & 0xFF;

            auth |= base->List.Data[i].Authority.OnlyRead == 1;
            auth <<= 1;
            auth |= base->List.Data[i].Authority.NotSvae == 1;
            auth <<= 1;
            auth |= base->List.Data[i].Authority.Hide == 1;
            _buf[pos++] = auth;
            count++;
        }
        _buf[0] = (count >> 8) & 0xFF;
        _buf[1] = (count >> 0) & 0xFF;
        res = pos;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_packSomeProperty
 * @brief  : To convert some data information into an array:[ID:2][Name:4][Size:2][Authority:1]
 * @param  : _buf           cache address
 *           _len           cache length
 *           _idlist        ID List
 *           _count         ID List Count
 *           _includehide   include hide data or not
 * @return : res
 */
uint32_t p7rm8t_packSomeProperty(uint8_t* _buf, uint32_t _len, uint16_t *_idlist,uint16_t _count, uint8_t _includehid)
{
    uint32_t res = 0;
    uint32_t pos = 0;
    uint8_t auth = 0;
    uint16_t id = 0;
    uint16_t count = 0;
    if (_buf == NULL || _len < P7RM8T_PROPERTY_LENGTH || _idlist == 0 || _count == 0) {
        res = 0;
    }
    else {
        pos = 2;
        for (int i = 0;i < _count && _len - pos >= P7RM8T_PROPERTY_LENGTH;i++) {
            if (_idlist[i] == 0 || (_includehid == 0 && base->List.Data[_idlist[i] - 1].Authority.Hide == 1)) {
                continue;
            }
            id = _idlist[i];
            _buf[pos++] = (_idlist[i] >> 8) & 0xFF;
            _buf[pos++] = (_idlist[i] >> 0) & 0xFF;
            memcpy(&_buf[pos], base->List.Data[id - 1].Name, P7RM8T_NAME_LENGTH);
            pos += P7RM8T_NAME_LENGTH;
            _buf[pos++] = (base->List.Data[id - 1].Size >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[id - 1].Size >> 0) & 0xFF;

            auth |= base->List.Data[id - 1].Authority.OnlyRead == 1;
            auth <<= 1;
            auth |= base->List.Data[id - 1].Authority.NotSvae == 1;
            auth <<= 1;
            auth |= base->List.Data[id - 1].Authority.Hide == 1;
            _buf[pos++] = auth;
            count++;
        }
        _buf[0] = (count >> 8) & 0xFF;
        _buf[1] = (count >> 0) & 0xFF;
        res = pos;
    }

    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_packAllValue
 * @brief  : To convert all data into an array:[ID:2][size:2][data:Size]
 * @param  : _buf           cache address
 *           _len           cache length
 *           _includehide   include hide data or not
 * @return : res
 */
uint32_t p7rm8t_packAllValue(uint8_t *_buf,uint32_t _len,uint8_t _includehid)
{
    uint32_t res = 0;
    uint32_t pos = 0;
    uint16_t count = 0;
    if (_buf == NULL || _len == 0) {
        res = 0;
    }
    else {
        pos = 2;
        for (int i = 0;i < base->List.Count;i++) {
            if (_includehid == 0 && base->List.Data[i].Authority.Hide == 1) {
                continue;
            }
            if (_len < base->List.Data[i].Size + 4 + pos) {
                break;
            }
            _buf[pos++] = (base->List.Data[i].ID >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[i].ID >> 0) & 0xFF;
            _buf[pos++] = (base->List.Data[i].Size >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[i].Size >> 0) & 0xFF;
            memcpy(&_buf[pos], (const void*)base->List.Data[i].Addr, base->List.Data[i].Size);
            pos += base->List.Data[i].Size;
            count++;
        }
        _buf[0] = (count >> 8) & 0xFF;
        _buf[1] = (count >> 0) & 0xFF;
        res = pos;
    }
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_packSomeValue
 * @brief  : To convert part of the data into an array:[ID:2][size:2][data:Size]
 * @param  : _buf           cache address
 *           _len           cache length
 *           _idlist        ID List
 *           _count         ID List Count
 *           _includehide   include hide data or not
 * @return : res
 */
uint32_t p7rm8t_packSomeValue(uint8_t *_buf,uint32_t _len, uint16_t *_idlist,uint16_t _count,uint8_t _includehid)
{
    uint32_t res = 0;
    uint32_t pos = 0;
    uint16_t id = 0;
    uint16_t count = 0;
    if (_buf == NULL || _len == 0|| _idlist == 0 || _count == 0) {
        res = 0;
    }
    else {
        pos = 2;
        for (int i = 0;i < base->List.Count;i++) {
            if (_idlist[i] == 0 || (_includehid == 0 && base->List.Data[_idlist[i] - 1].Authority.Hide == 1)) {
                continue;
            }
            id = _idlist[i];
            if (_len - pos < base->List.Data[id - 1].Size + 4) {
                break;
            }
            _buf[pos++] = (base->List.Data[id - 1].ID >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[id - 1].ID >> 0) & 0xFF;
            _buf[pos++] = (base->List.Data[id - 1].Size >> 8) & 0xFF;
            _buf[pos++] = (base->List.Data[id - 1].Size >> 0) & 0xFF;
            memcpy(&_buf[pos], (const void*)base->List.Data[id - 1].Addr, base->List.Data[id - 1].Size);
            pos += base->List.Data[id - 1].Size;
            count++;
        }
        _buf[0] = (count >> 8) & 0xFF;
        _buf[1] = (count >> 0) & 0xFF;
        res = pos;
    }
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_unpackPropertyByArray
 * @brief  : Parsed arrays are formatted as properties
 * @param  : _data          cache address
 *           _len           cache length
 * @return : res
 */
int p7rm8t_unpackPropertyByArray(p7rm8t_data_t *_valbuf,uint32_t _count, uint8_t* _data, uint32_t _len)
{
    int res = 0;
    uint16_t count = 0;
    uint32_t pos = 0;
    uint16_t id = 0;
    if (_data == NULL || _len < 10 || _valbuf == 0 || _count == 0) {
        goto end;
    }
    count = (_data[0] << 8) | _data[1];
    if (count > _count) {
        count = _count;
    }
    pos = 2;
    for (int i = 0;i < count && pos + P7RM8T_PROPERTY_LENGTH <= _len;i++) {
        id = (_data[pos] << 8) | _data[pos + 1];
        if (id == 0) {
            pos += P7RM8T_PROPERTY_LENGTH;
            continue;
        }
        _valbuf[i].ID = id;
        pos += 2;
        memcpy(_valbuf[i].Name, &_data[pos], P7RM8T_NAME_LENGTH);
        pos += P7RM8T_NAME_LENGTH;
        _valbuf[i].Size = (_data[pos] << 8) | _data[pos + 1];
        pos += 2;
        _valbuf[i].Authority.OnlyRead = (_data[pos] & P7RRM8T_Authority_OnlyRead) == P7RRM8T_Authority_OnlyRead;
        _valbuf[i].Authority.NotSvae = (_data[pos] & P7RRM8T_Authority_NotSave) == P7RRM8T_Authority_NotSave;
        _valbuf[i].Authority.Hide = (_data[pos] & P7RRM8T_Authority_Hide) == P7RRM8T_Authority_Hide;
        pos += 1;
        res++;
    }
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_unpackValueByArray
 * @brief  : Parsed arrays are formatted as numeric values
 * @param  : _data          cache address
 *           _len           cache length
 * @return : res
 */
int p7rm8t_unpackValueByArray(p7rm8t_data_t *_valbuf,uint32_t _count,uint8_t* _data, uint32_t _len)
{
    int res = 0;
    uint16_t count = 0;
    uint32_t pos = 0;
    uint16_t id = 0;
    if (_data == NULL || _len < 10) {
        goto end;
    }
    count = (_data[0] << 8) | _data[1];
    if (count > _count) {
        count = _count;
    }
    pos = 2;
    for (int i = 0;i < count;i++) {
        id = (_data[pos] << 8) | _data[pos + 1];
        if (id == 0) {
            break;
        }
        _valbuf[i].ID = id;
        pos += 2;
        _valbuf[i].Size = (_data[pos] << 8) | _data[pos + 1];
        pos += 2;
        _valbuf[i].Addr = (uint32_t)(&_data[pos]);
        pos += _valbuf[i].Size;
        res++;
    }
end:
    return res;
}

/**-------------------------------------------------------------------
 * @fn     : p7rm8t_updateValueByArray
 * @brief  : Parse array update values
 * @param  : _data          cache address
 *           _len           cache length
 * @return : res
 */
int p7rm8t_updateValueByArray(uint8_t* _data, uint32_t _len,p7rm8t_err_t *_errlist,uint32_t _count)
{
    int res = 0;
    int errbuf = 0;
    uint32_t errcount = 0;
    uint16_t count = 0;
    uint32_t pos = 0;
    uint16_t id = 0;
    p7rm8t_data_t val_buf = {0};
    if (_data == NULL || _len < 10) {
        goto end;
    }
    count = (_data[0] << 8) | _data[1];
    pos = 2;
    for (int i = 0;i < count;i++) {
        id = (_data[pos] << 8) | _data[pos + 1];
        if (id == 0) {
            break;
        }
        val_buf.ID = id;
        pos += 2;
        val_buf.Size = (_data[pos] << 8) | _data[pos + 1];
        pos += 2;
        val_buf.Addr = (uint32_t)(&_data[pos]);
        pos += val_buf.Size;
        errbuf = p7rm8t_set(val_buf.ID, (uint8_t*)val_buf.Addr, val_buf.Size);
        if (errbuf != 0 && errcount < _count && _errlist != 0) {
            _errlist[errcount].ID = val_buf.ID;
            if (errbuf == -1) {
                _errlist[errcount].ErrCode = P7RM8T_ERR_NOMATCHID;
            }
            else if (errbuf == -2) {
                _errlist[errcount].ErrCode = P7RM8T_ERR_NOADDR;
            }
            else if (errbuf == -3) {
                _errlist[errcount].ErrCode = P7RM8T_ERR_ONLYREAD;
            }
            else if (errbuf == -4) {
                _errlist[errcount].ErrCode = P7RM8T_ERR_OVERSIZE;
            }
            errcount++;
        }
        res++;
    }
end:
    return res;
}

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------------
This file was generate by BaseCFileCreateTools.
Version : 0.7 base
--------------------------------------------------------------------*/

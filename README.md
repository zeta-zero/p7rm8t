# p7rm8t
The full name of [p7rm8t] is Parameter management.Used to manage data centrally - this data can be set externally to the MCU, simplifying the triviality of encoding and decoding when transmitting data from the MCU.

# Demo
### File : /test/test.c

Initialize and register parameters.

```c
    char catname[] = "Woof";
    int catage = 2;
    uint8_t emotion = 0;
    uint16_t id = 0;

    p7rm8t_init(0, 0);
    id = p7rm8t_reg(catname, sizeof(catname), "name");
    p7rm8t_setProperty(id, P7RRM8T_Authority_OnlyRead);

    id = p7rm8t_reg(&catage, sizeof(catage), "age");
    p7rm8t_setProperty(id, P7RRM8T_Authority_Normal);

    id = p7rm8t_reg(&emotion, sizeof(emotion), "emo");
    p7rm8t_setProperty(id, P7RRM8T_Authority_Hide);
```

# Source Code
### File : /src/p7rm8t.c

# API

```c
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
```

# Version

### Version 1.0.0
Release Date - 2024/01/29

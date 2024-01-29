#include "stdio.h"
#include "../src/p7rm8t.h"

int main(void)
{
    int res = 0;
    char catname[] = "Woof";
    int catage = 2;
    uint8_t buf[1024];
    uint32_t len = 0;
    uint8_t emotion = 0;
    uint16_t id = 0;
    p7rm8t_data_t valdata[5];
    int count = 0;

    p7rm8t_init(0, 0);
    id = p7rm8t_reg(catname, sizeof(catname), "name");
    p7rm8t_setProperty(id, P7RRM8T_Authority_OnlyRead);

    id = p7rm8t_reg(&catage, sizeof(catage), "age");
    p7rm8t_setProperty(id, P7RRM8T_Authority_Normal);

    id = p7rm8t_reg(&emotion, sizeof(emotion), "emo");
    p7rm8t_setProperty(id, P7RRM8T_Authority_Hide);

    len = p7rm8t_packAllProperty(buf, 1024, 0);
    printf("Base Info : \r\n");
    for (int i = 0;i < len;i++) {
        printf("0x%x ", buf[i]);
        if (i % 16 == 15) {
            printf("\r\n");
        }
    }
    printf("\r\n");

    len = p7rm8t_packAllValue(buf, 1024, 0);
    printf("Base Val : \r\n");
    for (int i = 0;i < len;i++) {
        printf("0x%x ", buf[i]);
        if (i % 16 == 15) {
            printf("\r\n");
        }
    }
    printf("\r\n\r\n");

    catage += 3;
    emotion = 50;
    p7rm8t_setProperty(3, P7RRM8T_Authority_Normal);

    len = p7rm8t_packAllProperty(buf, 1024, 0);
    printf("Chg Info : \r\n");
    for (int i = 0;i < len;i++) {
        printf("0x%x ", buf[i]);
        if (i % 16 == 15) {
            printf("\r\n");
        }
    }
    printf("\r\n");
    count = p7rm8t_unpackPropertyByArray(valdata, 5, buf, len);

    len = p7rm8t_packAllValue(buf, 1024, 0);
    printf("Chg Val : \r\n");
    for (int i = 0;i < len;i++) {
        printf("0x%x ", buf[i]);
        if (i % 16 == 15) {
            printf("\r\n");
        }
    }
    printf("\r\n");
    count = p7rm8t_unpackValueByArray(valdata, 5, buf, len);
    for (int i = 0;i < count;i++) {
        printf("Name      : %s\r\n",valdata[i].Name);
        printf("ID        : %d\r\n",valdata[i].ID);
        printf("Size      : %d\r\n", valdata[i].Size);
        printf("Authority : %d\r\n", valdata[i].Authority);
        printf("Val       : ");
        for (int j = 0;j < valdata[i].Size;j++) {
            printf("0x%x ", ((uint8_t*)valdata[i].Addr)[j]);
        }
        printf("\r\n");
    }
    printf("\r\n\r\n");



    return res;
}
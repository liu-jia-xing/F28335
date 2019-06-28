/*
 * Key.c
 *
 *  Created on: 2019��5��30��
 *      Author: ToFFF
 */
#include "TheHead.h"

/*****************************************************************************************************/

/**************************************��������************************************************/
Uint16 Keys[3][3] = {1,2,3,4,5,6,7,8,9};//10,11,12,13,14,15,16};//���ݱ�����16��������Ӧ
Uint16 KEY = 0;                                             //ʵʱ������Ϣ����
Uint16 KX_On = 0;   //���߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-3��
Uint16 KY_On = 0;   //���߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-3��
Uint16 KX_Status[4]={0};     // KX_Status[3]--->KX3״̬�� KX_Status[0]δʹ��

/*******************************����IO��ʼ��***********************************/
void InitKeyGpio(void)
{

    EALLOW;

    //////////////////////////////�����ĸ�IO������Ϊ�������Ϊ��ɨ��////////////////////////////

    //KY4: ��ӦPCB���� KEY_MATRIX��IO23
//  GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;                                      // Enable pullup on GPIO11
//    GpioDataRegs.GPASET.bit.GPIO0 = 1;                                    // Load output latch
//    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;                                   // GPIO11 = GPIO
//    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;                                    // GPIO11 = output

    //KY3: ��ӦPCB���� KEY_MATRIX��IO24
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPBSET.bit.GPIO48 = 1;                                     // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;                                     // GPIO11 = output

    //KY2: ��ӦPCB���� KEY_MATRIX��IO25
    GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;                                     // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;                                     // GPIO11 = output

    //KY1: ��ӦPCB���� KEY_MATRIX��IO26
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;                                     // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;                                     // GPIO11 = output


    //////////////////////////////�����ĸ�IO������Ϊ���룬��Ϊ��ɨ��////////////////////////////

    //KX4: ��ӦPCB���� KEY_MATRIX��IO27
  //  GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;                                       // Enable pullup on GPIO11
  //  GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;                                      // ����Ϊһ��IO��
  //  GpioCtrlRegs.GPBDIR.bit.GPIO50  = 0;                                      // IO�ڷ���Ϊ����

    //KX3: ��ӦPCB���� KEY_MATRIX��IO28
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;                                    // ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO51  = 0;                                    // IO�ڷ���Ϊ����

    //KX2: ��ӦPCB���� KEY_MATRIX��IO29
    GpioCtrlRegs.GPBPUD.bit.GPIO52 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;                                    // ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO52  = 0;                                    // IO�ڷ���Ϊ����

    //KX1: ��ӦPCB���� KEY_MATRIX��IO30
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO53  = 0;                                   // ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO53   = 0;                                       // IO�ڷ���Ϊ����

    EDIS;
    RstAllKY();
}


/*********************************************��ʱ����************************************************/
void delay(Uint32 t)
{
    Uint32 i = 0;
    for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

/****************************************4��ȫ������͵�ƽ************************************************/
void RstAllKY(void)
{
//  RST_KY4 ;
    RST_KY3 ;
    RST_KY2 ;
    RST_KY1 ;
}
/*****************************************************************************************************/

/****************************************��ȡ4��IO��ƽ״̬************************************************/
void GetKX(void)
{
    Uint16 x;           // �����
    Uint16 temp[4];     // temp[0]��ʹ��

    //��ȡ�������ߵ�ƽֵ���޼�����ʱKX_Status[1~3]��Ϊ1
    // Ϊ�˰���������������Ρ�
    KX_Status[1] = KX1_STATUS;
    KX_Status[2] = KX2_STATUS;
    KX_Status[3] = KX3_STATUS;
//  KX_Status[4] = KX4_STATUS;

    for (x=1;x<4;x++)
    {
        temp[x]=KX_Status[x];
    }
    delay(200);  // �Ե�

    //Ϊ���������ٴζ�ȡ�������ߵ�ƽֵ
    KX_Status[1] = KX1_STATUS;
    KX_Status[2] = KX2_STATUS;
    KX_Status[3] = KX3_STATUS;
//  KX_Status[4] = KX4_STATUS;

    for (x=1;x<4;x++)
    {
        if (temp[x]!=KX_Status[x]) // ���μ�ⲻͬ����δ���´�������ΪH
            KX_Status[x]=1;
    }
}
/*****************************************************************************************************/

/**************************************��ȡ������λ��**************************************/
// �жϰ��������У���¼�б��1~3�ڱ���KX_On�У����ް������£���KX_On=0��
void GetKX_On(void)
{
    Uint16 x;   // �����


    GetKX();  // �õ�����״̬

    //  �ӵ�1~3�������ж��Ƿ��м�����
    for (x=1;x<4;x++)
    {
        if(KX_Status[x] == 0)   // ��ָ�������м�����
        {
            KX_On = x;          // ��¼�м����µ����߱�ţ������һ��λ��
        }
    }
}
/*****************************************************************************************************/

/*******************************ָ��������ߵ�ƽ******************************************/
void Set_KY(Uint16 y)
{
    if(y==1){SET_KY1;}
    if(y==2){SET_KY2;}
    if(y==3){SET_KY3;}
    //if(y==4){SET_KY4;}
}
/*****************************************************************************************************/

/*******************************ָ��������͵�ƽ**********************************************/
void Rst_KY(Uint16 y)
{
    if(y==1){RST_KY1;}
    if(y==2){RST_KY2;}
    if(y==3){RST_KY3;}
    //if(y==4){RST_KY4;}
}
/*****************************************************************************************************/

/*******************************��ȡ������λ��**********************************************/
// ���а������µ��б��KY_On�Ͱ������Key
void GetKY_On(void)
{
    Uint16 y;       // �����

    if (KX_On==0)  // û�а�������
    {
        KY_On=0;
    }
    else
    {
        for (y=1;y<4;y++) //���м��
        {
            Set_KY(y);     // ����xָ������ΪH
            delay(2000);       // ��һ�ȣ���ָ��������H������

            GetKX();      // ����������ֵ��˵����ֻ��ָ����KX_On�м��ɣ����ﲻ������д������
            if(KX_Status[KX_On])  // ���ָ��KX_On�е�ƽΪH
            {
                KY_On = y;        // �õ������
            }
            Rst_KY(y);            // �ָ�L��ƽ

            if (KY_On!=0)  break;

        }
    }
}
/*****************************************************************************************************/


/*****************************************************************************************************/
void GetKey(void)
{
    KEY=0;
    KX_On=0;
    KY_On=0;

    RstAllKY();   // ������������ΪL
    GetKX_On();   // �õ��������߱�ţ�����KX_On��

    if (KX_On!=0)    // �м�����
    {
        GetKY_On();   // �õ��������߱�ţ�����KY_On��
        KEY = Keys[KX_On-1][KY_On-1];   // ��ѯ����ֵ
    }

}
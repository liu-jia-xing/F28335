/********************************************************************
* �ļ����� 	����ɨ�����
* ����:  	���г��򣬰��°������۲����Key��ֵ����ֵ�Ӵ�1~16,�޼�����ʱΪ0)��
*       ���߱�ţ�1-4��δ����ʱ���Ϊ0
*       ���߱�ţ�1-4��δ����ʱ���Ϊ0
* ���ú���: ReadKey();

* ʹ����Դ��
* ����KX1~KX4������F28335��GPI063~GPI050
* ����KY1~KY4������F28335��GPI052/53/58/59

**********************************************************************/
/********************************************************************
����˵��������ɨ��ԭ��1��3�ж�Ӧ��IO����Ϊ���,3�ж�Ӧ��IO����Ϊ���롣
				2�����ް������£�3������IO����Ϊ�ߵ�ƽ����Ϊ���ⲿ�������裩
				3�����������£���Ӧ��IO�����ͣ�����Ϊ��X�У������Ϊ�͵�ƽ�����������У�X�У��������������
				4����ʱ�����θı�3�е����Ϊ�ߣ�����������������ʱ����X�е�ƽ���±�Ϊ�ߡ����������С������������
********************************************************************/

#include "F28x_Project.h"

//#include "LED_M.h"               // LED״̬�궨��
//#include "LED.h"
#include "KEY4x4.h"
#include "LCD12864.h"
#include "Delay.h"
#include <math.h>
#include "stdlib.h"


/**************************************�궨��************************************************/
#define SET_KY4			GpioDataRegs.GPBSET.bit.GPIO63 = 1						//Y4����
#define SET_KY3			GpioDataRegs.GPESET.bit.GPIO130 = 1    						//Y3����
#define SET_KY2			GpioDataRegs.GPESET.bit.GPIO131 = 1						//Y2����
#define SET_KY1			GpioDataRegs.GPCSET.bit.GPIO66 = 1						//Y1����

#define RST_KY4			GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1					//Y4����
#define RST_KY3			GpioDataRegs.GPECLEAR.bit.GPIO130 = 1 					//Y3����
#define RST_KY2			GpioDataRegs.GPECLEAR.bit.GPIO131 = 1 						//Y2����
#define RST_KY1			GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1						//Y1����

#define KX4_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO25							//X4״̬
#define KX3_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO27							//X3״̬
#define KX2_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO26							//X2״̬
#define KX1_STATUS	 	GpioDataRegs.GPCDAT.bit.GPIO64							//X1״̬
/*****************************************************************************************************/
// ����˳�򣬰��б�ţ���һ��K1-K4��K5-K8��K9-K12��K13-K16
Uint16 Keys[4][4] = {1,5,9,13, 2,6,10,14, 3,7,11,15,  4,8,12,16};//���ݱ���16��������Ӧ

Uint16 Key = 0;												//ʵʱ������Ϣ����
Uint16 KX_On = 0;   //���߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-4��
Uint16 KY_On = 0;   //���߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-4��
Uint16 KX_Status[5]={0};     // KX_Status[4]--->KX4״̬�� KX_Status[0]δʹ��

/*********************************************��������************************************************/
void InitKeyGpio(void);  // ��ʼ����������
void delay(Uint32 t);
void RstAllKY(void);   // ������������ΪL��ƽ
void GetKX(void);      // �õ�����״̬
void GetKX_On(void);   // �õ��������������б��
void SetKY(Uint16 y);  // ����ָ������ΪH��ƽ
void RstKY(Uint16 y);  // ����ָ������ΪL��ƽ
void GetKey(void);     // �õ����µİ�����ţ�����Key��

void Key_Process();
void Change_WaveType();
void add_argu();
void sub_argu();
void select_argu();

//void main(void)
//{
//   InitSysCtrl();
//
//   InitKeyGpio(); // �������ų�ʼ��
//   //InitLedGpio();
//
//
//   DINT;
//   InitPieCtrl();
//   IER = 0x0000;
//   IFR = 0x0000;
//   InitPieVectTable();
//
//	while(1)	//��ѭ��
//	{
//		GetKey();  // �õ�����Key��ֵ
//
//		// ���䰴������
//
//		DSP28x_usDelay(100000);  // ���μ��֮����0.1s������ϣ�����׼ȷ��⣬����ִ�������������ӳ١�
//	}
//}


/*********************************************��������**************************************************/
// ��ʼ�������LED�Ƶ�����Ϊ�������  52,60����1��52��61����2 52��54����3��52��63����4


/*******************************����IO��ʼ��***********************************/
void InitKeyGpio(void)
{

	EALLOW;

	// ��ɨ�� ����

	// KY4: GPIO59
	GpioCtrlRegs.GPBPUD.bit.GPIO63  = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPBSET.bit.GPIO63  = 1;   									// Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO63  = 1;   									// GPIO11 = output

	//KY3: GPIO58
	GpioCtrlRegs.GPEPUD.bit.GPIO130  = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPESET.bit.GPIO130  = 1;   									// Load output latch
    GpioCtrlRegs.GPEMUX1.bit.GPIO130 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPEDIR.bit.GPIO130  = 1;   									// GPIO11 = output

	//KY2: GPIO53
	GpioCtrlRegs.GPEPUD.bit.GPIO131  = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPESET.bit.GPIO131  = 1;   									// Load output latch
    GpioCtrlRegs.GPEMUX1.bit.GPIO131 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPEDIR.bit.GPIO131  = 1;   									// GPIO11 = output

	//KY1: GPIO52
    GpioCtrlRegs.GPCPUD.bit.GPIO66  = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPCSET.bit.GPIO66  = 1;   									// Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO66  = 1;   									// GPIO11 = output


	//////////////////////////////�����ĸ�IO������Ϊ���룬��Ϊ��ɨ��////////////////////////////

	// KX4: GPIO60
    GpioCtrlRegs.GPCPUD.bit.GPIO64  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPCDIR.bit.GPIO64  = 0;   									// IO�ڷ���Ϊ����

	// KX3: GPIO61
    GpioCtrlRegs.GPAPUD.bit.GPIO26  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPADIR.bit.GPIO26  = 0;   									// IO�ڷ���Ϊ����

 	// KX2: GPIO62
    GpioCtrlRegs.GPAPUD.bit.GPIO27  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPADIR.bit.GPIO27  = 0;   									// IO�ڷ���Ϊ����

	// KX1: GPIO63
    GpioCtrlRegs.GPAPUD.bit.GPIO25   = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO25  = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPADIR.bit.GPIO25   = 0;   									// IO�ڷ���Ϊ����

    EDIS;

    // ����������0���ȴ���������
    RST_KY4;
    RST_KY3;
    RST_KY2;
    RST_KY1;
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
	RST_KY4	;
	RST_KY3	;
	RST_KY2	;
	RST_KY1	;
}
/*****************************************************************************************************/

/****************************************��ȡ4��IO��ƽ״̬************************************************/
void GetKX(void)
{
	Uint16 x;           // �����
	Uint16 temp[5];     // temp[0]��ʹ��

	//��ȡ�������ߵ�ƽֵ���޼�����ʱKX_Status[1~3]��Ϊ1
	// Ϊ�˰���������������Ρ�
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
	KX_Status[4] = KX4_STATUS;

	for (x=1;x<5;x++)
	{
		temp[x]=KX_Status[x];
	}
	//delay(2000);	 // �Ե�
	//Delay_ms(100);

	//Ϊ���������ٴζ�ȡ�������ߵ�ƽֵ
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
	KX_Status[4] = KX4_STATUS;

	for (x=1;x<5;x++)
	{
		if (temp[x]!=KX_Status[x]) // ���μ�ⲻͬ����δ���´�����ΪH
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

	//	�ӵ�1~4�������ж��Ƿ��м�����
	for (x=1;x<5;x++)
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
	if(y==4){SET_KY4;}
}
/*****************************************************************************************************/

/*******************************ָ��������͵�ƽ**********************************************/
void Rst_KY(Uint16 y)
{
	if(y==1){RST_KY1;}
	if(y==2){RST_KY2;}
	if(y==3){RST_KY3;}
	if(y==4){RST_KY4;}
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
	    for (y=1;y<5;y++) //���м��
	    {
			Set_KY(y);     // ����xָ������ΪH
			delay(3000);	   // ��һ�ȣ���ָ��������H������

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
	Key=0;
	KX_On=0;
	KY_On=0;
	static a=0;
	RstAllKY();   // ������������ΪL
	GetKX_On();   // �õ��������߱�ţ�����KX_On��

	if (KX_On!=0)    // �м�����
	{
		GetKY_On();   // �õ��������߱�ţ�����KY_On��
		Key = Keys[KX_On-1][KY_On-1];   // ��ѯ����ֵ
		a=Key;
	}

}

//extern unsigned char wavenum;
//extern float freq;
//extern float volt;
//extern char MODIFY_FLAG;
//extern char MENU_FLAG;
//extern Uint16 DataRenewFlag;

//extern char INDEX;                //data_array����
//extern uchar block_array[];

//unsigned int RndIndex = 0;   // ��������������
//float data_array[33] ={0};         //32�����ݵ�
//float input_data;
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
Uint16 case1clear=0;
Uint16 case2clear=0;
Uint16 Mune_Flag=0;
Uint16 numcount=0;
Uint16 NumsCount=0;
float data_array[];
void Key_Process()
{
    if(Mune_Flag==0){
        TestLcd2();
        switch(Key)
        {
        case 1: Mune_Flag=1;
        case 2: Mune_Flag=2;
        }
    }

    if(Mune_Flag==1)
    {
//        Uint16 temp =0;
//        for (temp=0;temp<32;temp++)
 //       {
//            data_array[NumsCount]=rand()*3.3;
//            NumsCount++;
//        }
        TestLcd3();

/*        if(NumsCount==31)
        {
            if(case2clear==0)
            {
                LcdClearAll();
                case2clear++;
            }
            switch(Key)
            {
                case 16:
                {
                    Mune_Flag=0;
                    case2clear=0;
                    NumsCount++;
                    break;
                 }
                default: break;

            }
        }*/
    }
    if(Mune_Flag==2)
    {
        if(case1clear==0)
        {
            LcdClearAll();
            case1clear++;
        }
       // TestLcd();
        switch(Key)
        {
            case 16:
            {
                Mune_Flag=0;
                case1clear=0;
            }
        }
     }
}
/*    switch(Key)
    {
    case 1: numcount=numcount*10+numcount;break;
    case 2: numcount=numcount*10+numcount;break;
    case 4: numcount=numcount*10+numcount;break;
    case 5: numcount=numcount*10+numcount;break;
    case 6: numcount=numcount*10+numcount;break;
    case 8: numcount=numcount*10+numcount;break;
    case 9: numcount=numcount*10+numcount;break;
    case 10: numcount=numcount*10+numcount;break;
    case 12: numcount=numcount*10+numcount;break;
    case 13: break;
    case 14: break;
    case 16: break;

    default: break;

    }
    Key = 0;
}*/


/*void Change_WaveType()
{
	switch(wavenum)
	{
	case 1: wavenum=2; break;
	case 2: wavenum=3; break;
	case 3: wavenum=4; break;
	case 4: wavenum=1; break;
	default: break;
	}
}*/

/*void add_argu()
{
	if(MODIFY_FLAG == 0){
		freq+=100;
		if(freq>=210e3)
		{
			freq=210e3;
		}
	}
	else if(MODIFY_FLAG == 1)
	{
		volt+=0.1;
		if(volt>=4.9)
		{
			volt=4.9;
		}
	}

}
*/
/*void sub_argu()
{
	if(MODIFY_FLAG == 0)
	{
		freq-=100;
		if(freq<=20)
		{
			freq=20;
		}
	}
	else if(MODIFY_FLAG == 1)
	{
		volt-=0.1;
		if(volt<0.2)
		{
			volt=0.2;
		}
	}

}*/

/*void select_argu()
{
	if(MODIFY_FLAG == 0)
	{
		MODIFY_FLAG = 1;
	}
	else
		MODIFY_FLAG = 0;
}*/
//===========================================================================
// No more.
//===========================================================================


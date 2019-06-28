/********************************************************************
* 文件名： 	按键扫描程序
* 描述:  	运行程序，按下按键，观测变量Key的值（该值从从1~16,无键按下时为0)。
*       行线编号：1-4，未按下时编号为0
*       列线编号：1-4，未按下时编号为0
* 调用函数: ReadKey();

* 使用资源：
* 行线KX1~KX4，接入F28335的GPI063~GPI050
* 行线KY1~KY4，接入F28335的GPI052/53/58/59

**********************************************************************/
/********************************************************************
程序说明：按键扫描原理：1、3列对应的IO设置为输出,3行对应的IO设置为输入。
				2、若无按键按下，3行输入IO，均为高电平（因为有外部上拉电阻）
				3、若按键按下，对应行IO被拉低（假设为第X行），检测为低电平。按键所在行（X行），被鉴别出来。
				4、此时，依次改变3列的输出为高，当遇到按键所在列时，第X行电平重新变为高。按键所在列。被鉴别出来。
********************************************************************/

#include "F28x_Project.h"

//#include "LED_M.h"               // LED状态宏定义
//#include "LED.h"
#include "KEY4x4.h"
#include "LCD12864.h"
#include "Delay.h"
#include <math.h>
#include "stdlib.h"


/**************************************宏定义************************************************/
#define SET_KY4			GpioDataRegs.GPBSET.bit.GPIO63 = 1						//Y4拉高
#define SET_KY3			GpioDataRegs.GPESET.bit.GPIO130 = 1    						//Y3拉高
#define SET_KY2			GpioDataRegs.GPESET.bit.GPIO131 = 1						//Y2拉高
#define SET_KY1			GpioDataRegs.GPCSET.bit.GPIO66 = 1						//Y1拉高

#define RST_KY4			GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1					//Y4拉低
#define RST_KY3			GpioDataRegs.GPECLEAR.bit.GPIO130 = 1 					//Y3拉低
#define RST_KY2			GpioDataRegs.GPECLEAR.bit.GPIO131 = 1 						//Y2拉低
#define RST_KY1			GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1						//Y1拉低

#define KX4_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO25							//X4状态
#define KX3_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO27							//X3状态
#define KX2_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO26							//X2状态
#define KX1_STATUS	 	GpioDataRegs.GPCDAT.bit.GPIO64							//X1状态
/*****************************************************************************************************/
// 按键顺序，按列编号：第一列K1-K4，K5-K8，K9-K12，K13-K16
Uint16 Keys[4][4] = {1,5,9,13, 2,6,10,14, 3,7,11,15,  4,8,12,16};//数据表，与16个按键对应

Uint16 Key = 0;												//实时按键信息变量
Uint16 KX_On = 0;   //行线编号：未按下时编号为0,有按下分别为1-4，
Uint16 KY_On = 0;   //列线编号：未按下时编号为0,有按下分别为1-4，
Uint16 KX_Status[5]={0};     // KX_Status[4]--->KX4状态， KX_Status[0]未使用

/*********************************************函数声明************************************************/
void InitKeyGpio(void);  // 初始化按键引脚
void delay(Uint32 t);
void RstAllKY(void);   // 设置所有列线为L电平
void GetKX(void);      // 得到行线状态
void GetKX_On(void);   // 得到按键按下所在行编号
void SetKY(Uint16 y);  // 设置指定列线为H电平
void RstKY(Uint16 y);  // 设置指定列线为L电平
void GetKey(void);     // 得到按下的按键编号，放在Key中

void Key_Process();
void Change_WaveType();
void add_argu();
void sub_argu();
void select_argu();

//void main(void)
//{
//   InitSysCtrl();
//
//   InitKeyGpio(); // 按键引脚初始化
//   //InitLedGpio();
//
//
//   DINT;
//   InitPieCtrl();
//   IER = 0x0000;
//   IFR = 0x0000;
//   InitPieVectTable();
//
//	while(1)	//死循环
//	{
//		GetKey();  // 得到按键Key的值
//
//		// 补充按键功能
//
//		DSP28x_usDelay(100000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
//	}
//}


/*********************************************函数定义**************************************************/
// 初始化输出到LED灯的引脚为输出引脚  52,60数字1；52，61数字2 52，54数字3，52，63数字4


/*******************************按键IO初始化***********************************/
void InitKeyGpio(void)
{

	EALLOW;

	// 列扫描 引脚

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


	//////////////////////////////以下四个IO口设置为输入，作为行扫描////////////////////////////

	// KX4: GPIO60
    GpioCtrlRegs.GPCPUD.bit.GPIO64  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPCDIR.bit.GPIO64  = 0;   									// IO口方向为输入

	// KX3: GPIO61
    GpioCtrlRegs.GPAPUD.bit.GPIO26  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO26  = 0;   									// IO口方向为输入

 	// KX2: GPIO62
    GpioCtrlRegs.GPAPUD.bit.GPIO27  = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO27  = 0;   									// IO口方向为输入

	// KX1: GPIO63
    GpioCtrlRegs.GPAPUD.bit.GPIO25   = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO25  = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO25   = 0;   									// IO口方向为输入

    EDIS;

    // 所有列线置0，等待按键按下
    RST_KY4;
    RST_KY3;
    RST_KY2;
    RST_KY1;
}


/*********************************************延时函数************************************************/
void delay(Uint32 t)
{
	Uint32 i = 0;

	for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

/****************************************4列全部输出低电平************************************************/
void RstAllKY(void)
{
	RST_KY4	;
	RST_KY3	;
	RST_KY2	;
	RST_KY1	;
}
/*****************************************************************************************************/

/****************************************读取4行IO电平状态************************************************/
void GetKX(void)
{
	Uint16 x;           // 行序号
	Uint16 temp[5];     // temp[0]不使用

	//读取所有行线电平值，无键按下时KX_Status[1~3]均为1
	// 为了按键消抖，检测两次。
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
	KX_Status[4] = KX4_STATUS;

	for (x=1;x<5;x++)
	{
		temp[x]=KX_Status[x];
	}
	//delay(2000);	 // 稍等
	//Delay_ms(100);

	//为了消抖，再次读取所有行线电平值
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
	KX_Status[4] = KX4_STATUS;

	for (x=1;x<5;x++)
	{
		if (temp[x]!=KX_Status[x]) // 两次检测不同，按未按下处理，即为H
			KX_Status[x]=1;
	}
}
/*****************************************************************************************************/

/**************************************读取按键行位置**************************************/
// 判断按键所在行，记录行编号1~3在变量KX_On中，若无按键按下，则KX_On=0；
void GetKX_On(void)
{
	Uint16 x;   // 行序号


	GetKX();  // 得到行线状态

	//	从第1~4行依次判断是否有键按下
	for (x=1;x<5;x++)
	{
		if(KX_Status[x] == 0)   // 若指定行线有键按下
		{
			KX_On = x;          // 记录有键按下的行线编号，以最后一个位置
		}
	}
}
/*****************************************************************************************************/

/*******************************指定列输出高电平******************************************/
void Set_KY(Uint16 y)
{
	if(y==1){SET_KY1;}
	if(y==2){SET_KY2;}
	if(y==3){SET_KY3;}
	if(y==4){SET_KY4;}
}
/*****************************************************************************************************/

/*******************************指定列输出低电平**********************************************/
void Rst_KY(Uint16 y)
{
	if(y==1){RST_KY1;}
	if(y==2){RST_KY2;}
	if(y==3){RST_KY3;}
	if(y==4){RST_KY4;}
}
/*****************************************************************************************************/

/*******************************读取按键列位置**********************************************/
// 读有按键按下的列编号KY_On和按键编号Key
void GetKY_On(void)
{
	Uint16 y;       // 列序号

	if (KX_On==0)  // 没有按键按下
	{
		KY_On=0;
	}
	else
	{
	    for (y=1;y<5;y++) //逐列检测
	    {
			Set_KY(y);     // 设置x指定的列为H
			delay(3000);	   // 等一等，让指定列拉至H，消抖

			GetKX();      // 读所有行线值（说明：只需指定的KX_On行即可，这里不单独编写函数）
			if(KX_Status[KX_On])  // 如果指定KX_On行电平为H
			{
				KY_On = y;        // 得到列序号
			}
			Rst_KY(y);            // 恢复L电平

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
	RstAllKY();   // 设置所有列线为L
	GetKX_On();   // 得到按键行线编号，放在KX_On中

	if (KX_On!=0)    // 有键按下
	{
		GetKY_On();   // 得到按键列线编号，放在KY_On中
		Key = Keys[KX_On-1][KY_On-1];   // 查询按键值
		a=Key;
	}

}

//extern unsigned char wavenum;
//extern float freq;
//extern float volt;
//extern char MODIFY_FLAG;
//extern char MENU_FLAG;
//extern Uint16 DataRenewFlag;

//extern char INDEX;                //data_array索引
//extern uchar block_array[];

//unsigned int RndIndex = 0;   // 输入随机数的序号
//float data_array[33] ={0};         //32个数据点
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


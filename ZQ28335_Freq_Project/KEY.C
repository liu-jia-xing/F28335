/********************************************************************
* 文件名：  按键扫描程序
* 描述:   运行程序，按下按键，观测变量Key的值（该值从从1~9,无键按下时为0)。
*       行线编号：1-3，未按下时编号为0
*       列线编号：1-3，未按下时编号为0
* 调用函数: ReadKey();

* 使用资源：
* 行线KX1~KX3，接入F28335的GPI053~GPI051
* 行线KY1~KY3，接入F28335的GPI050~GPI048

**********************************************************************/
/********************************************************************
程序说明：按键扫描原理：1、3列对应的IO设置为输出,3行对应的IO设置为输入。
                2、若无按键按下，3行输入IO，均为高电平（因为有外部上拉电阻）
                3、若按键按下，对应行IO被拉低（假设为第X行），检测为低电平。按键所在行（X行），被鉴别出来。
                4、此时，依次改变3列的输出为高，当遇到按键所在列时，第X行电平重新变为高。按键所在列。被鉴别出来。
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "GUI.h"               // LED状态宏定义
#include "Delay.h"
#include "KEY.h"
#include "LED_M.h"               // LED状态宏定义
#include "LED.h"
/**************************************宏定义************************************************/
#define SET_KY4         GpioDataRegs.GPASET.bit.GPIO23 = 1                      //Y4拉高
#define SET_KY3         GpioDataRegs.GPASET.bit.GPIO24 = 1                      //Y3拉高
#define SET_KY2         GpioDataRegs.GPASET.bit.GPIO25 = 1                      //Y2拉高
#define SET_KY1         GpioDataRegs.GPASET.bit.GPIO26 = 1                      //Y1拉高

#define RST_KY4         GpioDataRegs.GPACLEAR.bit.GPIO23 = 1                        //Y4拉低
#define RST_KY3         GpioDataRegs.GPACLEAR.bit.GPIO24 = 1                    //Y3拉低
#define RST_KY2         GpioDataRegs.GPACLEAR.bit.GPIO25 = 1                    //Y2拉低
#define RST_KY1          GpioDataRegs.GPACLEAR.bit.GPIO26 = 1                    //Y1拉低

#define KX4_STATUS      GpioDataRegs.GPADAT.bit.GPIO27                          //X4状态
#define KX3_STATUS      GpioDataRegs.GPADAT.bit.GPIO28                          //X3状态
#define KX2_STATUS      GpioDataRegs.GPADAT.bit.GPIO29                          //X2状态
#define KX1_STATUS      GpioDataRegs.GPADAT.bit.GPIO30                          //X1状态
/*****************************************************************************************************/

/**************************************变量定义************************************************/
Uint16 Keys[4][4] = {1,2,3,10, 4,5,6,11, 7,8,9,12, 13,0,14,15};//数据表，与16个按键对应
Uint16 Key = 255;                                             //实时按键信息变量
Uint16 KX_On = 0;        //行线编号：未按下时编号为0,有按下分别为1-3，
Uint16 KY_On = 0;        //列线编号：未按下时编号为0,有按下分别为1-3，
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


/*********************************************函数定义**************************************************/

/*******************************按键IO初始化***********************************/
void InitKeyGpio(void)
{

    EALLOW;

    //////////////////////////////以下四个IO口设置为输出，作为列扫描////////////////////////////

    //KY4: 对应PCB端子 KEY_MATRIX：IO23
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO23 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;                                     // GPIO11 = output

    //KY3: 对应PCB端子 KEY_MATRIX：IO24
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO24 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;                                     // GPIO11 = output

    //KY2: 对应PCB端子 KEY_MATRIX：IO25
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO25 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;                                     // GPIO11 = output

    //KY1: 对应PCB端子 KEY_MATRIX：IO26
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO26 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;                                     // GPIO11 = output


    //////////////////////////////以下四个IO口设置为输入，作为行扫描////////////////////////////

    //KX4: 对应PCB端子 KEY_MATRIX：IO27
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;                                    // 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO27  = 0;                                    // IO口方向为输入

    //KX3: 对应PCB端子 KEY_MATRIX：IO28
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;                                    // 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO28  = 0;                                    // IO口方向为输入

    //KX2: 对应PCB端子 KEY_MATRIX：IO29
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;                                    // 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO29  = 0;                                    // IO口方向为输入

    //KX1: 对应PCB端子 KEY_MATRIX：IO30
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;                                     // Enable pullup on GPIO11
    GpioCtrlRegs.GPAMUX2.bit.GPIO30  = 0;                                   // 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO30   = 0;                                       // IO口方向为输入

    EDIS;
    RstAllKY();
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
    RST_KY4 ;
    RST_KY3 ;
    RST_KY2 ;
    RST_KY1 ;
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
    delay(200);  // 稍等

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

    //  从第1~3行依次判断是否有键按下
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
            delay(2000);       // 等一等，让指定列拉至H，消抖

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
    Key=255;
    KX_On=0;
    KY_On=0;

    RstAllKY();   // 设置所有列线为L
    GetKX_On();   // 得到按键行线编号，放在KX_On中

    if (KX_On!=0)    // 有键按下
    {
        GetKY_On();   // 得到按键列线编号，放在KY_On中
        Key = Keys[KX_On-1][KY_On-1];   // 查询按键值
    }

}
/****************************************************************************/
//按键处理模块部分

unsigned char MenuFlag = 0;         //菜单状态标志位
unsigned char SubSel_Flag = 0;      //子菜单选择标志

unsigned char Sub_Flag1=0;          //子菜单1数值输入切换标志位
unsigned char Sub_Flag2=0;          //子菜单2数值输入切换标志位
unsigned char Sub_Flag3=0;          //子菜单3数值输入切换标志位
unsigned char Sub_Flag4=0;          //子菜单4数值输入切换标志位
unsigned char Sub_position1=0;
unsigned char Sub_position2=0;
unsigned char Sub_position4=0;
unsigned char Sub_position3=0;
//点频测试数据
typedef struct Val
{
    unsigned long frequency_1;
    unsigned long frequency_2;
    unsigned int phase_1;
    unsigned int phase_2;
}val_dat;
unsigned int Val1=0;
typedef struct Val2
{
    unsigned long frequency_MAX;
    unsigned long frequency_MIN;
    unsigned long step;
}val2;
val2 SubSel3;
val2 SubSel4;
val_dat SubSel1;

void data_init()
{
    SubSel1.frequency_1=0;    //I路频率，相位
    SubSel1.frequency_2=0;
    SubSel1.phase_1=0;        //Q路频率，相位
    SubSel1.phase_2=0;
    SubSel3.frequency_MAX=0;
    SubSel3.frequency_MIN=0;
    SubSel3.step=0;
    SubSel4.frequency_MAX=0;
    SubSel4.frequency_MIN=0;
    SubSel4.step=0;
}
void Key0Fun(void)
{
    if(MenuFlag==1)
        {
            if(SubSel_Flag==1)
            {
                switch(Sub_Flag1%4)
                {
                case 1:
                    Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"0");
                    //数据累加
                    SubSel1.frequency_1*=10;
                    SubSel1.frequency_1+=1;
                    break;
                case 2:
                    Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"0");
                    //移位相加
                    SubSel1.phase_1*=10;
                    SubSel1.phase_1+=1;
                    break;
                case 3:
                    Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"0");
                    SubSel1.frequency_2*=10;
                    SubSel1.frequency_2+=1;
                    break;
                case 0:
                    Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"0");
                    if(Sub_position1<=2)
                    {
                        SubSel1.phase_2*=10;
                        SubSel1.phase_2+=1;
                    }
                    break;
                default:break;
                }

                //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
                if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                    if(Sub_position1<2)
                        Sub_position1++;}
                else
                {
                    if(Sub_position1<=9)
                        Sub_position1++;
                }
            }
            //子菜单二操作
            if(SubSel_Flag==2)
            {
                Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"0");
                if(Sub_position2<9)
                {
                    Val1*=10;
                    Val1+=1;
                    Sub_position2++;
                }
            }
            if(SubSel_Flag==3)
            {
                switch(Sub_Flag3%3)
                {
                case 1:
                    Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"0");
                    SubSel3.frequency_MAX*=10;
                    SubSel3.frequency_MAX+=1;
                    break;
                case 2:
                    Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"0");
                    SubSel3.frequency_MIN*=10;
                    SubSel3.frequency_MIN+=1;
                    break;
                case 0:
                    Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"0");
                    SubSel3.step*=10;
                    SubSel3.step+=1;
                    break;
                default:break;
                }
                if(Sub_position3<9)
                    Sub_position3++;
            }
            if(SubSel_Flag==4)
            {
                switch(Sub_Flag4%3)
                {
                case 1:Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"0");break;
                case 2:Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"0");break;
                case 0:
                    Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"0");
                    SubSel3.step*=10;
                    SubSel3.step+=1;
                    break;
                default:break;
                }
                if(Sub_position4<9)
                    Sub_position4++;
            }

        }
}
void Key1Fun(void)
{
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"1");
                //数据累加
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=1;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"1");
                //移位相加
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=1;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"1");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=1;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"1");
                if(Sub_position1<=2)
                {
                    SubSel1.phase_2*=10;
                    SubSel1.phase_2+=1;
                }
                break;
            default:break;
            }

            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        //子菜单二操作
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"1");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=1;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"1");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=1;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"1");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=1;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"1");
                SubSel3.step*=10;
                SubSel3.step+=1;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"1");break;
            case 2:Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"1");break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"1");
                SubSel3.step*=10;
                SubSel3.step+=1;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }

    }
}
void Key2Fun(void)
{
    LED2_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"2");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=2;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"2");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=2;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"2");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=2;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"2");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=2;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"2");
            if(Sub_position2<9){
                Val1*=10;
                Val1+=2;
                Sub_position2++;}
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"2");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=2;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"2");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=2;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"2");
                SubSel3.step*=10;
                SubSel3.step+=2;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"2");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=2;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"2");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=2;
                break;
            case 0:Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"2");
                SubSel4.step*=10;
                SubSel4.step+=2;
                break;

            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}
void Key3Fun(void)
{
    LED2_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"3");
                if(Sub_position1<9)
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=3;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"3");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=3;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"3");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=3;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"3");
                if(Sub_position1<2)
                {
                    SubSel1.phase_2*=10;
                    SubSel1.phase_2+=3;
                }
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2)
            {
                if(Sub_position1<2)
                    Sub_position1++;
            }
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"3");
            if(Sub_position2<9){
                Val1*=10;
                Val1+=3;
                Sub_position2++;}
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"3");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=3;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"3");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=3;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"3");
                SubSel3.step*=10;
                SubSel3.step+=3;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"3");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=3;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"3");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=3;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"3");
                SubSel4.step*=10;
                SubSel4.step+=3;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}

void Key4Fun(void)
{
    LED3_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"4");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=4;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"4");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=4;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"4");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=4;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"4");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=4;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"4");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=2;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"4");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=4;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"4");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=4;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"4");
                SubSel3.step*=10;
                SubSel3.step+=4;
                break;
            default:break;
            }
            if(Sub_position3<9)
            {
                Val1*=10;
                Val1+=4;
                Sub_position3++;
            }
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"4");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=1;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"4");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=4;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"4");
                SubSel4.step*=10;
                SubSel4.step+=4;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}

void Key5Fun(void)
{
    LED3_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"5");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=5;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"5");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=5;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"5");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=5;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"5");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=5;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"5");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=5;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"5");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=1;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"5");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=5;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"5");
                SubSel3.step*=10;
                SubSel3.step+=5;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"5");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=5;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"5");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=5;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"5");
                SubSel4.step*=10;
                SubSel4.step+=5;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }


}
void Key6Fun(void)
{
    LED4_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"6");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=6;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"6");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=6;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"6");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=6;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"6");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=6;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"6");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=6;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"6");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=6;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"6");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=6;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"6");
                SubSel3.step*=10;
                SubSel3.step+=6;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"6");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=6;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"6");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=6;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"6");
                SubSel4.step*=10;
                SubSel4.step+=6;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}
void Key7Fun(void)
{
    LED4_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"7");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=7;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"7");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=7;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"7");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=7;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"7");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=7;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"7");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=7;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"7");
                SubSel3.frequency_MAX*=10;
                                SubSel3.frequency_MAX+=7;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"7");
                SubSel3.frequency_MIN*=10;
                               SubSel3.frequency_MIN+=7;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"7");
                SubSel3.step*=10;
                SubSel3.step+=7;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"7");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=7;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"7");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=7;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"7");
                SubSel4.step*=10;
                SubSel4.step+=7;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}
void Key8Fun(void)
{
    LED5_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"8");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=8;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"8");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=8;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"8");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=8;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"8");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=8;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"8");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=8;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"8");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=1;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"8");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=8;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"8");
                SubSel3.step*=10;
                SubSel3.step+=8;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"8");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=8;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"8");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=8;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"8");
                SubSel4.step*=10;
                SubSel4.step+=8;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}
void Key9Fun(void)
{
    LED5_ON;
    if(MenuFlag==1)
    {
        if(SubSel_Flag==1)
        {
            switch(Sub_Flag1%4)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position1*8,67,BLACK,GRAY0,"9");
                SubSel1.frequency_1*=10;
                SubSel1.frequency_1+=9;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position1*8,107,BLACK,GRAY0,"9");
                SubSel1.phase_1*=10;
                SubSel1.phase_1+=8;
                break;
            case 3:
                Gui_DrawFont_GBK16(142+Sub_position1*8,147,BLACK,GRAY0,"9");
                SubSel1.frequency_2*=10;
                SubSel1.frequency_2+=9;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position1*8,187,BLACK,GRAY0,"9");
                SubSel1.phase_2*=10;
                SubSel1.phase_2+=9;
                break;
            default:break;
            }
            //用以判断各个输入项的长度  频率输入长度是9 相位的长度是3
            if(Sub_Flag1%4==0||Sub_Flag1%4==2){
                if(Sub_position1<2)
                    Sub_position1++;}
            else
            {
                if(Sub_position1<=9)
                    Sub_position1++;
            }
        }
        if(SubSel_Flag==2)
        {
            Gui_DrawFont_GBK16(142+Sub_position2*8,87,BLACK,GRAY0,"9");
            if(Sub_position2<9)
            {
                Val1*=10;
                Val1+=9;
                Sub_position2++;
            }
        }
        if(SubSel_Flag==3)
        {
            switch(Sub_Flag3%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position3*8,47,BLACK,GRAY0,"9");
                SubSel3.frequency_MAX*=10;
                SubSel3.frequency_MAX+=9;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position3*8,87,BLACK,GRAY0,"9");
                SubSel3.frequency_MIN*=10;
                SubSel3.frequency_MIN+=9;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position3*8,167,BLACK,GRAY0,"9");
                SubSel3.step*=10;
                SubSel3.step+=9;
                break;
            default:break;
            }
            if(Sub_position3<9)
                Sub_position3++;
        }
        if(SubSel_Flag==4)
        {
            switch(Sub_Flag4%3)
            {
            case 1:
                Gui_DrawFont_GBK16(142+Sub_position4*8,47,BLACK,GRAY0,"9");
                SubSel4.frequency_MAX*=10;
                SubSel4.frequency_MAX+=9;
                break;
            case 2:
                Gui_DrawFont_GBK16(142+Sub_position4*8,87,BLACK,GRAY0,"9");
                SubSel4.frequency_MIN*=10;
                SubSel4.frequency_MIN+=9;
                break;
            case 0:
                Gui_DrawFont_GBK16(142+Sub_position4*8,167,BLACK,GRAY0,"9");
                SubSel3.step*=10;
                SubSel3.step+=9;
                break;
            default:break;
            }
            if(Sub_position4<9)
                Sub_position4++;
        }
    }
}

void Key10Fun(void)
{
    Sub_position1=0;    //清除输入框的起始位置
    Sub_position4=0;
    Sub_position2=0;
    Sub_position3=0;
    LED6_ON;
    //主菜单功能选择切换  SubSle_Flag可表示子功能菜单项目
    if(MenuFlag==0)
    {
        SubSel_Flag%=4;
        SubSel_Flag++;     //SubSle_Flag在1,2,3,4中切换
        switch(SubSel_Flag)
        {
        case 1: ClearButtonUp(79,166,177,192,GRAY0);    //   清除4
                DisplayButtonUp(79,46,177,72);          //   显示1
                break;
        case 2: ClearButtonUp(79,46,177,72,GRAY0);      //   清除1
                DisplayButtonUp(79,86,177,112);         //   显示2
                break;
        case 3: ClearButtonUp(79,86,177,112,GRAY0);     //   清除2
                DisplayButtonUp(79,126,177,152);        //   显示3
                break;
        case 4: ClearButtonUp(79,126,177,152,GRAY0);    //   清除3
                DisplayButtonUp(79,166,177,192);        //   显示4
                break;
        }
    }
    //进入子菜单1 进行数值输入选择切换
    if((MenuFlag==1)&&(SubSel_Flag==1))
    {
        Sub_Flag1%=4;
        Sub_Flag1++;
        switch(Sub_Flag1)
        {
        case 1: ClearButtonUp(139,179,281,205,GRAY0);    //清除4
                DisplayButtonUp(139,59,281,85);          //显示1
                break;
        case 2: ClearButtonUp(139,59,281,85,GRAY0);      //清除1
                DisplayButtonUp(139,99,281,125);         //显示2
                break;
        case 3: ClearButtonUp(139,99,281,125,GRAY0);      //清除2
                DisplayButtonUp(139,139,281,165);         //显示3
                break;
        case 4: ClearButtonUp(139,139,281,165,GRAY0);     //清除3
                DisplayButtonUp(139,179,281,205);          //显示4
                break;
        }
    }
    //进入子菜单2 进行数值输入选择切换
        if((MenuFlag==1)&&(SubSel_Flag==2))
        {
            Sub_Flag2%=3;
            Sub_Flag2++;
            switch(Sub_Flag2)
            {
            case 1: ClearButtonUp(139,199,281,225,GRAY0);    //清除4
                    DisplayButtonUp(139,79,281,105); //x1,y1,x2,y2
            break;
            case 2: ClearButtonUp(139,79,281,105,GRAY0);      //清除1
                    DisplayButtonUp(139,159,281,185); //x1,y1,x2,y2
            break;
            case 3: ClearButtonUp(139,159,281,185,GRAY0);      //清除2
                    DisplayButtonUp(139,199,281,225); //x1,y1,x2,y2
            break;
            }
        }
    //进入子菜单3 进行数值输入选择切换
    if((MenuFlag==1)&&(SubSel_Flag==3))
    {
        Sub_Flag3%=3;
        Sub_Flag3++;
        switch(Sub_Flag3)
        {
        case 1: ClearButtonUp(139,159,281,185,GRAY0);    //清除3
                DisplayButtonUp(139,39,281,65);           //显示1
                break;
        case 2: ClearButtonUp(139,39,281,65,GRAY0);        //清除1
                DisplayButtonUp(139,79,281,105);          //显示2
                break;
        case 3: ClearButtonUp(139,79,281,105,GRAY0);      //清除2
                DisplayButtonUp(139,159,281,185);        //显示3
                break;
        }
    }
    //进入子菜单4 进行数值输入选择切换
    if((MenuFlag==1)&&(SubSel_Flag==4))
    {
        Sub_Flag4%=3;
        Sub_Flag4++;
        switch(Sub_Flag4)
        {
        case 1: ClearButtonUp(139,159,281,185,GRAY0);    //清除3
                DisplayButtonUp(139,39,281,65);           //显示1
                break;
        case 2: ClearButtonUp(139,39,281,65,GRAY0);        //清除1
                DisplayButtonUp(139,79,281,105);          //显示2
                break;
        case 3: ClearButtonUp(139,79,281,105,GRAY0);      //清除2
                DisplayButtonUp(139,159,281,185);        //显示3
                break;
        }
    }
}
void Key11Fun(void)
{
    LED6_ON;
    Sub_position1=0;    //清除输入框的起始位置
    Sub_position4=0;
    Sub_position3=0;
    Sub_position2=0;
    if(MenuFlag==0)
    {
        SubSel_Flag--;
        if(SubSel_Flag==0||SubSel_Flag>5)
        {
            SubSel_Flag=4;
        }
        switch(SubSel_Flag)
        {
        case 1: ClearButtonUp(79,86,177,112,GRAY0);
                DisplayButtonUp(79,46,177,72);
                break;
        case 2: ClearButtonUp(79,126,177,152,GRAY0);
                DisplayButtonUp(79,86,177,112);
                break;
        case 3: ClearButtonUp(79,166,177,192,GRAY0);
                DisplayButtonUp(79,126,177,152);
                break;
        case 4: ClearButtonUp(79,46,177,72,GRAY0);
                DisplayButtonUp(79,166,177,192);
                break;
        }
    }
    if((MenuFlag==1)&&(SubSel_Flag==1))
    {
        Sub_Flag1--;
        if(Sub_Flag1==0||Sub_Flag1>5)
        {
            Sub_Flag1=4;
        }
        switch(Sub_Flag1)
        {
        case 1: ClearButtonUp(139,99,281,125,GRAY0);     //清除2
                DisplayButtonUp(139,59,281,85);          //显示1
                break;
        case 2: ClearButtonUp(139,139,281,165,GRAY0);    //清除3
                DisplayButtonUp(139,99,281,125);         //显示2
                break;
        case 3: ClearButtonUp(139,179,281,205,GRAY0);    //清除4
                DisplayButtonUp(139,139,281,165);        //显示3
                break;
        case 4: ClearButtonUp(139,59,281,85,GRAY0);     //清除1
                DisplayButtonUp(139,179,281,205);       //显示4
                break;
        }
    }

    //菜单2切换输入框
    if((MenuFlag==1)&&(SubSel_Flag==2))
        {
            Sub_Flag2--;
            if(Sub_Flag2==0||Sub_Flag2>5)
            {
                Sub_Flag2=3;
            }
            switch(Sub_Flag2)
            {
            case 1: ClearButtonUp(139,159,281,185,GRAY0);     //清除2
                    DisplayButtonUp(139,79,281,105); //x1,y1,x2,y2
                    break;
            case 2: ClearButtonUp(139,199,281,225,GRAY0);    //清除3
                    DisplayButtonUp(139,159,281,185); //x1,y1,x2,y2
                    break;
            case 3: ClearButtonUp(139,79,281,105,GRAY0);    //清除4
                    DisplayButtonUp(139,199,281,225); //x1,y1,x2,y2
                    break;
            }
        }
    //进入子菜单3 进行数值输入选择切换
    if((MenuFlag==1)&&(SubSel_Flag==3))
    {
        Sub_Flag3--;
        if(Sub_Flag3==0||Sub_Flag3>3)
        {
            Sub_Flag3=3;
        }
        switch(Sub_Flag3)
        {
        case 1: ClearButtonUp(139,79,281,105,GRAY0);      //清除2
                DisplayButtonUp(139,39,281,65);           //显示1
                break;
        case 2: ClearButtonUp(139,159,281,185,GRAY0);    //清除3
                DisplayButtonUp(139,79,281,105);          //显示2
                break;
        case 3: ClearButtonUp(139,39,281,65,GRAY0);        //清除1
                DisplayButtonUp(139,159,281,185);        //显示3
                break;
        }
    }
    //进入子菜单4 进行数值输入选择切换
    if((MenuFlag==1)&&(SubSel_Flag==4))
    {
        Sub_Flag4--;
        if(Sub_Flag4==0||Sub_Flag4>3)
        {
            Sub_Flag4=3;
        }
        switch(Sub_Flag4)
        {
        case 1: ClearButtonUp(139,79,281,105,GRAY0);      //清除2
                DisplayButtonUp(139,39,281,65);           //显示1
                break;
        case 2: ClearButtonUp(139,159,281,185,GRAY0);    //清除3
                DisplayButtonUp(139,79,281,105);          //显示2
                break;
        case 3: ClearButtonUp(139,39,281,65,GRAY0);        //清除1
                DisplayButtonUp(139,159,281,185);        //显示3
                break;
        }
    }
}
void Key12Fun(void)
{
    LED7_ON;
}
void Key13Fun(void)
{
    LED7_ON;
    if(MenuFlag==1)
    {
        Draw_Mainmenu();
        SubSel_Flag=0;
        Sub_Flag1=0;
        Sub_Flag2=0;
        Sub_Flag3=0;
        Sub_Flag4=0;
    }
    MenuFlag=0;
}
void Key14Fun(void)
{
    LED8_ON;
}
void Key15Fun(void)
{
    LED8_ON;
    if(MenuFlag==0)
    {
        switch(SubSel_Flag)
        {
        case 1: Draw_Submenu1();break;
        case 2: Draw_Submenu2();break;
        case 3: Draw_Submenu3();break;
        case 4: Draw_Submenu4();break;
        }
        MenuFlag=1;
    }
}

unsigned char ifpressed=0;
void KeyFun(void)
{
    switch(Key)
    {
    case 0: if(!ifpressed)Key0Fun();ifpressed=1;break;
    case 1: if(!ifpressed)Key1Fun();ifpressed=1;break;
    case 2: if(!ifpressed)Key2Fun();ifpressed=1;break;
    case 3: if(!ifpressed)Key3Fun();ifpressed=1;break;
    case 4: if(!ifpressed)Key4Fun();ifpressed=1;break;
    case 5: if(!ifpressed)Key5Fun();ifpressed=1;break;
    case 6: if(!ifpressed)Key6Fun();ifpressed=1;break;
    case 7: if(!ifpressed)Key7Fun();ifpressed=1;break;
    case 8: if(!ifpressed)Key8Fun();ifpressed=1;break;
    case 9: if(!ifpressed)Key9Fun();ifpressed=1;break;
    case 10: if(!ifpressed)Key10Fun();ifpressed=1;break;
    case 11: if(!ifpressed)Key11Fun();ifpressed=1;break;
    case 12: if(!ifpressed)Key12Fun();ifpressed=1;break;
    case 13: if(!ifpressed)Key13Fun();ifpressed=1;break;
    case 14: if(!ifpressed)Key14Fun();ifpressed=1;break;
    case 15: if(!ifpressed)Key15Fun();ifpressed=1;break;
    default:
        ifpressed=0;
        LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
        LED5_OFF; LED6_OFF; LED7_OFF; LED8_OFF;
        break;
    }
}


//===========================================================================
// No more.
//===========================================================================


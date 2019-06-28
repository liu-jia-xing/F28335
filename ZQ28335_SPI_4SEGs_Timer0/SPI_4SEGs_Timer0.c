/********************************************************************
* 功能：数码管显示程序,定时器刷新，可显示数字、字符及混合形式
*
* 特别说明：
*    函数InitSpiaGpio()默认使用GPIO16-19脚，但本板使用GPIO54-57，需修改。
*    修改自带的函数，不要直接修改代码，修改后的函数也不要放在common文件夹中
*    本函数换成这个函数名，InitSpia54to57Gpio()，放在本文件中
*
* 描述：
* （1）设置数码管刷新频率：tintfreq，单位：Hz，每次刷新一位数码管
*     float tintfreq=200;          // 定时中断频率，Hz
* （2）每个字符的显示时间在CpuTimer0.InterruptCount中控制，默认为1秒（刷新tintfreq次）
*     CpuTimer0.InterruptCount==(Uint16)tintfreq
* （3）需显示的字符放在变量DispSym，顺序与视觉一致
*     unsigned char DispSym[]={"Aa19"};   // A~F,a~f,及0~9混合显示
* （4）函数GetSymCode查询DispSym的段码，默认为1秒更新一次；
* （5）函数GetNumCode查询DispData的段码，默认为1秒更新一次；
*
*硬件电路：SPI接口使用GPIO54-57,连接串并转换芯片74CH164,
*       GPIO54接SIMO,GPIO56接CLK，其他未使用
*       GPIO16-17接SEG1-2,GPIO62-63接SEG3-4;SEG1为高位
*控制方式：
*       定时刷新
*
*  Created on: 2016-11-19
*      Author: 九江学院电子工程学院  江金龙
**********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


/********************************宏定义数码管位选 IO 接口*******************************************/
//数据第4位，SEG1（最高位）
#define  SET_BIT4	GpioDataRegs.GPASET.bit.GPIO16	 = 1 		//与外设板 8_LEDS 端子的 IO16 对应
#define  RST_BIT4	GpioDataRegs.GPACLEAR.bit.GPIO16 = 1		//与外设板 8_LEDS 端子的 IO16对应

//数据第3位，SEG2
#define  SET_BIT3   GpioDataRegs.GPASET.bit.GPIO17	 = 1		//与外设板 8_LEDS 端子的 IO17 对应
#define  RST_BIT3	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1		//与外设板 8_LEDS 端子的 IO17 对应

//数据第2位，SEG3
#define  SET_BIT2   GpioDataRegs.GPBSET.bit.GPIO62	 = 1		//与外设板 8_LEDS 端子的 IO62 对应
#define  RST_BIT2	GpioDataRegs. GPBCLEAR.bit.GPIO62 = 1		//与外设板 8_LEDS 端子的 IO62 对应

//数据第1位，SEG4（最低位）
#define  SET_BIT1   GpioDataRegs.GPBSET.bit.GPIO63	 = 1		//与外设板 8_LEDS 端子的 IO63 对应
#define  RST_BIT1	GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1		//与外设板 8_LEDS 端子的 IO63 对应
/*****************************************************************************************************/


/*********************************************函数申明************************************************/
void InitSpia54to57Gpio(void);  //SPI引脚初始化
void Init4SEGsGpio(void);      // 设置SEG1~4片选引脚初始化为L(H有效显示)
void InitSpiFun(void);           // SPI功能初始化
void InitSpiFifo(void);      // SPI的FIFO功能初始化，使能FIFO发送功能，接收功能未开启

void DispDataSplit(Uint16 data);// 4位数拆分为4个数，保存到数组DispDataBit中，[0]为个位
void SellectSeg(Uint16 i);     // 数码管位选函数，i为0~3，对应SEG1~4
void SpiXmitData(Uint16 a);        // 将数据a发送到SPITXBUF

interrupt void cpu_timer0_isr(void);

/*****************************************************************************************************/

/************************************定义相关变量*********************************************/
unsigned char msg[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//共阳段码：0~9
unsigned char msg2_ABC[6]={0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e};	//共阳段码：A~F
unsigned char msg2_abc[6]={0xa0, 0x83, 0xa7, 0xa1, 0x84, 0x8e};	//共阳段码：a~f
unsigned char msg3_N[1]={0xbf};	//共阳段码：负号
unsigned char Syms[6][4]= {"aaaa","bbbb","cccc","dddd","eeee","ffff"};
unsigned char Syms2[6][4]= {"AAAA","BBBB","CCCC","DDDD","EEEE","FFFF"};

//unsigned char msg[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	//共阳段码：0~9
//unsigned char msg2_ABC[6]={0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71};	//共阴段码：A~F
//unsigned char msg2_abc[6]={0x5F, 0x83, 0x58, 0xa1, 0x7b, 0x8e};	//共阴段码：a~f
// 字符0~9的编码为：0x30~0x39;  字符A~Z的编码为：0x41~0x5A;  字符a~z的编码为：0x61~0x7A;
unsigned char DispSym[]={"Aa19"};   // A~F,a~f,及0~9混合显示

unsigned int DispDataBit[4] = {0};											//存放拆分后的四位数字
Uint16 DispData = 0;															//显示的数字
int16  Nnum =-3;
//Uint16 Loop = 0;															//循环扫描变量
//Uint16 loop = 0;

/*****************************************************************************************************/


float tintfreq=200;       // 定时中断频率，Hz
float tintprd=0;          // 定时中断周期，us

static Uint16 Loop_8SEG=0; // 8段数码管专用序号
Uint16 Disp_8SEG_Code[4]={0xC0,0xC0,0xC0,0xC0};   // 8位数码管显示的字段编码,初始化为4个0


// 初始化SPIA模块引脚，SPISIMOA、SPISOMIA、SPICLKA和SPISTEA分别在GPIO16~19或GPIO54~57
void InitSpia54to57Gpio()
{

   EALLOW;
//    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)


    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.  (SPI)
// Comment out other unwanted lines.
// 每个引脚分4个模式：0：仅与SYSCLK同步；1:    3个采样周期；2： 6个采样周期，3：不同步

//    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO19 (SPISTEA)


/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

//    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1; // Configure GPIO57 as SPISTEA

    EDIS;
}

/******************************数码管位选 IO 接口初始化*******************************************/
// 设置SEG1~4为I/O输出，并初始化为L
void Init4SEGsGpio(void)
{  
    EALLOW;
	
	//GpioCtrlRegs.GPBPUD.bit.GPIO16= 0;   	// Enable pullup on GPIO16
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;  	// GPIO16 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;   	// GPIO16 = output
    GpioDataRegs.GPASET.bit.GPIO16 = 1;   	// Load output latch

	//GpioCtrlRegs.GPBPUD.bit.GPIO59= 0;   	// Enable pullup on GPIO17
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;  	// GPIO17 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;   	// GPIO17 = output
    GpioDataRegs.GPASET.bit.GPIO17 = 1;   	// Load output latch
    
	//GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;   	// Enable pullup on GPIO62
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;  	// GPIO62 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;   	// GPIO62 = output
    GpioDataRegs.GPBSET.bit.GPIO62 = 1;   	// Load output latch
    
    //GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;   	// Enable pullup on GPIO63
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;  	// GPIO63 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;   	// GPIO63 = output
    GpioDataRegs.GPBSET.bit.GPIO63 = 1;   	// Load output latch

    EDIS;  
    
    RST_BIT1;
    RST_BIT2;
    RST_BIT3;
    RST_BIT4;   
}
/*****************************************************************************************************/

/*********************************************Spi初始化************************************************/
// spi模块功能初始化，
// SPICCR.all =0x004F;数据下降沿输出、上升沿输入，数据长度16位
// SPICTL.all =0x0006; 主机模式，时钟相位正常，使能4个SPI引脚
// SPIBRR =0x007F;设置波特率=LSPCLK/128=150M/4/128=292K(好像是比特率)
// SPICCR.all =0x00DF;环路模式，方便测试
void InitSpiFun(void)
{
	SpiaRegs.SPICCR.all =0x004F;	    // Reset on, Falling Edge out, 16-bit char bits
	                                    //0x000F对应Rising Edge，0x004F对应Falling Edge
	SpiaRegs.SPICTL.all =0x0006;    	// Enable master mode, normal phase,
                                        // enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR =0x007F;            // 波特率寄存器：波特率=LSPCLK/(SPIBRR+1)，
	                                    // 最大为LSPCLK/4，最小为LSPCLK/128（0x7F）
    SpiaRegs.SPICCR.all =0x00DF;		// Relinquish SPI from Reset(环路模式)
    SpiaRegs.SPIPRI.bit.FREE = 1;       // Set so breakpoints don't disturb xmission（自由运行）
}
/*****************************************************************************************************/

/****************************************Spi模块FIFO设置**********************************************/

// 使能FIFO发送功能，接收功能未开启
void InitSpiFifo()
{
	// Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}


/******************************数码管位选函数（从低位到高位扫描）***************************************************/
// 数码管位选函数，BIT1 对应最右边的一位
void SellectSeg(Uint16 i)
{
	switch(i)
	{
		case 0:
			SET_BIT1;	//选通数码管最低位
			RST_BIT2;	//关断数码管其他位
			RST_BIT3;
			RST_BIT4;
			break;

		case 1:
			RST_BIT1;
			SET_BIT2;  //选通数码管第二位
			RST_BIT3;
			RST_BIT4;
			break;

		case 2:
			RST_BIT1;
			RST_BIT2;
			SET_BIT3;  //选通数码管第三位
			RST_BIT4;
			break;

		case 3:
			RST_BIT1;
			RST_BIT2;
			RST_BIT3;
			SET_BIT4;  //选通数码管最高位
			break;

		default:
			break;
	}
}
/*****************************************************************************************************/

/************************** 拆分要显示的四位数保存到数组DispDataSplit【】*****************************/
// 4位数拆分为4个数，保存到数组DispDataBit[4],DispDataSplit[0]为个位
void DispDataSplit(Uint16 data)
{
	DispDataBit[3] = data / 1000;						//千位数
	DispDataBit[2] = data % 1000 / 100 ;				//百位数
	DispDataBit[1] = data % 100 / 10;					//十位数
	DispDataBit[0] = data % 10;							//个位数
}
/*****************************************************************************************************/

/*****************************************************************************************************/
//
/*********************************************Spi发送************************************************/
// 将数据a发送到SPITXBUF
void SpiXmitData(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    


void Refresh_8SEG(void)
{
	//static Uint16 Loop=0;
	SellectSeg(Loop_8SEG);								//选择要扫描的数码管位
	SpiXmitData(Disp_8SEG_Code[Loop_8SEG]);					//串行输出要显示的数字
	Loop_8SEG++;Loop_8SEG%=4;
}

// 字符0~9的编码为：0x30~0x39;
// 字符A~Z的编码为：0x41~0x5A;
// 字符a~z的编码为：0x61~0x7A;
//unsigned char msg[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//共阳段码：0~9
//unsigned char msg2_ABC[6]={0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e};	//共阳段码：A~F
//unsigned char msg2_abc[6]={0xa0, 0x83, 0xa7, 0xa1, 0x84, 0x8e};	//共阳段码：a~f

void GetSymCode(unsigned char*  DispSym)
{
	unsigned int i=0;

	for (i=0;i<4;i++)
	{
		if (DispSym[i]>0x60)
			Disp_8SEG_Code[3-i]=msg2_abc[DispSym[i]-'a']; // 小写字母
		else if (DispSym[i]>0x40)
			Disp_8SEG_Code[3-i]=msg2_ABC[DispSym[i]-'A']; // 大写字母
		else
			Disp_8SEG_Code[3-i]=msg[DispSym[i]-'0'];      // 数字
	}

}

// 得到4位非负整数的字符编码
void GetNumCode(Uint16 DispData)
{
	Uint16 loop;

	DispDataSplit(DispData);         // 拆分为4个数字，放在DispDataBit[4]中
	for (loop=0;loop<4;loop++)   // 查询4位的数字编码
	{
		Disp_8SEG_Code[loop]=msg[DispDataBit[loop]];
	}
}

// 得到4位负整数的字符编码
void GetNNumCode(int16 DispData)
{
	Uint16 loop;
	Uint16 tmp;

	if (DispData<0)
	{
		tmp=-DispData;
	}

	DispDataSplit(tmp);         // 拆分为4个数字，放在DispDataBit[4]中
	for (loop=0;loop<4;loop++)   // 查询4位的数字编码
	{
		Disp_8SEG_Code[loop]=msg[DispDataBit[loop]];
	}

	if (DispData<0)
		Disp_8SEG_Code[3]=msg3_N[0];
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    if (CpuTimer0.InterruptCount==(Uint16)tintfreq)//每秒结束时更新显示字符段码
    {
    	CpuTimer0.InterruptCount=0;

		// 以下两行显示数字DispData
		DispData++;	DispData%=10000;     // DispData自动加1，最大9999；
		GetNumCode(DispData);


    	//GetNNumCode(Nnum);  // Nnum=-3 显示负数
    	//GetSymCode(DispSym);// 显示DispSym中的字符

		//DispData++;	DispData%=6;
    	//GetSymCode(Syms[DispData]);    // 显示Syms中的字符

		//DispData++;	DispData%=6;
    	//GetSymCode(Syms2[DispData]);    // 显示Syms2中的字符
   }

    Refresh_8SEG();

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //必不可少，允许本组的其他中断
}


//===========================================================================
// No more.
//===========================================================================


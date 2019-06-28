//程序功能：
//       片内存储器与片外的RAM、FLASH存储器相互传递数据

//外围电路：
//   详见文档《ZQ28335原开发板电路理图.pdf》

//存储区域：
//       本程序的数据传递涉及到四个存储区，为编程方便，分成称为A~D区
//   A区：片内 RAML7的前半部分，起始地址为0x00F000，单元数为0x800个;
//   B区：片内 RAML7的后半部分，起始地址为0x00F800，单元数为0x800个;
//   C区：片外FLASH芯片(SST39VF800)，起始地址为0x10 0000，单元数为512K(0x4 0000)个;
//   D区：片外EXRAM芯片(IS61LV25616)，在起始地址为0x20 0000，单元数为256K(0x2 0000)个;

// 文档结构：
//   (1)头文件; (2)宏定义; (3)变量定义; (4)函数声明; (5)主函数及其他函数

//主函数结构：
//   (1)系统控制初始化; (2)引脚初始化; (3)中断初始化; (4)功能初始化; (5)死循环

// (1)头文件

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
//上面两行头文件与下面的一行头文件等价
// #include "DSP28x_Project.h"     // Device Headerfile and Examples Include Files

// (2)宏定义
/**************************************************************************/
// SST39VF400A and SST39VF800 芯片有关定义
#define 	SST_ID                   0x00BF         /* SST Manufacturer's ID code   */
#define 	SST_39VF400A             0x2780         /* SST39VF400/SST39VF400A device code */
#define 	SST_39VF800A             0x2781         /* SST39VF800/SST39VF800A device code */
#define		TimeOutErr				 1
#define		VerifyErr				 2
#define		WriteOK					 0
#define		EraseErr				 3
#define		EraseOK					 0			
#define		SectorSize				 0x800
#define		BlockSize				 0x8000

// (3)变量定义
// 存储器起始地址定义
Uint16  *AStartAddr =(Uint16  *)0x00F000;   // A区起始地址，RAML7前半部分
Uint16  *BStartAddr =(Uint16  *)0x00F800;   // B区起始地址，RAML7后半部分
Uint16  *CStartAddr =(Uint16  *)0x100000;   // C区起始地址，外部FLASH存储器
Uint16  *DStartAddr =(Uint16  *)0x200000;   // D区起始地址，外部RAM存储器

// (4)函数声明
// 延迟函数
void delay_loop(Uint32 x);
#define  execute_30us     delay_loop(1000)
#define  execute_150ms    delay_loop(5000000)

// 擦除RAM存储器里的数据，檫除后，每个单元均为0
void	EraseRam(Uint16 *SrcStartAddr, Uint16 Length);
// 擦除整个FLASH，擦除后，每个单元均为0xFFFF
Uint16	EraseFlash(void);
// 写数据到FLASH
Uint16	WriteFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);
// 写数据到RAM
void	WriteRam(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);
// 从FLASH存储器读数据(辅助函数)
void	ReadFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);



// (5)主函数及其他函数
void main(void)
{
	Uint32 i; // 临时变量，用于复制序号

	InitSysCtrl();

	// 初始化外部总线接口（Xintf）为16位总线接口
	// 占用引脚：GPIO28-31、34-47、64-87
	InitXintf();  // in DSP2833x_Xintf.c

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();



	// 檫除B区前0x400单元数据，檫除后均为0
	EraseRam(AStartAddr, 0x400);
	EraseRam(BStartAddr, 0x400);
	EraseRam(DStartAddr, 0x400);

    // 将A区前0x400单元分别赋值为0~0x3FF
    for (i=0;i<0x400;i++)
        *(AStartAddr + i) = i*2;

	// 檫除C区(外接FLASH（SST39VF800）)数据，檫除后每个单元均为0xFFFF
	EraseFlash();   // 执行本函数需要几百ms

	// A-->B:  -->RAM
	WriteRam(BStartAddr, AStartAddr,  0x400);

	EraseRam(BStartAddr, 0x400);

	WriteFlash(BStartAddr, CStartAddr,  0x400);

	WriteRam(CStartAddr, DStartAddr,  0x400);

	WriteRam(AStartAddr, DStartAddr,  0x400);

	for(;;);
}

// 延迟函数
void delay_loop(Uint32 x)
{
    Uint32      i;
    for (i = 0; i < x; i++) {}
}

// 擦除RAM存储器里的数据，檫除后，每个单元均为0
// Uint16 *SrcStartAddr：存储单元起始地址
// Uint16 Length：单元个数
void	EraseRam(Uint16 *SrcStartAddr, Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(SrcStartAddr + i) = 0;
}


// 擦除整个FLASH，擦除后，每个单元均为0xFFFF
// 函数返回0：擦除完成（EraseOK=0）
// 函数返回1：擦除超时退出（TimeOutErr=1）
// 函数返回3：擦除错误（EraseErr=3）
Uint16	EraseFlash(void)
{
	Uint16	i,Data;
	Uint32	TimeOut;

	// 擦除整个FLASH命令
	*(CStartAddr + 0x5555) = 0xAAAA;
	*(CStartAddr + 0x2AAA) = 0x5555;
	*(CStartAddr + 0x5555) = 0x8080;
	*(CStartAddr + 0x5555) = 0xAAAA;
	*(CStartAddr + 0x2AAA) = 0x5555;
	*(CStartAddr + 0x5555) = 0x1010;

	i = 0;
	TimeOut = 0;
	execute_150ms;  // 擦除等待

	while(i<5)    // 擦除检验
	{
		Data = *(CStartAddr +  0x3FFFF);   // 判断是否擦除到0x3FFFF单元

		if	(Data == 0xFFFF)	i++;       // 未清除，继续
		else	i=0;

		if ( ++TimeOut>0x1000000)	return (TimeOutErr);  //TimeOutErr=1，超时退出
	}

	for	(i=0;i<0x4000;i++)         // 对前0x4000个单元进行擦除检验
	{
		Data = *(CStartAddr + i);

		if (Data !=0xFFFF)	return (EraseErr); // EraseErr=3
	}

	return  (EraseOK);      // EraseOK=0
}


// 写数据到FLASH
// Uint16 *SrcStartAddr： 源数据起始地址
// Uint16 *DesStartAddr：FLASH单元起始地址，destination addr（目标地址）
// Uint16 Length: 单元个数
// 函数返回0：全部写到FLASH完成（WriteOK=0）
// 函数返回1：擦除超时退出（TimeOutErr=1）
// 函数返回2：写数据错误（VerifyErr=2）
Uint16	WriteFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16	Length)
{
	Uint32	i,TimeOut;
	Uint16	Data1,Data2,j;

	for	(i=0;i<Length;i++)
	{
		// 写FLASH数据命令（时序）
		*(CStartAddr + 0x5555)= 0xAA;
		*(CStartAddr + 0x2AAA)= 0x55;
		*(CStartAddr + 0x5555) = 0xA0;
		*(DesStartAddr + i) = *(SrcStartAddr + i);

		TimeOut = 0;     // 用定时器自动修改TimeOut，看是否超时
		j=0;
		execute_30us;    // 写FLASH等待

		while(j<5)    // 判断已写的FLASH单元，多次读出是否一致（可靠读出）
		{
			Data1 = *(DesStartAddr + i);
			Data2 = *(DesStartAddr + i);
			if	(Data1 == Data2)	j++;
			else	j=0;
			if ( ++TimeOut>0x1000000)	return (TimeOutErr); //TimeOutErr=1

		}
	}


	for (i=0;i<Length;i++)    // 判断写数据是否有错
	{
		Data1 = *(DesStartAddr + i);
		Data2 = *(SrcStartAddr + i);
		if  (Data1 != Data2)	return (VerifyErr);  // VerifyErr=2
	}
	return  (WriteOK);    // WriteOK=0
}

// 写数据到RAM
// Uint16 *SrcStartAddr：RAM源起始地址
// Uint16 *SrcStartAddr：数据目的起始地址
// Uint16 Length：复制单元数
void WriteRam(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(DesStartAddr + i) = *(SrcStartAddr +i);
}


// 从FLASH存储器读数据
// Uint16 *SrcStartAddr： FLASH源数据起始地址
// Uint16 *DesStartAddr：存储目标单元起始地址，destination addr（目标地址）
// Uint16 Length: 单元个数
void ReadFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(DesStartAddr +i) =  *((volatile	unsigned  int  *)(SrcStartAddr + i));
}

//===========================================================================
// No more.
//===========================================================================






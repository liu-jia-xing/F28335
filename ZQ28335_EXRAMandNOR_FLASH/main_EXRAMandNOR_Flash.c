//�����ܣ�
//       Ƭ�ڴ洢����Ƭ���RAM��FLASH�洢���໥��������

//��Χ��·��
//   ����ĵ���ZQ28335ԭ�������·��ͼ.pdf��

//�洢����
//       ����������ݴ����漰���ĸ��洢����Ϊ��̷��㣬�ֳɳ�ΪA~D��
//   A����Ƭ�� RAML7��ǰ�벿�֣���ʼ��ַΪ0x00F000����Ԫ��Ϊ0x800��;
//   B����Ƭ�� RAML7�ĺ�벿�֣���ʼ��ַΪ0x00F800����Ԫ��Ϊ0x800��;
//   C����Ƭ��FLASHоƬ(SST39VF800)����ʼ��ַΪ0x10 0000����Ԫ��Ϊ512K(0x4 0000)��;
//   D����Ƭ��EXRAMоƬ(IS61LV25616)������ʼ��ַΪ0x20 0000����Ԫ��Ϊ256K(0x2 0000)��;

// �ĵ��ṹ��
//   (1)ͷ�ļ�; (2)�궨��; (3)��������; (4)��������; (5)����������������

//�������ṹ��
//   (1)ϵͳ���Ƴ�ʼ��; (2)���ų�ʼ��; (3)�жϳ�ʼ��; (4)���ܳ�ʼ��; (5)��ѭ��

// (1)ͷ�ļ�

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
//��������ͷ�ļ��������һ��ͷ�ļ��ȼ�
// #include "DSP28x_Project.h"     // Device Headerfile and Examples Include Files

// (2)�궨��
/**************************************************************************/
// SST39VF400A and SST39VF800 оƬ�йض���
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

// (3)��������
// �洢����ʼ��ַ����
Uint16  *AStartAddr =(Uint16  *)0x00F000;   // A����ʼ��ַ��RAML7ǰ�벿��
Uint16  *BStartAddr =(Uint16  *)0x00F800;   // B����ʼ��ַ��RAML7��벿��
Uint16  *CStartAddr =(Uint16  *)0x100000;   // C����ʼ��ַ���ⲿFLASH�洢��
Uint16  *DStartAddr =(Uint16  *)0x200000;   // D����ʼ��ַ���ⲿRAM�洢��

// (4)��������
// �ӳٺ���
void delay_loop(Uint32 x);
#define  execute_30us     delay_loop(1000)
#define  execute_150ms    delay_loop(5000000)

// ����RAM�洢��������ݣ��߳���ÿ����Ԫ��Ϊ0
void	EraseRam(Uint16 *SrcStartAddr, Uint16 Length);
// ��������FLASH��������ÿ����Ԫ��Ϊ0xFFFF
Uint16	EraseFlash(void);
// д���ݵ�FLASH
Uint16	WriteFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);
// д���ݵ�RAM
void	WriteRam(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);
// ��FLASH�洢��������(��������)
void	ReadFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length);



// (5)����������������
void main(void)
{
	Uint32 i; // ��ʱ���������ڸ������

	InitSysCtrl();

	// ��ʼ���ⲿ���߽ӿڣ�Xintf��Ϊ16λ���߽ӿ�
	// ռ�����ţ�GPIO28-31��34-47��64-87
	InitXintf();  // in DSP2833x_Xintf.c

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();



	// �߳�B��ǰ0x400��Ԫ���ݣ��߳����Ϊ0
	EraseRam(AStartAddr, 0x400);
	EraseRam(BStartAddr, 0x400);
	EraseRam(DStartAddr, 0x400);

    // ��A��ǰ0x400��Ԫ�ֱ�ֵΪ0~0x3FF
    for (i=0;i<0x400;i++)
        *(AStartAddr + i) = i*2;

	// �߳�C��(���FLASH��SST39VF800��)���ݣ��߳���ÿ����Ԫ��Ϊ0xFFFF
	EraseFlash();   // ִ�б�������Ҫ����ms

	// A-->B:  -->RAM
	WriteRam(BStartAddr, AStartAddr,  0x400);

	EraseRam(BStartAddr, 0x400);

	WriteFlash(BStartAddr, CStartAddr,  0x400);

	WriteRam(CStartAddr, DStartAddr,  0x400);

	WriteRam(AStartAddr, DStartAddr,  0x400);

	for(;;);
}

// �ӳٺ���
void delay_loop(Uint32 x)
{
    Uint32      i;
    for (i = 0; i < x; i++) {}
}

// ����RAM�洢��������ݣ��߳���ÿ����Ԫ��Ϊ0
// Uint16 *SrcStartAddr���洢��Ԫ��ʼ��ַ
// Uint16 Length����Ԫ����
void	EraseRam(Uint16 *SrcStartAddr, Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(SrcStartAddr + i) = 0;
}


// ��������FLASH��������ÿ����Ԫ��Ϊ0xFFFF
// ��������0��������ɣ�EraseOK=0��
// ��������1��������ʱ�˳���TimeOutErr=1��
// ��������3����������EraseErr=3��
Uint16	EraseFlash(void)
{
	Uint16	i,Data;
	Uint32	TimeOut;

	// ��������FLASH����
	*(CStartAddr + 0x5555) = 0xAAAA;
	*(CStartAddr + 0x2AAA) = 0x5555;
	*(CStartAddr + 0x5555) = 0x8080;
	*(CStartAddr + 0x5555) = 0xAAAA;
	*(CStartAddr + 0x2AAA) = 0x5555;
	*(CStartAddr + 0x5555) = 0x1010;

	i = 0;
	TimeOut = 0;
	execute_150ms;  // �����ȴ�

	while(i<5)    // ��������
	{
		Data = *(CStartAddr +  0x3FFFF);   // �ж��Ƿ������0x3FFFF��Ԫ

		if	(Data == 0xFFFF)	i++;       // δ���������
		else	i=0;

		if ( ++TimeOut>0x1000000)	return (TimeOutErr);  //TimeOutErr=1����ʱ�˳�
	}

	for	(i=0;i<0x4000;i++)         // ��ǰ0x4000����Ԫ���в�������
	{
		Data = *(CStartAddr + i);

		if (Data !=0xFFFF)	return (EraseErr); // EraseErr=3
	}

	return  (EraseOK);      // EraseOK=0
}


// д���ݵ�FLASH
// Uint16 *SrcStartAddr�� Դ������ʼ��ַ
// Uint16 *DesStartAddr��FLASH��Ԫ��ʼ��ַ��destination addr��Ŀ���ַ��
// Uint16 Length: ��Ԫ����
// ��������0��ȫ��д��FLASH��ɣ�WriteOK=0��
// ��������1��������ʱ�˳���TimeOutErr=1��
// ��������2��д���ݴ���VerifyErr=2��
Uint16	WriteFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16	Length)
{
	Uint32	i,TimeOut;
	Uint16	Data1,Data2,j;

	for	(i=0;i<Length;i++)
	{
		// дFLASH�������ʱ��
		*(CStartAddr + 0x5555)= 0xAA;
		*(CStartAddr + 0x2AAA)= 0x55;
		*(CStartAddr + 0x5555) = 0xA0;
		*(DesStartAddr + i) = *(SrcStartAddr + i);

		TimeOut = 0;     // �ö�ʱ���Զ��޸�TimeOut�����Ƿ�ʱ
		j=0;
		execute_30us;    // дFLASH�ȴ�

		while(j<5)    // �ж���д��FLASH��Ԫ����ζ����Ƿ�һ�£��ɿ�������
		{
			Data1 = *(DesStartAddr + i);
			Data2 = *(DesStartAddr + i);
			if	(Data1 == Data2)	j++;
			else	j=0;
			if ( ++TimeOut>0x1000000)	return (TimeOutErr); //TimeOutErr=1

		}
	}


	for (i=0;i<Length;i++)    // �ж�д�����Ƿ��д�
	{
		Data1 = *(DesStartAddr + i);
		Data2 = *(SrcStartAddr + i);
		if  (Data1 != Data2)	return (VerifyErr);  // VerifyErr=2
	}
	return  (WriteOK);    // WriteOK=0
}

// д���ݵ�RAM
// Uint16 *SrcStartAddr��RAMԴ��ʼ��ַ
// Uint16 *SrcStartAddr������Ŀ����ʼ��ַ
// Uint16 Length�����Ƶ�Ԫ��
void WriteRam(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(DesStartAddr + i) = *(SrcStartAddr +i);
}


// ��FLASH�洢��������
// Uint16 *SrcStartAddr�� FLASHԴ������ʼ��ַ
// Uint16 *DesStartAddr���洢Ŀ�굥Ԫ��ʼ��ַ��destination addr��Ŀ���ַ��
// Uint16 Length: ��Ԫ����
void ReadFlash(Uint16 *SrcStartAddr, Uint16 *DesStartAddr,  Uint16 Length)
{
	Uint16	i;

	for	(i=0;i<Length;i++)
		*(DesStartAddr +i) =  *((volatile	unsigned  int  *)(SrcStartAddr + i));
}

//===========================================================================
// No more.
//===========================================================================






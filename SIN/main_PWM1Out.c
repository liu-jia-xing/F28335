/* 设置PWM1的频率为600Hz，设置CMPA的值，
 * 改变PWM1A和PWM1B路的占空比，从而改变LED1和LED2的亮度
 * SYSCLK时钟经HSPCLKDIV和CLKDIV分频后，得到TBCLK
 * 增计数，设置PWM1_Freq改变频率，设置CMPA改变占空比；

 */


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "PWM1Out.h"
#include "LCD.h"
#include "KEY.h"

//#define TIMES      198 // 采样次数
//#define BUF_SIZE   198  // 采样缓冲区大小

/*********************************************************************************/
unsigned char  distance[7];
float Distance = 0;//测量实际频率，单位Hz
int qian = 0;
int bai = 0;//存储实际频率的百位 用于转化为12864的显示代码
int shi = 0;//存储十位
int ge = 0;//存储个位
int shi_x = 0;//存储小数部分的十位
int ge_x = 0;//存储小数部分的个位

void getDistanceCode(void){
	//数据清零
	 qian = 0;
	 bai = 0;
	 shi = 0;
	 ge = 0;
	 shi_x = 0;
	 ge_x = 0;

	qian = (int)(Distance/1000)%10;
	bai = (int)(Distance/100)%10;
	shi = (int)(Distance/10)%10;
	ge = (int)(Distance/1)%10;

	shi_x = (int)(Distance*100)/10%10;
	ge_x = (int)(Distance*100)/1%10;

	distance[0] = '0'+qian;
	distance[1] = '0'+bai;
	distance[2] = '0'+shi;
	distance[3] = '0'+ge;
	distance[4] = '.';
	distance[5] = '0'+shi_x;
	distance[6] = '0'+ge_x;

}
/*********************************************************************************/

//float SampleTable[BUF_SIZE];

unsigned char duty[BUF_SIZE];
Uint16 TBPrd1=600;
int Hz;
float PWM1_Freq=600;
// ADC CLK = 12.5MHz
unsigned char game[BUF_SIZE]={300, 329, 358, 387, 414, 441, 466, 490, 512, 531, 549, 564, 577, 587, 594, 598, 600, 598, 594, 587, 577, 564, 549, 531, 512, 490, 466, 441, 414, 387, 358, 329, 300, 270, 241, 212, 185, 158, 133, 109, 87, 68,
                      50, 35, 22, 12, 5, 1, 0, 1, 5, 12, 22, 35, 50, 68, 87, 109, 133, 158, 185, 212, 241, 270,};// 存放采用数据,sin
float sanjiao[BUF_SIZE]={-1.0000,-0.9798,-0.9596,-0.9394,-0.9192,-0.8990,-0.8788,-0.8586,-0.8384,-0.8182,-0.7980,-0.7778,-0.7576,
						 -0.7374,-0.7172,-0.6970,-0.6768,-0.6566,-0.6364,-0.6162,-0.5960,-0.5758,-0.5556,-0.5354,-0.5152,-0.4949,
						 -0.4747,-0.4545,-0.4343,-0.4141,-0.3939,-0.3737,-0.3535,-0.3333,-0.3131,-0.2929,-0.2727,-0.252,-0.2323,
						 -0.2121,-0.1919,-0.1717,-0.151,-0.1313,-0.1111,-0.0909,-0.0707,-0.0505,-0.0303,-0.0101,0.0101,0.0303,
						 0.0505,0.0707,0.0909,0.1111,0.1313,0.1515,0.1717,0.1919,0.2121,0.2323,0.2525,0.2727,0.2929,
						 0.3131,0.3333,0.3535,0.3737,0.3939,0.4141,0.4343,0.4545,0.4747,0.4949,0.5152,0.5354,0.5556,
						 0.5758,0.5960,0.6162,0.6364,0.6566,0.6768,0.6970,0.7172,0.7374,0.7576,0.7778,0.7980,0.8182,
						 0.8384,0.8586,0.8788,0.8990,0.9192,0.9394,0.9596,0.9798,1.0000,0.9798,0.9596,0.9394,0.9192,
						 0.8990,0.8788,0.8586,0.8384,0.8182,0.7980,0.7778,0.7576,0.7374,0.7172,0.6970,0.6768,0.6566,
						 0.6364,0.6162,0.5960,0.5758,0.5556,0.5354,0.5152,0.4949,0.4747,0.4545,0.4343,0.4141,0.3939,
						 0.3737,0.3535,0.3333,0.3131,0.2929,0.2727,0.2525,0.2323,0.2121,0.1919,0.1717,0.1515,0.1313,
						 0.1111,0.0909,0.0707,0.0505,0.0303,0.0101,-0.0101,-0.0303,-0.0505,-0.0707,-0.0909,-0.1111,-0.1313,
						 -0.1515,-0.1717,-0.1919,-0.2121,-0.2323,-0.2525,-0.2727,-0.2929,-0.3131,-0.3333,-0.3535,-0.3737,-0.3939,
						 -0.4141,-0.4343,-0.4545,-0.4747,-0.4949,-0.5152,-0.5354,-0.5556,-0.5758,-0.5960,-0.6162,-0.6364,-0.6566,
						 -0.6768,-0.6970,-0.7172,-0.7374,-0.7576,-0.7778,-0.7980,-0.8182,-0.8384,-0.8586,-0.8788,-0.8990,-0.9192,
						 -0.9394,-0.9596,-0.9798 };

void main(void)
{
//	Uint16 i;


	InitSysCtrl();
	Init_KeyGpio(); // 按键引脚初始化
   // EPWM模块每组输出2路波形，分别用A和B区分，默认它们反相；
   // EPWM1模块的EPWM1A输出引脚是GPIO0, EPWM1B输出引脚是GPIO1
   InitEPwm1Gpio();    // 若不需要输出波形，可不初始化该模块引脚
   InitLcdGpio(); // 按键引脚初始化，使用GPIO2~5。

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   // LCD上电复位，约80ms的L电平
	RST_0;
	delay_loop(632000); //80ms的延时 上电延时
	RST_1;
	delay_loop(632000);

	// 设置LCD模块有关功能参数，并清屏
	InitLcdFun();
	LcdClearTxt();
 	LcdClearBmp();


	DispFig8(jju_fig8);  // 显示图片，九江学院，8位数据，顺序结构
	LcdDelay(60,5);

	LcdClearBmp();

	// 显示中文：九江学院、九江学院
	DispCHN(0,0,jju,8);   // 显示汉字编码字符

	DispCHN(1,0,dian,1);
	// 显示英文字符和数字：0-9，英文字符（大小写）
	DispENG(1,1,str_ENG,5);
	DispCHN(1,4,Class,1);
	DispENG(1,5,str_10,2);
	DispCHN(1,6,number,1);
	DispCHN(2,2,name,2);
	DispCHN(2,4,design,2);
	DispENG(3,1,str_date,10);
	LcdDelay(60,5);
	LcdClearTxt();          // 清除全部字符

	DispCHN(0,0,echo,5);//信号发生器

	DispCHN(1,0,pinlv,3);//显示“频率为”
	DispENG(3,6,str_hz,2);

   SetupEPwm1Parm();

   EALLOW;
   PieVectTable.TINT0=&cpu_timer0_isr;
   EDIS;

   InitCpuTimers();
   #if (CPU_FRQ_150MHZ)
   ConfigCpuTimer(&CpuTimer0, 150, (1/PWM1_Freq)*1000000);
	#endif
   #if (CPU_FRQ_100MHZ)
   ConfigCpuTimer(&CpuTimer0, 100, (1/PWM1_Freq)*1000000);
	#endif


   	StartCpuTimer0();

   	//InitAdc();    // 使能ADC模块，并上电
   //SetAdcParm(); // 设置ADC模块有关参数
   	IER=M_INT1;
   	PieCtrlRegs.PIEIER1.bit.INTx7=1;
   	EINT;
   	ERTM;//实时中断

//   for (i=0; i<BUF_SIZE; i++)
//   {
//	 game[i] = 0;
//   }

//for(i=0;i<TIMES;i++)
//	{
//  	duty[i]=(game[i]+1)/2;
//  	//duty[i]=(sanjiao[i]+1)/2;
//	}   		// 补充按键功能


  while(1)	//死循环
  {
	int i = 0;
	GetKey();  // 得到按键Key的值
	switch(Key)
		{
		case 1:
		  {
			  Distance=0;
			for(i = 0; i < BUF_SIZE; i++){
				duty[i]=(game[i]);
				//DispENG(3,1,distance,6);
			}
			   Distance = PWM1_Freq/198;
			   getDistanceCode( );
			   DispCHN(1,0,pinlv1,5);//显示“频率为”
			   DispENG(3,1,distance,7);
		   }
		break;
		case 2:
		{
			 Distance=0;
			for(i = 0; i < BUF_SIZE; i++){
				duty[i]=(sanjiao[i]+1)/2;
				//DispENG(3,1,distance,6);
			}
			   Distance = PWM1_Freq/198;
			   getDistanceCode( );
			   DispCHN(1,0,pinlv2,3);//显示“频率为”
			   DispENG(3,1,distance,7);
		}
		break;
		case 3:{
			   for(i = 0;i<BUF_SIZE;i++){
				   duty[i]=0.5;
				//DispENG(3,1,distance,6);
			      }
		   Distance = PWM1_Freq;
		   getDistanceCode( );
		   DispCHN(1,0,pinlv3,5);//显示“频率为”
		   DispENG(3,1,distance,7);
		       }break;
		case 4:
			  {
	        Hz=6000;
	        PWM1_Freq += Hz;
					//   TBPrd1=(Uint16)(150e6/16/(PWM1_Freq)-1); //增 或 减计数模式,=46875<65535 OK
	        StopCpuTimer0();
	        ConfigCpuTimer(&CpuTimer0, 150, (1/PWM1_Freq)*1000000);
	        StartCpuTimer0();
	        SetupEPwm1Parm();
	        if(duty[1]==0.5)
	        {
	        	Distance = 0;
	        	Distance = PWM1_Freq;
	        }
	        else
	        {	Distance = 0;
	        	Distance = PWM1_Freq/198;
	        }
	        getDistanceCode( );
	        DispENG(3,1,distance,7);
	          }DSP28x_usDelay(3000000);break;
		 case 5:
	         {
	        Hz=600;
	        PWM1_Freq -= Hz;
	        StopCpuTimer0();
	        ConfigCpuTimer(&CpuTimer0, 150, (1/PWM1_Freq)*1000000);
	        StartCpuTimer0();
								//TBPrd1=(Uint16)(150e6/16/(PWM1_Freq)-1); //增 或 减计数模式,=46875<65535 OK
	        if(duty[1]==0.5)
	        {
	        	Distance = 0;
	        	Distance = PWM1_Freq;
	        }
	        else
	        {
	        	Distance = 0;
	        	Distance = PWM1_Freq/198;
	        }
	        getDistanceCode( );
	        DispENG(3,1,distance,7);
	        SetupEPwm1Parm();
	        }DSP28x_usDelay(3000000);break;

		default: break;

		}

		DSP28x_usDelay(3000000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
	}
}






//===========================================================================
// No more.
//===========================================================================

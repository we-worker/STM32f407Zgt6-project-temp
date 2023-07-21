#include <ad9851.h>

void delay_ns(int16_t ns)
{
	while (ns--)
		;
}

void ad9851_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG, ENABLE);

	// 除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ad9851_init2(void) // I/O端口配置
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	// 除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	// 除A4为复位位，其余为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // 推挽输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//***************************************************//
//              ad9851复位(串口口模式)                 //
//---------------------------------------------------//
void ad9851_reset_serial()
{
	ad9851_w_clk = 0;
	ad9851_fq_up = 0;
	// rest信号
	ad9851_rest = 0;
	ad9851_rest = 1;
	delay_us(10);
	ad9851_rest = 0;
	// w_clk信号
	ad9851_w_clk = 0;
	ad9851_w_clk = 1;
	delay_us(10);
	ad9851_w_clk = 0;
	// fq_up信号
	ad9851_fq_up = 0;
	ad9851_fq_up = 1;
	delay_us(10);
	ad9851_fq_up = 0;
}

/*************************************************************************
						ad9851复位函数，清除DDS累加器和相位延时器为0Hz和0相位，
						同时默认为并行模式以及禁止6倍倍乘器工作
						但未清除40位的输入寄存器
**************************************************************************/
void ad9851_reset()
{
	ad9851_w_clk = 0;
	ad9851_fq_up = 0;
	// rest信号
	ad9851_rest = 0;
	ad9851_rest = 1;
	delay_us(10);
	ad9851_rest = 0;
	// P5=0xfb;
}

/*ad9851串行输入函数*/
void ad9851_wr_serial1(unsigned char w0, double frequence)
{
	unsigned char i, w;
	long int y;
	double x;
	// 计算频率的HEX值
	x = 4294967295 / 180e6; // 适合30M晶振，4294967295是2的32次方
	// 如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
	frequence = frequence / 10;
	frequence = frequence * x;
	y = frequence;
	ad9851_fq_up = 0;
	// 写w4数据
	w = (y >>= 0);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 写w3数据
	w = (y >> 8);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//写w2数据
	w = (y >> 16);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//写w1数据
	w = (y >> 24);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//写w0数据
	w = w0;
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//移入始能
	delay_us(10);
	ad9851_fq_up = 1;
	delay_us(10);
	ad9851_fq_up = 0;
}

char AD9851_DataBus;


void sendAD9851_DataBus(void){
	for (int i = 0; i < 8; i++)
	{
 switch (i) {
            case 0:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA0 = 1;
                } else {
                    AD98851_DATA0 = 0;
                }
                break;
            case 1:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA1 = 1;
                } else {
                    AD98851_DATA1 = 0;
                }
                break;
            case 2:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA2 = 1;
                } else {
                    AD98851_DATA2 = 0;
                }
                break;
            case 3:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA3 = 1;
                } else {
                    AD98851_DATA3 = 0;
                }
                break;
            case 4:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA4 = 1;
                } else {
                    AD98851_DATA4 = 0;
                }
                break;
            case 5:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA5 = 1;
                } else {
                    AD98851_DATA5 = 0;
                }
                break;
            case 6:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA6 = 1;
                } else {
                    AD98851_DATA6 = 0;
                }
                break;
            case 7:
                if ((AD9851_DataBus >> i) & 0x01) {
                    AD98851_DATA7 = 1;
                } else {
                    AD98851_DATA7 = 0;
                }
                break;
        }
	}
	
}
//***************************************************//
//          向ad9851中写命令与数据(并口)             //
//---------------------------------------------------//
void ad9851_wr_parrel(u8 w0, double frequence)
{
	u32 w;
	long int y;
	double x;
	// 计算频率的HEX值
	x = 4294967295 / 180; // 适合180M晶振/180为最终时钟频率（或30M六倍频）
	// 如果时钟频率不为180MHZ，修改该处的频率值，单位MHz ！！！
	frequence = frequence / 1000000;
	frequence = frequence * x;
	y = frequence;
	// 写w0数据
	w = w0;
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
	delay_us(1);
	ad9851_w_clk = 0;
	// 写w1数据
	w = (y >> 24);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// 写w2数据
	w = (y >> 16);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0.
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// 写w3数据
	w = (y >> 8);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// 写w4数据
	w = (y >>= 0);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// 移入始能
	ad9851_fq_up = 1;
		delay_us(1);
	ad9851_fq_up = 0;
}

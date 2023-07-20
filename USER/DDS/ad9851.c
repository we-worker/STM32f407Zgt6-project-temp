#include <ad9851.h>

void delay_ns(int16_t ns)
{
	while (ns--)
		;
}

void ad9851_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	// ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG, ENABLE);

	// ��A4Ϊ��λλ������Ϊ�͵�ƽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // �������
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void ad9851_init2(void) // I/O�˿�����
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

	// ��A4Ϊ��λλ������Ϊ�͵�ƽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // �������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // �������
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	// ��A4Ϊ��λλ������Ϊ�͵�ƽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // �������
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
//              ad9851��λ(���ڿ�ģʽ)                 //
//---------------------------------------------------//
void ad9851_reset_serial()
{
	ad9851_w_clk = 0;
	ad9851_fq_up = 0;
	// rest�ź�
	ad9851_rest = 0;
	ad9851_rest = 1;
	delay_us(10);
	ad9851_rest = 0;
	// w_clk�ź�
	ad9851_w_clk = 0;
	ad9851_w_clk = 1;
	delay_us(10);
	ad9851_w_clk = 0;
	// fq_up�ź�
	ad9851_fq_up = 0;
	ad9851_fq_up = 1;
	delay_us(10);
	ad9851_fq_up = 0;
}

/*************************************************************************
						ad9851��λ���������DDS�ۼ�������λ��ʱ��Ϊ0Hz��0��λ��
						ͬʱĬ��Ϊ����ģʽ�Լ���ֹ6������������
						��δ���40λ������Ĵ���
**************************************************************************/
void ad9851_reset()
{
	ad9851_w_clk = 0;
	ad9851_fq_up = 0;
	// rest�ź�
	ad9851_rest = 0;
	ad9851_rest = 1;
	delay_us(10);
	ad9851_rest = 0;
	// P5=0xfb;
}

/*ad9851�������뺯��*/
void ad9851_wr_serial1(unsigned char w0, double frequence)
{
	unsigned char i, w;
	long int y;
	double x;
	// ����Ƶ�ʵ�HEXֵ
	x = 4294967295 / 180e6; // �ʺ�30M����4294967295��2��32�η�
	// ���ʱ��Ƶ�ʲ�Ϊ180MHZ���޸ĸô���Ƶ��ֵ����λMHz  ������
	frequence = frequence / 10;
	frequence = frequence * x;
	y = frequence;
	ad9851_fq_up = 0;
	// дw4����
	w = (y >>= 0);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// дw3����
	w = (y >> 8);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//дw2����
	w = (y >> 16);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//дw1����
	w = (y >> 24);
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//дw0����
	w = w0;
	for (i = 0; i < 8; i++)
	{
		ad9851_bit_data = (w >> i) & 0x01;
		delay_us(10);
		ad9851_w_clk = 1;
		delay_us(10);
		ad9851_w_clk = 0;
	}
	// 	//����ʼ��
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
//          ��ad9851��д����������(����)             //
//---------------------------------------------------//
void ad9851_wr_parrel(u8 w0, double frequence)
{
	u32 w;
	long int y;
	double x;
	// ����Ƶ�ʵ�HEXֵ
	x = 4294967295 / 180; // �ʺ�180M����/180Ϊ����ʱ��Ƶ�ʣ���30M����Ƶ��
	// ���ʱ��Ƶ�ʲ�Ϊ180MHZ���޸ĸô���Ƶ��ֵ����λMHz ������
	frequence = frequence / 1000000;
	frequence = frequence * x;
	y = frequence;
	// дw0����
	w = w0;
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
	delay_us(1);
	ad9851_w_clk = 0;
	// дw1����
	w = (y >> 24);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// дw2����
	w = (y >> 16);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0.
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// дw3����
	w = (y >> 8);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// дw4����
	w = (y >>= 0);
	AD9851_DataBus = w | (w ^ 0xff) << 16; // w0
	sendAD9851_DataBus();
	ad9851_w_clk = 1;
		delay_us(1);
	ad9851_w_clk = 0;
	// ����ʼ��
	ad9851_fq_up = 1;
		delay_us(1);
	ad9851_fq_up = 0;
}

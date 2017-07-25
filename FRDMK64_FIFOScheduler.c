#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
* Definitions
******************************************************************************/
#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW3_NAME

#define SLOW_BLINK      (10000000)
#define FAST_BLINK      (1000000)
#define BLINK_DELAY     SLOW_BLINK
/*******************************************************************************
* Prototypes
******************************************************************************/
static void task1();
static void task2();
static void task3();
static void task4();
static void task5();

void delay_time(int);
/*******************************************************************************
* Variables
******************************************************************************/
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;
volatile bool g_ButtonPresssw2 = false;
/*******************************************************************************
* Code
******************************************************************************/
/*!
* @brief Interrupt service fuction of switch.
*
* This function toggles the LED
*/
void BOARD_SW_IRQ_HANDLER(void)
{
	/* Clear external interrupt flag. */
	GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
	/* Change state of button. */
	g_ButtonPress = true;
}

void BOARD_SW2_IRQ_HANDLER(void)
{
	/* Clear external interrupt flag. */
	GPIO_ClearPinsInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
	/* Change state of button. */
	g_ButtonPresssw2 = true;
}
/*!
* @brief Main function
*/
int main(void)
{
	/* Define the init structure for the input switch pin */
	gpio_pin_config_t sw_config = {
		kGPIO_DigitalInput, 0,
	};
	
	/* Define the init structure for the output LED pin */
	gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput, 0,
	};
	
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	int state = 0;
	int FIFOHandlr[6] ={4,3,5,2,1,6};
	int FIFOcount = 0;
	
	/* Print a note to terminal. */
	PRINTF("\r\n Scheduler FIFO example\r\n");
	PRINTF("\r\n Task #1 turns on Red Led ");
	PRINTF("\r\n Task #2 turns on Blue Led");
	PRINTF("\r\n Task #3 turns on Green Led");
	PRINTF("\r\n Task #4 turns on Purple Led");//red + blue
	PRINTF("\r\n Task #5 turns on Yellow Led");//red + green
	
	/* Init input switch GPIO. */
	PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(BOARD_SW_IRQ);
	GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
	
	/* Init input switch GPIO. */
	PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(BOARD_SW2_IRQ);
	GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config);
	
	/* Init output LED GPIO. */
	GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
	GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &led_config);
	GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config); //enciende verde
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
	
	while (1)
	{
		switch(state)
		{
			case(0):
				{ if (g_ButtonPress)
				{  	PRINTF(" %s is pressed \r\n", BOARD_SW_NAME);
				/* Reset state of button. */
				g_ButtonPress = false;
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				else
					{	PRINTF("\r\n Initial State: Priorities set. Wait for start to run tasks.");
					state = 0;
					break;
					}
				}
				
				case(1):
				{  task1();
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				
				case(2):
				{ 	task2();
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				
				case(3):
				{	task3();
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				
				case(4):
				{	task4();
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				
				case(5):
				{	task5();
				state=FIFOHandlr[FIFOcount];
				FIFOcount++;
				break;
				}
				case(6):
				{	PRINTF("\n\r Fin de ciclo");
				break;
				}
		}
		
	}
}

static void task1()
{
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
	delay_time(BLINK_DELAY);
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
}

static void task2()
{
	GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN);
	delay_time(BLINK_DELAY);
	GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN);
}

static void task3()
{
	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
	delay_time(BLINK_DELAY);
	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
	
}

static void task4()
{
	GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
	delay_time(BLINK_DELAY);
	GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
}

static void task5()
{
	
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
	delay_time(BLINK_DELAY);
	GPIO_TogglePinsOutput(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN);
}

void delay_time(int number){
	int cnt;
	for(cnt=0;cnt<number;cnt++);
}

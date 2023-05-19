 //includes to use tivaware as our driver
  #include <stdint.h>
  #include <stdbool.h>
  #include "inc/hw_memmap.h"
  #include "driverlib/sysctl.h"
  #include "driverlib/gpio.h"
  #include "inc/hw_types.h"
  #include "inc/hw_gpio.h"
  #include "driverlib/systick.h"
  #include "driverlib/sysctl.h"
  #include "driverlib/interrupt.h"
  #include "inc/hw_ints.h"
  #include "inc/hw_nvic.h"
  #include "driverlib/timer.h"

//defining the pins for the traffic light and pedestrian light
  #define CAR_GREEN_LED_1 GPIO_PIN_2
  #define CAR_YELLOW_LED_1 GPIO_PIN_3
  #define CAR_RED_LED_1 GPIO_PIN_4
  #define PEDESTRIAN_GREEN_LED_1 GPIO_PIN_5
  #define PEDESTRIAN_RED_LED_1 GPIO_PIN_6
  #define CAR_GREEN_LED_2 GPIO_PIN_0
  #define CAR_YELLOW_LED_2 GPIO_PIN_2
  #define CAR_RED_LED_2 GPIO_PIN_3
  #define PEDESTRIAN_GREEN_LED_2 GPIO_PIN_4
  #define PEDESTRIAN_RED_LED_2 GPIO_PIN_5

//function prototypes
void GPIO_init(void);
void TIMERS_init(void);
void delay(uint32_t period);
void DelayTimer(uint32_t BASE ,uint32_t period);
void TrafficTimer();
void PedestrianHandler();
void ResumeSequence1();
void ResumeSequence2();
void IntTimeOut_1();
void IntTimeOut_2();
void delay(uint32_t period);
void TrafficTimer();
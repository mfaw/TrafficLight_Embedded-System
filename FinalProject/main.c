  #include "functions.h"

//states for the traffic light
  typedef enum{ start_state, green_state, yellow_state, red_state, interrupted_state} State;

//states for the pedestrian interrupts
  typedef enum{pedestrian_state_1, pedestrian_state_2, pedestrian_state_3, no_interrupt_state } Interrupt;

//initializing the start state and next state of the traffic light 
  State state = start_state; 
  State nextState = start_state;

//initializing the start state and next state of the pedestrian light interrupt 
  Interrupt currentInterrupt_state = no_interrupt_state;
  Interrupt nextInterrupt_state = no_interrupt_state;

//defining an old state used to return to previous state after interrupt
  State previous_traffic_state; 

//flag for each pedestrian light to allow interrupt routine to happen
  uint8_t interrupts_flag_pedestrain_1 = 0;  
  uint8_t interrupts_flag_pedestrain_2 = 0;

//defining timer values that will have the remaining time to continue the sequence
  uint32_t timer_value_1 = 0;
  uint32_t timer_value_2 = 0;

//flag to allow returning to the previous state before interrupts happen
//if 1 it returns to the previous state, if 0 it doesn't return
  uint8_t before_state_allowed_1 = 1;
  uint8_t before_state_allowed_2 = 1;

//variable to choose which port to work on
// turn 0 -> port A -> traffic light 1, pedestrain_1 -> pin_0
// turn 1 -> port B -> traffic light 1, pedestrain_2 -> pin_4
  uint8_t turn = 0;


  int main()
  {
    
    GPIO_init(); // intialize GPIOs
    TIMERS_init(); // initialize the timers

    //system starts with all traffic lights and pedestrian lights as red
    GPIOPinWrite(GPIO_PORTA_BASE,(CAR_GREEN_LED_1 | CAR_YELLOW_LED_1 |CAR_RED_LED_1|PEDESTRIAN_GREEN_LED_1|PEDESTRIAN_RED_LED_1),(CAR_RED_LED_1 | PEDESTRIAN_RED_LED_1 )); //CAR RED LED 1 ON and PEDESTRIAN RED LED 1 ON
    GPIOPinWrite(GPIO_PORTB_BASE,(CAR_GREEN_LED_2 | CAR_YELLOW_LED_2 |CAR_RED_LED_2|PEDESTRIAN_GREEN_LED_2|PEDESTRIAN_RED_LED_2),(CAR_RED_LED_2 | PEDESTRIAN_RED_LED_2)); //CAR RED LED 2 ON and PEDESTRIAN RED LED 2 ON

    //delay 1s to go to start state
    DelayTimer(TIMER0_BASE , 1000); 

    while(1){
      //check traffic light state
      switch (state)
      {
        //if state is green 
          case green_state: 
          //if turn is 1 then, traffic light 2 in port B will be green 
              if(turn){
                  GPIOPinWrite(GPIO_PORTB_BASE , CAR_RED_LED_2 | CAR_YELLOW_LED_2,0); //CAR RED LED 2 OFF and CAR YELLOW LED 2 OFF
                  GPIOPinWrite(GPIO_PORTB_BASE ,CAR_GREEN_LED_2, CAR_GREEN_LED_2); //CAR GREEN LED 2 OFF
              }else{
                //if turn is not 1 (0) then, traffic light 1 in port A will be green 
                  GPIOPinWrite(GPIO_PORTA_BASE , CAR_RED_LED_1 | CAR_YELLOW_LED_1,0); //CAR RED LED 1 OFF and CAR YELLOW LED 1 OFF
                  GPIOPinWrite(GPIO_PORTA_BASE ,CAR_GREEN_LED_1, CAR_GREEN_LED_1); ////CAR GREEN LED 1 OFF
              }
              break;
          
          //if state is yellow 
          case yellow_state:
          //if turn is 1 then, traffic light 2 in port B will be yellow
              if(turn){
                  GPIOPinWrite(GPIO_PORTB_BASE ,CAR_RED_LED_2 | CAR_GREEN_LED_2 ,0); //CAR RED LED 2 OFF and CAR GREEN LED 2 OFF
                  GPIOPinWrite(GPIO_PORTB_BASE,CAR_YELLOW_LED_2,CAR_YELLOW_LED_2); //CAR GREEN LED 2 ON

              }else{
                //if turn is not 1 (0) then, traffic light 1 in port A will be yellow 
                  GPIOPinWrite(GPIO_PORTA_BASE ,CAR_RED_LED_1 | CAR_GREEN_LED_1 ,0); //CAR RED LED 1 OFF and CAR GREEN LED 1 OFF
                  GPIOPinWrite(GPIO_PORTA_BASE,CAR_YELLOW_LED_1,CAR_YELLOW_LED_1); //CAR GREEN LED 2 ON
              }
              break;

          //if state is red
          case red_state:
          //if turn is 1 then, traffic light 2 in port B will be red
              if(turn){
                  GPIOPinWrite(GPIO_PORTB_BASE,CAR_GREEN_LED_2 | CAR_YELLOW_LED_2,0); //CAR YELLOW LED 2 OFF and CAR GREEN LED 2 OFF
                  GPIOPinWrite(GPIO_PORTB_BASE ,CAR_RED_LED_2, CAR_RED_LED_2); // CAR RED LED 2 ON
              }else{
                //if turn is not 1 (0) then, traffic light 1 in port A will be red
                  GPIOPinWrite(GPIO_PORTA_BASE,CAR_GREEN_LED_1 | CAR_YELLOW_LED_1,0); //CAR YELLOW LED 1 OFF and CAR GREEN LED 1 OFF
                  GPIOPinWrite(GPIO_PORTA_BASE ,CAR_RED_LED_1, CAR_RED_LED_1); //CAR RED LED 1 ON
              }
              break;
          default:
              break;
      }
    
      //check the current interrupt state
      switch(currentInterrupt_state){
        //state where switch 0 is pressed for pedestrian light 1 to be green in port A
        //switch OFF the CAR GREEN LED 1 and switch ON the CAR RED LED 1 then switch ON the PEDESTRIAN GREEN LED 1 ON
          case pedestrian_state_1:
              GPIOPinWrite(GPIO_PORTA_BASE,CAR_GREEN_LED_1 | CAR_YELLOW_LED_1,0); //CAR YELLOW LED 1 OFF and CAR GREEN LED 1 OFF
              GPIOPinWrite(GPIO_PORTA_BASE ,CAR_RED_LED_1,CAR_RED_LED_1); //CAR RED LED 1 ON
              GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_GREEN_LED_2,0); 
              GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_RED_LED_2,PEDESTRIAN_RED_LED_2);
              GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_RED_LED_1,0);
              GPIOPinWrite(GPIO_PORTA_BASE, PEDESTRIAN_GREEN_LED_1,PEDESTRIAN_GREEN_LED_1); //PEDESTRIAN GREEN LED 1 ON

              break;
          
        //state where switch 1 is pressed for pedestrian light 2 to be green in port B
        //switch OFF the CAR GREEN LED 2 and switch ON the CAR RED LED 2 then switch ON the PEDESTRIAN GREEN LED 2 ON
          case pedestrian_state_2:
                GPIOPinWrite(GPIO_PORTB_BASE ,CAR_GREEN_LED_2 | CAR_YELLOW_LED_2,0); //CAR YELLOW LED 2 OFF and CAR GREEN LED 2 OFF
                GPIOPinWrite(GPIO_PORTB_BASE ,CAR_RED_LED_2,CAR_RED_LED_2);  //CAR RED LED 2 ON
                GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_GREEN_LED_1,0);
                GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_RED_LED_1,PEDESTRIAN_RED_LED_1);
                GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_RED_LED_2 ,0);
                GPIOPinWrite(GPIO_PORTB_BASE, PEDESTRIAN_GREEN_LED_2,PEDESTRIAN_GREEN_LED_2); //PEDESTRIAN GREEN LED 2 ON
              break;

        //state where both switches are pressed 
        //switch OFF both CAR GREEN LEDS and YELLOW LEDS, switch ON both CAR RED LEDS, then switch ON both PEDESTRIAN GREEN LEDS  
          case pedestrian_state_3:
              GPIOPinWrite(GPIO_PORTA_BASE ,CAR_GREEN_LED_1 | CAR_YELLOW_LED_1,0); //CAR YELLOW LED 1 OFF and CAR GREEN LED 1 OFF
              GPIOPinWrite(GPIO_PORTA_BASE ,CAR_RED_LED_1,CAR_RED_LED_1); //CAR RED LED 1 ON
              GPIOPinWrite(GPIO_PORTB_BASE ,CAR_GREEN_LED_2 | CAR_YELLOW_LED_2,0); //CAR YELLOW LED 2 OFF and CAR GREEN LED 2 OFF
              GPIOPinWrite(GPIO_PORTB_BASE ,CAR_RED_LED_2,CAR_RED_LED_2); //CAR RED LED 2 ON
              GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_RED_LED_1 ,0); //PEDESTRIAN RED LED 1 OFF
              GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_RED_LED_2 ,0); //PEDESTRIAN RED LED 2 OFF
              GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_GREEN_LED_2 ,PEDESTRIAN_GREEN_LED_2); //PEDESTRIAN GREEN LED 1 ON
              GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_GREEN_LED_1 ,PEDESTRIAN_GREEN_LED_1); //PEDESTRIAN GREEN LED 2 ON
          break;

        // in case of no interrupt, continue traffic light sequence normally
        //both PEDESTRIAN GREEN LED OFF and both PEDESTRIAN RED LED ON
          case no_interrupt_state:
              GPIOPinWrite(GPIO_PORTB_BASE ,PEDESTRIAN_GREEN_LED_2 ,0);
              GPIOPinWrite(GPIO_PORTB_BASE,PEDESTRIAN_RED_LED_2 ,PEDESTRIAN_RED_LED_2);
              GPIOPinWrite(GPIO_PORTA_BASE ,PEDESTRIAN_GREEN_LED_1 ,0);
              GPIOPinWrite(GPIO_PORTA_BASE,PEDESTRIAN_RED_LED_1 ,PEDESTRIAN_RED_LED_1);
          break;

          default:
          break;
      }
      //update next state to be the state for the traffic light
      state = nextState;
      //update next interrupt state to be the current interrupt state
      currentInterrupt_state = nextInterrupt_state;
    }
    return 0;
  }
 
 //function to handle pedestrian press interrupts 
  void PedestrianHandler(){
    //debounce delay
    delay(40); 

    //check which switch is pressed
    //if switch 0 is pressed and interrupt flag is set to 0 
    //handling first pin which is first port
    if(GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_0) == 0 && interrupts_flag_pedestrain_1 == 0 ){

      //find current interrupt state    
      switch (currentInterrupt_state)
      { 
      
      //if there is no interrupt
      case no_interrupt_state: 
      //if the traffic is not in the red state or it is in the yellow state and it has less than 1 second left in it's timer, meaning it will switch to red in less than 1 second
      //the interrupt will halt the system

        if((turn == 0 && state != red_state) || (state == yellow_state && TimerValueGet(TIMER0_BASE , TIMER_A) < 16000 * 1000)){ 
          //set next interrupt state to pedestrian_state_1 because this is the state of switch 0 pressed
          //this will make pedestrian green led 1 on
          nextInterrupt_state = pedestrian_state_1; 

          //save the current traffic light state in old state to get back to it in resume sequence
          previous_traffic_state = state; 

          //set the next traffic light state to the interrupted state because now the main traffic is interrupted
          nextState = interrupted_state; 

          //flag to allow returning to previous state before interrupt
          before_state_allowed_1 = 1;  

          //disable main timer in case system halts
          TimerDisable(TIMER0_BASE, TIMER_BOTH);

          //delay for 2 seconds because the PEDESTRIAN GREEN LED 1 will be ON for 2 seconds
          DelayTimer(TIMER1_BASE , 2000); 
        }else{
          //if the if above doesn't satisfy 
          //we will not have to halt the system so it will continue in it's normal sequence
          //set next interrupt state to pedestrian_state_1 because this is the state of switch 0 pressed
          //this will make pedestrian green led 1 on
          nextInterrupt_state = pedestrian_state_1;

          //no halting of system so flag is set to zero
          before_state_allowed_1 = 0;

          //system doesn't return to it's old state bc system didn't halt
          previous_traffic_state = state; 

          //delay for 2 seconds because the PEDESTRIAN GREEN LED 1 will be ON for 2 seconds
          DelayTimer(TIMER1_BASE , 2000);
        }
        break;

      //handling case if PEDESTRIAN GREEN LED 1 is ON meaning switch 0 is still pressed
      //if switch is pressed again during the same pedestrian period of crossing, bonus
      case pedestrian_state_1: 

        //updates the timer_value_1 with the remaining timer value of the crossing
        timer_value_1 += (2000 * 16000 - TimerValueGet(TIMER1_BASE , TIMER_A)); 
        // if the value in the timer_value_1 is less than 2 seconds we will refresh the timer
        if(timer_value_1 < 2000 * 16000){
          //the interrupt will be handled normally as previous state 
          if((turn == 0 && state != red_state )|| (state == yellow_state && TimerValueGet(TIMER0_BASE , TIMER_A) < 16000 * 1000)){
            nextInterrupt_state = pedestrian_state_1; 
            nextState = interrupted_state; 
            before_state_allowed_1 = 1; 
            TimerDisable(TIMER0_BASE, TIMER_BOTH);
            DelayTimer(TIMER1_BASE , 2000);
          }else{
            //if timer value is more than 2 seconds the timer is not refreshed it will go to the handler
            //the interrupt will be handled normally as previous state
            nextInterrupt_state = pedestrian_state_1;
            before_state_allowed_1 = 0; 
            DelayTimer(TIMER1_BASE , 2000);
          }
        } 
        break;

      //case if PEDESTRIAN 2 GREEN LED ON and we pressed on switch 0 
      //both PEDESTRIAN GREEN LED will be ON, bonus case
      case pedestrian_state_2:  
      //next interrupt state will be state where both leds are on which is pedestrian_state_3
          nextInterrupt_state = pedestrian_state_3;
          nextState = interrupted_state;
          before_state_allowed_1 = 1;
          DelayTimer(TIMER1_BASE , 2000);
          TimerDisable(TIMER0_BASE, TIMER_BOTH);
          break;

      //case where both LEDS are on and switch 0 is pressed again
      case pedestrian_state_3:
      //updates the timer with the remaining timer value of the crossing
        timer_value_1 += (2000 * 16000 - TimerValueGet(TIMER1_BASE , TIMER_A)); 
        // if the remaining time in the timer is less than 2 seconds we will refresh the timer
        if(timer_value_1 < 2000 * 16000){
          nextInterrupt_state = pedestrian_state_3;
          nextState = interrupted_state;
          before_state_allowed_1 = 1;
          DelayTimer(TIMER1_BASE , 2000);
        } 
        break;
      default:
        break;
      }

    //handling the case if switch 1 is pressed
    // will be the same handler as above the only difference is we will use TIMER2 instead of TIMER1
    //also case pedestrian_state_2 will be the one that shows switch is already pressed because this is the other port and pin
    }else if (GPIOPinRead(GPIO_PORTF_BASE , GPIO_PIN_4) == 0 && interrupts_flag_pedestrain_2 == 0){
      switch (currentInterrupt_state)
      {
        //normal case with no interrupts
      case no_interrupt_state: 
        if(turn == 1 || (state == yellow_state && TimerValueGet(TIMER0_BASE , TIMER_A) < 16000 * 1000)){
          nextInterrupt_state = pedestrian_state_2;
          previous_traffic_state = state;
          nextState = interrupted_state;
          before_state_allowed_2 = 1;
          DelayTimer(TIMER2_BASE , 2000);
          TimerDisable(TIMER0_BASE, TIMER_BOTH);
        }else{
          nextInterrupt_state = pedestrian_state_2;
          before_state_allowed_2 = 0;
          previous_traffic_state = state;
          DelayTimer(TIMER2_BASE , 2000);
        }
        break;

      //handling case if PEDESTRIAN GREEN LED 2 is ON meaning switch 1 is still pressed
      //if switch is pressed again during the same pedestrian period of crossing, bonus
      case pedestrian_state_2: 
      timer_value_2 +=  (2000 * 16000 - TimerValueGet(TIMER1_BASE , TIMER_A)); 
        if(timer_value_2 < 2000 * 16000){
          if(turn == 1 || (state == yellow_state && TimerValueGet(TIMER0_BASE , TIMER_A) < 16000 * 1000)){
            nextInterrupt_state = pedestrian_state_2;
            nextState = interrupted_state;
            before_state_allowed_2 = 1;
            TimerDisable(TIMER0_BASE, TIMER_BOTH);
            DelayTimer(TIMER2_BASE , 2000);
          }else{
            nextInterrupt_state = pedestrian_state_2;
            before_state_allowed_2 = 0;
            DelayTimer(TIMER2_BASE , 2000);
          }
        }
        break;

      //case if PEDESTRIAN 1 GREEN LED ON and we pressed on switch 1
      //both PEDESTRIAN GREEN LED 2 will be ON, bonus case
      case pedestrian_state_1: 
          nextInterrupt_state = pedestrian_state_3;
          nextState = interrupted_state;
          before_state_allowed_2 = 1;
          DelayTimer(TIMER2_BASE , 2000);
          TimerDisable(TIMER0_BASE, TIMER_BOTH);
          break;

       //case where both LEDS are on and switch 0 is pressed again
      case pedestrian_state_3: 
        timer_value_2 +=  (2000 * 16000 - TimerValueGet(TIMER1_BASE , TIMER_A)); 
        if(timer_value_2 < 2000 * 16000){
          nextInterrupt_state = pedestrian_state_3;
          nextState = interrupted_state;
          before_state_allowed_2 = 1;
          DelayTimer(TIMER2_BASE , 2000);
        }
        break;
      default:
        break;
      }

    }
    //clear interrupts of both pins once handler is done
    GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0);
    GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
  }

 //function to resume sequence of traffic light if interruped for port A, switch 0 
  void ResumeSequence1(){
//check state of current interrupt state, cases will be state 1 and state 3 only because we are dealing with one switch
    switch (currentInterrupt_state)
    {
    case pedestrian_state_1: 
      TimerEnable(TIMER0_BASE, TIMER_BOTH); 
      TimerDisable(TIMER1_BASE, TIMER_BOTH); 
      //if system is allowed to return old state, meaning system halts
      //make the next traffic state the old state
      if(before_state_allowed_1){ 
        nextState =  previous_traffic_state;
      }
      before_state_allowed_1 = 1; 
      //as long as this flag is 1 we can't serve another interrupt, if condition in the Pedestrian handler requires this flag to be zero to handle an interrupt
      interrupts_flag_pedestrain_1 = 1;  
      //refresh the timer value
      timer_value_1 = 0; 
      //load one second to timer 3 to handle the case of delay the request of the pedestrian to cross if the button was pressed after 1 second from the end of the Period of pedestrian crossing
      //bonus case
      //will wait for timer timeout to handle another case
      TimerLoadSet(TIMER3_BASE, TIMER_A, 16000 * 1000); 
      TimerEnable(TIMER3_BASE, TIMER_A);   
      nextInterrupt_state = no_interrupt_state; 
      break;

      //same as previous state but will not need to return to old state as the system didn't halt
    case pedestrian_state_3: 
      TimerDisable(TIMER1_BASE, TIMER_BOTH); 
      before_state_allowed_1 = 1; 
      interrupts_flag_pedestrain_1 = 1; 
      timer_value_1 = 0; 
      TimerLoadSet(TIMER3_BASE, TIMER_A, 16000 * 1000); 
      TimerEnable(TIMER3_BASE, TIMER_A);  
      nextInterrupt_state = pedestrian_state_2;   
      break;
    default:  
      break;
    }
    //clear the timer interrupt at timeout
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  }

  //same function as ResumeSequence1 but hahndling port B, pin 1
  //this will handle state 2 and 3 
  void ResumeSequence2(){
    switch (currentInterrupt_state)
    {
    case pedestrian_state_2: 
      TimerEnable(TIMER0_BASE, TIMER_BOTH);
      TimerDisable(TIMER2_BASE, TIMER_BOTH); 
      if(before_state_allowed_2){
        nextState = previous_traffic_state; 
      }
      before_state_allowed_2 = 1;
      interrupts_flag_pedestrain_2 = 1; 
      timer_value_2 = 0;
      TimerLoadSet(TIMER4_BASE, TIMER_A, 16000 * 1000);
      TimerEnable(TIMER4_BASE, TIMER_A);
      nextInterrupt_state = no_interrupt_state;
      break;
    case pedestrian_state_3:
      TimerDisable(TIMER2_BASE, TIMER_BOTH);
      before_state_allowed_2 = 1;
      interrupts_flag_pedestrain_2 = 1;
      timer_value_2 = 0;
      TimerLoadSet(TIMER4_BASE, TIMER_A, 16000 * 1000);
      TimerEnable(TIMER4_BASE, TIMER_A);  
      nextInterrupt_state = pedestrian_state_1;
      break;

    default:  
      break;
  
    }
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  }

//timeout for first pedestrian timer and setting flag to 0 to be able to handle other interrupts
  void IntTimeOut_1 (){ 
    interrupts_flag_pedestrain_1 = 0; 
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT); 
    TimerDisable(TIMER3_BASE, TIMER_A); 
  }

  //timeout for second pedestrian timer and setting flag to 0 to be able to handle other interrupts
  void IntTimeOut_2 (){
    interrupts_flag_pedestrain_2 = 0;
    TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT); 
    TimerDisable(TIMER4_BASE, TIMER_A);
  }

//delay time with given period to be used to make lights wait for a number of milliseconds
  void DelayTimer(uint32_t BASE ,uint32_t period){
      TimerLoadSet(BASE, TIMER_A, 16000 * period);
      TimerEnable(BASE, TIMER_BOTH);
  }

//delay function with systick for debouncing when pressing on switch
  void delay(uint32_t period)
  {
    SysTickDisable();
    SysTickPeriodSet(period  *16000);
    SysTickEnable();
    while (NVIC_ST_CTRL & 1<<16 == 0){}
    SysTickDisable();
  }

//function to upate next state of each traffic light state and give delay to decide each traffic light will
//be on for how manay seconds 
  void TrafficTimer(){
//check state 
    switch (state)
      {
        //start state goes to green state
        //green state shall be green for 5 seconds
      case start_state: 
          nextState = green_state;
          DelayTimer(TIMER0_BASE , 5000);
          break;
      
      //after green state the traffic light should be yellow for 2 secods
      case green_state:
          nextState = yellow_state; 
          DelayTimer(TIMER0_BASE , 2000); 
          break;
      
      //after yellow state the traffic light should be red for 2 second 
      case yellow_state:
          nextState = red_state; 
          DelayTimer(TIMER0_BASE , 1000); 
          break;

      //after red state the traffic late will be green again for 5 seconds based on the turn 
      //if 0 port a, traffic led 1 will be on
      case red_state:
          nextState = green_state; 
          //update new turn value
          turn = (turn + 1) %2; 
          DelayTimer(TIMER0_BASE , 5000); 
          break;
      default:
        break;
      }
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  }

//initializing GPIO port A and B and port F to use switch 1 and 4
  void GPIO_init(void){
      
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){};
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){};
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){};

      HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY; 
      HWREG(GPIO_PORTF_BASE+GPIO_O_CR) = GPIO_PIN_0;
    
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0); 
      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4); 
      GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
      GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

      GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, CAR_GREEN_LED_1 | CAR_YELLOW_LED_1 |CAR_RED_LED_1|PEDESTRIAN_GREEN_LED_1|PEDESTRIAN_RED_LED_1 );
      GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, CAR_GREEN_LED_2 | CAR_YELLOW_LED_2 |CAR_RED_LED_2|PEDESTRIAN_GREEN_LED_2|PEDESTRIAN_RED_LED_2 );

      GPIOIntDisable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
      GPIOIntDisable(GPIO_PORTF_BASE,GPIO_INT_PIN_0);
      //making PedestrianHandler the handler of interrupts for pin 0 and 4
      GPIOIntRegisterPin(GPIO_PORTF_BASE,  GPIO_PIN_4 , PedestrianHandler);
      GPIOIntRegisterPin(GPIO_PORTF_BASE,  GPIO_PIN_0 , PedestrianHandler);
      IntRegister(INT_GPIOF_TM4C129, PedestrianHandler);
      GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);
      GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 , GPIO_FALLING_EDGE); 
      GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4); 
      GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_0);
      IntEnable(INT_GPIOF_TM4C129);

  }

//initializing the timers
  //registering each timer to a specific handler
  void TIMERS_init(void){

    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){};
    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER1 );
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)){};
    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER2 );
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2)){};
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); 
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3)){}  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); 
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER4)){}

    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
    TimerDisable(TIMER0_BASE, TIMER_BOTH); 
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
    TimerControlStall(TIMER0_BASE, TIMER_BOTH,true);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_BOTH, TrafficTimer);

    

    TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
    TimerDisable(TIMER1_BASE, TIMER_BOTH); 
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    TimerControlStall(TIMER1_BASE, TIMER_BOTH,true);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER1_BASE, TIMER_BOTH, ResumeSequence1);

    TimerClockSourceSet(TIMER2_BASE, TIMER_CLOCK_SYSTEM);
    TimerDisable(TIMER2_BASE, TIMER_BOTH); 
    TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT);
    TimerControlStall(TIMER2_BASE, TIMER_BOTH,true);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER2_BASE, TIMER_BOTH, ResumeSequence2);

    TimerClockSourceSet(TIMER3_BASE, TIMER_CLOCK_SYSTEM);
    TimerDisable(TIMER3_BASE, TIMER_BOTH); 
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);
    TimerControlStall(TIMER3_BASE, TIMER_BOTH,true);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER3_BASE, TIMER_BOTH, IntTimeOut_1);

    TimerClockSourceSet(TIMER4_BASE, TIMER_CLOCK_SYSTEM);
    TimerDisable(TIMER4_BASE, TIMER_BOTH); 
    TimerConfigure(TIMER4_BASE, TIMER_CFG_ONE_SHOT);
    TimerControlStall(TIMER4_BASE, TIMER_BOTH,true);
    TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER4_BASE, TIMER_BOTH, IntTimeOut_2);
  }
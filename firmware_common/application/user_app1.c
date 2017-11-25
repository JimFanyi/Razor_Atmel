/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }
   

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void BuzzerRun(void)
{
  static u32 u32BuzzerTimer = 0;
  u32BuzzerTimer++;
  if ( u32BuzzerTimer == 10)
  {
    AT91C_BASE_PIOA ->PIO_SODR = PA_28_BUZZER1;
  }
  
  else if( u32BuzzerTimer == 20)
  {
    AT91C_BASE_PIOA ->PIO_CODR = PA_28_BUZZER1;
    u32BuzzerTimer = 0;
  }
}

static void TurnOnLed(void)
{
  if ( AT91C_BASE_PIOB ->PIO_PDSR & PB_00_BUTTON1)
  {  
    AT91C_BASE_PIOB ->PIO_CODR = PB_18_LED_BLU;
  }
  
  else
  {
    AT91C_BASE_PIOB ->PIO_SODR = PB_18_LED_BLU;
  }
}

static void TurnOnBuzzer(void)
{
  static u32 u32BuzzerTimer_1ms = 0;
  if ( AT91C_BASE_PIOB ->PIO_PDSR & PB_01_BUTTON2 )
  {
    AT91C_BASE_PIOA ->PIO_CODR = PA_28_BUZZER1;
  }
  
  else
  {
    BuzzerRun();
    

  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{

   static u32 u32LedToggleTimer         = 0;       //Set the timer for LedToggle
   static u32 u32BuzzerFreqencyTimer    = 0;       //Set a timer that controls the frequency of beepper
   static bool bLedBLUE                 = FALSE;   //A bool that shows which LED is ON
   static bool bIsBuzzerHigh            = FALSE;   //A bool that shows the current state of beeper
   
   /*Enable PIO of BUZZER1*/
   AT91C_BASE_PIOA->PIO_PER = PA_28_BUZZER1;
   
   /*Check if BUTTON1 is pressed*/
   if ( AT91C_BASE_PIOB ->PIO_PDSR & PB_00_BUTTON1)
   {
    /*If button is not pressed, close the LEDs.*/
    AT91C_BASE_PIOB ->PIO_CODR = PB_18_LED_BLU;
    AT91C_BASE_PIOB ->PIO_CODR = PB_13_LED_WHT;
   } 
 
   else
  {
    /*When button is pressed, timers begin to count.*/
    u32LedToggleTimer++;
    u32BuzzerFreqencyTimer++;
    
    /*Check which LED is ON*/
    if (bLedBLUE == FALSE) 
    {
     
      /*We assume that the blue LED turns on first, so the original state is FALSE.
        We use the bool two toggle white and blue LED.*/
      
      AT91C_BASE_PIOB ->PIO_SODR = PB_18_LED_BLU;
      AT91C_BASE_PIOB ->PIO_CODR = PB_13_LED_WHT;
      if (u32LedToggleTimer == 500)
      {
        /*When time is up, reset the timer and turn to white LED.*/
        bLedBLUE = TRUE;
        u32LedToggleTimer = 0;
      }
    }
    
    if (bLedBLUE == TRUE)
    {
      AT91C_BASE_PIOB ->PIO_SODR = PB_13_LED_WHT;
      AT91C_BASE_PIOB ->PIO_CODR = PB_18_LED_BLU;
      
       if (u32LedToggleTimer == 500)
      {
        bLedBLUE = FALSE;
        u32LedToggleTimer = 0;
      }
    }
    
    if (bIsBuzzerHigh == FALSE)
    {
        AT91C_BASE_PIOA->PIO_SODR = PA_28_BUZZER1;
        if (u32BuzzerFreqencyTimer == 2)
        {
          bIsBuzzerHigh = TRUE;
          u32BuzzerFreqencyTimer = 0;
        }
    }
    
    if (bIsBuzzerHigh == TRUE)
    {
      AT91C_BASE_PIOA->PIO_CODR = PA_28_BUZZER1;
      if (u32BuzzerFreqencyTimer == 2)
        {
          bIsBuzzerHigh = FALSE;
          u32BuzzerFreqencyTimer = 0;
        }
      
    }
    
    
  }
   
   
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

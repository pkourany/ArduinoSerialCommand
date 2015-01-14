// Demo Code for SerialCommand Library
// Steven Cogswell
// May 2011

// If you want to use HardwareSerial only, and not have to include SoftwareSerial support, you 
// can define SERIALCOMMAND_HARDWAREONLY in SerialCommand.h, which will cause it to build without
// SoftwareSerial support.   This makes the library act as it used to before SoftwareSerial 
// support was added, and you don't need this next include: 
//#include <SoftwareSerial.h>  

#if defined (SPARK)
#include "SerialCommand.h"

#define arduinoLED D7   // Spark Core LED on board
#else
#include <SerialCommand.h>

#define arduinoLED 13   // Arduino LED on board
#endif


SerialCommand SCmd;   // The demo SerialCommand object

void setup()
{  
  pinMode(arduinoLED,OUTPUT);      // Configure the onboard LED for output
  digitalWrite(arduinoLED,LOW);    // default to LED off

  commandSerial.begin(9600); 
#if (commandSerial != debugSerial)
  debugSerial.begin(9600);
#endif

  // Setup callbacks for SerialCommand commands 
  SCmd.addCommand("ON",LED_on);       // Turns LED on
  SCmd.addCommand("OFF",LED_off);        // Turns LED off
  SCmd.addCommand("HELLO",SayHello);     // Echos the string argument back
  SCmd.addCommand("P",process_command);  // Converts two arguments to integers and echos them back 
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
  debugSerial.println("Ready");

}

void loop()
{  
  SCmd.readSerial();     // We don't do much, just process serial commands
}


void LED_on()
{
  debugSerial.println("LED on"); 
  digitalWrite(arduinoLED,HIGH);
}

void LED_off()
{
  debugSerial.println("LED off"); 
  digitalWrite(arduinoLED,LOW);
}

void SayHello()
{
  char *arg;  
  arg = SCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL)      // As long as it existed, take it
  {
    debugSerial.print("Hello "); 
    debugSerial.println(arg);
  } 
  else {
    debugSerial.println("Hello, whoever you are");
  }
}


void process_command()    
{
  int aNumber;  
  char *arg; 

  debugSerial.println("We're in process_command"); 
  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
    debugSerial.print("First argument was: "); 
    debugSerial.println(aNumber);
  } 
  else {
    debugSerial.println("No arguments"); 
  }

  arg = SCmd.next(); 
  if (arg != NULL) 
  {
    aNumber=atol(arg); 
    debugSerial.print("Second argument was: "); 
    debugSerial.println(aNumber);
  } 
  else {
    debugSerial.println("No second argument"); 
  }

}

// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  debugSerial.println("What?"); 
}


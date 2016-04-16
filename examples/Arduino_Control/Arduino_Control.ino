#include <DataServeriOS.h>
#include <SPI.h>
#include <Ethernet.h>

/* Arduino Control example
 - Comments are welcome, arduinocontrolapp@gmail.com
 
 Created 16-04-2016 by M. Heeres
 */

//***************************************************//
// Security password, limited to 15 characters, use to prevent unauthorized access. Mandatory! //
char secpass[] = "PASSWORD";
int Port = 80; // Port 80 is http
//************************************************//

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[]= {0,0,0,0};
int PWMPorts[] = {6,9};
int ownFunctionForPorts[] = {5,7};
int ownReturnForPorts[] = {0,3};
EthernetServer server(Port);
DataServeriOS dataServer(&server,secpass,true,(char*)"UNO",&substitudeLoop,PWMPorts,2,ownFunctionForPorts,2,&ownFunction,&ownDigitalReturn,ownReturnForPorts,2,&ownAnalogReturn);
/*                        ^        ^      ^      ^        ^            ^      ^        ^            ^      ^              ^                ^          ^      ^
                          |        |      |      |        |            |      |        |            |      |              |                |          |      Pointer to own analog return
                          |        |      |      |        |            |      |        |            |      |              |                |          Number of ports for own return (length of array)
                          |        |      |      |        |            |      |        |            |      |              |                Array of ports for which you wish to return your own value
                          |        |      |      |        |            |      |        |            |      |              Pointer to own digital return, belongs to own function
                          |        |      |      |        |            |      |        |            |      Poiter to own function
                          |        |      |      |        |            |      |        |            Number of ports for which your own function (length of array)
                          |        |      |      |        |            |      |        Array of ports for which you wish your own function to be triggered
                          |        |      |      |        |            |      Number of ports whihc are to be used as PWM (length of array)
                          |        |      |      |        |            Array of ports which should act as PWM
                          |        |      |      |        Pointer to the substitude loop()
                          |        |      |      "UNO" or "MEGA", dicating the number of available ports
                          |        |      Debug? Prints request and return from and to the app
                          |        The password
                          Pointer to the EthernetServer
*/
                                                                                                                                            

void setup()
{
  Ethernet.begin(mac,ip);
  // Enable if debugging is required
  //Serial.begin(9600);
}

void loop()
{
  // The dataServer loop takes care of communication, use the substitude loop as normal loop
  dataServer.loop();
}

void substitudeLoop()
{
  // Normal loop() contents goes here..
}

void ownFunction(int pin)
{
  // Do some action other than the default switching for the previously defined ports
  // The app expects a return, you are responcable to provide this here. This should be "1","0" or "\"p255\"" in case of PWM
  // The same value is requested in 'ownDigitalReturn' when getting all values
  // (prefix 'p' indicates to the app that this port is used for PWM)
  dataServer.printValueForDigitalPort((char*)"1",pin);
}

char* ownDigitalReturn(int pin)
{
  // Return "1","0" or "\"p255\""
  // (prefix 'p' indicates to the app that this port is used for PWM)
  return (char *)"1";
}

char* ownAnalogReturn(int pin)
{
  // Return a value for the previously defined ports
  if (pin == 0) {
    // Return a text value, or variable stored alsewhere
    return (char *)"Door open";
  }
  else if (pin == 3) {
    char t[128];
    // Return a formatted value (deg C), you can use unicode
    sprintf(t,"%d\u00B0C", analogRead(3));
    return t;
  }
  else return (char*)"";
}

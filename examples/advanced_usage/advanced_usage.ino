/*
@link https://github.com/voltprojects/arduino-cliprocessor
example for Cliprocessor By Rhys Bryant. to use.
type "led on" or "led off" in the serial monitor (without the quotes) and press enter.
Make sure line ending is set to "newline" 
*/
#include <cliProcessor.h>
CliProcessor cli;
/**
* cli callback for set pin command
* @return CmdResult ; the return value or status code 0 = no error
* @param char* data the full line  
**/
CmdResult set_pin(char* cmd) {
    CliHelpers::SetCmd value=CliHelpers::SetCmd(cmd);
    if(value.pin==-1)
        return CmdResult(1);
    if(value.value==1 || value.value==0) {
        digitalWrite(value.pin,value.value==1?HIGH:LOW);
    }
    else
        return CmdResult(2);
    return CmdResult(0);
}

/**
* cli callback for get pin command
* @return CmdResult ; the return value or status code 0 = no error
* @param char* data, the full line  
**/
CmdResult get_pin(char* data) {
    CliHelpers::SetCmd v=CliHelpers::SetCmd(data);
    if(v.pin==-1)
        return CmdResult(1);//if agrument 2 is not a valid number return error 1
    else {
        char tmp[10]="";
        sprintf(tmp,"%d",digitalRead(v.pin));//get the value of the pin x
        return CmdResult(tmp);//non error values must always be char*
    }
}
/**
* JSON demo
* get the details of all pins in json format
* why? formatting data in a standard format allows it to be more easly read by software. this demo uses
* JSON see http://en.wikipedia.org/wiki/JSON
**/
CmdResult get_all(char* data){
	JsonArray items;
        
	cli.aSyncResponse("getall", " ");//becouse we are not using a buffer we have to manualy format the response
	items.disableBuffer();//send directly instead of using a buffer
	for(int i=0;i<13;i++){
		JsonObject j;
		j.addEntry("mode","IN");
		j.addEntry("pin",CliHelpers::getValue(i));
		j.addEntry("state",(char*)(digitalRead(i)==HIGH?"HIGH":"LOW"));
		items.addEntry(j);
        Serial.println();//for user readabilty
	}
    items.close();//add the final ] to the output
	return CmdResult();// do not send any result
}
//basic examples
/**
* callback for led_on
*/
CmdResult led_on(char* data){
   digitalWrite(13,HIGH);
   return CmdResult(0);//0 error code 0 means the command ran successfully
}
/**
* callback for led_off
*/
CmdResult led_off(char* data){
   digitalWrite(13,LOW);
   return CmdResult(0);//0 error code 0 means the command ran successfully
}
void setup(){
  Serial.begin(9600);//setup the serial port
  pinMode(13,OUTPUT);//set pin 13 to output mode
  cli.addCmd("get","pin",get_pin);
  cli.addCmd("get","all",get_all);
  cli.addCmd("set","pin",set_pin);
  cli.addCmd("led","on",led_on);
  cli.addCmd("led","off",led_off);
  //send a message without first receiving a request. using the standard format used by this library
  cli.aSyncResponse("welcome", "please type a command");
}
void loop(){
  cli.checkSerial();//check for new incoming commands
  delay(10);
  
}
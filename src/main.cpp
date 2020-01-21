#include <Arduino.h>
#include <avr/wdt.h>
#include <ICSC.h>
#include <Servo.h>
#include <CLI.h>
#include <ANSI.h>

#define WAIT_SEC_AT_START 3

#define ROBOT_LEFT  0x10
#define ROBOT_RIGHT 0x11
#define BIN_FERTIG  0x12

ICSC icsc(Serial3,2,2);
stats_ptr status;

//Servo myservo;

char buffer[20];

void icsc_status(void);

void go_left(unsigned char station, char command, unsigned char len, char *data);
void go_right(unsigned char station, char command, unsigned char len, char *data);

CLI_COMMAND(go_degree);
CLI_COMMAND(helpFunc);
CLI_COMMAND(connectFunc);
CLI_COMMAND(ble_send);
CLI_COMMAND(ble_status);
CLI_COMMAND(icsc_status);
CLI_COMMAND(reset);

ANSI ansi;

volatile int status

byte rx_byte = 0;        // stores received byte


void setup() {
//  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
//  myservo.write(0);

  Serial3.begin(115200);
  Serial.begin(115200);
  // for(int i = 0; i < WAIT_SEC_AT_START;i++)
  // {
  //     Serial.print(i+1);
  //     delay(1000);
  //     ansi.clr_line();
  // }

  ansi.cls();

  CLI.setDefaultPrompt("SLAVE> ");
  CLI.onConnect(connectFunc);
  
  CLI.addCommand("go_degree", go_degree);
  CLI.addCommand("help", helpFunc);
  CLI.addCommand("reset", reset);
  CLI.addCommand("ble_send", ble_send);
  CLI.addCommand("ble_status",ble_status);
  CLI.addCommand("icsc_status",icsc_status);

  CLI.addClient(Serial);
  
  
  

  icsc.begin();
  icsc.registerCommand(ROBOT_LEFT, &go_left);
  icsc.registerCommand(ROBOT_RIGHT, &go_right);
}

void loop() {
  static unsigned long ts = millis();
  CLI.process();
  icsc.process();
  // if (Serial2.available()) {
  //   rx_byte = Serial2.read();
  //   // send byte to serial port 3
  //   Serial.write(rx_byte);
  // }

  
   if (millis() - ts >= 5000) {
     ts = millis();
//  //   Serial.println("Sende ...");
    icsc.send(1, BIN_FERTIG);
   }




}

CLI_COMMAND(icsc_status)
{
    status = icsc.stats();

    sprintf(buffer,"Col: %ld",status->collision);
    Serial.println(buffer);
    sprintf(buffer,"rx_packets: %ld",status->rx_packets);
    Serial.println(buffer);
    sprintf(buffer,"rx_bytes: %ld",status->rx_bytes);
    Serial.println(buffer);
    sprintf(buffer,"tx_packets: %ld",status->tx_packets);
    Serial.println(buffer);
    sprintf(buffer,"tx_bytes: %ld",status->tx_bytes);
    Serial.println(buffer);
    sprintf(buffer,"tx_fail: %ld",status->tx_fail);
    Serial.println(buffer);
    sprintf(buffer,"cs_errors: %ld",status->cs_errors);
    Serial.println(buffer);
    sprintf(buffer,"cb_run: %ld",status->cb_run);
    Serial.println(buffer);
    sprintf(buffer,"cb_bad: %ld",status->cb_bad);
    Serial.println(buffer);
    return 0;
}

void go_left(unsigned char station, char command, unsigned char len, char *data)
{
  Serial.println("Ich fahre links");
}
void go_right(unsigned char station, char command, unsigned char len, char *data)
{
  Serial.println("Ich fahre rechts");
}


CLI_COMMAND(go_degree) {
    int degree = atoi(argv[1]);
    dev->print("go to ");
    dev->print(argv[1]);
    dev->println(" degree!");
//    myservo.write(degree);
    return 0;
}


CLI_COMMAND(helpFunc) {
    dev->println("add <number 1> <number 2> - Add two numbers together");
    return 0;
}

CLI_COMMAND(reset) {
    dev->println("Reset the System - Bye Bye");
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}


   Serial.print("jump");
    return 0;
}


CLI_COMMAND(connectFunc) {
    dev->println("************* DOMA ARDUINO ************");
    dev->println("Welcome to the CLI test.");
    dev->println("Type 'help' to list commands.");
    dev->println();
}

CLI_COMMAND(ble_send) {
  dev->println("send");
  Serial2.println("Hallo du device");
  return 0;
}

CLI_COMMAND(ble_status) {
  dev->println("status: ");
  Serial2.print("AT");
  return 0;
}


//##########################//
//###     LIBRARIES      ###//######################################
//##########################//
#include <LiquidCrystal_I2C_Spark.h>    // LCD I2C 16*2 screen library
#include <Adafruit_DHT.h>               // DHT 11 library from ada fruit


//##########################//
//###       SENSORS      ###//#######################################
//##########################//
//PH SENSOR

#define SensorPin A0                    // ph sensor pin
unsigned long int avgValue;             // average value with data type uint
float b;                                // b float data type  
float phValue;                          // ph value in float data type
int buf[5],temp;                        // array of buffer 


// HUMIDITY 
#define DHTPIN D2                       // DHT 11 pin signal input to digital pin 2
#define DHTTYPE DHT11                   // setting DHT type 11
DHT dht(DHTPIN,DHTTYPE);                // setting DHT pin and type into a variable dht
int humidity;                           // define humidity as int data type
int tempC;                              // define temperature as int data type


// LDR
int pRes = D3;                          // photo resistor input digital pin 3
bool nightTime = false;                 // LDR status flag 


//##########################//
//###  REMOTE ACTUATORS  ###//########################################
//##########################//
//+++ REMOTE ACTUATOR +++//
// LIGHT
int rLight = D4;                        // remote light output signal digital pin 4

// FAN 
int rFan = D5;                          // remote fan output signal digital pin 5

// ROOF
int rOpeRoof = D6;                      // remote opening roof output signal digital pin 6
int rCloRoof = D7;                      // remote closing roof output signal digital pin 7

// LCD 
LiquidCrystal_I2C lcd(0x27,16,2);       // initialization I2C address of 0x27 with screen wide of 16 and 2 row
char ascii = 0xdf;                      // define ascii symbol
char moon =  0x29;                      // define bracket symbol
char sun =   0x2A;                      // define asterisk symbol

int swtchPin = A2;                      // switch mode output pin 
int watPump = A1;                       // ph pump output pin

//##########################//
//###        SETUP       ###//#########################################
//##########################//
void setup(){
  dht.begin();                                              // dht 11 sensor intialization 
  lcd.init();                                               // initialize the lcd             
  lcd.backlight();                                          // initialize the backlight of lcd screen
  lcd.clear();                                              // clear the lcd screen character
  lcd.setCursor(0,0);                                       // set the cursor at (0,0)
  lcd.print("Retrieving data");                             // print retrieving data
  lcd.setCursor(0,1);                                       // set the cursor at (0,1)
  lcd.print("Please wait... ");                             // print please wait
  
// SENSOR PINMODE INPUT     
    pinMode(pRes,INPUT);                                    // setting LDR to input pin mode

// REMOTE ACTUATOR - PINMODE OUTPUT
    pinMode(rFan,OUTPUT);                                   // setting remote fan to output pin mode
    pinMode(rLight,OUTPUT);                                 // setting remote light to output pin mode
    pinMode(rOpeRoof,OUTPUT);                               // setting remote open roof to output pin mode
    pinMode(rCloRoof,OUTPUT);                               // setting remote close roof to output pin mode
    pinMode(swtchPin,OUTPUT);                               // setting switch mode to output pin mode
    pinMode(watPump,OUTPUT);                                // setting pH pump to output pin mode
    double phValue;
    
// (PARTICLE CONSOLE/NODE-RED->DASHBOARD) - PARTICLE VARIABLES 
    Particle.variable("tempC",tempC);                       // define particle variable 
    Particle.variable("humidity",humidity);                 // define particle variable 
    Particle.variable("nightTime",nightTime);               // define particle variable 
    Particle.variable("phValue",phValue);                   // define particle variable 
    
    Particle.function("turn_Fan",turn_Fan);                 // define particle function
    Particle.function("turn_Light",turn_Light);             // define particle function
    Particle.function("turn_opeRoof",turn_opeRoof);         // define particle function
    Particle.function("turn_cloRoof",turn_cloRoof);         // define particle function
    Particle.function("swtchMod",swtchMod);                 // define particle function
    Particle.function("phPump",phPump);                     // define particle function
}


//##########################//
//###  LOOPING PROGRAM   ###//##########################################
//##########################//
void loop() {
    phCheck();              // ph check function 
    humidCheck();           // humidity check function 
    lightCheck();           // light check function
    tempCheck();            // temperature check function
    display();              // display lcd 
}

//##########################//
//###  SENSOR FUNCTIONS  ###//###########################################
//##########################//
//LIGHTCHECK #############################################################
void lightCheck(){                              // light check function defination
    if(digitalRead(pRes) == HIGH){              // if LDR sensed no light 
        nightTime = true;                       // it is night time - switch night time to true
        Particle.publish("LDR","Night");        // Particle function to publish ldr 
    }else {                                     
        nightTime = false;                      // it is day time - night time will be change to false 
        Particle.publish("LDR","Day");
    }
}    

    
// HUMIDITY CHECK ##########################################################
void humidCheck(){
    humidity = dht.getHumidity();                       // define getHumidity function as humidity
    Particle.publish("humidity",String(humidity)+"%");  // publish humidity value to the cloud console
    delay(1000);
}
    
// TEMPERATURE CHECK  ######################################################
void tempCheck(){
    tempC = dht.getTempCelcius();                       // define getTempCelcius function as tempC
    if(tempC>=27){                                      // if temperature is higher than 27
        Particle.publish("Fan","fan_On");               // turn on the fan
    }else {                                             // if no 
        Particle.publish("Fan","fan_Off");              // turn off the fan
    }
    Particle.publish("Temperature",String(tempC)+"°C"); // publish temperature value to the cloud console
    delay(1000);    
}

// PH CHECK##################################################################
void phCheck(){
    for(int i=0;i<5;i++)                // Get 5 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);       // place the ph value to the buf array
    delay(10);
  }
  for(int i=0;i<4;i++)        // sort the analog from small to large
  {
    for(int j=i+1;j<4;j++)    // set j as 1 place away from i
    {
      if(buf[i]>buf[j])       // compare the value in array i to value in array j
      {
        temp=buf[i];          
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=0;i<3;i++)        // start iteration of 3 
    avgValue+=buf[i];         // sum all reading value from array i 
  phValue=(float)avgValue*5/4096/7;     
  phValue=3.5*phValue;                      
  Particle.publish("phValue", String(phValue, 2)); // publish ph value to the cloud console
  delay (2000); 
}

// LCD #########################################################################
void display(){
    lcd.clear();                    // clear the lcd screen
    lcd.print("Humidity       %");  // print "Humidity       %"
    lcd.setCursor(0,1);             // set the cursor
    lcd.print("Temperature    C");  // print "Temperature    C"
    lcd.setCursor(14,1);            // set the cursor
    lcd.print(ascii);               // print ascii symbol
    lcd.setCursor(9,0);             // set cursor
    lcd.print(humidity);            // print humidity value 
    lcd.setCursor(12,1);            // set the cursor 
    lcd.print(tempC);               // print temperature value
    delay(2000);                

    lcd.clear();
    lcd.setCursor(0,0);             // set the cursor
    if (nightTime==true){           // if it is night
        lcd.clear();                // clear the lcd screen
        lcd.print("Status =     "); // print "Status =     "
        lcd.setCursor(9,0);         // set the cursor
        lcd.print("Night");         // print "Night"
        lcd.setCursor(15,0);        // set the cursor 
        lcd.print(moon);            // print bracket
    }else if(nightTime==false){     // if it is daytime
        lcd.clear();                // clear the lcd screen
        lcd.print("Status =     "); // print the "status =     "
        lcd.setCursor(9,0);         // set the cursor 
        lcd.print("Day");           // print day 
        lcd.setCursor(13,0);        // set the cursor 
        lcd.print(sun);             // print asterisk 
    }
    lcd.setCursor(0,1);             // set the cursor
    lcd.print("PH");                // print "PH"
    lcd.setCursor(4,1);             // set the cursor
    lcd.print(phValue);             // print the ph value
}


//#######################################//
//###  REMOTE -> PARTICLE FUNCTIONS   ###//
//#######################################//
// (PARTICLE FUNCTION/CLOUD FUNCTION) FOR NODE-RED - (RETRIEVING + CALLING VARIABLES)
// FAN FUNCTION //
int turn_Fan (String command){      // define turn_fan as particle function with string argument
    if(command == "on"){            // if the string argument is "on"
        digitalWrite(rFan,HIGH);    // fan is turned on 
        return 1;                   // return 1 
    }else if(command == "off"){     // if the string argument is "off"
        digitalWrite(rFan,LOW);     // fan is turned off
        return 0;                   // return 0
    }else { 
        return -1;                  // if the argument is invalid - return -1
    }
}

// LED LIGHT FUNCTION //
int turn_Light (String command){    // define turn_light as particle function with string argument
    if(command == "on"){            // if the string argument is "on"
        digitalWrite(rLight,HIGH);  // turn on the led-light
        return 1;                   // return 1
    }else if(command == "off"){     // if the string argument is "off"
        digitalWrite(rLight,LOW);   // turn off the led-light
        return 0;                   // return 0
    }else {                         
        return -1;                  // if the argument is invalid - return -1
    }
}

// ROOF MOTOR FUNCTION //
int turn_opeRoof (String command){                      // define turn_operoof as particle function with string argument
    if(command == "on"){                                // if the argument is "on"
        digitalWrite(rOpeRoof,HIGH);                    // turn on the motor 
        Particle.publish("opeRoof","opening");          // publish to the cloud console that operoof is opening 
        return 1;                                       // return 1 
    }else if(command == "off"){                         // if the argument is "off"
        digitalWrite(rOpeRoof,LOW);                     // turn off the motor 
        Particle.publish("opeRoof","stop opening");     // publish to the cloud console that operoof is stop opening
        return 0;                                       // return 0
    }else {
        return -1;                                      // if argument is invalid - return -1
    }
}

int turn_cloRoof (String command){                      // define turn_cloroof as particle function with string argument
    if(command == "on"){                                // if the argument is "on"
        digitalWrite(rCloRoof,HIGH);                    // turn on the motor
        Particle.publish("cloRoof","closing");          // publish to the cloud console that cloroof is closing
        return 1;                                       // return 1
    }else if(command == "off"){                         // if the argument is "off"
        digitalWrite(rCloRoof,LOW);                     // turn off the motor
        Particle.publish("cloRoof","stop closing");     // publish to the cloud console that cloroof is stop closing
        return 0;                                       // return 
    }else {
        return -1;                                      // if the argument is invalid - return -1
    }
}

int swtchMod (String command){                      // define swtchmod as particle function with string argument 
    if(command == "on"){                            // if the argument is "on"
        digitalWrite(swtchPin,HIGH);                // put signal as an output 
        Particle.publish("Mode","Remote mode");     // publish to the cloud console that mode is on remote mode
        return 1;                                   // return 1 
    }else if(command == "off"){                     // if the argument is "off"
        digitalWrite(swtchPin,LOW);                 // put no signal as an output
        Particle.publish("Mode","Program mode");    // publish to the cloud console that mode is on program mode
        return 0;                                   // return 0
    }else {
        return -1;                                  // if the argument is invalid - return -1
    }
}

int phPump(String command){             // define phPump as particle function with string argument 
    if(command == "on"){                // if the argument is "on"
        digitalWrite(watPump,HIGH);     // turn on the pump 
        return 1;                       // return 1 
    }else if(command == "off"){         // if the argument is "off"
        digitalWrite(watPump,LOW);      // turn off the pump
        return 0;                       // return 0
    }
    return -1;                          // if the argument is invalid - return -1
}
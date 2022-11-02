
/*****************************************************
 * Date: 18 june 2018
 * Written by: Usman Ali Butt
 * Property off: microcontroller-project.com
 * 
 * 
 * * Private mod 
 * Date mod 1 22 2022
 * 
 * 
 * ***************************************************/
#include <ESP8266WiFi.h>
 
//const char* ssid = "Your SSID";
//const char* password = "Your Wifi Password";




int BAT= A0;              //Analog channel A0 as used to measure battery voltage
int Power_of_Heavyweight = D8; // Ouput 
int Led_Power = D5; //LED 

float Voltage = 0.0;
float RatioFactor = 4.233;  //Resistors Ration Factor
float Power = 14.0; // Is on in 14 V
float Switching_Comparison = 0.5; // For 0.5 is of in 13.5 V
String Info_of_switching = " "; 

WiFiServer server(80);

void batery_voltage(){

float Rvalue=0.0; 
float Vvalue=0.0; 

  for(unsigned int i=0;i<10;i++){
    Vvalue=Vvalue+analogRead(BAT);         //Read analog Voltage
    delay(5);                              //ADC stable
    }  

Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
Rvalue=(float)(Vvalue/1024.0)*5;      //Convert Voltage in 5v factor
Voltage=Rvalue*RatioFactor;          //Find original voltage by multiplying with factor

}

void test_batery(){ 

  batery_voltage();

  if (float(Voltage) > Power) {

    digitalWrite( Power_of_Heavyweight, HIGH );
    digitalWrite( Led_Power, HIGH );
    Info_of_switching ="\n <br> Power_of_Heavyweight= HIGH;";

  }
  if(float(Voltage) < ( Power - Switching_Comparison)){

    digitalWrite( Power_of_Heavyweight, LOW );
    digitalWrite( Led_Power, LOW );
    Info_of_switching ="\n <br> Power_of_Heavyweight= LOW;";

  }
 //Serial.println(Info_of_switching);

}

void setup() {
  Serial.begin(9600);
  
  //init ouput  
  pinMode ( Power_of_Heavyweight, OUTPUT );
  pinMode ( Led_Power, OUTPUT );
    
  test_batery();
    
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    test_batery();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  server.begin(); // Start the server
  Serial.println("Server started");
  
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void loop() { 
  test_batery();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("Battery Voltage =");
  client.print(Voltage);
  client.println(Info_of_switching);
  client.println("</html>");
  
}
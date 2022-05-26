#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define SSID "testESP8266"
#define PASWD  "1q2w3e4r5"
int flag = 0;
unsigned long past = 0;
const char* ssid = SSID;
const char* password = PASWD;

const char pageMain[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <iframe src="/buttn" width="100%" height="150" name="up" frameBorder="0"></iframe>
    <iframe src="/text" width="100%" height="90" name="down" frameBorder="0"></iframe>
</body>
</html>
)=====";
 
const char pagebuttn[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8"/>
        <style>
            .frm{
                display: inline-block;
            }
            body{
                text-align: center;        
            }
            a{ 
              color:black;
              text-decoration:none;
              
              }
            
            .frm button{
                
                height: 120px;
                width: 120px;
                font-size : 35px;
                display: inline;
                border-radius: 50%;
            }
        </style>
    </head>
    <body>
        <form  class="frm" id="frm1" method="post" action="/form1">
            <button id="go" name="go" value="급수" onclick="toggle1()"><a id="aTag1" onclick="toggle1()" href="/select1" target="down">급수</a></button>
            
        </form>
        <form class="frm" id="frm2" method="post" action="/form2" >
            <button id="back" value="퇴수" onclick="toggle2()"><a id="aTag2" href="/select2" onclick="toggle2()" target="down">퇴수</a></button>
            
        </form>
        <form class="frm" id="frm3" method="post" action="/form3">
            <button id="stop" value="정지" onclick="toggle3()"><a id="aTag3" href="/select3" onclick="toggle3()" target="down">정지</a></button>
        </form>
       
             
    </body>
    <script>
        
        function toggle1(){
            document.getElementById("go").click();

        }
        function toggle2(){
            document.getElementById("back").click();
        }
        function toggle3(){
            document.getElementById("stop").click();
        }

    </script>
</html>
)=====";

 
const char pageselect1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <style>
            body {
                font-size: 60px;
                text-align: center;
            }
        </style>
    </head>
    <body>
        급수중
    </body>
</html>
)=====";

const char pageselect2[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <style>
            body {
                font-size: 60px;
                text-align: center;
            }
        </style>
    </head>
    <body>
        퇴수중
    </body>
</html>
)=====";

 const char pageselect3[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
    <style>
            body {
                font-size: 60px;
                text-align: center;
            }
        </style>
        
    </head>
    <body>
        정지
    </body>
</html>
)=====";
 
const char pagetext[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
    <style>
            body {
                font-size: 60px;
                text-align: center;
            }
        </style>
    </head>
    <body>
        현재상태
    </body>
</html>
)=====";

ESP8266WebServer server(80);

const int led = 16;

const int Relay1 = D2; //수위센서용
const int Relay2 = D1; //퇴수용
int level = analogRead(A0);

void handleMain() {
  String html = pageMain;
  server.send(200, "text/html", html);
}
 
void handleSecondPage() {
  String html = pagebuttn;
  server.send(200, "text/html", html);
}
 
void handleThridPage1() {
  String html = pageselect1;
  server.send(200, "text/html", html);
}
void handleThridPage2() {
  String html = pageselect2;
  server.send(200, "text/html", html);
}
void handleThridPage3() {
  String html = pageselect3;
  server.send(200, "text/html", html);
}
void handleForthPage() {
  String html = pagetext;
  server.send(200, "text/html", html);
}
void handleRoot() {
  String html = pageMain;
  server.send(200, "text/html",  html);
}
void handleRoot1() {
  String html = pagebuttn;
  server.send(200, "text/html", html);
  
  digitalWrite(Relay1, HIGH);
    
  
}
void handleRoot2() {
  
  String html = pagebuttn;
  server.send(200, "text/html",  html);
  
  digitalWrite(Relay2, HIGH);
    
 
}
void handleRoot3() {
  String html = pagebuttn;
  server.send(200, "text/html",  html);
  
  Serial.println("cc");
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
    
}

void handleNotFound() {



  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 1);
  digitalWrite(Relay1,HIGH);
  digitalWrite(Relay2,HIGH);
  
  
  

}

void setup(void) {
  pinMode(Relay1,OUTPUT); // 급수
  pinMode(Relay2,OUTPUT); // 퇴수
  pinMode(led, OUTPUT);
  
  digitalWrite(led, 1);
  digitalWrite(Relay1,LOW);
  digitalWrite(Relay2,LOW);
  Serial.begin(115200);

  IPAddress ip = IPAddress(192,168,4,20);
  IPAddress gateWay = IPAddress(192,168,4,1);
  IPAddress subNet = IPAddress(255,255,255,0);

  WiFi.softAPConfig(ip,gateWay,subNet);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  
  server.on("/", handleMain);
  server.on("/buttn", handleSecondPage);
  server.on("/select1", handleThridPage1);
  server.on("/select2", handleThridPage2);
  server.on("/select3", handleThridPage3);
  server.on("/text", handleForthPage);
  server.on("/form1", handleRoot1);
  server.on("/form2", handleRoot2);
  server.on("/form3", handleRoot3);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  
  
  int level = analogRead(A0);
  Serial.println("..");
  Serial.println(level);
  delay(500);                                                         
  server.handleClient();

  if(digitalRead(Relay2)){
    unsigned long nnow = millis();
    if(nnow - past >= 20000){
    past = nnow;
    flag = 1;
    }
  if(flag == 1){
    digitalWrite(Relay2,LOW);
    flag = 0;
    }
  }
  if(level >=700){
    digitalWrite(Relay1, LOW);
  }
}

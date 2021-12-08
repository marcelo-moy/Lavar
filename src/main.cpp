#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#define prueba 1
// Set these to your desired credentials....
const char *ssid = "Lavar";
const char *password = "Eficento013";

const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "value";
const char* PARAM_INPUT_3 = "value1";

const int output = 0;
const int output1 = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Mandar cambios del servidor al cliente html
AsyncEventSource events("/events");
//<!-- ... --> para comentar en HTML

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Eficento Control Timer</title>
   <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.4rem;}
    p {font-size: 1rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .slider2 { -webkit-appearance: none; margin: 14px; width: 300px; height: 20px; background: #ccc;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider2::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 30px; height: 30px; background: #2f4468; cursor: pointer;}
    .slider2::-moz-range-thumb { width: 30px; height: 30px; background: #2f4468; cursor: pointer; } 
  </style>
</head>
<body>
  <p style="background-color:#0C767C;text-align: center;" >
    <br>
    <svg xml:space="preserve" width="230%" height="230%" version="1.1" style="shape-rendering:geometricPrecision; text-rendering:geometricPrecision; image-rendering:optimizeQuality; fill-rule:evenodd; clip-rule:evenodd"
     viewBox="0 0 6904 3244"
     <defs>
       <style type="text/css">
         <![CDATA[
           .fil3 {fill:#00CED5}
           .fil1 {fill:#2FA53E}
           .fil6 {fill:#44AB54}
           .fil5 {fill:#99C127}
           .fil7 {fill:#F6F600}
           .fil4 {fill:#FE9A24}
           .fil8 {fill:#FECE00}
           .fil2 {fill:red}
           .fil0 {fill:black;fill-rule:nonzero}
          ]]>
        </style>
      </defs>
      <g id="Eficento">
        <path class="fil0" d="M1799 2879l4 -55 5 -116 3 -114c0,-7 0,-13 0,-20l0 -24 25 0 23 0c-2,40 -4,82 -6,126 -2,70 -4,138 -5,203l-23 0c-3,0 -12,0 -26,0zm116 0c8,-101 12,-182 13,-244l21 0c5,0 12,0 21,0l-3 48c4,-7 7,-12 10,-16 2,-3 5,-7 9,-11 4,-4 9,-8 14,-11 6,-4 12,-7 20,-9 7,-2 14,-4 22,-5 8,-1 15,-2 23,-2 14,0 27,2 40,5 13,4 23,9 31,17 9,7 14,16 17,25 3,10 4,20 4,31 0,2 0,6 0,12 0,6 0,14 -1,23l-5 102c0,9 0,21 -1,35 -7,0 -14,0 -20,0 -8,0 -15,0 -22,0 2,-19 3,-38 4,-56l3 -56c1,-20 1,-34 1,-44 0,-14 -2,-25 -7,-34 -5,-8 -13,-15 -23,-19 -11,-5 -23,-7 -36,-7 -18,0 -34,4 -48,13 -12,8 -21,20 -28,35 -5,11 -8,27 -10,48 -2,25 -4,65 -5,120 -9,0 -16,0 -21,0 -5,0 -13,0 -23,0zm298 0c8,-101 12,-182 12,-244l22 0c5,0 12,0 21,0l-3 48c4,-7 7,-12 9,-16 3,-3 6,-7 10,-11 4,-4 9,-8 14,-11 6,-4 12,-7 19,-9 8,-2 15,-4 23,-5 8,-1 15,-2 23,-2 14,0 27,2 40,5 13,4 23,9 31,17 9,7 14,16 17,25 3,10 4,20 4,31 0,2 0,6 0,12 0,6 -1,14 -1,23l-5 102c0,9 -1,21 -1,35 -7,0 -14,0 -20,0 -8,0 -15,0 -22,0 1,-19 3,-38 3,-56l3 -56c1,-20 2,-34 2,-44 0,-14 -2,-25 -7,-34 -5,-8 -13,-15 -23,-19 -11,-5 -23,-7 -36,-7 -19,0 -35,4 -48,13 -12,8 -22,20 -28,35 -5,11 -8,27 -10,48 -3,25 -4,65 -5,120 -9,0 -16,0 -21,0 -5,0 -13,0 -23,0zm421 -250c17,0 33,3 49,7 15,5 28,13 39,23 11,11 20,24 26,40 6,16 8,33 8,50 0,19 -3,38 -9,55 -6,18 -15,33 -26,45 -11,12 -25,21 -41,27 -16,6 -33,8 -52,8 -17,0 -33,-2 -49,-7 -15,-5 -28,-13 -39,-24 -12,-11 -20,-25 -26,-41 -6,-15 -8,-32 -8,-49 0,-27 5,-50 15,-71 11,-21 27,-37 47,-47 21,-11 43,-16 66,-16zm-1 31c-16,0 -31,4 -45,12 -14,8 -24,21 -31,37 -6,17 -10,34 -10,54 0,17 3,32 9,46 5,13 14,24 27,33 12,8 27,12 45,12 12,0 24,-2 34,-6 11,-4 21,-11 29,-20 8,-10 14,-21 18,-35 4,-14 5,-28 5,-43 0,-14 -1,-28 -5,-40 -4,-12 -10,-22 -18,-30 -8,-7 -17,-13 -27,-16 -10,-3 -20,-4 -31,-4zm236 219l-8 -24c-3,-8 -10,-26 -20,-54l-60 -166c9,0 17,0 23,0 6,0 13,0 21,0l5 16c2,5 4,13 8,24l57 168 76 -185 10 -23c9,0 17,0 22,0 6,0 13,0 23,0l-31 67 -63 140 -16 37c-10,0 -17,0 -23,0 -6,0 -14,0 -24,0zm193 -205c1,-8 3,-18 4,-32 19,-5 36,-8 50,-10 15,-2 29,-3 42,-3 15,0 28,1 41,4 13,3 23,7 31,13 9,6 14,13 17,21 3,7 4,16 4,24 0,11 -1,30 -2,56 -2,26 -3,51 -4,73 -1,22 -1,42 -1,59 -9,0 -16,0 -21,0 -6,0 -13,0 -23,0l5 -66 4 -78c-6,5 -10,8 -13,9 -3,2 -6,4 -9,5 -4,2 -8,3 -12,5 -4,1 -13,3 -25,7 -12,3 -21,6 -26,8 -11,3 -20,7 -26,10 -6,4 -12,8 -16,12 -5,4 -8,8 -9,12 -2,6 -3,11 -3,16 0,9 2,17 7,23 5,7 12,11 20,14 8,2 17,3 28,3 7,0 14,0 21,-1 7,-1 17,-4 30,-8 -3,7 -5,16 -8,30 -12,2 -22,3 -29,4 -7,0 -14,0 -21,0 -22,0 -39,-2 -52,-7 -12,-5 -21,-13 -27,-24 -6,-10 -9,-22 -9,-34 0,-7 1,-14 3,-20 2,-6 5,-12 9,-17 3,-5 9,-10 15,-15 7,-5 14,-9 22,-12 7,-3 16,-7 26,-10 10,-3 26,-7 48,-12 7,-2 13,-4 18,-5 5,-2 9,-4 13,-5 4,-2 8,-4 10,-6 3,-2 6,-5 8,-8 3,-3 5,-6 6,-9 1,-4 1,-7 1,-10 0,-8 -2,-15 -7,-20 -6,-6 -12,-10 -21,-12 -8,-3 -18,-4 -29,-4 -10,0 -21,1 -32,3 -11,2 -21,4 -29,6 -8,2 -17,6 -29,11zm426 168c0,3 -1,8 -2,15 -2,7 -3,14 -4,21 -12,2 -23,4 -34,5 -11,1 -20,1 -28,1 -18,0 -33,-2 -48,-7 -15,-4 -28,-12 -38,-23 -11,-10 -19,-24 -25,-39 -5,-16 -7,-33 -7,-50 0,-24 4,-47 12,-68 9,-21 22,-38 40,-50 18,-12 42,-18 73,-18 23,0 45,3 67,9 1,14 2,25 2,34 -15,-5 -27,-8 -37,-10 -10,-1 -18,-2 -27,-2 -14,0 -26,2 -37,7 -12,6 -21,13 -29,22 -8,10 -14,21 -17,34 -3,13 -5,27 -5,40 0,18 3,34 8,48 6,14 16,24 30,32 14,7 29,11 47,11 17,0 37,-4 59,-12zm67 -260c0,-15 0,-26 0,-34 9,0 17,1 24,1 8,0 16,-1 23,-1 -1,5 -1,17 -2,34 -8,0 -16,0 -22,0 -6,0 -14,0 -23,0zm-12 297l5 -61 5 -104 2 -79 22 0 21 0c-2,25 -3,51 -5,78 -1,28 -2,58 -3,90 -1,33 -2,58 -2,76 -10,0 -17,0 -21,0 -4,0 -12,0 -24,0zm232 -250c17,0 33,3 48,7 16,5 29,13 40,23 11,11 20,24 25,40 6,16 9,33 9,50 0,19 -3,38 -9,55 -6,18 -15,33 -26,45 -11,12 -25,21 -41,27 -16,6 -34,8 -52,8 -17,0 -34,-2 -49,-7 -15,-5 -28,-13 -40,-24 -11,-11 -19,-25 -25,-41 -6,-15 -9,-32 -9,-49 0,-27 6,-50 16,-71 11,-21 26,-37 47,-47 21,-11 43,-16 66,-16zm-1 31c-16,0 -32,4 -45,12 -14,8 -24,21 -31,37 -6,17 -10,34 -10,54 0,17 3,32 8,46 6,13 15,24 27,33 13,8 28,12 46,12 12,0 23,-2 34,-6 11,-4 20,-11 28,-20 9,-10 15,-21 18,-35 4,-14 6,-28 6,-43 0,-14 -2,-28 -6,-40 -3,-12 -9,-22 -17,-30 -8,-7 -17,-13 -27,-16 -11,-3 -21,-4 -31,-4zm-19 -57l23 -63c8,0 16,0 24,0 8,0 16,0 25,0 -11,15 -25,36 -42,63 -5,0 -10,-1 -15,-1 -5,0 -10,1 -15,1zm187 276c8,-101 12,-182 13,-244l21 0c6,0 13,0 21,0l-3 48c4,-7 7,-12 10,-16 2,-3 5,-7 9,-11 4,-4 9,-8 15,-11 5,-4 11,-7 19,-9 7,-2 15,-4 22,-5 8,-1 16,-2 23,-2 14,0 27,2 40,5 13,4 23,9 32,17 8,7 13,16 16,25 3,10 4,20 4,31 0,2 0,6 0,12 0,6 0,14 -1,23l-5 102c0,9 0,21 0,35 -8,0 -14,0 -21,0 -8,0 -15,0 -22,0 2,-19 3,-38 4,-56l3 -56c1,-20 1,-34 1,-44 0,-14 -2,-25 -7,-34 -5,-8 -12,-15 -23,-19 -11,-5 -23,-7 -36,-7 -18,0 -34,4 -48,13 -12,8 -21,20 -28,35 -5,11 -8,27 -10,48 -2,25 -4,65 -5,120 -9,0 -16,0 -21,0 -5,0 -12,0 -23,0zm428 0l6 -91 7 -169 1 -37c0,-10 0,-20 0,-32 37,0 74,0 112,0l76 0 25 0c-1,11 -2,22 -2,33l-22 -1c-5,0 -14,-1 -27,-1 -13,0 -26,0 -38,0 -38,0 -65,0 -80,1l-4 113c25,0 47,1 67,1 21,0 49,-1 84,-1 -1,11 -2,22 -2,33l-83 -1c-17,0 -39,0 -68,1l-4 119 25 1c45,0 75,0 90,-1 15,0 32,-1 52,-2 -1,12 -2,23 -2,34 -39,0 -71,0 -98,0l-115 0zm273 0c8,-101 12,-182 12,-244l22 0c5,0 12,0 21,0l-3 48c4,-7 7,-12 9,-16 3,-3 6,-7 10,-11 4,-4 9,-8 14,-11 6,-4 12,-7 19,-9 8,-2 15,-4 23,-5 8,-1 15,-2 23,-2 13,0 27,2 40,5 12,4 23,9 31,17 8,7 14,16 17,25 3,10 4,20 4,31 0,2 0,6 0,12 0,6 -1,14 -1,23l-5 102c0,9 -1,21 -1,35 -7,0 -14,0 -20,0 -8,0 -15,0 -22,0 1,-19 2,-38 3,-56l3 -56c1,-20 2,-34 2,-44 0,-14 -2,-25 -7,-34 -5,-8 -13,-15 -23,-19 -11,-5 -23,-7 -36,-7 -19,0 -35,4 -48,13 -12,8 -22,20 -28,35 -5,11 -8,27 -10,48 -3,25 -4,65 -5,120 -9,0 -16,0 -21,0 -5,0 -13,0 -23,0zm500 -38c-2,9 -4,20 -6,34 -16,3 -29,6 -40,7 -12,2 -23,2 -34,2 -20,0 -38,-2 -54,-7 -16,-5 -30,-13 -41,-25 -12,-12 -20,-26 -25,-41 -5,-16 -8,-32 -8,-49 0,-24 5,-46 14,-67 9,-20 23,-36 42,-48 18,-12 41,-18 70,-18 14,0 28,2 42,7 13,4 24,10 33,19 9,9 16,19 20,32 5,13 8,27 8,42 0,6 -1,12 -2,17l-184 0c0,9 -1,16 -1,20 0,17 4,33 11,47 7,14 18,25 31,32 14,8 30,11 48,11 21,0 46,-5 76,-15zm-163 -115l144 0c-1,-10 -1,-18 -2,-24 -1,-5 -3,-11 -7,-17 -3,-6 -7,-11 -12,-16 -5,-4 -11,-7 -19,-10 -7,-2 -15,-3 -23,-3 -9,0 -18,1 -26,4 -9,2 -16,6 -22,10 -7,5 -12,10 -16,16 -5,6 -8,12 -10,17 -2,6 -5,13 -7,23zm224 153c6,-93 9,-167 9,-222l-1 -22c9,0 16,0 21,0 6,0 13,0 20,0l-1 40c6,-10 11,-18 15,-23 5,-5 10,-9 16,-12 6,-4 12,-6 20,-8 7,-2 14,-3 22,-3 7,0 14,1 20,3 0,10 1,22 2,35 -10,-3 -19,-4 -27,-4 -9,0 -18,1 -25,4 -8,4 -15,8 -20,15 -7,6 -11,14 -15,23 -4,7 -6,16 -8,27 -1,11 -3,32 -4,62 0,22 0,45 0,69l0 16c-9,0 -16,0 -22,0 -5,0 -12,0 -22,0zm189 71c21,6 39,10 53,13 14,2 27,3 39,3 13,0 26,-2 37,-6 10,-3 18,-7 25,-13 5,-4 9,-10 12,-16 5,-10 8,-21 9,-35 2,-12 3,-34 4,-69 -4,8 -8,13 -11,18 -3,4 -7,9 -12,14 -6,4 -12,9 -19,12 -7,4 -15,7 -25,9 -9,2 -19,3 -28,3 -15,0 -29,-1 -42,-6 -12,-4 -24,-10 -34,-20 -10,-9 -18,-21 -23,-35 -5,-15 -8,-30 -8,-46 0,-21 4,-42 12,-62 8,-19 19,-36 34,-49 15,-13 32,-22 51,-28 19,-5 40,-8 62,-8 25,0 56,3 93,10l-3 30 -7 135c0,3 -1,19 -1,49l-2 50c-1,11 -2,20 -4,28 -2,7 -5,13 -8,19 -3,6 -6,10 -11,15 -4,5 -9,9 -15,12 -5,4 -11,7 -18,9 -9,4 -18,6 -28,7 -11,2 -22,3 -32,3 -16,0 -32,-1 -47,-2 -15,-2 -32,-5 -51,-9l-1 -14c0,-3 0,-10 -1,-21zm185 -285c-18,-3 -34,-5 -50,-5 -16,0 -30,2 -44,5 -13,4 -25,11 -37,21 -11,9 -20,22 -26,38 -6,15 -9,31 -9,48 0,16 3,30 8,43 6,12 15,22 27,29 12,6 25,9 40,9 13,0 25,-2 35,-6 10,-5 19,-12 27,-22 8,-9 15,-21 19,-33 3,-9 5,-23 7,-40 1,-14 2,-43 3,-87zm298 176c-2,9 -4,20 -6,34 -16,3 -29,6 -41,7 -11,2 -22,2 -33,2 -21,0 -39,-2 -55,-7 -16,-5 -29,-13 -41,-25 -11,-12 -19,-26 -24,-41 -5,-16 -8,-32 -8,-49 0,-24 5,-46 14,-67 9,-20 23,-36 41,-48 19,-12 42,-18 70,-18 15,0 29,2 42,7 14,4 25,10 34,19 8,9 15,19 20,32 5,13 7,27 7,42 0,6 0,12 -1,17l-184 0c-1,9 -1,16 -1,20 0,17 4,33 11,47 7,14 17,25 31,32 14,8 30,11 48,11 21,0 46,-5 76,-15zm-163 -115l143 0c0,-10 -1,-18 -2,-24 -1,-5 -3,-11 -6,-17 -3,-6 -7,-11 -12,-16 -5,-4 -12,-7 -19,-10 -7,-2 -15,-3 -24,-3 -9,0 -17,1 -26,4 -8,2 -15,6 -22,10 -6,5 -11,10 -16,16 -4,6 -7,12 -10,17 -2,6 -4,13 -6,23zm55 -123l22 -63c8,0 16,0 25,0 7,0 16,0 25,0 -11,15 -25,36 -42,63 -5,0 -10,-1 -16,-1 -5,0 -10,1 -14,1zm292 244c-2,8 -4,19 -6,32 -11,2 -19,4 -26,4 -6,1 -12,1 -18,1 -14,0 -26,-1 -34,-5 -9,-4 -16,-9 -20,-17 -5,-8 -7,-19 -7,-33 0,-2 0,-5 0,-9 0,-5 1,-20 2,-47l6 -108 -32 0c1,-7 1,-17 2,-29l33 0c0,-9 1,-25 2,-46 15,-3 28,-6 38,-9 -1,24 -1,42 -2,55l67 0c-1,10 -1,19 -1,29l-67 0 -7 136 0 10c0,12 1,20 4,26 3,6 7,10 12,13 5,3 11,4 17,4 4,0 8,0 13,-1 4,-1 13,-3 24,-6zm49 -265c0,-15 0,-26 0,-34 9,0 17,1 24,1 8,0 16,-1 23,-1 -1,5 -1,17 -2,34 -8,0 -16,0 -22,0 -6,0 -14,0 -23,0zm-12 297l5 -61 5 -104 2 -79 22 0 21 0c-2,25 -3,51 -5,78 -1,28 -2,58 -3,90 -1,33 -2,58 -2,76 -10,0 -17,0 -21,0 -4,0 -12,0 -24,0zm289 -37c0,3 -1,8 -2,15 -1,7 -3,14 -4,21 -11,2 -22,4 -33,5 -11,1 -21,1 -29,1 -17,0 -33,-2 -48,-7 -14,-4 -27,-12 -38,-23 -11,-10 -19,-24 -24,-39 -5,-16 -8,-33 -8,-50 0,-24 4,-47 13,-68 8,-21 21,-38 39,-50 18,-12 43,-18 74,-18 22,0 45,3 66,9 1,14 2,25 3,34 -15,-5 -28,-8 -37,-10 -10,-1 -19,-2 -27,-2 -14,0 -27,2 -38,7 -11,6 -21,13 -29,22 -8,10 -13,21 -17,34 -3,13 -5,27 -5,40 0,18 3,34 9,48 5,14 15,24 29,32 14,7 30,11 47,11 17,0 37,-4 59,-12z"/>
        <path class="fil0" d="M6456 2674c1,-8 3,-18 4,-32 19,-5 36,-8 50,-10 15,-2 29,-3 42,-3 15,0 28,1 41,4 13,3 23,7 31,13 9,6 14,13 17,21 3,7 4,16 4,24 0,11 -1,30 -2,56 -2,26 -3,51 -4,73 -1,22 -1,42 -1,59 -9,0 -16,0 -21,0 -6,0 -13,0 -23,0l5 -66 4 -78c-6,5 -10,8 -13,9 -3,2 -6,4 -9,5 -4,2 -8,3 -12,5 -4,1 -13,3 -25,7 -12,3 -21,6 -26,8 -11,3 -20,7 -26,10 -6,4 -12,8 -16,12 -5,4 -8,8 -9,12 -2,6 -3,11 -3,16 0,9 2,17 7,23 5,7 12,11 20,14 8,2 17,3 28,3 7,0 14,0 21,-1 7,-1 17,-4 30,-8 -3,7 -5,16 -8,30 -12,2 -21,3 -29,4 -7,0 -14,0 -21,0 -22,0 -39,-2 -51,-7 -13,-5 -22,-13 -28,-24 -6,-10 -9,-22 -9,-34 0,-7 1,-14 3,-20 2,-6 5,-12 9,-17 3,-5 9,-10 15,-15 7,-5 14,-9 22,-12 7,-3 16,-7 26,-10 10,-3 26,-7 48,-12 7,-2 13,-4 18,-5 5,-2 9,-4 13,-5 4,-2 8,-4 10,-6 3,-2 6,-5 8,-8 3,-3 5,-6 6,-9 1,-4 1,-7 1,-10 0,-8 -2,-15 -7,-20 -6,-6 -12,-10 -20,-12 -9,-3 -19,-4 -30,-4 -10,0 -21,1 -32,3 -11,2 -21,4 -29,6 -8,2 -17,6 -29,11z"/>
      </g>
      <g id="eslogan">
       <path class="fil1" d="M4068 787c160,94 1686,-417 1955,-470 -121,338 -233,348 -368,979 -83,393 -95,176 -171,299 30,45 -5,14 50,41 16,8 71,22 81,23 170,10 145,-293 187,-423 88,-273 119,-554 332,-871 65,-96 139,-97 278,-135 112,-30 243,-35 314,-100 44,-40 48,-99 -7,-125 -59,-29 -420,87 -470,114 -116,61 -139,86 -280,132 -206,67 -414,131 -621,187 -473,128 -740,210 -1211,319l-69 30z"/>
       <path class="fil2" d="M316 1131c11,4 4,6 33,7 19,0 32,-2 53,-6l317 -98c129,-44 308,-77 438,-105 -23,51 -56,78 -101,115 -115,99 -219,270 -340,348 -168,108 -582,94 -716,203 68,36 539,-76 641,-97 -24,129 -252,355 -261,463 181,25 519,-59 720,-81 103,-11 225,79 231,-31 3,-62 -37,-84 -97,-76 -40,5 -151,33 -211,44 -126,23 -337,60 -466,57 19,-103 94,-212 144,-291 78,-121 110,-128 275,-171 180,-46 538,-121 642,-230 -20,-10 -543,149 -722,178 28,-69 41,-53 92,-108l192 -222c22,-30 61,-76 103,-103 68,-44 204,-78 298,-98 127,-27 258,-28 294,-110 -62,-29 -292,54 -392,62 -198,17 -152,34 -404,98 -163,41 -660,168 -763,252z"/>
       <path class="fil3" d="M6713 1251c-51,282 -377,411 -399,137 -14,-169 242,-293 345,-275 79,14 67,64 54,138zm-713 228l167 -8c46,206 244,321 455,172 93,-65 303,-321 280,-512 -33,-271 -481,-223 -739,105 -66,83 -154,126 -163,243z"/>
       <path class="fil4" d="M2012 1753c22,-57 197,-292 301,-314 77,-16 82,10 113,-65 -101,-83 -223,32 -297,91l-230 233c-121,135 -274,192 -460,245 -166,48 -127,44 -144,86 157,10 250,-51 394,-64 -66,175 -483,758 -490,1160 -2,104 6,112 105,119 -46,-203 17,-297 79,-481 82,-243 242,-583 396,-772 172,-211 331,-85 398,-190 -41,-79 -55,-56 -165,-48z"/>
       <path class="fil5" d="M3928 1664l-1 141c77,62 130,181 294,119 127,-49 183,-97 243,-209 -23,-39 -15,-25 -22,-85 -67,49 -217,249 -360,172 -78,-82 166,-261 249,-337 85,-77 298,-233 264,-290 -9,-15 -1,-53 -121,-54 -38,-1 -90,7 -127,17 -318,77 -328,299 -419,526zm186 -170c85,-44 219,-182 265,-268 -123,21 -242,156 -265,268z"/>
       <path class="fil6" d="M4442 1630c7,60 -1,46 22,85 93,-76 288,-410 370,-444 22,474 -131,343 -118,532 138,71 165,-113 240,-227 76,-116 130,-208 220,-303 9,83 -13,215 -43,292 -30,80 -109,153 12,213 64,-39 48,-60 65,-156 31,-180 138,-428 9,-541 -135,35 -193,326 -310,412 33,-134 80,-319 -54,-380 -89,-10 -404,499 -413,517z"/>
       <path class="fil7" d="M3927 1805l1 -141c-195,105 -854,279 -786,-45 15,-68 66,-122 117,-159 302,-216 243,-90 427,-118 217,-33 86,-341 -418,-29 -102,63 -260,204 -257,368 5,395 722,184 916,124z"/>
       <path class="fil8" d="M2585 2008c60,-10 65,-25 90,-84 15,-34 27,-74 40,-115 31,-94 54,-168 61,-271 -86,48 -239,343 -191,470z"/>
       <path class="fil8" d="M2869 1192c42,-3 67,-13 102,-38 37,-26 60,-45 75,-75 -69,-28 -190,17 -177,113z"/>
      </g>
    </svg>
  </p>
  
  <p>Control temporizado 
  de marcha y 
  contramarcha</p>
  <p>Tiempo de Marcha/contramarcha <span id="timerValue">%TIMERVALUE%</span> s</p>
  <p><input type="range" onchange="updateSliderTimer(this)" id="timerSlider" min="1" max="180" value="%TIMERVALUE%" step="1" class="slider2"></p>
  <p>Tiempo de Pausa <span id="timerValue1">%TIMERVALUE1%</span> s</p>
  <p><input type="range" onchange="updateSliderTimer1(this)" id="timerSlider1" min="1" max="60" value="%TIMERVALUE1%" step="1" class="slider2"></p>
  %BUTTONPLACEHOLDER%
   <p><span id="mtiempo"></span>s Estado <span id="maqestado">%MESTADO%</span> </p>

  <p style="text-align: center;" >
    <svg version="1.1"
	  id="svg-spinner"
	  xmlns="http://www.w3.org/2000/svg"
	  xmlns:xlink="http://www.w3.org/1999/xlink"
	  x="0px"
	  y="0px"
	  viewBox="0 0 80 80"
	  xml:space="preserve"
    width="150%" height="150%"
	>
	  <path
	    id="spinner"
	    fill="%STATECOLOR%"
	    d="M40,72C22.4,72,8,57.6,8,40C8,22.4, 22.4,8,40,8c17.6,0,32,14.4,32,32c0,1.1-0.9,2-2,2 s-2-0.9-2-2c0-15.4-12.6-28-28-28S12,24.6,12,40s12.6, 28,28,28c1.1,0,2,0.9,2,2S41.1,72,40,72z"
	  >
	  <animateTransform
      id="mover"
	    attributeType="xml"
	    attributeName="transform"
	    type="rotate"
      from="-90 40 40"
	    to="%ANGULO% 40 40"
	    
	    dur="0.8s"
	    repeatCount="indefinite"
	  >
	  </path>
</svg>
</p>

<script>
var nIntervId;

function toggleCheckbox(element) {
  document.getElementById("Estado").disable=true;
  var xhr = new XMLHttpRequest();
  if(element.checked){ 
    xhr.open("GET", "/update?state=1", true); 
    xhr.onload = function (e) {
    if (xhr.readyState === 4) {
      if (xhr.status === 200) {
       console.log(xhr.responseText);
       } else {
       console.error(xhr.statusText);
     }
   }
  };
  xhr.onerror = function (e) {
   console.error(xhr.statusText);
  };
  xhr.send();
   //document.getElementById('togglee').style.visibility = 'hidden';
   //document.getElementById("spinner").style.fill = '#00CED5';
    //var intervalID = window.setInterval(Actualizadatos, 1000);

  /*
    var count = sliderValue, timer = setInterval(function()  {
      count--; document.getElementById("timerValue").innerHTML = count;
      if(count == 0){ 
        clearInterval(timer); document.getElementById("timerValue").innerHTML = document.getElementById("timerSlider").value; 
        
       }
    }, 1000);
    sliderValue = sliderValue*1000;
    
    setTimeout(function(){ xhr.open("GET", "/update?state=0", true); 
    document.getElementById(element.id).checked = false; xhr.send(); }, sliderValue);
    */ 
    
  }else{
    xhr.open("GET", "/update?state=0", true);
    document.getElementById(element.id).checked = false; xhr.send();
   // document.getElementById("spinner").style.fill = '#9c9c9c';
  } 
  
}
function updateSliderTimer(element) {
  var sliderValue = document.getElementById("timerSlider").value;
  document.getElementById("timerValue").innerHTML = sliderValue;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
function updateSliderTimer1(element) {
  var sliderValue1 = document.getElementById("timerSlider1").value;
  document.getElementById("timerValue1").innerHTML = sliderValue1;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value1="+sliderValue1, true);
  xhr.send();
}
function Iniciazar(){
  if(document.getElementById("Estado").checked == false){
     document.getElementById("spinner").style.fill = '#9c9c9c';
     document.getElementById("spinner").animateTransform.to = "-270 40 40";
     //document.getElementById('spinner').style.visibility = 'hidden';
  }else{
    document.getElementById("spinner").style.fill = '#00CED5';
    
  }
}
window.onload = Iniciazar();
//function Actualizadatos(){

//}
const angulos = [ "-90", "-90", "280","-90","-450"];
const Vestados = [ "Proceso Detenido ", "Pausa pre M", "Marcha","Pausa pre C","Contramarcha"];

if (!!window.EventSource) {
 var source = new EventSource('/events');
 source.addEventListener('open', function(e) {
   console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
   if (e.target.readyState != EventSource.OPEN) {
     console.log("Events Disconnected");
    }
  }, false);
  source.addEventListener('message', function(e) {
  console.log("message", e.data);
  }, false);
  source.addEventListener('mestado', function(e) {
   console.log("mestado", e.data);
   document.getElementById("maqestado").innerHTML = Vestados[e.data-1]
   if (e.data==1){
        document.getElementById("spinner").style.fill = '#9c9c9c';
    }else{
        document.getElementById("spinner").style.fill = '#00CED5';
    }
  
   document.getElementById("mover").setAttribute("to",angulos[e.data-1]+" 40 40");
  }, false);

  source.addEventListener('muestratiempo', function(e) {
   console.log("mtiempo", e.data);
   document.getElementById("mtiempo").innerHTML = e.data;
    
  }, false);
  evtSource.onerror = function(e) {
  alert("EventSource failed.");
};
}
</script>
</body>
</html>
)rawliteral";

/* Estructura que carga y guarda en eeprom */

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#include <ESP8266TimerInterrupt.h>
#define TIMER_INTERVAL_MS        1000   //1000
volatile uint32_t TimerCount = 0;
String estado = "1"; //0 apago 1 encendido
String timerSliderValue = "10";
String timerSliderValue1 = "1";
int tempo;
int tempo1;

struct config_t {
    uint8_t estadog;
    uint8_t Tmarchag;
    uint8_t Tpausag;
    uint8_t TTotal;
} config;

void eepromsave() {
    EEPROM.begin(sizeof(config));
    EEPROM.put(0, config); //Guardo la config
    EEPROM.end();
}
//Cargo de la eeprom el struct config
void eepromload() {
    EEPROM.begin(sizeof(config));
    EEPROM.get(0, config); //Cargo config
    EEPROM.end();
}
//Inicializo valores para eeprom
void eeprominit() {
       config.estadog= 0;
       config.Tmarchag=10;
       config.Tpausag=10;
       config.TTotal=1;
    //Guardo valores por defecto en la eeprom 
    eepromsave(); 
}


ESP8266Timer ITimer;

void printResult(uint32_t currTime)
{
 Serial.print(F("Time = ")); Serial.print(currTime); 
 Serial.print(F(", TimerCount = ")); Serial.println(TimerCount);
}
void TimerHandler()
{
  static bool toggle = false;

  // Flag for checking to be sure ISR is working as Serial.print is not OK here in ISR
  TimerCount++;

  //timer interrupt toggles pin LED_BUILTIN
  //digitalWrite(LED_BUILTIN, toggle);
  //digitalWrite(output, toggle);
  
  toggle = !toggle;
  //estado=toggle;
  //Serial.print(F("\nEstado=")); Serial.print(estado); 
}

String outputState(){
  
  if(estado=="0" || estado=="9"){
    return "";
  }
  else {
    return "checked";
  }
    return "";
}

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    
    String outputStateValue = outputState();
    
    buttons+= "<p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"Estado\" " + outputStateValue + "><span class=\"slider\"></span></label></p>";
    return buttons;
  }
  else if(var == "TIMERVALUE"){
    return timerSliderValue;
  }
  else if(var == "TIMERVALUE1"){
    return timerSliderValue1;
  }
  else if(var == "STATECOLOR"){
     if(estado=="0" || estado=="9")
     return "#9c9c9c";
    else 
      return "#00CED5";
  }
  else if(var == "ANGULO"){
    if (estado == "3") 
    return "280";
    else if(estado == "5") {
      return "-450";
    } else return "-90";
    
  }
  else if(var == "MESTADO"){
    return estado;
  }
  return String();
}

void setup() {
  //Dejar solo la primera vez para que se graven los datos y no sea 0000000
  //eepromsave();
 
  digitalWrite(output, HIGH);
  pinMode(output, OUTPUT);
  digitalWrite(output1, HIGH);
  pinMode(output1, OUTPUT);
  
  delay(1000); //Agregado para que despues del reset pase un tiempo y el serial no sufra jiter

  // Serial port for debugging purposes
  Serial.begin(115200);
  while (!Serial);
  
  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Configurando Punto de Acceso...");
// You can remove the password parameter if you want the AP to be open. 
  Serial.print(ssid);
  Serial.print(password);
  delay(2000);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP Dirección IP: ");
  Serial.println(myIP);
  //Serial.print("\nMOde del puerto 2=%i",
  // Print ESP Local IP Address
  //Serial.println(WiFi.localIP());
 
  //para entender timer
  Serial.print(F("\nStarting Change_Interval on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP8266_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); 
  Serial.println(F(" MHz"));
  
   // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    Serial.print(F("Starting  ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
  

  #define CHECK_INTERVAL_MS     10000L
  #define CHANGE_INTERVAL_MS    20000L

  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
     // digitalWrite(output,inputMessage.toInt());
      Serial.printf("\n%s=",PARAM_INPUT_1); Serial.print(inputMessage); 
      estado = inputMessage;
      if (estado=="0"){
        config.estadog=0;
        digitalWrite(output,HIGH);
        digitalWrite(output1,HIGH);
        eepromsave();
      }
    }
    else {
      inputMessage = "No message sent";
    }
    //Serial.println(inputMessage);
    
    request->send(200, "text/plain", "OK");
  });
  
  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      timerSliderValue = inputMessage;
    }else if (request->hasParam(PARAM_INPUT_3)){
        inputMessage = request->getParam(PARAM_INPUT_3)->value();
      timerSliderValue1 = inputMessage;
    }else  {
      inputMessage = "No message sent";
    }
    //Serial.println(inputMessage);
    int args = request->args();
    for(int i=0;i<args;i++){
      Serial.printf("%s=%s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
    request->send(200, "text/plain", "OK");
  });
  //para manejar eventos y mandar cosas del server al cliente
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  server.addHandler(&events);
  // Start server
  server.begin();

  eepromload();

   estado = String(config.estadog);
   timerSliderValue = String( config.Tmarchag);
   timerSliderValue1 = String( config.Tpausag);

  //para luego agregar el tiempo total
  //uint8_t TTotal;

}

void loop() {
  
  // put your main code here, to run repeatedly:

  static uint32_t lastTime = 0;
  static uint32_t lastTimedisplay = 0;
  static uint32_t lastChangeTime = 0;
  static uint32_t currTime;
  static uint32_t multFactor = 0;
  static uint16_t tiempo_de_pausa=1; //en segundos
  static uint16_t tiempo_de_marcha=10;
  volatile static uint8_t marcha=0;
  volatile static uint8_t contramarcha=0;
  static uint32_t ftime = 0;
  String sftime="0";
  currTime = millis();
   if (estado == "0") //Empieza con la pausa
  { 
    TimerCount=0;
    marcha=0;
    contramarcha=0;
    lastTime = TimerCount;
    events.send("1","mestado",millis());
    estado="9";
  }

  if (estado == "1") //Empieza con la pausa
  { 
    estado="2";
    TimerCount=0;
    marcha=0;
    contramarcha=0;
    lastTime = TimerCount;
    Serial.print(F("\nTiempo anterior de Pausa [Seg]=")); Serial.println(tiempo_de_pausa);
    if (tiempo_de_pausa == timerSliderValue1.toInt()){
      Serial.print(F("\nEs igual a"));
    }else
     Serial.print(F("\nNo es lo mismo a"));

    tiempo_de_pausa= timerSliderValue1.toInt();
    Serial.print(F("\nTiempo de Pausa [Seg]=")); Serial.println(tiempo_de_pausa);
    
    Serial.print(F("\nTiempo anterior de Marcha [Seg]=")); Serial.println(tiempo_de_marcha);
    if (tiempo_de_marcha == timerSliderValue.toInt()){
      Serial.print(F("\nEs igual a"));
    }else{
      Serial.print(F("\nNo es lo mismo a"));
    }
    tiempo_de_marcha= timerSliderValue.toInt();
    Serial.print(F("\nTiempo de Marcha [Seg]=")); Serial.println(tiempo_de_marcha);
      // hay que grabar de todos modos porque hace falta el estado
      config.Tmarchag=tiempo_de_marcha;
      config.Tpausag=tiempo_de_pausa;
      config.estadog=1;
      eepromsave();
      events.send(estado.c_str(),"mestado",millis());
  }

  if (estado == "2") //Pausa Esperando para marcha
  { 
      
    if (TimerCount - lastTime > tiempo_de_pausa)
    { 
      lastTime = TimerCount;
      estado="3";
      digitalWrite(output,LOW);
      marcha=1;
      events.send(estado.c_str(),"mestado",millis());
    }
  }    
  if (estado == "3") //Marcha Esperando para pausa
  { 
    if (TimerCount - lastTime > tiempo_de_marcha)
    { 
      lastTime = TimerCount;
      estado="4";
      marcha=0;
      digitalWrite(output,HIGH);
      digitalWrite(output1,HIGH);
      contramarcha=0;
      events.send(estado.c_str(),"mestado",millis());
    }
  }    
  if (estado == "4") //Parado Esperando para contramarcha
  { 
    if (TimerCount - lastTime > tiempo_de_pausa)
    { 
      lastTime = TimerCount;
      estado="5";
      contramarcha=1;
      digitalWrite(output1,LOW);
      events.send(estado.c_str(),"mestado",millis());
    }
  }    
  if (estado == "5") //Contramarcha Esperando para pausa
  { 
    if (TimerCount - lastTime > tiempo_de_marcha)
    { 
      lastTime = TimerCount;
      estado="2";
      contramarcha=0;
      digitalWrite(output1,HIGH);
      events.send(estado.c_str(),"mestado",millis());
    }
  }    
  if (TimerCount - lastTimedisplay > 1)
  {
   lastTimedisplay=TimerCount;
   if ((estado=="5")||(estado=="3")){
     ftime= tiempo_de_marcha-(TimerCount-lastTime);
   } else if ((estado=="2")||(estado=="4")){
     ftime= tiempo_de_pausa-(TimerCount-lastTime);
   } else {ftime=0;}
   if (ftime>tiempo_de_marcha) ftime=tiempo_de_marcha;
  sftime=String(ftime);
      events.send(sftime.c_str(),"muestratiempo",millis());
      
    Serial.print(F("\n")); Serial.print(TimerCount-lastTime); 
     Serial.print(F("/")); Serial.print(lastTime); 
     Serial.print(F("/")); Serial.print(TimerCount); 
    Serial.print(F(" Est ")); Serial.print(estado); 
      
    if (contramarcha==1) Serial.print(F(" <- "));
    else Serial.print(F("    "));
    if (marcha==1) Serial.print(F("->"));
    else Serial.print(F("  "));
    
  }
  
}
//Ideas
/*

chequear errores de comunicacion
chequear sui se envio antes de enviar
corregir tiempo para que sea de a 1

agragar logueo

agregar que la pagina sepa si esta conectada al dispositivo
y en todo caso hacer retries si se corta

*/
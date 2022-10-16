#include <Arduino.h>
#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define pinSensor1 7 //pino que ligou o sensor de presenca
#define CLK 2 //pino do display
#define DIO 3 //pino do display
#define PIN  6 // Pino do led
#define NUMPIXELS 30 // quantidade de led da fita
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//define variable
int frequencia = 10;
int contTime = 0;
int cont = 0;
int contAcendimentoLed=0;
int prepara = 0;
int led = 0;
int inicio_led= 2000;
int t_acedimento = 20;
int t1 = 0;
int t2 = 0;
int t3 = 0;
int tm = 0;
int goal = 1000;

//start display
TM1637Display display(CLK, DIO);
void setup() {
  pinMode(pinSensor1, INPUT); //ler o pino do sensor como entrada
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
 clock_prescale_set(clock_div_1);
#endif
// END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  display.setBrightness(0x0f);
  bool estadoSensor1 = digitalRead(pinSensor1);
  
 
  if (estadoSensor1==0 && cont ==0) {        
      cont = 1;
      t1 = contTime;
      prepara = 0;
      led = 0;
      Serial.println("Inicio");
      for(int i=0; i<NUMPIXELS; i++) {     
         pixels.setPixelColor(i, pixels.Color(0, 150, 0));
         pixels.show();
         prepara = 1;             
       }
            
   }      
   if (estadoSensor1==1 && cont == 1) {
      t2 = contTime;       
      if ((t2-t1)>1000){   
        //Serial.println("Retirou 1");
        cont = 3;
      }            
   }
   if (estadoSensor1==0 && cont ==3) {        
      cont = 4;
      t3 = contTime;
      //Serial.print("TM: ");
      tm =  t3-t2;
      display.showNumberDec((inicio_led + ((t_acedimento*1.73)*NUMPIXELS))-(contTime-t1), false);
      //Serial.println(goal-tm);
      
   } 
   if (estadoSensor1==1 && cont == 4) {      
      //t2 = contTime;
      //Serial.println("Fim");      
      cont = 0;
      prepara = 0;
      led = 0;    
                 
   }

    //Fazer os leds correr.
   if ((contTime-t1) > inicio_led && (contTime-t1) < (inicio_led + ((t_acedimento*1.73)*NUMPIXELS))){
      if (led==0){
        //Apagar os leds
        for(int i=0; i<NUMPIXELS; i++) {     
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            pixels.show();        
                             }
                  } 
      ///acender os leds em sequencia        
      if (led <= NUMPIXELS && prepara == 1){
          if (contAcendimentoLed <= (t_acedimento/frequencia) && prepara == 1){
                contAcendimentoLed=contAcendimentoLed+1;
                pixels.setPixelColor(led, pixels.Color(255, 0, 0));
                pixels.show();
                   
                    }
          if (contAcendimentoLed >= (t_acedimento/frequencia) && contAcendimentoLed < ((t_acedimento/frequencia)*2) && prepara == 1) {
                contAcendimentoLed=contAcendimentoLed+1;
                pixels.setPixelColor(led, pixels.Color(0, 0, 0));
                pixels.show(); 
                  }
          if (contAcendimentoLed >= ((t_acedimento/frequencia)*2) && prepara == 1) {
                contAcendimentoLed=0;
                led = led+1;       
                }
     
            }
      if (led > NUMPIXELS && prepara == 1){
        //display.showNumberDec((contTime-t1), false);        
        prepara = 0;        
             }
      }

     
        
   contTime = contTime + frequencia;
  //Serial.println(contTime-t1);
  delay(frequencia);
   
   
}

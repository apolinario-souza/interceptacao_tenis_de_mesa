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


// Se quiser mudar algo é aqui
int t_acedimento = 30; //tempo que o led fica aceso, se diminuir fica mais rápido 30 me parece bom
int frequencia = 10;
int inicio_led= 2000; //intervalo entre o posicionamento da raquete e início dos leds


//define variable
int contTime = 0;
int cont = 0;
int contAcendimentoLed=0;
int prepara = 0;
int led = 0;
int tm = 0;
int t1 = 0;
int t2 = 0;
int t3 = 0;

//start display
TM1637Display display(CLK, DIO);
void setup() {
  pinMode(pinSensor1, INPUT); //ler o pino do sensor como entrada
  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
 clock_prescale_set(clock_div_1);
#endif
// END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  display.setBrightness(0x0f);
  bool estadoSensor1 = digitalRead(pinSensor1); //pega o valor do estado do sensor de presença
  
 // Pega o primeiro momento que colocou a raquete e set tudo e inicia a contagem
  if (estadoSensor1==0 && cont ==0) {        
      cont = 1;
      t1 = contTime;
      prepara = 0;
      led = 0;
     //acende os leds amarelos
     for(int i=0; i<NUMPIXELS; i++) {     
         pixels.setPixelColor(i, pixels.Color(204, 204, 0));
         pixels.show();
         prepara = 1;             
       }
            
   } 

   //Pega primeiro momento que retirou a raquete       
   if (estadoSensor1==1 && cont == 1) {
      t2 = contTime;       
      if ((t2-t1)>1000){ //Estipulei um tempo mínimo de 1s entre o primeiro momento que coloco a requete e retiro a 1ª vez. 
        cont = 3;
      }            
   }

   // Quando passa pela segunda vez no sensor
   if (estadoSensor1==0 && cont ==3) {        
      cont = 4;
      t3 = contTime;      
      tm =  t3-t2;
      display.showNumberDec((inicio_led + ((t_acedimento*1.73)*NUMPIXELS))-(contTime-t1), false); 
      //Meta menos o tempo que passou na 2ª vez pelo sensor    
      
   } 
   // Tempo que saiu pela 2ª vez no sensor
   if (estadoSensor1==1 && cont == 4) {       
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
        prepara = 0;        
             }
      }
      //acender os leds verdes após 3 segundos da ação 
       if ((contTime-t1) > (inicio_led + ((t_acedimento*1.73)*NUMPIXELS))+3000) {        
        for(int i=0; i<NUMPIXELS; i++) {     
            pixels.setPixelColor(i, pixels.Color(0, 150, 0));
            pixels.show();        
                             }
        
        }

     
        
  contTime = contTime + frequencia;  
  delay(frequencia);
   
   
}

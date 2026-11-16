// The Trooper - version LCD I2C + buzzer pasivo (sin rele)
// Adaptado de la transcripcion de Roco (Rocoblog):
// https://rocoblog.blogspot.com/2022/04/musica-con-arduino-uno-y-un-buzzer.html
// Libreria: "LiquidCrystal I2C" de Frank de Brabander (Administrar bibliotecas)
//
// Conexiones (Arduino UNO):
//   LCD I2C : GND -> GND | VCC -> 5V | SDA -> A4 | SCL -> A5
//   Buzzer  : (+) -> D12 | (-) -> GND
//   "Rele"  : reemplazado por el LED de la placa (D13), parpadea al ritmo
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR 0x27   // si la pantalla queda en blanco, probar 0x3F
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

const int PIN_LUZ = LED_BUILTIN;  // antes RELAY_PIN = 4; con un rele real, poner 4

int pinBuzzer = 12;
  int Estado = LOW;

int C_ =  261/2;
int Cs_=  277/2;
int D_ =  293/2 ;
int Ds_=  311/2;
int E_ =  329/2 ;
int F_ =  349/2 ;
int Fs_=  369/2;
int G_ =  391/2 ;
int Gs_=  415/2;
int A_ =  440/2 ;
int As_=  466/2;
int B_ =  493/2 ;


int C =  261;
int Cs=  277;
int D =  293 ;
int Ds=  311;
int E =  329 ;
int F =  349 ;
int Fs=  369;
int G =  391 ;
int Gs=  415;
int A =  440 ;
int As=  466;
int B =  493 ;



int C2   =524;
int Cs2  =555;
int D2   =588;
int Ds2  =623;
int E2   =660;
int F2   =699;
int Fs2  =740;
int G2   =784;
int Gs2  =831;
int A_2   = 880;
int As2  =933;
int B2   =988;


int  C3   =524*2;
int Cs3   =555*2;
int  D3   =588*2;
int Ds3   =623*2;
int  E3   =660*2;
int  F3   =699*2;
int Fs3   =740*2;
int  G3   =784*2;
int Gs3   =831*2;
int A_3   =880*2;
int As3   =933*2;
int  B3   =988*2;

int  C4   =524*4;
int Cs4   =555*4;
int  D4   =588*4;
int Ds4   =623*4;
int  E4   =660*4;
int  F4   =699*4;
int Fs4   =740*4;
int  G4   =784*4;
int Gs4   =831*4;
int A_4   =880*4;
int As4   =933*4;
int  B4   =988*4;


int tempo = 158;  
int semi = 60000/( tempo * 4 *2);

int corch = 2*semi;
int negra = corch*2;
int np = corch*3;

int blanca = negra*2;
int redonda = blanca*2;
int rep = 3*negra;
int bnp = 3*negra+3*corch;

int b =blanca;
int r =2*b;
int n = negra;
int c = corch;
int s = semi;
int f = s/2;
int sf = f/2;

int cp = c+s;
int Sil = 10;

int retardo = 115;

void nota(int nota, int duracion){
  tone(pinBuzzer,nota, duracion);
  delay(duracion);
  noTone(pinBuzzer);
  delay(duracion);
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   THE TROOPER  ");
  lcd.setCursor(0,1);
  lcd.print("   Iron Maiden  ");

  pinMode(PIN_LUZ, OUTPUT);
  delay(500);
}

void loop() {

Intro_1();
Punteo();
delay(1500);
}


void Intro_1(){
  // [1]

  suenaRele();
  nota(E2,c);
  nota(E2,c);
  
  nota(E2,s);
  nota(D2,s);
  nota(B,c);

  nota(D2,c);
  nota(D2,c);

  nota(D2,s);
  nota(C2,s);
  nota(A,c);

  // [2]
  nota(C2,c);
  nota(C2,c);

  nota(C2,s);
  nota(B,s);
  nota(G,c);

              suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(G2,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(E2,n);

  // ..[3]
  //nota(E2,c);

  
  nota(E2,c);
  
  nota(E2,s);
  nota(D2,s);
  nota(B,c);

  nota(D2,c);
  nota(D2,c);

  nota(D2,s);
  nota(C2,s);
  nota(A,c);

  // [4]
  nota(C2,c);
  nota(C2,c);

  nota(C2,s);
  nota(B,s);
  nota(G,c);
  
             suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(G2,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(E2,n);

  // ..[5]
  //nota(E2,c);
  nota(E2,c);
  
  suenaRele();  // ---
  nota(E2,s);
  nota(D2,s);
  nota(B,c);

suenaRele();  // ---
  nota(D2,c);
  nota(D2,c);

suenaRele();  // ---
  nota(D2,s);
  nota(C2,s);
  nota(A,c);

  // [6]
  suenaRele();  // ---
  nota(C2,c);
  nota(C2,c);

suenaRele();  // ---
  nota(C2,s);
  nota(B,s);
  nota(G,c);
  
             suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(G2,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(E2,n);
  
   
  // ..[7]
  //nota(E2,c);
  nota(E2,c);

  suenaRele();  // ---
  nota(E2,s);
  nota(D2,s);
  nota(B,c);

suenaRele();  // ---
  nota(D2,c);
  nota(D2,c);

suenaRele();  // ---
  nota(D2,s);
  nota(C2,s);
  nota(A,c);

  // [8]
  nota(C2,c);
  nota(C2,c);

  nota(C2,s);
  nota(B,s);suenaRele();  // ---
  nota(G,s);suenaRele();  // ---
  nota(Sil,s);

  suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(G2,c);suenaRele();  // ---
  nota(D2,c);suenaRele();  // ---
  nota(E2,n);
  
   }

void Punteo(){
  // [9]
  //nota(E2,c);suenaRele();  // ---
  nota(G3,c);

  suenaRele();  // ---
  
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);


  suenaRele();  // ---
  nota(B3,c);
  nota(G3,c);

  suenaRele();  // ---
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);

  // [10]
  Tararara();
  
  // [11]
  Rapida();
 
  // [12]
  Tararara();
  
  // [13]
  Rapida();

  // [14]
  Tararara();
  // [15]
  Rapida();
  // [16]
  Tararara_();
  // [17]
  Rapida();
  // [18]
  Tararara();
  // [19]
  Rapida();
  // [20]
  Tararara();
  // [21]
  Rapida();
  // [22]
  Tararara();
  // [23]
  Rapida();
  // [24]
  Tararara_();
    
  };
   

void Rapida(){
 // [11]  
  suenaRele();  // ---
  nota(G3,c);

  suenaRele();  // ---
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);

  suenaRele();  // ---
  nota(B3,c);
  nota(G3,c);

  suenaRele();  // ---
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);
  nota(Fs3,n/6);
  nota(G3,n/6);

  }


  void Tararara(){
  suenaRele();  // ---
  nota(G3,c);
  nota(E3,c);
  
  suenaRele();  // ---
  nota(E3,n);

  suenaRele();  // ---
  nota(D3,c);suenaRele();  // ---
  nota(G3,c);suenaRele();  // ---
  nota(D3,c);suenaRele();  // ---
  nota(E3,n);
     }

void Tararara_(){
  suenaRele();  // ---
  nota(G3,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(E3,s);suenaRele();  // ---
  nota(Sil,s);
  
  suenaRele();  // ---
  nota(E3,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  nota(Sil,s);

  suenaRele();  // ---
  nota(D3,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  
  nota(G3,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  
  nota(D3,s);suenaRele();  // ---
  nota(Sil,s);suenaRele();  // ---
  
  nota(E3,n);
  
     }

// Mantiene el nombre original: alterna la "luz" (LED D13 / rele) en cada llamada
void suenaRele(){
  if ( Estado == LOW ){
    Estado = HIGH;
  }
  else {
    Estado = LOW;
  }
  digitalWrite(PIN_LUZ, Estado);
}

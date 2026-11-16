// Alma Misionera - version LCD I2C (backpack PCF8574) + buzzer pasivo
// Libreria: "LiquidCrystal I2C" de Frank de Brabander (Administrar bibliotecas)
//
// Conexiones (Arduino UNO):
//   LCD I2C : GND -> GND | VCC -> 5V | SDA -> A4 | SCL -> A5
//   Buzzer  : (+) -> D12 | (-) -> GND
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR 0x27   // si la pantalla queda en blanco, probar 0x3F
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

int pinBuzzer = 12;


int C__=  261/4;
int Cs__=  277/4;
int D__ =  293/4 ;
int Ds__=  311/4;
int E__ =  329/4 ;
int F__ =  349/4 ;
int Fs__=  369/4;
int G__ =  391/4 ;
int Gs__=  415/4;
int A__ =  440/4 ;
int As__=  466/4;
int B__ =  493/4 ;


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


int tempo =90;  
int semi = 60000/( tempo * 4 *2);

int corch = 2*semi;
int negra = corch*2;
int np = corch*3;

int blanca = negra*2;
int redonda = blanca*2;
int rep = 3*negra;
int bnp = 3*negra+3*corch;

int b =blanca; int r =2*b;
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


byte laDuda[8] = {
  B00000,
  B00100,
  B00000,
  B00100,
  B01000,
  B10000,
  B10001,
  B01110
};

byte Exclama[8] = {
  B00000,
  B00100,
  B00000,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};
void setup() {
 lcd.init();
 lcd.backlight();

 lcd.createChar(7, laDuda);
 lcd.createChar(8, Exclama);

 lcd.setCursor(0,0);
 lcd.print("      O O       ");
 lcd.setCursor(0,1);
 lcd.print("       -        ");

 delay(1900);
}
void loop() {

Llevame();
DondeFalte();
 delay(n*10);
}


// fa sol do re
void Llevame(){
//  Fa # - Do #   
textopan("      /         ","Llevame         ");
  nota(F2 ,c);
  nota(G2 ,c);
  nota_sf(A_2,c);
textopan("donde           ","los hombres     ");
  nota(A_2,c);
  nota(As2,c);
  nota(A_2,s);
  nota_sf( G2,cp);
  nota( G2,n);
textopan("Necesiten tus   ","palabras        ");
  nota(A_2,s);
  nota( G2,s);
  nota_sf( F2,c);
  nota( D2,c);
  nota( F2,c);
  nota( D2,s);
  nota_sf( F2,cp);
  nota( D2,c);

textopan("necesiten       ","                ");
  nota( F2,c);
  nota( G2,s);
  nota_sf(A_2,s+c);
  nota( F2,n+c);
  nota(Sil,s);
textopan("mis ganas       ","de vivir        ");
  nota(C2 ,s);
  nota(A_2,s);
  nota(A_2,s);
  nota( G2,c);
  nota( F2,s);
  nota( G2,s+3*n);
}
void DondeFalte(){
//  Fa # - Do #   
textopan("Donde falte     ","                ");
  nota(F2 ,c);
  nota(G2 ,c);
  nota(A_2,c);
  nota(A_2,c);
textopan("Donde falte     ","la esperanza    ");
  nota(As2,c);
  nota(A_2,s);
  nota( G2,cp);
  nota( G2,n);
textopan("donde falte     ","                ");
  nota(A_2,s);
  nota( G2,s);
  nota( F2,c);
  nota( D2,c);
textopan("donde falte   / ","      la ALEGRIA"); 
  nota( F2,c);
  nota( D2,s);
  nota( F2,cp);
  nota( D2,c);

textopan("simplemente     ","                ");
  nota( F2,c);
  nota( G2,s);
  nota(A_2,s+c);
  nota( F2,n+c);
  nota(Sil,s);
textopan("por no saber    ","de ti           ");
  nota(C2 ,s);
  nota(A_2,s);
  nota(A_2,s);
  nota( G2,c);
  nota( G2,c);
  nota( F2,3*n);
}
void nota_sf6(int N ,int d ){

    int  ttt = sf/2;
    nota( N*0.70710678118 , ttt);
    nota( N*0.74915353843 , ttt);
    nota( N*0.79370052598 , ttt);
    nota( N*0.84089641525 , ttt);
    nota( N*0.89089871814 , ttt);
    nota( N*0.94387431268 , ttt);
    
    nota( N              , d - 6*ttt);


}
void notat_sf(int N ,int d ){

  N = N * 0.89089871814 ;
    int  ttt = sf/2;
    
    nota( N*0.84089641525 , ttt);
    nota( N*0.89089871814 , ttt);
    nota( N*0.94387431268 , ttt);
    
    nota( N              , d - 3*ttt);


}
void nota_sf(int N ,int d ){

    int  ttt = sf/2;
    
    nota( N*0.84089641525 , ttt);
    nota( N*0.89089871814 , ttt);
    nota( N*0.94387431268 , ttt);
    
    nota( N              , d - 3*ttt);


}
void textopan(String S1,String S2){
  
 lcd.setCursor(0,0);
 lcd.print(S1);
 lcd.setCursor(0,1);
 lcd.print(S2);

  
  }
  void textoAbajo(String S2){
 
 lcd.setCursor(0,1);
 lcd.print(S2);

  
  }
  void ImprimeCorcheaAbajo(int i){

  lcd.setCursor( i, 1); //put it on line 3, column 19.
  lcd.write(byte(7)); //print our custom char backslash

  
  }

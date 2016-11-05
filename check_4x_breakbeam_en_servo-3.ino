/* 
  4 ir sensors in galton board. 
  passerende kogel laat led bij baan oplichten gedurende 2 sec.
  servo doseert kogels
  
  servo op pin 9
  leds op 5 tm 8
  ir lezer op 10 tm 13 

  deze werkt goed en snel, hiermee gb getest op 4 nov 2016
*/
 
#define led1PIN 5
#define led2PIN 6
#define led3PIN 7
#define led4PIN 8

#define SERVO_PIN 9     
  
#define baan1PIN 10
#define baan2PIN 11
#define baan3PIN 12
#define baan4PIN 13 

// variables will change:

int baan1State = 0;
int baan2State = 0;
int baan3State = 0;
int baan4State = 0;
int inrolHoek=104;    //servo:  109 (inrol positie) en  79 (uitrolpositie) zijn goede waarden voor beide borden
int uitrolHoek = 77;
int detectie=0;

int tellerEen=0; //voor de statistiek
int tellerTwee=0;
int tellerDrie=0;
int tellerVier=0;
int totaalRun=1;
int percentage;
 
void setup() 
{
    
  pinMode(led1PIN, OUTPUT); 
  pinMode(led2PIN, OUTPUT);
  pinMode(led3PIN, OUTPUT);
  pinMode(led4PIN, OUTPUT);     
      
  digitalWrite(baan1PIN, HIGH); // turn on the pullup
  digitalWrite(baan2PIN, HIGH); 
  digitalWrite(baan3PIN, HIGH); 
  digitalWrite(baan4PIN, HIGH);
   
  Serial.begin(9600);
  
  digitalWrite(led1PIN, HIGH);
  digitalWrite(led2PIN, HIGH);
  digitalWrite(led3PIN, HIGH);
  digitalWrite(led4PIN, HIGH);
  delay(250);

  draaiServo(inrolHoek); //test bij opstarten
  delay(1000);
  draaiServo(uitrolHoek);
  delay(1000);

}
 

void loop(){
 

   Serial.print("runnr:  ");
   Serial.println(totaalRun);
   draaiServo(uitrolHoek); // kogel valt (ongeveer 0.5 sec voor dit commando nodig. kogel is dan nog ruim boven detectors

while (detectie==0)// scannen poorten. dit is een snelle loop en kort genoeg om detectie te garanderen

    {
      baan1State = digitalRead(baan1PIN); // low = straal onderbroken = detectie
      baan2State = digitalRead(baan2PIN);
      baan3State = digitalRead(baan3PIN);
      baan4State = digitalRead(baan4PIN);

      if (baan1State == LOW) {detectie=1; digitalWrite(led1PIN, HIGH);Serial.println("baan1 ");tellerEen+=1;}
      if (baan2State == LOW) {detectie=1; digitalWrite(led2PIN, HIGH);Serial.println("baan2 ");tellerTwee+=1;}
      if (baan3State == LOW) {detectie=1; digitalWrite(led3PIN, HIGH);Serial.println("baan3 ");tellerDrie+=1;}
      if (baan4State == LOW) {detectie=1; digitalWrite(led4PIN, HIGH);Serial.println("baan4 ");tellerVier+=1;}
    }
   
  draaiServo(inrolHoek); // kogel klaarleggen
    
   Serial.println("----- "); // statistieken
   Serial.print("baan1 ");
   Serial.print(tellerEen);
   Serial.print(" percentage ");
   percentage = 100*tellerEen/totaalRun;
   Serial.println(percentage);
   
   Serial.print("baan2 ");
   Serial.print(tellerTwee);
   Serial.print(" percentage ");
   percentage = 100*tellerTwee/totaalRun;
   Serial.println(percentage);
   
   Serial.print("baan3 ");
   Serial.print(tellerDrie);
   Serial.print(" percentage ");
   percentage = 100*tellerDrie/totaalRun;
   Serial.println(percentage);
   
   Serial.print("baan4 ");
   Serial.print(tellerVier);
   Serial.print(" percentage ");
   percentage = 100*tellerVier/totaalRun;
   Serial.println(percentage);
   
   Serial.println("----- ");
   
   totaalRun+=1; // totaal aantal runs
   delay(1700); // wachttijd rond 10000, voor test korter
 
  digitalWrite(led1PIN, LOW); // doof alle leds
  digitalWrite(led2PIN, LOW);
  digitalWrite(led3PIN, LOW);
  digitalWrite(led4PIN, LOW);
   
  detectie=0; // herstart de valcyclus
          
  }



void draaiServo(int draaiHoek) // als je dit 15 x reset dan is de volledige draai (180 graden ) gemaakt. 30 graden ongeveer in 3x
{
    int count=0; // teller
    int pulsLengte; //waarden pulslengte: tussen 500 en 2500. 1500 is middenstand / neutraal
    
    pulsLengte=map(draaiHoek,0,180,500,2500); // hoek in graden. afhankelijk van servo. meestal tussen 0 en 180
    
    while (count<16)
    {
      count+=1; 
      
      // commando blijven schrijven naar servopin voor verdraaien, dit moet herhaald worden anders stopt servo voordat eindstand bereikt is (geen geheugen in servo) 
      // 30 keer moet genoeg zijn. kan korter bij kleine hoekverschillen. tijdsduur ongeveer 0.5 sec bij count 15
      
      pinMode(SERVO_PIN, OUTPUT);      // bepaal dat SERVO_PIN output pin is
      digitalWrite(SERVO_PIN, HIGH);   // start puls
      delayMicroseconds(pulsLengte);   // delay bepaalt pulslengte
      digitalWrite(SERVO_PIN, LOW);    // beeindig puls
      delay(25);
    }
    count=0; // reset counter
}
 


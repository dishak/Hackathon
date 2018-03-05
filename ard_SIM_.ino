//The machine is suitable to dispense 4 types of components
int dirPin1 = 4;                        //stepper motor-1  to dispense breadboard
int stepperPin1 = 5;
int dirPin2 = 6;                       //stepper motor-2  to dispense controller
int stepperPin2 = 7;
int dirPin3 = 8; 
int stepperPin3 = 9;                  //stepper motor-3  to dispense measuring instrument
int dirPin4= 10;
int stepperPin4 = 11;                //stepper motor-4    to dispense resisotr box
void setup() {
  // put your setup code here, to run once:
  pinMode(4,OUTPUT); pinMode(5,OUTPUT); //for breadboard
   pinMode(6,OUTPUT); pinMode(7,OUTPUT);  //for contorller
    pinMode(8,OUTPUT); pinMode(9,OUTPUT);  //for measuring instrument
     pinMode(10,OUTPUT); pinMode(11,OUTPUT);//for resistor box
Serial.begin(9600);
Serial1.begin(9600);
}
String c1="",d1="";int f1=0,f2=0,f3=0,f4=0;
void loop() 
{
 // put your main code here, to run repeatedly:
if(Serial1.available()>0)
{
  //delay(300);
 if(Serial1.find("a"))            //get's data from esp to trigger particular demanded item
  {
f1=Serial1.parseInt();

Serial.print(f1);
steppermotor(4,5,f1);  
  }
   if(Serial1.find("c"))
   {
f2=Serial1.parseInt();
Serial.print(f2);
steppermotor(6,7,f2);  
   }
   if(Serial1.find("e"))
   {
f3=Serial1.parseInt();
Serial.print(f3);
steppermotor(8,9,f3);  
   }
      if(Serial1.find("g"))
   {
f4=Serial1.parseInt();
Serial.print(f4);
steppermotor(10,11,f4);  
   }   
   Serial.println("sending confirmation that dispensing over");
  Serial.println("a"+String(1)+"b");    //confirmation that dispensing is over
  Serial1.println("a"+String(1)+"b");     
}

delay(300);
}
void steppermotor(int dpin,int spin,int turns)    //control dispensing
{
  
  Serial.println("got into stepper control");
  for(int y=0;y<turns;y++)
  {
    Serial.println("the dpin got is:"+String(dpin));
    Serial.println("the spin got is:"+String(spin));
        Serial.println("the turns got is:"+String(turns));    
   pinMode(dpin, OUTPUT);
  digitalWrite(dpin,LOW);
 for(int i=0;i<200;i++)
 {
   digitalWrite(spin, HIGH);
   delayMicroseconds(10000);
   digitalWrite(spin, LOW);
   delayMicroseconds(10000);
 }
 pinMode(dpin, OUTPUT);
 delay(500);
}

 //sending command that dispensed all items
  return;
}


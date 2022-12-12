// Nixie Tube Driver V2 by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Nixie-Tube-Driver-V2 
//
// Driving Nixie Tubes Example #4
//
// This example demonstrates how to control 4 nixie tubes and 4 dots.
// The control is carried out using 2 Nixie Tube Driver V2.
// How to connect nixie tubes: http://bit.ly/How2Drive4NixieTubes4Dots 

#define DIN_PIN   5          // Nixie driver (shift register) serial data input pin             
#define CLK_PIN   6          // Nixie driver clock input pin
#define EN_PIN    7          // Nixie driver enable input pin

// Bit array: 
// 22 bits - first driver - nixie tubes, dot1, dot2, 
// 2 bits for gaps
// 22 bits - second driver - nixie tubes, dot3, dot4, 
// 2 bits for gaps
boolean nixieDisplayArray[48];

// Cathodes assignment to the position in the 48 bit array
// Each cathode of nixie tubes is connected to the corresponding output of the shift registers
// Bit numbers
byte nixie1[]={
//   0   1   2   3   4   5   6   7   8   9  
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9 };
byte nixie2[]={
//   0   1   2   3   4   5   6   7   8   9
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
byte nixie3[]={
//   0   1    2   3   4   5   6   7   8   9  
     24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };
byte nixie4[]={
//   0   1   2   3   4   5   6   7   8   9
    34, 35, 36, 37, 38, 39, 40, 41, 42, 43 };    

byte dot1 = 20;   // K21 first driver
byte dot2 = 21;   // K22 first driver
byte dot3 = 44;   // K21 second driver
byte dot4 = 45;   // K22 second driver

void setup() 
{  
    pinMode(DIN_PIN, OUTPUT); 
    digitalWrite(DIN_PIN, LOW);    
    
    pinMode(CLK_PIN, OUTPUT);
    digitalWrite(CLK_PIN, LOW);         
  
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);
}

void loop ()
{
  // NixieDisplay(digit1, digit2, digit3, digit4);

  NixieDisplay(1, 2, 3, 4);

  // SetDot (dotNumber = 1-4, dotState = 0/1)
  SetDot(1, HIGH);
  SetDot(2, LOW);
  SetDot(3, HIGH);
  SetDot(4, LOW);
  
  delay(1000);     
}

void NixieDisplay(byte digit1, byte digit2, byte digit3, byte digit4)
{
  // Convert the desired numbers to the bit numbers for the nixieDisplayArray[]
  digit1 = nixie1[digit1];
  digit2 = nixie2[digit2];
  digit3 = nixie3[digit3];
  digit4 = nixie4[digit4];

  // Clear bit array except dots bits  
  for (int i = 47; i >= 0; i--)
  {
    if(i != dot1 || i != dot1 || i != dot3 || i != dot4) nixieDisplayArray[i] = 0;   
  }
    
  // Set the bits corresponding to the nixie tubes cathodes
  nixieDisplayArray[digit1] = 1;
  nixieDisplayArray[digit2] = 1;
  nixieDisplayArray[digit3] = 1;
  nixieDisplayArray[digit4] = 1;
  
  ShiftOutData();
}

void SetDot(byte dotNumber, boolean dotState)
{
  if(dotNumber == 1)  
  {
    if(dotState == HIGH) nixieDisplayArray[dot1] = 1;
    else nixieDisplayArray[dot1] = 0;  
  }
  
  if(dotNumber == 2)  
  {
    if(dotState == HIGH) nixieDisplayArray[dot2] = 1;
    else nixieDisplayArray[dot2] = 0;  
  }
  
  if(dotNumber == 3)  
  {
    if(dotState == HIGH) nixieDisplayArray[dot3] = 1;
    else nixieDisplayArray[dot3] = 0;  
  }
  
  if(dotNumber == 4)  
  {
    if(dotState == HIGH) nixieDisplayArray[dot4] = 1;
    else nixieDisplayArray[dot4] = 0;  
  }
  
  ShiftOutData();
}

void ShiftOutData()
{
  // Ground EN pin and hold low for as long as you are transmitting
  digitalWrite(EN_PIN, 0); 
  // Clear everything out just in case to
  // prepare shift register for bit shifting
  digitalWrite(DIN_PIN, 0);
  digitalWrite(CLK_PIN, 0);  
 
  // Send data to the nixie drivers 
  for (int i = 47; i >= 0; i--)
  {    
    // Set high only the bit that corresponds to the current nixie digit
    digitalWrite(DIN_PIN, nixieDisplayArray[i]);
    
    // Register shifts bits on upstroke of CLK pin 
    digitalWrite(CLK_PIN, 1);
    // Set low the data pin after shift to prevent bleed through
    digitalWrite(CLK_PIN, 0);  
  }   
  
  // Return the EN pin high to signal chip that it 
  // no longer needs to listen for data
  digitalWrite(EN_PIN, 1);
    
  // Stop shifting
  digitalWrite(CLK_PIN, 0);    
}

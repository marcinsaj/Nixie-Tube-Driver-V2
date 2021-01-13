// Nixie Tube Driver V2 by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Nixie-Tube-Driver-V2 
//
// Driving Nixie Tubes Example #2
//
// This example demonstrates how to control two nixie tubes.
// The control is carried out using one Nixie Tube Driver V2.
// How to connect nixie tubes: http://bit.ly/How2Drive2NixieTubes2Dots

#define DIN_PIN   5          // Nixie driver (shift register) serial data input pin             
#define CLK_PIN   6          // Nixie driver clock input pin
#define EN_PIN    7          // Nixie driver enable input pin

// Bit array for 2 nixie tubes, 2 not connected outputs, 2 bits for gaps
boolean nixieDisplayArray[24];

// Cathodes assignment to the position in the 24 bit array
// Each cathode of nixie tubes is connected to the corresponding output of the shift registers
// Bit numbers
byte nixie1[]={
//   0   1   2   3   4   5   6   7   8   9  
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9 };
byte nixie2[]={
//   0   1   2   3   4   5   6   7   8   9
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };


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
  // NixieDisplay(digit1, digit2);
  NixieDisplay(2, 0);  
}

void NixieDisplay(byte digit1, byte digit2)
{
  // Convert the desired numbers to the bit numbers for the nixieDisplayArray[]
  digit1 = nixie1[digit1];
  digit2 = nixie2[digit2];

  // Clear bit array
  for (int i = 23; i >= 0; i--)
  {
    nixieDisplayArray[i] = 0;      
  }
    
  // Set the bits corresponding to the nixie tubes cathodes
  nixieDisplayArray[digit1] = 1;
  nixieDisplayArray[digit2] = 1;
  
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
  for (int i = 23; i >= 0; i--)
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

void changeAmp( int data1,
                int data2,
                int data3,
                int data4,
                int data5,
                int data6,
                int data7,
                int data8, int *amplitudeData);

int yellowToggle = 0;
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;
int mReset = 11;
int mStrobe = 12;
int mInput = 1;
int yelTog[]={0,0,0,0,0,0,0,0};
 
byte gndData = 0;
byte redData = 255;
byte bluData = 255;
byte greData = 255;

void setup() 
{
  pinMode(mReset, OUTPUT);
  pinMode(mStrobe, OUTPUT);

  //Sets Digital pins 4,5,6 as outputs
  DDRD = DDRD | B01110000;  

  //Sets Digital pins
  DDRB = DDRB | B011000;
  
  updateShiftRegister();
  Serial.begin(9600);
}

void shiftOutFast(uint8_t val)
{
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    
    // sends data to serial data pin
      if(val & (1 << i))
        PORTD = B00010000;
      else
        PORTD = B00000000;     

    // Clocks the shifting pin.
    PORTD = B01000000;
    PORTD = B00000000;       
  }
}

void loop() 
{ 
  int ad[] = {8,5,6,2,3,7,1,0};
  int decTimer = 0;
  int decTimerLimit = 4;
  while(1)
 {
    // READS MSGEQ7 OUTPUT 
    digitalWrite(mReset, HIGH);     // DPM doesn't seem to work for this. Weird     
    delayMicroseconds(100);
    PORTB = B000000;
    
    for (int i=0;i<7;i++){
        PORTB = B000000;   
        delayMicroseconds(72); 
        int freqAmp = analogRead(mInput);
    
        int PWMvalue = freqAmp; 

        if((PWMvalue)*8/200>ad[i]){
        
          ad[i] = (PWMvalue)*8/250;
          ad[i]-=1;
        }

        if(decTimer>decTimerLimit)
          ad[i]--;

        // Check Limits  
        if(ad[i]>8)
          ad[i]=8;
        if(ad[i]<0)
          ad[i]=0;
          
        PORTB = B010000;  
    }

    //Serial.print("\n");
    ad[7] = 0;

    if(decTimer>decTimerLimit){
      decTimer=0;
    }
    decTimer++;
  
    int ead[] = {4,8,5,2,7,5,4,0};
    updateScreen(ad, 5);
 }
}

void updateShiftRegister()
{
   PORTD = B00100000;
   shiftOutFast(bluData);
   shiftOutFast(redData);
   shiftOutFast(greData);
   shiftOutFast(gndData);
   PORTD = B00000000;
}


//** OTHER FUNCTIONS **//

void changeAmp( int data1,
                int data2,
                int data3,
                int data4,
                int data5,
                int data6,
                int data7,
                int data8, int *amplitudeData)
{
  amplitudeData[0]=data1; 
  amplitudeData[1]=data2; 
  amplitudeData[2]=data3; 
  amplitudeData[3]=data4; 
  amplitudeData[4]=data5; 
  amplitudeData[5]=data6; 
  amplitudeData[6]=data7; 
  amplitudeData[7]=data8;               
}

                       
void updateScreen(int *amplitudeData, int cycles){
  for(int i=0;i<cycles;i++){
    lightUpCol(1,amplitudeData[0]);
    lightUpCol(2,amplitudeData[1]);
    lightUpCol(3,amplitudeData[2]);
    lightUpCol(4,amplitudeData[3]);
    lightUpCol(5,amplitudeData[4]);
    lightUpCol(6,amplitudeData[5]);
    lightUpCol(7,amplitudeData[6]);
    lightUpCol(8,amplitudeData[7]);
  }  
}

void lightUpCol(int col, int value){
    int colValues[] ={128,64,32,16,8,4,2,1};

    redData = 255;
    greData = 255;
    bluData = 255 - colValues[col-1];  //  o
  
    int gndDataValues[9] = {0,128,192,224,240,248,252,254,255};
    gndData = gndDataValues[value]&(192);  
    updateShiftRegister();

    
    redData = 255;
    bluData = 255;
    greData = 255 - colValues[col-1];  // 

    if(yelTog[col-1]>3){
      gndData = gndDataValues[value]&(124); 
      yelTog[col-1]=0;
    }
    else{
      gndData = gndDataValues[value]&(112); 
       yelTog[col-1]++;
    } 
    updateShiftRegister();
    
    redData = 255 - colValues[col-1];
    bluData = 255;
    greData = 255;  // 
  
    gndData = gndDataValues[value]&(15);  
    updateShiftRegister();
    
}

 
void turnOffAll()
{
  gndData = 0;
  redData = 255;
  bluData = 255;
  greData = 255;
  updateShiftRegister();
}
 



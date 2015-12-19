import processing.serial.*;

Serial myPort;        		// The serial port
int positions = 1000;       // Number of horizontal positions on the graph 

float[] y1Val = new float[positions]; 
float[] y2Val = new float[positions]; 
float[] y3Val = new float[positions]; 
float[] y4Val = new float[positions]; 
float[] y5Val = new float[positions]; 

PFont f;
boolean temEvento;
String inString;

void setup () {
  size(positions, 600);   
  
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);  //
  myPort.bufferUntil('\n');
  
  background(0);
  
  f = createFont("Arial",16,true);
  
  for(int i=0; i<y1Val.length; i++)
  {
    y1Val[i] = 0; //Zera o vetor de posições para o inicio do programa
  	y2Val[i] = 0;
  	y3Val[i] = 0;
  }
}

void draw () {
  if(temEvento){
     if (inString != null) {
    inString = trim(inString);                // trim off whitespaces.
    println(inString);
    float[] inData = float(splitTokens(inString, ";"));           // convert to a number.
    println(inData.length);
    if(inData.length == 5){
     
      for(int i=0; i<y1Val.length-1; i++)
      {
        y1Val[i] = y1Val[i+1];
        y2Val[i] = y2Val[i+1];
        y3Val[i] = y3Val[i+1];
        y4Val[i] = y4Val[i+1];
        y5Val[i] = y5Val[i+1];
        noSmooth();
        point(width, y1Val[i]);
        point(width, y2Val[i]);
        point(width, y3Val[i]);
        point(width, y4Val[i]);
        point(width, y5Val[i]);
      }
      
      for(int i=0; i<y1Val.length-1; i++)
      {
        y1Val[i] = y1Val[i+1];
        y2Val[i] = y2Val[i+1];
        y3Val[i] = y3Val[i+1];
        y4Val[i] = y4Val[i+1];
        y5Val[i] = y5Val[i+1];
      }
     //   print(inData[0]);
      y1Val[y1Val.length-1] = inData[0];
      y2Val[y2Val.length-1] = inData[1];
      y3Val[y3Val.length-1] = inData[2];
      y4Val[y4Val.length-1] = inData[3];
      y5Val[y5Val.length-1] = inData[4];
         
      background(0);   
      stroke(255);  
      for(int i=1; i<y1Val.length; i++)
      {  
        stroke(255,0,0); 
        line(i-1, 5*height/6 - y1Val[i-1]*1.5, i, 5*height/6 - y1Val[i]*1.5);
        stroke(0,255,0);
        line(i-1, 5*height/6 - y2Val[i-1]*1.5, i, 5*height/6 - y2Val[i]*1.5);
        stroke(0,0,255);
        line(i-1, 5*height/6 - y3Val[i-1]*1.5, i, 5*height/6 - y3Val[i]*1.5);
        stroke(0,255,255);
        line(i-1, 5*height/6 - y4Val[i-1]*1.5, i, 5*height/6 - y4Val[i]*1.5);
        stroke(255,255,0);
        line(i-1, 5*height/6 - y5Val[i-1]*1.5, i, 5*height/6 - y5Val[i]*1.5);
      }   
      
      noStroke();   
      fill(0);
      rectMode(CENTER);
      rect(100,100,100,100);
      textFont(f,16);                 // STEP 4 Specify font to be used
      fill(255);                        // STEP 5 Specify font color 
      text(inData[0], 100, 100);  // STEP 6 Display Text
      text(inData[1], 200, 100);  // STEP 6 Display Text
      text(inData[2], 300, 100);  // STEP 6 Display Text
      text(inData[3], 400, 100);  // STEP 6 Display Text
      text(inData[4], 500, 100);  // STEP 6 Display Text
     }
   temEvento = false; 

     }
  }
  
}

void serialEvent (Serial myPort) {
  inString = myPort.readStringUntil('\n');
   temEvento = true;
 
}


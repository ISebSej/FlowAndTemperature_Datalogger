import processing.serial.*;
Serial mySerial;
PrintWriter output;

String fakeConsole = "By: BES \nPress any key to stop recording \nDO NOT CLOSE THE WINDOW WITH X\n";
int counter = 0;
void Println(String in){
  println(in);
  fakeConsole = fakeConsole + in;
  counter++;
  if (counter == 25){
    fakeConsole = in;
    counter = 0; 
  }
}

void setup() {
   size(400,800);
   
   
   mySerial = new Serial( this, Serial.list()[0], 115200 );    

   int d = day();    // Values from 1 - 31
   int m = month();  // Values from 1 - 12
   int y = year();   // 2003, 2004, 2005, etc.
   int h = hour();
   int mi = minute();
   int se = second();
   
   String s = String.valueOf(y) +"_"+  String.valueOf(m)+"_"+  String.valueOf(d) +"_"+  String.valueOf(h)+"_"+  String.valueOf(mi)+"_" + String.valueOf(se);
   
   output = createWriter( "data" + s + ".txt" );
   output.println( "TIMESTAMP " + d +"-"+m+"-"+y + "  " + h +":"+mi+":"+se );
}

void draw() { // loop
   background(0);
   pushMatrix();
   // Your drawing here.
   popMatrix();
   fill(255);
   textSize(16);
   text(fakeConsole, 20, 20, width-80, height-80);
   
    if (mySerial.available() > 0 ) {
         String value = mySerial.readString();
         if ( value != null ) {
              output.print( value );
              Println(value);
         }
    }
}

void keyPressed() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    exit();  // Stops the program
}

void stop() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
} 

import processing.serial.*; 
Serial mySerial;
PrintWriter output; 

void setup() {
  print(Serial.list());
  String portName = Serial.list()[1]; 
  print(portName);
  mySerial = new Serial(this, portName, 115200); //Configura a porta serial
  output = createWriter( "2_0m_T01.csv" ); //Cria o objeto arquivo para gravar os dados
  output.print(day()); //Escreve no arquivo as horas e os minutos atuais seguido do valor lido pelo sensor
  output.print(" ");
  output.print(month());
  output.print(" ");
  output.print(year()); 
  output.print(hour()); //Escreve no arquivo as horas e os minutos atuais seguido do valor lido pelo sensor
  output.print(":");
  output.println(minute());
}

void draw() { //Mesma coisa que a funçao loop do Arduino
  if (mySerial.available() > 0 ) { //Se receber um valor na porta serial
    String value = mySerial.readStringUntil('\n'); //Le o valor recebido
    print(value);
    if ( value != null ) {
      if (value.substring(0,1).equals("D")) {
        output.print(value.substring(1)); 
      }
    }
  }
}

void keyPressed() { //Se alguma tecla for pressionada
  output.flush(); // Termina de escrever os dados pro arquivo
  output.close(); // Fecha o arquivo
  exit(); // Para o programa
}

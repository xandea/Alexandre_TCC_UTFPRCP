void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0,INPUT);

}

float defineDistancia(){
  VALEU_VOLTS = 0;
  
  delay(1000);
  VALEU_VOLTS = analogRead(A0)*0.00322265625; // 3,3V/1024 = 0.00322265625

  if(VALEU_VOLTS < 0.1){                             //Sem resistor
    return 0.3;  
  }else if(VALEU_VOLTS = 3.30){                      //Ligado diretamente no 3.3V
    return 0.5;    
  }else if(VALEU_VOLTS > 2.60 && VALEU_VOLTS < 2.8){ //Ligado no resistor de 100k
    return 1.0;        
  }else if(VALEU_VOLTS > 2.10 && VALEU_VOLTS < 2.30){ //Ligado no resistor de 200k
    return 2.0;        
  }else if(VALEU_VOLTS > 1.65 && VALEU_VOLTS < 2.0){ //Ligado no resistor de 300k
    return 4.0;        
  }else if(VALEU_VOLTS > 1.40 && VALEU_VOLTS < 1.65){ //Ligado no resistor de 430k
    return 6.0;        
  }else if(VALEU_VOLTS > 1.10 && VALEU_VOLTS < 1.35){ //Ligado no resistor de 620k
    return 9.0;        
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(defineDistancia());
  
}

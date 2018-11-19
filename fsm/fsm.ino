#define Buttonpin 2 //设置按钮为2号口

static int count=0;

//中断处理程序，处理连续的1
void CountContinutionOne(){
  //延迟100毫秒,来区分长击与短击
  delay(100);
  if(LOW==digitalRead(Buttonpin)){
    count++;
    Serial.println(count);
  }else{
    //不是连续的1，将count重新设为0
    count=0;
    Serial.println(count);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Buttonpin,INPUT);
  //Enable中断管脚, 中断服务程序为CountContinutionOne(), 监视引脚变化,当引脚为LOW即按下按钮时，调用中断程序
  attachInterrupt( digitalPinToInterrupt(Buttonpin), CountContinutionOne, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}

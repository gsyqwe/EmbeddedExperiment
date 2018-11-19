#include <FreeRTOS_AVR.h>

#define Ledpin 10 //Led灯连接到10号口
#define Buzzerpin 8 //蜂鸣器连接到8号口
#define Buttonpin 5 //按钮连接到5号口
#define LightSensorpin A4 //光线传感器连接到A4号口
#define AnalogSensorpin A1 //电位器连接到A1号口

//信号
SemaphoreHandle_t sem;

void setup() {
  // put your setup code here, to run once:
  portBASE_TYPE s1, s2, s3;
  pinMode(Ledpin,OUTPUT);
  pinMode(Buzzerpin,OUTPUT);
  digitalWrite(Buzzerpin,HIGH);
  digitalWrite(Ledpin,HIGH);
  pinMode(Buttonpin,INPUT);
  // 初始化sem
  sem = xSemaphoreCreateCounting(1, 0);
  Serial.begin(9600);
  // 创建优先级为3,最先执行
  s1 = xTaskCreate(Button_Led_On, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);

  // 创建优先级为2的任务，第二个执行
  s2 = xTaskCreate(Analog_Led_On, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);

  // 创建优先级为1的任务，最后执行
  s3 = xTaskCreate(Light_Led_Buzzer, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);

  vTaskStartScheduler();
  while(1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

//第一个运行的task,将Led由关到开
void Button_Led_On(void* arg){
  while(1){
    if(LOW==digitalRead(Buttonpin)){
      digitalWrite(Buzzerpin,LOW);
    }
    //延迟
    vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
    digitalWrite(Buzzerpin,HIGH);
    //解放信号
    xSemaphoreGive(sem);
    //延迟
    //vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}

//通过电位器控制Led的亮度
void Analog_Led_On(void *args){
  digitalWrite(Ledpin,HIGH);
  while(1){
    Serial.println(digitalRead(Ledpin));
    if(HIGH==digitalRead(Ledpin)){
      int AnalogSens=0;
      AnalogSens=analogRead(AnalogSensorpin);
      analogWrite(Ledpin,AnalogSens);
      // 延迟
      //vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
      //解放信号
      xSemaphoreGive(sem);
      //延迟
      //vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
    }else{
      xSemaphoreGive(sem);
    }
  }
}

//通过光线传感器控制Led的亮度如果过亮就响起报警器并关闭Led灯
void Light_Led_Buzzer(){
  while(1){
    if(HIGH==digitalRead(Ledpin)){
      int LightSens=0;
      LightSens=analogRead(LightSensorpin);
      analogWrite(Ledpin,LightSens/1023*255);
      // 延迟
      //vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
      //解放信号
      xSemaphoreGive(sem);
    }else{
      //解放信号
      xSemaphoreGive(sem);
    }
  }
}

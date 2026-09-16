#include<SoftwareSerial.h>
SoftwareSerial BT(8, 9); // RX,TX
SoftwareSerial softSerial1(4,5);
char val;
String vals;
int BPM;//定义参数接收蓝牙的值
boolean IsNewLine = false;
const int highRate=90;
const int lowRate=40;

 int sendMeg(String phonenum,String text)//发送信息的函数，直接调用即可，无需修改
  {

        //变量声明
        char comdata=0;//判断接收到的最后一位是否正确
        String phone="";//电话号码存储
        phone="AT+CMGS=\""+phonenum+'\"';  //AT指令，用于拨打号码

        while (softSerial1.available() > 0){ //先清除软接口里面的文字
        comdata = softSerial1.read();  //每次读一个char字符，并相加
        delay(2);
        }    
        softSerial1.println("AT");//对GSM发送AT,看响应，软接口应该返回"OK"
        delay(1000);
        while (softSerial1.available() > 2){ //如果软接口返回的字符量大于2
        comdata = softSerial1.read();  //每次读一个char字符，并相加
        Serial.print(comdata);//打印错误
        delay(2);
        }
        if(comdata!='K')//判断没有收到OK
        {
        Serial.println("error1");//错误，arduino没连上GMS，也可能是GMS坏了
        return  0;
        }
        comdata=0;     
        softSerial1.println("AT+CMGF=1");//设置GSM文本模式（用于发送英文短信）
        delay(500);
        while (softSerial1.available() > 2){ //如果软接口返回的字符量大于2
        comdata = softSerial1.read();  //清除软接口里面的文字
        delay(2);
        }
        
        if(comdata!='K')//没有收到OK，GSM没设置成GSM文本模式，应该不会出现这个问题
        {
        Serial.println("error2");
        return  0;
        } 
        comdata=0;
        softSerial1.println(phone);//发送的目标号码
        delay(1000);
        while (softSerial1.available() > 1){ //如果软接口返回的字符量大于1
        comdata = softSerial1.read();  //清除软接口里面的文字
        delay(2);
        }
        
        if(comdata!='>')//没有收到 >
        {
        Serial.println("error3"); //拨打电话失败，原因可能是供电不足，或者SIM模块有问题，反正就是没能发短信
        return  0;
        }
        comdata=0;
        softSerial1.print(text);//发送所写内容
        delay(500);
        softSerial1.write(0x1A); //结束输入文本，并发送短信
        Serial.println("successfully send message!");  //成功   
    
 }

// for循环若收不到消息会卡死 暂时用来看逻辑
int sendsim2(String phone_num,String text,SoftwareSerial &p_serial)
{
  p_serial.println("AT");
  delay(500);
  p_serial.println("AT+CSCS=\"GSM\"");
  delay(500);
  p_serial.println("AT+CMGF=1");
  delay(500);
  p_serial.print("AT+CMGS=\"");
  p_serial.print(phone_num);
  p_serial.println("\"");
 // Serial.println("1");
 //for(;p_serial.read()!='>';) ;
 // Serial.println("1");
  p_serial.print(text);
  delay(500);
  p_serial.write(0x1A);
  delay(500);
  p_serial.flush();
  delay(100);
  
  }



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600);//蓝牙波率设置为9600
  //监听软串口通信
//  softSerial1.begin(115200);
//  softSerial1.listen();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BT.available()){
    val=BT.read();
    vals=vals+val;
    // BT.println(val);
    if(val=='.'){
//      Serial.println(vals);
      BPM=atoi(vals.c_str());
      Serial.println(BPM);
      if(BPM>highRate){
        Serial.println("warning!!the heart rate is too high!");
        sendMeg("13973112505","warning!!the heart rate is too high!");// 发送信息
        // sendsim2("13973112505","ddd",softSerial1);
        delay(500*1000); 
      }
//      else if(BPM<lowRate){ 
//        Serial.println("warning!!the heart rate is too low!");
//        sendMeg("13973112505","warning!!the heart rate is too low!");//  发送信息
//        // sendsim2("13973112505","ddd",softSerial1);
//        delay(500*1000);}
      vals="";
    }
  }
  delay(1);
}

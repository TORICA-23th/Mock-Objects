// raspberry pi poco

//under serial1
//RP 1 -> underRX
//RP 2 <- underTX

//air serial2
//RP 1 -> airRX
//RP 2 <- airTX

char sendUART_BUF[256];

volatile float air_dps_altitude_m = 71;
volatile float air_dps_pressure_hPa = 1013.0;
volatile float air_dps_temperature_deg = 30;
volatile float air_sdp_airspeed_ms = 5.0;
volatile float air_sdp_differentialPressure_Pa = 1013.0;
volatile float air_sdp_temperature_deg = 30;

volatile float under_dps_pressure_hPa = 1013.0;
volatile float under_dps_temperature_deg = 30;
volatile float under_dps_altitude_m = 70;
volatile float under_urm_altitude_m = 0.5;

volatile float true_m = 10.5;

unsigned long int last_send_time = 0;

unsigned long int loop_count = 180;
double down_count = 0;
unsigned long int urm_count = 0;
int flag = true;

//乱数生成
float randNumber;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(460800);
  Serial2.begin(460800);

  //乱数生成
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - last_send_time > 31) {
    last_send_time = millis();

    loop_count++;
        
    //乱数生成
    randNumber = random(92, 112); 
    air_sdp_airspeed_ms = randNumber / 10;

    //Serial.println(air_sdp_airspeed_ms);
    
    if(millis() < 15000){
      under_urm_altitude_m = 0.5;
    }
    else{
      down_count = loop_count / 180;
      air_dps_altitude_m = (9.9 / down_count) + 60;
      under_dps_altitude_m = (10 / down_count) + 61;
      if(under_dps_altitude_m < 66){
        under_urm_altitude_m = 10 / down_count;
      }
      else{
        under_urm_altitude_m = 10.0;
      }
    }
    Serial.println(under_urm_altitude_m);

    //under
    //気圧[hPa],温度[deg],気圧高度[m],超音波高度[m]
    sprintf(sendUART_BUF, "%.2f,%.2f,%.2f,%.2f\n", under_dps_pressure_hPa, under_dps_temperature_deg, under_dps_altitude_m, under_urm_altitude_m);
    Serial1.print(sendUART_BUF);

    //air
    //気圧[hPa],温度[deg],気圧高度[m],差圧[Pa],対気速度[m/ss]  
    sprintf(sendUART_BUF, "%.2f,%.2f,%.2f,%.2f,%.2f\n", air_dps_pressure_hPa, air_dps_temperature_deg, air_dps_altitude_m, air_sdp_differentialPressure_Pa, air_sdp_airspeed_ms);
    Serial2.print(sendUART_BUF);
  }
}

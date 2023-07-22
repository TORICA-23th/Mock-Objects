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

volatile float true_m = 10.6;
unsigned long int last_send_time = 0;

int loop_count = 0;
int flag_landed = false;

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
  if (millis() - last_send_time > 40) {
    last_send_time = millis();

    //乱数生成
    randNumber = random(92, 112);
    air_sdp_airspeed_ms = randNumber / 10;

    Serial.println(air_sdp_airspeed_ms);

    if (millis() < 15000) {
      under_urm_altitude_m = 0.6;
    } else if (!flag_landed) {
      true_m -= 0.006799;
      air_dps_altitude_m -= 0.006800;
      under_dps_altitude_m -= 0.006800;
    } else {
      if (true_m <= 0.006799) {
        flag_landed = true;
      }
      if (true_m <= 5.0) {
        under_urm_altitude_m = true_m;
      } else {
        under_urm_altitude_m = 10.0;
      }
    }

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

// raspberry pi poco

//under serial1
//RP 1 -> underRX
//RP 2 <- underTX

//air serial2
//RP 1 -> airRX
//RP 2 <- airTX

char sendUART_BUF[256];

volatile float air_dps_altitude_m = 0;
volatile float air_dps_pressure_hPa = 0;
volatile float air_dps_temperature_deg = 0;
volatile float air_sdp_airspeed_ms = 0;
volatile float air_sdp_differentialPressure_Pa = 0;
volatile float air_sdp_airspeed_mss = 0;
volatile float air_sdp_temperature_deg = 0;

volatile float under_dps_pressure_hPa = 0;
volatile float under_dps_temperature_deg = 0;
volatile float under_dps_altitude_m = 0;
volatile float under_urm_altitude_m = 0;

unsigned long int last_send_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(460800);
  Serial2.begin(460800);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - last_send_time>40) {
    last_send_time = millis();
    
    //under
    //気圧[hPa],温度[deg],気圧高度[m],超音波高度[m]
    sprintf(sendUART_BUF, "%.2f,%.2f,%.2f,%.2f\n", under_dps_pressure_hPa, under_dps_temperature_deg, under_dps_altitude_m, under_urm_altitude_m);
    SerialOUT.print(sendUART_BUF);

    //air
    //気圧[hPa],温度[deg],気圧高度[m],差圧[Pa],対気速度[m/ss]  
    sprintf(sendUART_BUF, "%.2f,%.2f,%.2f,%.2f,%.2f\n", air_dps_pressure_hPa, air_dps_temperature_deg, air_dps_altitude_m, air_sdp_differentialPressure_Pa, air_sdp_airspeed_ms);
    SerialOUT.print(sendUART_BUF);
  }
}

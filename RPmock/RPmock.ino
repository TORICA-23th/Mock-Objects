// raspberry pi poco

//under serial1
//RP GP0 -> underRX
//RP GP1 <- underTX

//air serial2
//RP GP8 -> airRX
//RP GP9 <- airTX

enum {
  LINEAR,
  DIVE
} mode = LINEAR;

char sendUART_BUF[256];

const float air_dps_pressure_hPa = 1013.0;
const float air_dps_temperature_deg = 30;
const float air_sdp_differentialPressure_Pa = 1013.0;
const float air_sdp_temperature_deg = 30;

const float under_dps_pressure_hPa = 1013.0;
const float under_dps_temperature_deg = 30;


const float const_air_dps_m = 71;
float air_dps_altitude_m = const_air_dps_m;
float air_sdp_airspeed_ms = 10.2;


const float const_under_dps_m = 70;
float under_dps_altitude_m = const_under_dps_m;
float under_urm_altitude_m = 0.6;

const float const_platform_alt_m = 10.6;
float true_m = const_platform_alt_m;


unsigned long int loop_count = 180;
double down_count = 0;
bool flag_landed = false;

//乱数生成
float randNumber;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(460800);
  Serial2.begin(460800);

  pinMode(LED_BUILTIN, OUTPUT);

  //乱数生成
  randomSeed(analogRead(0));
}

void loop() {
  static unsigned long int last_send_time = 0;
  if (millis() - last_send_time > 33) {
    last_send_time = millis();

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    loop_count++;

    randNumber = random(92, 112);
    air_sdp_airspeed_ms = randNumber / 10;
    Serial.println(air_sdp_airspeed_ms);

    if (millis() < 15000) {
      // 離陸前
      under_urm_altitude_m = 0.6;
    } else if (!flag_landed) {
      // 離陸後 ~ 着水前

      // true_mの更新
      if (mode == LINEAR) {
        true_m -= 0.006799;
      } else if (mode == DIVE) {
        down_count = loop_count / 180;
        true_m = 10 / down_count;
      }

      // 気圧高度の更新
      air_dps_altitude_m = (true_m - const_platform_alt_m) * 0.94 + const_air_dps_m;
      under_dps_altitude_m = (true_m - const_platform_alt_m) * 0.96 + const_under_dps_m;

      // 超音波高度の更新
      if (true_m <= 5.0) {
        under_urm_altitude_m = true_m;
      } else {
        under_urm_altitude_m = 10.0;
      }

      // 着水判定
      if (true_m <= 0.006799) {
        flag_landed = true;
      }
    } else {
      // 着水後
      // 更新無し
        under_urm_altitude_m = true_m;
    }

    //超音波にノイズのせる
    // 1/100(TBD)の確率で外れ値を0.2~1.5秒連続して出力する
    static int noise_count = 0;
    static float noise_val = 0;
    if (noise_count > 0) {
      noise_count--;
      under_urm_altitude_m = noise_val;
    } else {
      if (random(0, 100) == 1) {
        noise_count = random(6, 50);
        noise_val = (float)random(0, 1000) / 100.0;
        if (noise_val > 5) {
          noise_val = 10;
        }
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

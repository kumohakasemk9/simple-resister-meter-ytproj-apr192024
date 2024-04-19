void setseg(int);

void setup() {
    //4 ... 9 10 ... 13 A0 A1 Outputs
    for(int i = 0; i < 10; i++) {
      pinMode(i + 4, OUTPUT);
    }
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
	pinMode(A3, INPUT);
}

void loop() {
    uint32_t mon;
    uint8_t dotpos = 3;
    const uint8_t CP[] = {4, A0, 13, 10}; /* COM pins */
    double meas_final;
	static double v_avg = 0;
    static uint8_t meas_cnt = 0;
	v_avg += analogRead(A3);
    meas_cnt++;
    if(meas_cnt == 20) {
      meas_cnt = 0;
		double volt = (v_avg / 20.0) * (5.0 / 1024.0);
		v_avg = 0;
		// V1 : V2 = R1 : R2
		// R1 = (V1 : V2) * R2
		double vratio = (5 - volt) / volt;
		//if(volt_avg < 0.001) {
		//	meas_final = -1; //Open
		//} else {
			meas_final = vratio * 10000.0;
		//}
    }
	if(meas_final < 1000000) {
    mon = meas_final;
    for(uint8_t i = 0; i < 3; i++) {
      if(mon < 10000) {break;}
      mon = mon / 10;
      dotpos--;
    }
	} else{
		meas_final = -1;
	}
    for(uint8_t i = 0; i < 4; i++) {
      for(uint8_t j = 0; j < 4; j++) {
        if(j == i) {
          digitalWrite(CP[j], LOW);
        } else {
          digitalWrite(CP[j], HIGH);
        }
        if(dotpos == i) {
          digitalWrite(7, HIGH);
        } else {
          digitalWrite(7, LOW);
        }
      }
		if(meas_final > 0.0) {
			setseg(mon % 10);
			mon = mon / 10;
		} else {
			setseg(10);
		}
      delay(10);
    }
}

void setseg(int s) {
  /*
  Pin VS Segment
  4 5 6 7 8 9 10 11 12 13 A0 A1
    g c . d e     a  f        b
  */
  const uint8_t PINS[] = {11, A1, 6, 8, 9, 12, 5}; /* pin numbers in order of a...g */
  const uint8_t PTNS[] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f, 0x40}; /* pattern of 0...9 */
  for(uint8_t i = 0; i < 7; i++) {
    if(PTNS[s] & (1 << i)) {
      digitalWrite(PINS[i], HIGH);
    } else {
      digitalWrite(PINS[i], LOW);
    }
  }
}

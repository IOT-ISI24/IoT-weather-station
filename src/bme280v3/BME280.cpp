#include "BME280.h"
#include "Arduino.h"


BME280::BME280() {
  address1 = 0x76;	// adres standardowy
  mode1 = 3;		    // tryb normal
  stanby1 = 0;	  	// zalecane do trybu normalnego
  filter1 = 0;		  // wylaczenie filtra
  t_ovs = 1;  		  // nadprobkowanie tem x1
  h_ovs = 1;	    	// nadprobkowanie wilgotnosci x1
  p_ovs = 1;	    	// nadprobkowanie cisnienia x1
  }


// sprawdzenie czy istnieje polaczenie i2c
BME280::~BME280(void) {

  if (i2c_dev) {
    delete i2c_dev;
  }

}

/* @brief inicjalizacjia i2c dla danego adresu
* @param addr adres czujnika
* @param theWire pointer do wire.h
*/
bool BME280::begin(uint8_t addr, TwoWire *theWire){
    if (i2c_dev)
      delete i2c_dev;
    i2c_dev = new Adafruit_I2CDevice(addr, theWire);
    if (!i2c_dev->begin()){
      return false;
  }

    /* sprawdzenie id sensora odczzytanie wartosci rejestru BME280_REGISTER_CHIPID
    jezeli wartosc jest rozna od 0x60, wowczas to nie jest bme280*/
  _sensorID = read(BME280_REGISTER_CHIPID,0);
  if (_sensorID != 0x60)
    return false;

  // reset czujnika - zapis do rejestru BME280_REGISTER_SOFTRESET wartosc 0xb6 wywolujac w ten sosob reset
  write(BME280_REGISTER_SOFTRESET, 0xB6);


  // odczekanie na ponowne wlaczenia
  delay(50);

  // odczytaj parametry callibration
  readCoefficients();


  delay(100);

  return true;
}



/* ponizej znajduja sie dodatkowe funkcje do kalibracji, po wrowadzeniu przez uzytkownika zwracaja odpowienia wartosc */

		/* @brief ustawienia adresu
    * @param 0X76 standard
		* @param 0x77 alternatywny
    */
uint8_t BME280::Address(uint8_t address){
	return address;
}

	/* @brief wybor trybu
  * @param BME280_SLEEP_MODE
  * @param BME280_FORCED_MODE
  * @param BME280_NORMAL_MODE
  */
uint8_t BME280::Mode(uint8_t mode){
	return mode;
}
 
    /*@brief ustawienia filtra
	  * @param FIlterMode_OFF
		* @param FIlterMode_2
		* @param FIlterMode_4
		* @param FIlterMode_8
		* @param FIlterMode_16
    */
uint8_t BME280::Filter(uint8_t filter_mode){
	return filter_mode;
}
  /* @brief ustawienia nadprobkowania wilgotnosci
		* @param	BME280_OVS_OFF
		* @param	BME280_OVS_1X
		* @param	BME280_OVS_2X
		* @param	BME280_OVS_4X
		* @param	BME280_OVS_8X
		* @param	BME280_OVS_16X
   */
uint8_t BME280::HumOVS(uint8_t humOVS_mode){
	return humOVS_mode;
}

  /* @brief ustawienia nadprobkowania temperatury
		* @param	BME280_OVS_OFF
		* @param	BME280_OVS_1X
		* @param	BME280_OVS_2X
		* @param	BME280_OVS_4X
		* @param	BME280_OVS_8X
		* @param	BME280_OVS_16X
   */
uint8_t BME280::TempOVS(uint8_t tempOVS_mode){
	return tempOVS_mode;
}

  /* @brief ustawienia nadprobkowania cisnienia
		* @param	BME280_OVS_OFF
		* @param	BME280_OVS_1X
		* @param	BME280_OVS_2X
		* @param	BME280_OVS_4X
		* @param	BME280_OVS_8X
		* @param	BME280_OVS_16X
   */
uint8_t BME280::PresOVS(uint8_t presOVS_mode){
	return presOVS_mode;
}

  /* @brief ustawiania czasu standby
	  * @param  StandbyTime_0_5_MS
		* @param	StandbyTime_62_5_MS
		* @param	StandbyTime_125_MS
		* @param	StandbyTime_250_MS
		* @param	StandbyTime_500_MS
		* @param	StandbyTime_1000_MS
		* @param	StandbyTime_2000_MS
		* @param	StandbyTime_4000_MS
  */
uint8_t BME280::Standby(uint8_t standby_mode){
	return standby_mode;
}

/* konfiguracja pomiarow i utawaianie odpowiednich parametrow (NULL oznacza parametry predefiniowane na poczaczku pliku),
wartosci poszczegolnych parametrow mozna wpisac z wykorztsaniem powyzszych funkcji lub recznie, zarowna wartosci jak i 
odwolac sie do odpowiednich zmiennych, stalych okreslonych w strukturach pliku .h*/

/*konfiguracja pomiarow
* @param mode ustawnie trybu
* @param filter ustawienia filtra
* @param stanby ustawienia standby
* @param presovs ustawienia nadprobkowania cisnienia
* @param humovs ustawienia nadprobkowania wilgotnosci
* @param tempovs ustawienia nadprobkowania temperatury
*/
void BME280::Meas_config(uint8_t mode, uint8_t filter, uint8_t standby, uint8_t presovs, uint8_t humovs, uint8_t tempovs){
  if (mode != NULL){
    mode1 = mode;		      // tryb
  }
  if (standby != NULL){
    stanby1 = standby;		// ustaw filtra
  }
  if (filter != NULL){
    filter1 = filter;	  	// ustawienie filtra
  }
  if (tempovs != NULL){
    t_ovs = tempovs;  		// nadprobkowanie tem 
  }
  if (humovs != NULL){
    h_ovs = humovs;		    // nadprobkowanie wilgotnosci
  }
  if (presovs != NULL){
    p_ovs = presovs;      // nadprobkowanie cisnienia
  }

  uint8_t dataToWrite = 0;
  // zeby mozliwa byla konfiguracja nalezy wprowadzic czujnik w stan uspienia
  write(BME280_REGISTER_CONTROL, BME280_SLEEP_MODE);
  // konfiguracja czasu stnadby oraz filtru zapisanie do rejestru, na odpowiedniej pozycji danych odpowiadajacych 
  // konfiguracji czasu standby oraz rodzaju uzytego filtra
	dataToWrite = (stanby1 << 0x5) & 0xE0;
	dataToWrite |= (filter1 << 0x02) & 0x1C;
	write(BME280_REGISTER_CONFIG, dataToWrite);

	// ustawiamy opcje wilgotnosci, a nastepnie umozliwiamy aktywacje zapisujac jej wartosc do rejestru 
	dataToWrite = h_ovs & 0x07;
	write(BME280_REGISTER_CONTROLHUMID, dataToWrite);

	// ustawienia pozostalych pomiarow w podobny sposob jak wilgotnosci
	// nadprobkowanie temperatury
	dataToWrite = (t_ovs << 0x5) & 0xE0;
	// nadprobkowanie cisniena
	dataToWrite |= (p_ovs << 0x02) & 0x1C;
	// ustawianie trybu pracy
	dataToWrite |= (mode1) & 0x03;
	//zapisz bajt do rejestru
	write(BME280_REGISTER_CONTROL, dataToWrite);

}

/* @brief dane do rejestru
* @param reg rejestr do ktorego beda zapisywane dane
* @param value dane, wartosc zapisywana do rejestru
*/
void BME280::write(byte reg, byte value) {
  byte buffer[2];
  buffer[1] = value;
    buffer[0] = reg;
    i2c_dev->write(buffer, 2);
}

/* @brief odczytaj dane z rejestru
* @param reg rejestr z ktorego odczytujemy wartosc
* @param variable_id id powiazane z typem zmiennych do ktorych beda przypisywane wartosci
*/
uint32_t BME280::read(byte reg, uint8_t variable_id) {
  // gdy id = 0, wowczas typ zmiennej uint8_t
    if(variable_id==0){
      uint8_t buffer[1];
        buffer[0] = uint8_t(reg);
        i2c_dev->write_then_read(buffer, 1, buffer, 1);
      return buffer[0];
    }
    // gdy id = 1, wowczas typ zmiennej uint16_t
    else if(variable_id==1){
        uint8_t buffer[2];
          buffer[0] = uint8_t(reg);
          i2c_dev->write_then_read(buffer, 1, buffer, 2);
        return uint16_t(buffer[0]) << 8 | uint16_t(buffer[1]);
      }
      // gdy id = 2, wowczas typ zmiennej uint16_t zapisywane od najmniej znaczacego bajtux
      else if(variable_id==2){
        uint8_t buffer[2];
          buffer[0] = uint8_t(reg);
          i2c_dev->write_then_read(buffer, 1, buffer, 2);
        return uint16_t(buffer[0]) | uint16_t(buffer[1]<<8);
      }//gdy id = 2, wowczas typ zmiennej uint32_t
      else if(variable_id ==3){
          uint8_t buffer[3];
            buffer[0] = uint8_t(reg);
            i2c_dev->write_then_read(buffer, 1, buffer, 3);
          return uint32_t(buffer[0]) << 16 | uint32_t(buffer[1]) << 8 |
                uint32_t(buffer[2]);
        }


}



void BME280::readCoefficients(void) {
  // Odczyt danych z rejestrow i zapis do odpowienich wartosci w strukturze kalibracyjnej

  Serial.println(calibration.dig_T1);
  Serial.println(calibration.dig_T2);
  Serial.println(calibration.dig_T3);


	calibration.dig_T1 = read(BME280_REGISTER_DIG_T1,2);
  calibration.dig_T2 = read(BME280_REGISTER_DIG_T2,2);
  calibration.dig_T3 = read(BME280_REGISTER_DIG_T3,2);

  calibration.dig_P1 = read(BME280_REGISTER_DIG_P1,2);
  calibration.dig_P2 = read(BME280_REGISTER_DIG_P2,2);
  calibration.dig_P3 = read(BME280_REGISTER_DIG_P3,2);
  calibration.dig_P4 = read(BME280_REGISTER_DIG_P4,2);
  calibration.dig_P5 = read(BME280_REGISTER_DIG_P5,2);
  calibration.dig_P6 = read(BME280_REGISTER_DIG_P6,2);
  calibration.dig_P7 = read(BME280_REGISTER_DIG_P7,2);
  calibration.dig_P8 = read(BME280_REGISTER_DIG_P8,2);
  calibration.dig_P9 = read(BME280_REGISTER_DIG_P9,2);

  calibration.dig_H1 = read(BME280_REGISTER_DIG_H1,0);
  calibration.dig_H2 = read(BME280_REGISTER_DIG_H2,2);
  calibration.dig_H3 = read(BME280_REGISTER_DIG_H3,0);
  calibration.dig_H4 = ((int8_t)read(BME280_REGISTER_DIG_H4,0) << 4) |
                         (read(BME280_REGISTER_DIG_H4 + 1,0) & 0xF);
  calibration.dig_H5 = ((int8_t)read(BME280_REGISTER_DIG_H5 + 1,0) << 4) |
                         (read(BME280_REGISTER_DIG_H5,0) >> 4);
  calibration.dig_H6 = (int8_t)read(BME280_REGISTER_DIG_H6,0);
}



/*odczyt temperatury, wykorzystuje wzory udostenione w dokumentacji */
float BME280::readTemperature(void) {
  int32_t var1, var2;

  int32_t adc_T = read(BME280_REGISTER_TEMPDATA,3);
  Serial.print("Raw adc_T: ");
  Serial.println(adc_T, HEX);

  if (adc_T == 0x800000)
    return NAN;
  adc_T >>= 4;

  var1 = (int32_t)((adc_T / 8) - ((int32_t)calibration.dig_T1 * 2));
  var1 = (var1 * ((int32_t)calibration.dig_T2)) / 2048;
  var2 = (int32_t)((adc_T / 16) - ((int32_t)calibration.dig_T1));
  var2 = (((var2 * var2) / 4096) * ((int32_t)calibration.dig_T3)) / 16384;

  t_fine = var1 + var2 + t_fine_adjust;

  int32_t T = (t_fine * 5 + 128) / 256;

  Serial.println((float)T / 100);

  return (float)T / 100;

}
// funkcje konpensacji temperatury
float BME280::getTemperatureCompensation(void) {
  return float((t_fine_adjust * 5) >> 8) / 100.0;
};
void BME280::setTemperatureCompensation(float adjustment) {
  t_fine_adjust = ((int32_t(adjustment * 100) << 8)) / 5;
};


/*odczyt cisnienia, wykorzystuje wzory udostenione w dokumentacji */
float BME280::readPressure(void) {
  int64_t var1, var2, var3, var4;

  readTemperature();

  int32_t adc_P = read(BME280_REGISTER_PRESSUREDATA,3);
  if (adc_P == 0x800000)
    return NAN;
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)calibration.dig_P6;
  var2 = var2 + ((var1 * (int64_t)calibration.dig_P5) * 131072);
  var2 = var2 + (((int64_t)calibration.dig_P4) * 34359738368);
  var1 = ((var1 * var1 * (int64_t)calibration.dig_P3) / 256) +
         ((var1 * ((int64_t)calibration.dig_P2) * 4096));
  var3 = ((int64_t)1) * 140737488355328;
  var1 = (var3 + var1) * ((int64_t)calibration.dig_P1) / 8589934592;

  if (var1 == 0) {
    return 0;
  }

  var4 = 1048576 - adc_P;
  var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
  var1 = (((int64_t)calibration.dig_P9) * (var4 / 8192) * (var4 / 8192)) /
         33554432;
  var2 = (((int64_t)calibration.dig_P8) * var4) / 524288;
  var4 = ((var4 + var1 + var2) / 256) + (((int64_t)calibration.dig_P7) * 16);

  float P = var4 / 256.0;

  return P;
}

/*odczyt wilgotnosci, wykorzystuje wzory udostenione w dokumentacji */
float BME280::readHumidity(void) {
  int32_t var1, var2, var3, var4, var5;

  readTemperature();

  int32_t adc_H = read(BME280_REGISTER_HUMIDDATA,1);
  if (adc_H == 0x8000)
    return NAN;

  var1 = t_fine - ((int32_t)76800);
  var2 = (int32_t)(adc_H * 16384);
  var3 = (int32_t)(((int32_t)calibration.dig_H4) * 1048576);
  var4 = ((int32_t)calibration.dig_H5) * var1;
  var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
  var2 = (var1 * ((int32_t)calibration.dig_H6)) / 1024;
  var3 = (var1 * ((int32_t)calibration.dig_H3)) / 2048;
  var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
  var2 = ((var4 * ((int32_t)calibration.dig_H2)) + 8192) / 16384;
  var3 = var5 * var2;
  var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
  var5 = var3 - ((var4 * ((int32_t)calibration.dig_H1)) / 16);
  var5 = (var5 < 0 ? 0 : var5);
  var5 = (var5 > 419430400 ? 419430400 : var5);
  uint32_t H = (uint32_t)(var5 / 4096);

  return (float)H / 1024.0;
}



// spr id czujnika
uint32_t BME280::sensorID(void) {
   return _sensorID; 
   }

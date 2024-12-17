#ifndef __BME280_H__
#define __BME280_H__

#include "Arduino.h"

#include <Adafruit_I2CDevice.h>

#define BME280_ADDRESS (0x76)           // Primary I2C Address

#define BME280_ADDRESS_ALTERNATE (0x77) // Alternate Address

// REJESTRY //
enum {
  /* Rejestry dig od temp */
  BME280_REGISTER_DIG_T1 = 0x88,
  BME280_REGISTER_DIG_T2 = 0x8A,
  BME280_REGISTER_DIG_T3 = 0x8C,

  /*Rejestry dig od cisnienia*/
  BME280_REGISTER_DIG_P1 = 0x8E,
  BME280_REGISTER_DIG_P2 = 0x90,
  BME280_REGISTER_DIG_P3 = 0x92,
  BME280_REGISTER_DIG_P4 = 0x94,
  BME280_REGISTER_DIG_P5 = 0x96,
  BME280_REGISTER_DIG_P6 = 0x98,
  BME280_REGISTER_DIG_P7 = 0x9A,
  BME280_REGISTER_DIG_P8 = 0x9C,
  BME280_REGISTER_DIG_P9 = 0x9E,

  /*Rejestry dig od wilgotnosci*/
  BME280_REGISTER_DIG_H1 = 0xA1,
  BME280_REGISTER_DIG_H2 = 0xE1,
  BME280_REGISTER_DIG_H3 = 0xE3,
  BME280_REGISTER_DIG_H4 = 0xE4,
  BME280_REGISTER_DIG_H5 = 0xE5,
  BME280_REGISTER_DIG_H6 = 0xE7,


  BME280_REGISTER_CHIPID = 0xD0,        // id modulu
  BME280_REGISTER_SOFTRESET = 0xE0,     // reset

  BME280_REGISTER_CONTROLHUMID = 0xF2,  // ustawienia wilgotnosci
  BME280_REGISTER_STATUS = 0XF3,        // stasus
  BME280_REGISTER_CONTROL = 0xF4,       // ustawienia pomiaru
  BME280_REGISTER_CONFIG = 0xF5,        // konfiguracja
  BME280_REGISTER_PRESSUREDATA = 0xF7,  // Rejestr do obliczen cisnienia
  BME280_REGISTER_TEMPDATA = 0xFA,      // Rejestr do obliczen temperatury
  BME280_REGISTER_HUMIDDATA = 0xFD      // Rejestr do obliczen wilgotnosci
}BME280_registers;


/* Struktura kalibracyjna czujnika - zmienne do zapisu odczytanych danych */
typedef struct SensorCalibration {
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;

  uint16_t dig_P1; 
  int16_t dig_P2;  
  int16_t dig_P3;  
  int16_t dig_P4;  
  int16_t dig_P5;  
  int16_t dig_P6;  
  int16_t dig_P7;  
  int16_t dig_P8;  
  int16_t dig_P9;  

  uint8_t dig_H1; 
  int16_t dig_H2; 
  uint8_t dig_H3; 
  int16_t dig_H4; 
  int16_t dig_H5; 
  int8_t dig_H6;  
} ;

/* GLOWNA KLASA OPERACYJNA*/
class BME280 {
public:
  /*konstruktor*/
  BME280();

  /* wartosci do wyboru nadprobkowania */
  enum  BME280_OverSampling {
    BME280_OVSNONE = 0X00,
    BME280_OVSX1 = 0X01,
    BME280_OVSX2 = 0X02,
    BME280_OVSX4 = 0X03,
    BME280_OVSX8 = 0X04,
    BME280_OVSX16 = 0X05
  };

	/* wartosci do wyboru trybu pracy */
  enum  BME280_POWER_MODE {
      BME280_SLEEP_MODE	= 0,
			BME280_FORCED_MODE	= 1,
			BME280_NORMAL_MODE	= 3
  };


	/* opcje filtra */
  enum  BME280_Filter {
    BME280_FIlterMode_OFF = 0,
    BME280_FIlterMode_X2 = 1,
    BME280_FIlterMode_X4 = 2,
    BME280_FIlterMode_X8 = 3,
    BME280_FIlterMode_X16 = 4
  };

	/* wartosci do wyboru czasu standby */
  enum  BME280_StandbyTime {
    BME280_StandbyTime_0_5_MS = 0,
    BME280_StandbyTime_MS_62_5 = 1,
    BME280_StandbyTime_MS_125 = 2,
    BME280_StandbyTime_MS_250 = 3,
    BME280_StandbyTime_MS_500 = 4,
    BME280_StandbyTime_MS_1000 = 5
  };
/* FUNKCJE DO KALIBRACJI USTAWIEN */

		// 0X76 - standard;
		// 0x77 - adress v2;
		uint8_t Address(uint8_t address);

		// BME280_SLEEP_MODE;
		// BME280_FORCED_MODE;
		// BME280_NORMAL_MODE;
		uint8_t Mode(uint8_t mode);

		//	StandbyTime_0_5_MS;
		//	StandbyTime_62_5_MS;
		//	StandbyTime_125_MS;
		//	StandbyTime_250_MS;
		//	StandbyTime_500_MS;
		//	StandbyTime_1000_MS;
		//	StandbyTime_2000_MS;
		//	StandbyTime_4000_MS;
		uint8_t Standby(uint8_t standby);

		
		//	FIlterMode_OFF;
		//	FIlterMode_2;
		//	FIlterMode_4;
		//	FIlterMode_8;
		//	FIlterMode_16;
		uint8_t Filter(uint8_t Filter);

		//	BME280_OVS_OFF;
		//	BME280_OVS_1X;
		//	BME280_OVS_2X;
		//	BME280_OVS_4X;
		//	BME280_OVS_8X;
		//	BME280_OVS_16X;
		uint8_t TempOVS(uint8_t tempOVS);

		//	BME280_OVS_OFF;
		//	BME280_OVS_1X;
		//	BME280_OVS_2X;
		//	BME280_OVS_4X;
		//	BME280_OVS_8X;
		//	BME280_OVS_16X;
		uint8_t HumOVS(uint8_t humOVS);

		//	BME280_OVS_OFF;
		//	BME280_OVS_1X;
		//	BME280_OVS_2X;
		//	BME280_OVS_4X;
		//	BME280_OVS_8X;
		//	BME280_OVS_16X;
		uint8_t PresOVS(uint8_t presOVS);


	//wartosci globalne
		uint8_t address1;
		uint8_t mode1;
		uint8_t filter1;
		uint8_t t_ovs;
		uint8_t p_ovs;
		uint8_t h_ovs;
		uint8_t stanby1;
    int32_t _sensorID;  // id czujnika
    int32_t t_fine;     // zmienna temp

    int32_t t_fine_adjust = 0; // zmienna kompensacji temp
  
  

  ~BME280(void);
  /* funkcja rozpoczynajaca pomiar */
  bool begin(uint8_t addr = BME280_ADDRESS, TwoWire *theWire = &Wire);
  /* konfiguracja pomiarow */
  void Meas_config(uint8_t mode, uint8_t filter, uint8_t standby, uint8_t presovs, uint8_t humovs, uint8_t tempovs);

  /* odczyt pomiarow */
  float readTemperature(void);
  float readPressure(void);
  float readHumidity(void);

  /* kompensacja temperatury*/
  float getTemperatureCompensation(void);
  void setTemperatureCompensation(float);

  /* odczyt id czujnika*/
  uint32_t sensorID(void);



protected:
  /* pointer magistralii i2c*/
  Adafruit_I2CDevice *i2c_dev = NULL;


  /* przechowywanie kalibracji */
  SensorCalibration calibration; 


  void readCoefficients(void);

  
  void write(byte reg, byte value);
  uint32_t read(byte reg, uint8_t);



  
};

#endif

/*
 * ADXL345_i2c.c
 *
 *  Created on: Feb 25, 2023
 *      Author: dogan
 */

#include "ADXL345_i2c.h"



uint8_t i;
uint8_t data_rec[6];

float xg, yg, zg;

void Scan_I2C_Address()
{
	for(i=0; i<=255; i++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1, i, 1, 10) == HAL_OK)
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_11);
			break;
		}
	}
}

void ADXL_Write(uint8_t reg, uint8_t value)
{
	uint8_t data[2]={0};
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADDRESS, data, 2, 100);
}

void ADXL_Read(uint8_t reg, uint8_t numberOfBytes)
{
	HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDRESS, reg, 1, data_rec, numberOfBytes, 100);

	// 281.satır ve sonrasının ardından bu noktaya gelip, verileri okuruz. Veriler, DATAX0, DATAX1, DATAY0, DATAY1, DATAZ0, DATAZ1
	//şeklinde 0x32 ila 0x37 arasındaki Kayıtlarda depolanır. Her kayıt 8 bit veri depolar. bu sebeple 8 bitlik bir değişkene 6 farklı değeri kaydettik.

	// Burada MEM Read yapıyoruz. Çünkü belirli bir bellek adresinden yani 0x32'den okuyoruz ve bu adresten itibaren 6 byte veri okumak zorundayız.
	// Bundan sonra yapılması gereken işlem DATAX0 ve DATAX1 deki veriler tek bir veriye dönüştürülmeli bu işlem 125.satırda yapılır
}

void ADXL_Init()
{
	ADXL_Read(0x00, 1);		// Read adres: 0x00 and 1 bit if bits = 0xE5 sensor is true work
	// Kodlarımızı yazmadan önce, cihazın DEVID değerini kontrol ederek her şeyin yolunda olup olmadığından emin olmakta fayda var.
	// DEVID kaydı (0x00), 0xE5 sabit bir cihaz kimliği koduna sahiptir. Kod 0xE5 ile eşleşirse, çalışmaya devam edebilir, dataları okuyabiliriz.
	// 271.satır

	ADXL_Write(0x2D, 0);	// Reset all bits

	// Setup for measurement
	ADXL_Write(0x2D, 0x08);	// Measure bit-1, wake up 0, 0 at 8 hz (Ölçüm bitini set, wake up bitini reset ve frekansı 8Hz olarak ayarladık.)
	// Set to Range +- 4g
	ADXL_Write(0x31, 0x01);	// +- 4g range
	// And ok ready to read for value
}

void values(int16_t *x, int16_t *y, int16_t *z )
{

	 ADXL_Read(0x32, 6);
		  *x = (data_rec[1] << 8) | data_rec[0];
		  *y = (data_rec[3] << 8) | data_rec[2];
		  *z = (data_rec[5] << 8) | data_rec[4];

		  // Bu işlemin ardından eksendeki ivmeyi kontrol etmek için bu verileri g şeklinde dönüştürmemiz gerekir. Yukarıdakileri
		  // başlatma bölümünde +-4g aralığını belirttiğimiz gibi kontrol etmelisiniz. Datasheete göre +-4g aralığında ölçek faktörü 7,8mg/LSB'dir.

		  // Bu yüzden g'ye dönüştürmek için .0078 ile çarpmamız gerekiyor.


		  HAL_Delay(10);


}


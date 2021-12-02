#ifndef MPU_HPP
#define MPU_HPP

#include <Wire.h>

//Estutura para armazenar os dados do MPU
struct MPUData {
    int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
};

class MPU6050_Solar {
   private:
    int _MPUAddress;
    unsigned int _I2C_timeout;
    MPUData _data;

   public:
    MPU6050_Solar(int MPUAddress, MPUData& data, int I2C_timeout = 1000) {
        _MPUAddress = MPUAddress;
        _I2C_timeout = I2C_timeout;
        _data = data;
    }

    void init() {
        // Configura o MPU
        // -- Configuração do Gyro -- //
        Wire.beginTransmission(_MPUAddress);
        Wire.write(0x1B);  // Registro de configuração do Gyro
        Wire.write(0x00);  // Configura o Full Scale Range para + ou - 250 graus por segundo
        Wire.endTransmission(true);
        // -- Configuração do Acelerômetro -- //
        Wire.beginTransmission(_MPUAddress);
        Wire.write(0x1C);  // Registro de configuração do Gyro
        Wire.write(0x00);  // Configura o Full Scale Range para + ou - 250 graus por segundo
        Wire.endTransmission(true);
        // Ativa o MPU
        Wire.beginTransmission(_MPUAddress);
        Wire.write(0x6B);  // Registro PWR_MGMT_1
        Wire.write(0);     // definido como zero (ativa o MPU-6050)
        Wire.endTransmission(true);
        // Aguarda o MPU estabilizar a leitura
        delay(100);
    }

    void readMPU() {
        Wire.beginTransmission(_MPUAddress);
        Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        //Solicita os dados do sensor
        Wire.requestFrom(_MPUAddress, 14, true);
        //Armazena o valor dos sensores nas variaveis correspondentes
        _data.AcX = Wire.read() << 8 | Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        _data.AcY = Wire.read() << 8 | Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        _data.AcZ = Wire.read() << 8 | Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        _data.Tmp = Wire.read() << 8 | Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
        _data.GyX = Wire.read() << 8 | Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        _data.GyY = Wire.read() << 8 | Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        _data.GyZ = Wire.read() << 8 | Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

        //-- DEBUG --//
#ifdef DEBUG_MPU
        Serial.print(" | X= ");
        Serial.print(_data.AcX);
        Serial.print(" | Y= ");
        Serial.print(_data.AcY);
        Serial.print(" | Z= ");
        Serial.print(_data.AcZ);
#endif
    }
};

#endif  // MPU_HPP
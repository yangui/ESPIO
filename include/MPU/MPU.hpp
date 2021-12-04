#ifndef MPU_HPP
#define MPU_HPP

#include <Wire.h>

//Estutura para armazenar os dados do MPU
struct MPUData {
    int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
    double roll, pitch, yaw;
};

class MPU6050_Solar {
   private:
    int _MPUAddress;
    MPUData _data;
    const unsigned int _LSB_to_G = 16384;
    const double _LSB_to_rad_per_second = 131 * 0.01745;
    const int _LSB_to_celsius = 340;

   public:
    MPU6050_Solar(int MPUAddress) {
        _MPUAddress = MPUAddress;
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

    void readMPU(MPUData &_data) {
        Wire.beginTransmission(_MPUAddress);
        Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        //Solicita os dados do sensor
        Wire.requestFrom(_MPUAddress, 14, true);
        //Armazena o valores dos sensores nas variaveis correspondentes
        _data.AcX = Wire.read() << 8 | Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        _data.AcY = Wire.read() << 8 | Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        _data.AcZ = Wire.read() << 8 | Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        _data.Tmp = Wire.read() << 8 | Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
        _data.GyX = Wire.read() << 8 | Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        _data.GyY = Wire.read() << 8 | Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        _data.GyZ = Wire.read() << 8 | Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

        // Converte as unidades dos dados recebidos para as correspondentes
        // Aceleração  = G
        // Temperatura = Celsius
        // Gyro        = Rad/s
        _data.AcX *= _LSB_to_G;
        _data.AcY *= _LSB_to_G;
        _data.AcZ *= _LSB_to_G;
        _data.Tmp = _data.Tmp * _LSB_to_celsius + 35;
        _data.GyX *= _LSB_to_rad_per_second;
        _data.GyY *= _LSB_to_rad_per_second;
        _data.GyZ *= _LSB_to_rad_per_second;

        //-- DEBUG --//

#ifdef DEBUG_MPU
        Serial.print(" | X Gs: ");
        Serial.printf("%05d | ", _data.AcX);
        Serial.print(" | Y Gs: ");
        Serial.printf("%05d | ", _data.AcY);
        Serial.print(" | Z Gs: ");
        Serial.printf("%05d | ", _data.AcZ);
        Serial.print("\t");
#endif
    }
};

#endif  // MPU_HPP
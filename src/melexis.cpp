/*
 * melexis.cpp
 * created by Brendan Berg on 05.02.2018
 *
 * Copyright (c) 2018 SiemBots
 */


#include "melexis.hpp"


Melexis::Melexis(uint8_t _address)
{
    i2c_init();

    address = _address;

    Melexis::read();
}

void Melexis::read(void)
{
    // start reading linearized temperature
    i2c_start_wait(address+I2C_WRITE);
    i2c_write(0x07);
    i2c_rep_start(address+I2C_READ);

    // read 1 byte and then send ack.
    uint8_t data_low = i2c_readAck();
    // read 1 byte and then send ack.
    uint8_t data_high = i2c_readAck();

    // stop
    i2c_readNak();
    i2c_stop();

    // remove error bit and combine data bytes
    temperatureRaw = ((data_high & 0x7F) << 8) | data_low;

    // convert to non-retarded-unit (Fahrenheit is retarded)
    temperatureCelsius = Melexis::convertToCelsius(temperatureRaw);
}

float Melexis::getTemperature(void)
{
    return temperatureCelsius;
}

float Melexis::convertToCelsius(uint16_t _raw)
{
    // some datasheet magic
    return ((float)(_raw))*0.02 - 273.15;
}

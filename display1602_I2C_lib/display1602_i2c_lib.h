/**
 * @file display1602_i2c_lib.cpp
 * @author Denys Khmil
 * @brief This file contents display1602 class and functions declarations
 */
#ifndef DISPLAY1602_I2C_LIB
#define DISPLAY1602_I2C_LIB

#include "main.h"

/**
  * @brief LCD 1602 I2C class
  */
class display1602_i2c
{
public:
    display1602_i2c(I2C_HandleTypeDef _i2c);
    display1602_i2c(I2C_HandleTypeDef _i2c, uint8_t address);
    void clear();
    void set_cursor(uint8_t x, uint8_t y);
    void print(uint8_t data);
    void printString(const char* data);
    void diplay_cusor(uint8_t state);
    void display_blink(uint8_t state);
private:
    uint8_t settings;
    uint8_t config;
    I2C_HandleTypeDef i2c;
    uint8_t deviceAddr; 

    void displayInit();
    void send4bites(uint8_t data, uint8_t command);
    void sendbyte(uint8_t data, uint8_t command);
};


#endif
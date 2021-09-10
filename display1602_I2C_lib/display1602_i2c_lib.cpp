/**
 * @file display1602_i2c_lib.cpp
 * @author Denys Khmil
 * @brief This file contents all the display1602 library functions
 */
#include "display1602_i2c_lib.h"

/**
 * @brief display1602 constructor with defauld i2c address
 * @param _i2c: display i2c port.
 */
display1602_i2c::display1602_i2c(I2C_HandleTypeDef _i2c){
    this->i2c = _i2c;
    this->deviceAddr = 0b0100111;
    this->displayInit();
}

/**
 * @brief display1602 constructor with specified i2c address
 * @param _i2c: display i2c port.
 * @param _address: address of a display.
 */
display1602_i2c::display1602_i2c(I2C_HandleTypeDef _i2c, uint8_t _address){
    this->i2c = _i2c;
    this->deviceAddr = _address;
    this->displayInit();
}

/**
 * @brief Display initialization routine
 * @note This function initializes display to 4bit mode
 * @retval None
 */
void display1602_i2c::displayInit(){
    HAL_Delay(15);
    this->send4bites(0b00000011,0);
    HAL_Delay(5);
    this->send4bites(0b00000011,0);
	HAL_Delay(1);
    this->send4bites(0b00000011,0);

	HAL_Delay(1);
    this->send4bites(0b00000010,0);

    this->sendbyte(0b00101000,0);
    HAL_Delay(1);
    this->sendbyte(0b00001100,0);
    HAL_Delay(1);
    this->sendbyte(0b00000001,0);
    HAL_Delay(1);
    this->sendbyte(0b00000110,0);

    this->settings = 0b00001100;
    this->config = 0b00001100;
}

/**
 * @brief Display clear function
 * @note This function cleares display content
 * @retval None
 */
void display1602_i2c::clear(){
    this->sendbyte(0b00000001,0);
}

/**
 * @brief Sets cursor on a specified coordinates
 * @param x: X coordinate (vertical)
 * @param y: Y coordinate (horisontal)
 * @retval None
 */
void display1602_i2c::set_cursor(uint8_t x, uint8_t y){
    this->sendbyte(0b10000000|(x+0x40*y), 0);
	HAL_Delay(10);
}

/**
 * @brief Print one symbol on a display 
 * @param data: Symbol to print(uint8_t (char))
 * @retval None
 */
void display1602_i2c::print(uint8_t data){
    this->sendbyte(data,1);
    HAL_Delay(2);
}

/**
 * @brief Print a string on a display
 * @param data: String to print
 * @retval None
 */
void display1602_i2c::printString(const char* data){
    uint8_t i=0;
	while(data[i]!=0)
	{
		this->print(data[i]);
		HAL_Delay(1);
		i++;
	}
}

/**
 * @brief Turn cursor on/off 
 * @param state: Specify if you want to turn cursor on or off
 * 					@arg 0: OFF
 * 					@arg 1: ON
 * @retval None
 */
void display1602_i2c::diplay_cusor(uint8_t state){
	if(state){
		settings |= (1 << 1);
		this->sendbyte(settings, 0);
	}
	else{
		settings &= ~(1 << 1);
		this->sendbyte(settings, 0);
	}
}

/**
 * @brief Turn blink on/off 
 * @param state: Specify if you want to turn blink on or off
 * 					@arg 0: OFF
 * 					@arg 1: ON
 * @retval None
 */
void display1602_i2c::display_blink(uint8_t state){
    if(state){
			settings |= (1 << 0);
			this->sendbyte(settings, 0);
		}
		else{
			settings &= ~(1 << 0);
			this->sendbyte(settings, 0);
		}
}

/**
 * @brief Writes 4 bits
 * @note This function writes 4 bits to display pins.
 * @param data: 4 bits to send (only 0-3 bits will be send) 
 * @param command: Specify if you want to send data or command
 * 					@arg 0: Data
 * 					@arg 1: Command
 * @retval None
 */
void display1602_i2c::send4bites(uint8_t data, uint8_t command){
    uint8_t buffer[2];
    buffer[0] = this->config;
    buffer[0] = buffer[0]|(data << 4);
    if (command){
        buffer[0] |= (1 << 0); //data
    }
    buffer[1] = buffer[0] & ~(1 << 2);

    HAL_I2C_Master_Transmit(&this->i2c, (this->deviceAddr << 1), &buffer[0], 1, 100);
    HAL_I2C_Master_Transmit(&this->i2c, (this->deviceAddr << 1), &buffer[1], 1, 100);

}

/**
 * @brief Writes byte
 * @note This function writes 8 bits to display pins (Uses writeHalfByte function twice)
 * @param data: 4 bits to send (only 0-3 bits will be send)
 * @param command: Specify if you want to send data or command
 * 					@arg 0: Data
 * 					@arg 1: Command
 * @retval None
 */
void display1602_i2c::sendbyte(uint8_t data, uint8_t command){
    uint8_t buf = 0;
	buf = data >> 4;

    this->send4bites(buf, command);
	this->send4bites(data, command);
}
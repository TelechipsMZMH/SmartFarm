/*
 * display.h
 *
 *  Created on: Oct 29, 2025
 *      Author: mokta
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

#define PAGE_NUMBER 3;

typedef enum {
	Display_Page_No_0 = 0,
	Display_Page_No_1 = 1,
	Display_Page_No_2 = 2
} Display_Page_No;

typedef struct {
	uint8_t temp_integer;
	uint8_t temp_decimal;
	uint8_t humid_integer;
	uint8_t humid_decimal;

	uint8_t soil_moisture;

	bool is_heat_on;
	bool is_fan_on;
	bool is_light_on;

	Display_Page_No page_no;
} Display_Info;




void init_display(I2C_HandleTypeDef *hi2c);

void set_display_temp(uint8_t temp_int, uint8_t temp_dec);

void set_display_humid(uint8_t humid_int, uint8_t humid_dec);

void set_soil_moisture(uint8_t soil_moist);

void set_heat_state(bool state);

void set_fan_state(bool state);

void set_light_state(bool state);

void show_next_page();

void show_display();

#endif /* INC_DISPLAY_H_ */

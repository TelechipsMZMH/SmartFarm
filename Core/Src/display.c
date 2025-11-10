/*
 * display.c
 *
 *  Created on: Oct 29, 2025
 *      Author: mokta
 */

#include "stm32f429xx.h"

#include "display.h"
#include "i2c_lcd.h"

static Display_Info info;

uint8_t display_message[2][17];

uint8_t empty_box[8] = {
	0x00, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00
};
uint8_t black_box[8] = {
	0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00
};

void init_display(I2C_HandleTypeDef *hi2c)
{
	info.humid_integer = 00;
	info.humid_decimal = 0;
	info.temp_integer = 00;
	info.temp_decimal = 0;
	info.soil_moisture = 0;
	info.is_fan_on = false;
	info.is_heat_on = false;
	info.is_light_on = false;

	info.page_no = Display_Page_No_2;
	lcd_create_char(1, empty_box);
	lcd_create_char(2, black_box);

	HAL_Delay(100);
	lcd_init(hi2c);
	lcd_backlight(1);
}

void set_display_temp(uint8_t temp_int, uint8_t temp_dec)
{
	info.temp_integer = temp_int;
	info.temp_decimal = temp_dec;
}

void set_display_humid(uint8_t humid_int, uint8_t humid_dec)
{
	info.humid_integer = humid_int;
	info.humid_decimal = humid_dec;
}

void set_soil_moisture(uint8_t soil_moist)
{
	info.soil_moisture = soil_moist;
}

void set_heat_state(bool state)
{
	info.is_heat_on = state;
}

void set_fan_state(bool state)
{
	info.is_fan_on = state;
}

void set_light_state(bool state)
{
	info.is_light_on = state;
}

void show_next_page()
{
	info.page_no = (info.page_no + 1) % PAGE_NUMBER;
	show_display();
}

void show_display()
{
	switch(info.page_no) {
	case Display_Page_No_0:
		sprintf(display_message[0], "T:%02d.%01d%cC H:%02d.%01d%c",
				info.temp_integer % 100, info.temp_decimal % 10, 0xDF,
				info.humid_integer % 100, info.humid_decimal % 10 , 0x25);
	    sprintf(display_message[1], "Moist:");
		for(uint8_t i = 0; i < 10; i++)
	    {
	        if(i < info.soil_moisture)
	            strcat(display_message[1], "\xFF");  // ■ (0xFF: 꽉 찬 블록)
	        else
	            strcat(display_message[1], " ");     // 빈 공간
	    }
		break;
	case Display_Page_No_1:
		sprintf(display_message[0], "%c HEATER", info.is_heat_on ? 0x02 : 0x01);
		sprintf(display_message[1], "%c FAN", info.is_fan_on ? 0x02 : 0x01);
		break;
	case Display_Page_No_2:
		sprintf(display_message[0], "%c LIGHT", info.is_light_on ? 0x02 : 0x01);
		sprintf(display_message[1], "");
		break;
	default:
		sprintf(display_message[0], "");
		sprintf(display_message[1], "");
		return;
	}

	lcd_clear();
	lcd_set_cursor(0, 0);  // 1번째 줄, 첫 번째 칸
	lcd_print(display_message[0]);
	lcd_set_cursor(0, 1);  // 2번째 줄, 첫 번째 칸
	lcd_print(display_message[1]);
}

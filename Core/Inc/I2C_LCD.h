/* i2c_lcd.h */
#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "stm32f4xx_hal.h"

/* LCD I2C 주소 (스캔 결과에 따라 수정) */
#define LCD_ADDR (0x27 << 1)  // 7bit 주소를 8bit로 변환

/* LCD 명령어 */
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_FUNCTION_SET    0x20

/* Entry Mode 설정 */
#define LCD_ENTRY_RIGHT          0x00
#define LCD_ENTRY_LEFT           0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

/* Display Control */
#define LCD_DISPLAY_ON  0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON   0x02
#define LCD_CURSOR_OFF  0x00
#define LCD_BLINK_ON    0x01
#define LCD_BLINK_OFF   0x00

/* Function Set */
#define LCD_8BIT_MODE   0x10
#define LCD_4BIT_MODE   0x00
#define LCD_2LINE       0x08
#define LCD_1LINE       0x00
#define LCD_5x10DOTS    0x04
#define LCD_5x8DOTS     0x00

/* Backlight */
#define LCD_BACKLIGHT   0x08
#define LCD_NOBACKLIGHT 0x00

/* Enable bit */
#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

/* 함수 프로토타입 */
void lcd_init(I2C_HandleTypeDef *hi2c);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(char *str);
void lcd_backlight(uint8_t state);
void lcd_create_char(uint8_t location, uint8_t charmap[8]);
void lcd_write_custom_char(uint8_t location);


#endif /* I2C_LCD_H_ */

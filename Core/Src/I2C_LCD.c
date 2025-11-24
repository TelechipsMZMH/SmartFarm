/* i2c_lcd.c */
#include <i2c_lcd.h>

static I2C_HandleTypeDef *lcd_i2c;
static uint8_t backlight_state = LCD_BACKLIGHT;

/* i2c_lcd.c */

/* 커스텀 문자 생성 함수 */
void LCD_create_char(uint8_t location, uint8_t charmap[8])
{
    location &= 0x07;  // 0~7만 가능

    /* CGRAM 주소 설정 */
    lcd_send_cmd(0x40 | (location << 3));

    /* 8바이트 패턴 전송 */
    for(uint8_t i = 0; i < 8; i++)
    {
        lcd_send_data(charmap[i]);
    }

    /* 다시 DDRAM으로 복귀 */
    lcd_send_cmd(0x80);
}

/* 커스텀 문자 출력 */
void LCD_write_custom_char(uint8_t location)
{
    lcd_send_data(location & 0x07);
}

/* Low-level 함수: I2C로 바이트 전송 */
void lcd_send_byte(uint8_t data)
{
    HAL_I2C_Master_Transmit(lcd_i2c, LCD_ADDR, &data, 1, 100);
}

/* 4bit 모드로 상위/하위 니블 전송 */
void lcd_send_nibble(uint8_t nibble)
{
    uint8_t data = nibble | backlight_state;

    /* Enable High */
    lcd_send_byte(data | En);
    HAL_Delay(1);

    /* Enable Low */
    lcd_send_byte(data & ~En);
    HAL_Delay(1);
}

/* 명령어 전송 (RS=0) */
void lcd_send_cmd(uint8_t cmd)
{
    uint8_t upper_nibble = (cmd & 0xF0);
    uint8_t lower_nibble = ((cmd << 4) & 0xF0);

    lcd_send_nibble(upper_nibble);  // 상위 4bit
    lcd_send_nibble(lower_nibble);  // 하위 4bit
}
/* 데이터 전송 (RS=1) */
void lcd_send_data(uint8_t data)
{
    uint8_t upper_nibble = (data & 0xF0) | Rs;
    uint8_t lower_nibble = ((data << 4) & 0xF0) | Rs;

    lcd_send_nibble(upper_nibble);
    lcd_send_nibble(lower_nibble);
}

/* LCD 초기화 */
void lcd_init(I2C_HandleTypeDef *hi2c)
{
    lcd_i2c = hi2c;

    /* LCD 전원 안정화 대기 (최소 15ms) */
    HAL_Delay(50);

    /* 4bit 모드 초기화 시퀀스 */
    lcd_send_nibble(0x30);  // Function set: 8bit mode
    HAL_Delay(5);

    lcd_send_nibble(0x30);  // Function set: 8bit mode
    HAL_Delay(1);

    lcd_send_nibble(0x30);  // Function set: 8bit mode
    HAL_Delay(1);

    lcd_send_nibble(0x20);  // Function set: 4bit mode
    HAL_Delay(1);

    /* LCD 설정 */
    lcd_send_cmd(LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE | LCD_5x8DOTS);
    HAL_Delay(1);

    lcd_send_cmd(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
    HAL_Delay(1);

    lcd_send_cmd(LCD_CLEAR);
    HAL_Delay(2);

    lcd_send_cmd(LCD_ENTRY_MODE | LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT);
    HAL_Delay(1);

    lcd_send_cmd(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
    HAL_Delay(1);

    lcd_clear();
}

/* 화면 지우기 */
void lcd_clear(void)
{
    lcd_send_cmd(LCD_CLEAR);
    HAL_Delay(2);
}

/* 커서 위치 설정 */
void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

/* 문자열 출력 */
void LCD_print(uint8_t *str)
{
    while(*str) {
        lcd_send_data(*str++);
    }
}

/* 백라이트 제어 */
void LCD_backlight(uint8_t state)
{
    if(state) {
        backlight_state = LCD_BACKLIGHT;
    }
    else {
        backlight_state = LCD_NOBACKLIGHT;
    }

    lcd_send_byte(backlight_state);
}

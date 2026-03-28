
#ifndef _SG_LGPIO_H_
#define _SG_LGPIO_H_

#include <stdint.h>
#include <stdio.h>
#include <lgpio.h>

#define LGPIO_INPUT  0
#define LGPIO_OUTPUT 1

#define LGPIO_LOW  0
#define LGPIO_HIGH 1

extern int I2C_handle;
extern int SPI_handle;
extern int Gpio_handle;

void delay(uint32_t xms);
int Lgpio_export(void);
int Lgpio_set_pinmode(int pin_number,int mode);
int Lgpio_read_pin(int pin_number);
int Lgpio_write_pin(int Pin, int value);

void LG_SPI_Init(int dev, int chain, int baud);
void LG_SPI_writebyte(uint8_t value);
void LG_SPI_write_bytes(uint8_t *data, uint32_t len);
void LG_PWM(int pin_number, float freq, float duty);

#endif

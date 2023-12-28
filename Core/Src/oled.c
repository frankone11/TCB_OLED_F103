/*
 * oled.c
 *
 *  Created on: Nov 19, 2023
 *      Author: paco
 */

#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "arial.h"

#define ADDR 0x78U
#define MAX_DAT 32U

uint8_t buffer[1024] = {0};

void Oled_Comando(I2C_HandleTypeDef *hi2c, uint8_t comando)
{
	uint8_t buf[2] = { 0, 0 };
	buf[1] = comando;
	HAL_I2C_Master_Transmit(hi2c, ADDR, buf, 2, 1000);
}

void Oled_Comandos(I2C_HandleTypeDef *hi2c, const uint8_t *buffer, uint8_t tam)
{
	if(tam > MAX_DAT)
		return;
	uint8_t buf[MAX_DAT + 1] = {0};
	for(uint8_t i = 0; i < tam; i++)
		buf[i+1] = buffer[i];
	HAL_I2C_Master_Transmit(hi2c, ADDR, buf, tam+1, 1000);
}

void Oled_Dato(I2C_HandleTypeDef *hi2c, uint8_t dato)
{
	uint8_t buf[2] = { 0x40U, 0};
	buf[1] = dato;
	HAL_I2C_Master_Transmit(hi2c, ADDR, buf, 2, 100);
}

void Oled_Inicializa(I2C_HandleTypeDef *hi2c)
{
	const uint8_t init1[] =
	{
			0xAEU,
			0xD5U,
			0x80U,
			0xA8U
	};

	Oled_Comandos(hi2c, init1, sizeof(init1));
	Oled_Comando(hi2c, 0x3FU);

	const uint8_t init2[] =
	{
			0xD3U,
			0x00U,
			0x40U,
			0x8DU
	};

	Oled_Comandos(hi2c, init2, sizeof(init2));

	Oled_Comando(hi2c, 0x14U);

	const uint8_t init3[] =
	{
			0x20U,
			0x00U,
			0xA1U, //A0 | 1
			0xC8U
	};

	Oled_Comandos(hi2c, init3, sizeof(init3));

	Oled_Comando(hi2c, 0xDAU);
	Oled_Comando(hi2c, 0x12U);
	Oled_Comando(hi2c, 0x81U);
	Oled_Comando(hi2c, 0xCFU);
	Oled_Comando(hi2c, 0xD9U);
	Oled_Comando(hi2c, 0XF1U);

	const uint8_t init5[] =
	{
			0xDBU,
			0x40U,
			0xA4U,
			0xA6U,
			0x2EU,
			0xAFU,
	};

	Oled_Comandos(hi2c, init5, sizeof(init5));

	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Oled_Comandos(hi2c, initxy, sizeof(initxy));

	for(uint8_t j = 0; j < 8; j++)
	{
		for(uint8_t i = 0; i < 128; i++)
			Oled_Dato(hi2c, 0x00U);
	}
}

void Oled_Dibuja(I2C_HandleTypeDef *hi2c, const uint8_t *datos)
{
	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Oled_Comandos(hi2c, initxy, sizeof(initxy));

	for(uint8_t j = 0; j < 128; j++)
	{
		for(uint8_t i = 0; i < 8; i++)
			Oled_Dato(hi2c, *datos++);
	}
}

void Oled_DibujaTam(I2C_HandleTypeDef *hi2c, const uint8_t *datos, uint16_t tam)
{
	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Oled_Comandos(hi2c, initxy, sizeof(initxy));

	for(uint16_t i = 0; i < tam; i++)
	{
			Oled_Dato(hi2c, *datos++);
	}
}

void Oled_DibujaEjemplo(I2C_HandleTypeDef *hi2c)
{
	bool blanco = true;

	const uint8_t initxy [] =
	{
			0x21U,
			0x00U,
			0x7FU,
			0x22U,
			0x00U,
			0x07U
	};

	Oled_Comandos(hi2c, initxy, sizeof(initxy));

	for(uint8_t j = 0; j < 128; j++)
	{
		if(j % 16)
			blanco = !blanco;

		for(uint8_t i = 0; i < 8; i++)
		{
			if(blanco)
				Oled_Dato(hi2c, 0x00U);
			else
				Oled_Dato(hi2c, 0xFFU);
		}
	}
}

void Oled_LimpiaDisplay(I2C_HandleTypeDef *hi2c)
{
	memset(buffer, 0, 1024);
	Oled_Dibuja(hi2c, buffer);
}

void Oled_ColocaCaracter(uint8_t *dir, const char caracter)
{
	uint8_t noval[8] = { 0x18U, 0x3CU, 0x76U, 0x9BU, 0x66U, 0x3CU, 0x18U, 0x00U };
	const tImage *img = NULL;

	if(caracter < 0x20 || caracter == 127)
		memcpy(dir, noval, 8);
	else
	{
		img = Arial.chars[caracter - 0x20U].image;
		memcpy(dir, img->data, img->dataSize);
	}

}

void Oled_EnviaCadenaXY(I2C_HandleTypeDef *hi2c, const char *txt, uint8_t fila, uint8_t columna)
{
	uint16_t offset;
	offset = (fila * 128) + (columna * 8);
	if(!txt)
		return;
	while(*txt)
	{
		Oled_ColocaCaracter((uint8_t *)(buffer + offset), *txt++);
		offset += 8;
	}
}

void Oled_MuestraCadenaXY(I2C_HandleTypeDef *hi2c, const char *txt, uint8_t fila, uint8_t columna)
{
	uint16_t offset;
	offset = (fila * 128) + (columna * 8);
	if(!txt)
		return;
	while(*txt)
	{
		Oled_ColocaCaracter((uint8_t *)(buffer + offset), *txt++);
		offset += 8;
	}
}

void Oled_MuestraPantalla(I2C_HandleTypeDef *hi2c)
{
	Oled_Dibuja(hi2c, buffer);
}

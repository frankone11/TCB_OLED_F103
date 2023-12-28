/*
 * oled.h
 *
 *  Created on: Nov 19, 2023
 *      Author: paco
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#ifdef __cplusplus
extern "C" {
#endif

void Oled_Comando(I2C_HandleTypeDef *hi2c, uint8_t comando);
void Oled_Comandos(I2C_HandleTypeDef *hi2c, const uint8_t *buffer, uint8_t tam);
void Oled_Dato(I2C_HandleTypeDef *hi2c, uint8_t dato);
void Oled_Inicializa(I2C_HandleTypeDef *hi2c);
void Oled_Dibuja(I2C_HandleTypeDef *hi2c, const uint8_t *datos);
void Oled_DibujaEjemplo(I2C_HandleTypeDef *hi2c);
void Oled_LimpiaDisplay(I2C_HandleTypeDef *hi2c);
void Oled_EnviaCadenaXY(I2C_HandleTypeDef *hi2c, const char *txt, uint8_t fila, uint8_t columna);
void Oled_MuestraCadenaXY(I2C_HandleTypeDef *hi2c, const char *txt, uint8_t fila, uint8_t columna);
void Oled_MuestraPantalla(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_OLED_H_ */

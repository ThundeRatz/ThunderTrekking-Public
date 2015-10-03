#ifndef __HMC5883L_REGISTERS_H__
#define __HMC5883L_REGISTERS_H__

enum {
	HMC_CONFIGURATION_A,
	HMC_CONFIGURATION_B,
	HMC_MODE,
	HMC_DATA_OUT_X_MSB,
	HMC_DATA_OUT_X_LSB,
	HMC_DATA_OUT_Z_MSB,
	HMC_DATA_OUT_Z_LSB,
	HMC_DATA_OUT_Y_MSB,
	HMC_DATA_OUT_Y_LSB,
	HMC_STATUS,
	HMC_ID_A,
	HMC_ID_B,
	HMC_ID_C
};

// Configuration Register A:
#define HMC_SAMPLES_AVG_1	(0 << 5)
#define HMC_SAMPLES_AVG_2	(1 << 5)
#define HMC_SAMPLES_AVG_4	(2 << 5)
#define HMC_SAMPLES_AVG_8	(3 << 5)
#define HMC_OUTPUT_0_75HZ	(0 << 2)
#define HMC_OUTPUT_1_5HZ	(1 << 2)
#define HMC_OUTPUT_3HZ		(2 << 2)
#define HMC_OUTPUT_7_5HZ	(3 << 2)
#define HMC_OUTPUT_15HZ		(4 << 2)
#define HMC_OUTPUT_30HZ		(5 << 2)
#define HMC_OUTPUT_75HZ		(6 << 2)
#define HMC_NORMAL		0
#define HMC_POSITIVE		1
#define HMC_NEGATIVE		2

// Configuration Register B:
#define HMC_GAIN_1370		(0 << 5)
#define HMC_GAIN_1090		(1 << 5)
#define HMC_GAIN_820		(2 << 5)
#define HMC_GAIN_660		(3 << 5)
#define HMC_GAIN_440		(4 << 5)
#define HMC_GAIN_390		(5 << 5)
#define HMC_GAIN_330		(6 << 5)
#define HMC_GAIN_230		(7 << 5)

// Mode Register:
#define HMC_HIGH_SPEED_I2C	(1 << 7)
#define HMC_CONTINUOUS_MEASUREMENT	(0 << 0)
#define HMC_SINGLE_MEASUREMENT		(1 << 0)
#define HMC_IDLE			(1 << 1)

// Se houver overflowou underflow na leitura ou no cálculo com bias,
// -4096 é retornado (se houver overflow em uso normal do robô, ajustar
// bias!)
#define HMC_OVERFLOW		-4096

// Status Register:
#define HMC_LOCK			(1 << 1)
#define HMC_READY			(1 << 0)

// Identification Register A
#define HMC_ID_A_CHAR	'H'

// Identification Register B
#define HMC_ID_B_CHAR	'4'

// Identification Register C
#define HMC_ID_C_CHAR	'3'

#endif

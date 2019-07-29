/*
 * sram_test.c
 *
 *  Created on: 01/01/2001
 *      Author: Alexandre
 *  Altered on: 18/12/2018
 *		Author: Pedro Silva
 */

#include "sram_test.h"
#include <stdbool.h>
#include <stdlib.h>
#include "twi2c.h"

#define CLASSB_NSECS (8U)
#define CLASSB_OVERLAP (25UL)
#define CLASSB_SEC_SIZE (INTERNAL_SRAM_SIZE / CLASSB_NSECS)
#define CLASSB_SEC_REM (INTERNAL_SRAM_SIZE % CLASSB_NSECS)
#define CLASSB_OVERLAP_SIZE ((CLASSB_SEC_SIZE*CLASSB_OVERLAP)/100)

#define CLASSB_SRAM_INTRAWORD_TEST 1

#if (CLASSB_SEC_REM == 0)
#  define CLASSB_NSEC_TOTAL CLASSB_NSECS
#else
#  define CLASSB_NSEC_TOTAL CLASSB_NSECS + 1
#endif

#define INTERNAL_SRAM_START (0x800240)
#define INTERNAL_SRAM_SIZE (2048U)

static uint8_t classb_buffer[CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE];

bool sram_test_section(register uint8_t current_section);
bool classb_marchX(register volatile uint8_t p_sram[], register volatile uint8_t p_buffer[], register uint16_t size);



void sram_test(void)  {
	uint8_t rwrong;
	register bool has_errors = 0;

	for (register uint8_t i = 0; i < CLASSB_NSEC_TOTAL - 1; i++) {
		if (sram_test_section(i)) {
			has_errors = 1;
			break;
		}
	}
	if (has_errors)
	{
		rwrong = 0x00U;
	}
	else
	{
		rwrong = 0x9FU;
	}
	montagem(rwrong);

}

bool sram_test_section(register uint8_t current_section) {
	register uint8_t has_errors = 0;

	switch (current_section) {
	case 0:
		/* Test the buffer, which starts at INTERNAL_SRAM_START and ends at  CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE. There is no overlap with previous segments. */
		has_errors += classb_marchX((uint8_t *)INTERNAL_SRAM_START, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
		break;
	case 1:
		/* Test the first section, which size shrunk from below by the buffer when there is overlap. */
		/* In order to overlap with the buffer, we simply start at INTERNAL_SRAM_START + CLASSB_SEC_SIZE. */
		has_errors += classb_marchX((uint8_t *)INTERNAL_SRAM_START + CLASSB_SEC_SIZE, classb_buffer, CLASSB_SEC_SIZE);
		break;
	case CLASSB_NSECS:
		/* We test the last section of size SRAM_SIZE % CLASSB_NSECS */
		/* Limit size to the amount of memory remaining when dividing SRAM_SIZE with CLASSB_NSECS. */
		has_errors += classb_marchX((uint8_t *)INTERNAL_SRAM_START + CLASSB_NSECS * CLASSB_SEC_SIZE - CLASSB_OVERLAP_SIZE, classb_buffer, CLASSB_SEC_REM + CLASSB_OVERLAP_SIZE);
		break;
	default:
		/* Sections in the middle. We start CLASSB_OVERLAP_SIZE before the segment and test CLASSB_SEC_SIZE+CLASSB_OVERLAP_SIZE bytes */
		has_errors += classb_marchX((uint8_t *)INTERNAL_SRAM_START + current_section * CLASSB_SEC_SIZE - CLASSB_OVERLAP_SIZE, classb_buffer, CLASSB_SEC_SIZE + CLASSB_OVERLAP_SIZE);
		break;
	}

	return has_errors;
}

bool classb_marchX(register volatile uint8_t * p_sram, register volatile uint8_t * p_buffer, register uint16_t size)
{
	register uint16_t i = 0;
	register uint8_t has_error = 0;

	/* Save content of the section: copy to buffer unless we test the buffer */
	if (p_buffer != p_sram)
		for (uint16_t i = 0; i < size; i++)
			*(p_buffer+i) = *(p_sram+i);


	/* Test phase 1: write 0 to all bit locations. */
	for (i = 0; i < size; i++)
		*(p_sram+i) = 0x00;


	/* Test phase 2: read 0, write FF. */
	for (i = 0; i < size; i++)
	{
		if (*(p_sram+i) != 0x00)
			has_error = 1;
		else
			*(p_sram+i) = 0xFF;
	}


	/* Test phase 3: read FF, write 0 (reverse order) */
	for(i = size ; i>0; i--)
	{
		if (*(p_sram+i-1) != 0xFF)
			has_error = 1;
		else
			*(p_sram+i-1) = 0x00;
	}


	/* Test phase 4: read 0. */
	for (i = 0; i < size; i++)
		if (*(p_sram+i) != 0x00)
			has_error = 1;


#ifdef CLASSB_SRAM_INTRAWORD_TEST
	/* Intra-word march test. */
	for (i = 0; i < size; i++) {
		*(p_sram+i) = 0x55;
		if (*(p_sram+i) != 0x55)
			has_error = 1;

		*(p_sram+i) = 0xAA;
		if (*(p_sram+i) != 0xAA)
			has_error = 1;

		*(p_sram+i) = 0x33;
		if (*(p_sram+i) != 0x33)
			has_error = 1;

		*(p_sram+i) = 0xCC;
		if (*(p_sram+i) != 0xCC)
			has_error = 1;

		*(p_sram+i) = 0xF0;
		if (*(p_sram+i) != 0xF0)
			has_error = 1;
		*(p_sram+i) = 0x0F;
		if (*(p_sram+i) != 0x0F)
			has_error = 1;
	}

#endif


	/* Restore content of the section: copy from buffer, unless buffer is tested */
	if (p_buffer != p_sram)
		for (i = 0; i < size; i++)
			*(p_sram+i) = *(p_buffer+i);

	/* Call the error handler if there was an error. */
	return has_error;
}

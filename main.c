/* 
 * Copyright (C) 2010 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include "parse.h"

// See XAPP503 page 18
//
ParseStatus gotXTDOMASK(uint16 maskNumBits, const uint8 *maskBitmap) {
	uint16 numBytes = bitsToBytes(maskNumBits);
	printf("XTDOMASK(0x%02X, 0x", maskNumBits);
	while ( numBytes-- ) {
		printf("%02X", *maskBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXREPEAT(uint8 numRepeats) {
	printf("XREPEAT(0x%02X)\n", numRepeats);
	return PARSE_SUCCESS;
}

// See XAPP503 page 19
//
ParseStatus gotXRUNTEST(uint32 runTest) {
	printf("XRUNTEST(0x%08lX)\n", runTest);
	return PARSE_SUCCESS;
}
ParseStatus gotXSIR(uint8 sirNumBits, const uint8 *sirBitmap) {
	uint16 numBytes = bitsToBytes(sirNumBits);
	printf("XSIR(0x%02X, 0x", sirNumBits);
	while ( numBytes-- ) {
		printf("%02X", *sirBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXSDRSIZE(uint16 sdrSize) {
	printf("XSDRSIZE(0x%04X)\n", sdrSize);
	return PARSE_SUCCESS;
}

// See XAPP503 page 20
//
ParseStatus gotXSDRTDO(uint16 tdoNumBits, const uint8 *tdoBitmap, const uint8 *tdoMask) {
	const uint16 numBytes = bitsToBytes(tdoNumBits);
	uint16 i = numBytes;
	printf("XSDRTDO(0x%02X, 0x", tdoNumBits);
	while ( i-- ) {
		printf("%02X", *tdoBitmap++);
	}
	printf(", 0x");
	i = numBytes;
	while ( i-- ) {
		printf("%02X", *tdoBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXSDRB(uint16 tdiNumBits, const uint8 *tdiBitmap) {
	uint16 numBytes = bitsToBytes(tdiNumBits);
	printf("XSDRB(0x%02X, 0x", tdiNumBits);
	while ( numBytes-- ) {
		printf("%02X", *tdiBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXSDRC(uint16 tdiNumBits, const uint8 *tdiBitmap) {
	uint16 numBytes = bitsToBytes(tdiNumBits);
	printf("XSDRC(0x%02X, 0x", tdiNumBits);
	while ( numBytes-- ) {
		printf("%02X", *tdiBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXSDRE(uint16 tdiNumBits, const uint8 *tdiBitmap) {
	uint16 numBytes = bitsToBytes(tdiNumBits);
	printf("XSDRE(0x%02X, 0x", tdiNumBits);
	while ( numBytes-- ) {
		printf("%02X", *tdiBitmap++);
	}
	printf(")\n");
	return PARSE_SUCCESS;
}

// See XAPP503 page 22
//
ParseStatus gotXCOMPLETE() {
	printf("XCOMPLETE()\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXSTATE(TAPState tapState) {
	printf("XSTATE(");
	switch ( tapState ) {
		case TAPSTATE_TEST_LOGIC_RESET:
			printf("TEST_LOGIC_RESET");
			break;
		case TAPSTATE_RUN_TEST_IDLE:
			printf("RUN_TEST_IDLE");
			break;
		case TAPSTATE_SELECT_DR:
			printf("SELECT_DR");
			break;
		case TAPSTATE_CAPTURE_DR:
			printf("CAPTURE_DR");
			break;
		case TAPSTATE_SHIFT_DR:
			printf("SHIFT_DR");
			break;
		case TAPSTATE_EXIT1_DR:
			printf("EXIT1_DR");
			break;
		case TAPSTATE_PAUSE_DR:
			printf("PAUSE_DR");
			break;
		case TAPSTATE_EXIT2_DR:
			printf("EXIT2_DR");
			break;
		case TAPSTATE_UPDATE_DR:
			printf("UPDATE_DR");
			break;
		case TAPSTATE_SELECT_IR:
			printf("SELECT_IR");
			break;
		case TAPSTATE_CAPTURE_IR:
			printf("CAPTURE_IR");
			break;
		case TAPSTATE_SHIFT_IR:
			printf("SHIFT_IR");
			break;
		case TAPSTATE_EXIT1_IR:
			printf("EXIT1_IR");
			break;
		case TAPSTATE_PAUSE_IR:
			printf("PAUSE_IR");
			break;
		case TAPSTATE_EXIT2_IR:
			printf("EXIT2_IR");
			break;
		case TAPSTATE_UPDATE_IR:
			printf("UPDATE_IR");
			break;
		default:
			printf("ILLEGAL");
	}
	printf(")\n");
	return PARSE_SUCCESS;
}
ParseStatus gotXENDIR(uint8 tapState) {    // TODO: XENDIR accepts a TAPState
	printf("XENDIR(0x%02X)\n", tapState);
	return PARSE_SUCCESS;
}


// See XAPP503 page 23
//
ParseStatus gotXENDDR(uint8 tapState) {    // TODO: XENDDR accepts a TAPState
	printf("XENDDR(0x%02X)\n", tapState);
	return PARSE_SUCCESS;
}

int main(int argc, const char *argv[]) {
	FILE *file;
	uint8 buffer[128];
	size_t size;
	ParseStatus status;

	if ( argc != 2 ) {
		fprintf(stderr, "Synopsis: %s <file>\n", argv[0]);
		exit(1);
	}

	file = fopen(argv[1], "rb");
	if ( file == NULL ) {
		fputs("File error\n", stderr);
		exit(2);
	}

	parseInit();
	while ( !feof(file) ) {
		size = fread(buffer, 1, 128, file);
		status = parse(buffer, size);
		if ( status != PARSE_SUCCESS ) {
			fprintf(stderr, "Parse error %d\n", status);
			exit(3);
		}
	}

	// terminate
	fclose(file);
	return 0;
}


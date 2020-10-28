/*
   Copyright (c) 2020 Boot&Work Corp., S.L. All rights reserved

   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AES_CMAC.h"

static const uint8_t const_Rb[16] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};
static const uint8_t const_Zero[16] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////////////////////////////////////
AES_CMAC::AES_CMAC(AESTiny128& aes128) : aes128(aes128) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AES_CMAC::generateMAC(uint8_t* mac, const uint8_t* key, const uint8_t* data, size_t dataLen) {
	aes128.setKey(key, 16);

	for (int i = 0; i < 16; i++) {
		mac[i] = 0;
	}

	aes128.encryptBlock(mac, const_Zero);
	for (int i = 0; i < 16; ++i) {
		X[i] = mac[i];
	}

	shiftLeft(X, sizeof(X));
	if (mac[0] & 0x80) {
		xor128(X, X, const_Rb);
	}

	for (int i = 0; i < 16; ++i) {
		Y[i] = X[i];
	}
	shiftLeft(Y, sizeof(Y));
	if (X[0] & 0x80) {
		xor128(Y, Y, const_Rb);
	}

	int n = (dataLen + 15) / 16;
	int flag = 0;
	if (n == 0) {
		n = 1;
	} else if ((dataLen % 16) == 0) {
		flag = 1;
	}

	if (flag) {
		xor128(mac, &data[16 * (n - 1)], X);
	} else {
		padding(mac, &data[16 * (n - 1)], dataLen % 16);
		xor128(mac, mac, Y);
	}
	for (int i = 0; i < 16; i++) {
		X[i] = 0;
	}
	for (int i = 0; i < n - 1; i++) {
		xor128(Y, X, &data[16 * i]);
		aes128.encryptBlock(X, Y);
	}

	xor128(Y, mac, X);
	aes128.encryptBlock(mac, Y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AES_CMAC::shiftLeft(uint8_t* buff, uint8_t buffLen) {
  while (buffLen--) {
    uint8_t next = buffLen ? buff[1] : 0;

    uint8_t val = (*buff << 1);
    if (next & 0x80) {
      val |= 1;
    }
    *buff++ = val;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AES_CMAC::xor128(uint8_t* out, const uint8_t* a, const uint8_t* b) {
  for (int i = 0; i < 16; i++) {
    *out++ = (*a++) ^ (*b++);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AES_CMAC::padding(uint8_t* pad, const uint8_t* lastb, int len) {
  for (int i = 0; i < 16; i++) {
    if (i < len) {
      *pad++ = *lastb++;
    } else if (i == len) {
      *pad++ = 0x80;
    } else {
      *pad++ = 0x00;
    }
  }
}

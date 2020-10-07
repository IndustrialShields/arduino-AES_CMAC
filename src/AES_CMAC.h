#ifndef __AES_CMAC_H__
#define __AES_CMAC_H__

#include <Arduino.h>
#include <AES.h>

class AES_CMAC {
	public:
		explicit AES_CMAC(AESTiny128& aes128);

	public:
		void generateMAC(uint8_t* mac, const uint8_t* key, const uint8_t* data, size_t dataLen);

	private:
		void shiftLeft(uint8_t* buff, uint8_t buffLen);
		void xor128(uint8_t* out, const uint8_t* a, const uint8_t* b);
		void padding(uint8_t* pad, const uint8_t* lastb, int len);

	private:
		AESTiny128& aes128;
		uint8_t X[16];
		uint8_t Y[16];
};

#endif // __AES_CMAC_H__

# arduino-AES_CMAC
AES-CMAC library for Arduino

## Gettings started

### Prerequisites
1. The [Arduino IDE](http://www.arduino.cc) 1.8.0 or higher
2. The **AES_CMAC** module requires to import the **AES** module from [Arduino Crypto library](https://www.arduino.cc/reference/en/libraries/crypto/).

### Installing
1. Download the [library](https://github.com/IndustrialShields/arduino-AES_CMAC) from the GitHub as a "ZIP" file.
2. From the Arduino IDE, select the downloaded "ZIP" file in the menu "Sketch/Include library/Add .ZIP library". You have to install both libraries, **AES** and **AES_CMAC**.
3. Now you can open any example from the "File/Examples/AES_CMAC" menu.

### AES_CMAC
The **AES_CMAC** module is made to encrypt data using the Advanced Encryption Standard algorithm which is based in a symmetric key system. It creates a 16 byte size output. It does not have a decryption method so it works like a hash. 
```c++
#include <AES_CMAC.h>
#include <AES.h>
```
First of all, the symmetric key is stablished. It is a 16 uint8_t table. This key is just an example and it is very important to set up a new one when it is needed. 
```c++
const uint8_t key[16] = {
  0x2b, 0x7e, 0x15, 0x16,
  0x28, 0xae, 0xd2, 0xa6,
  0xab, 0xf7, 0x15, 0x88,
  0x09, 0xcf, 0x4f, 0x3c,
};
```

After that, raw data input is made. It is a uint8_t table of an arbitrary size.
```c++
const uint8_t data_raw[] = {
  0x6b, 0xc1, 0xbe, 0xe2,
  0x2e, 0x40, 0x9f, 0x96,
  0xe9, 0x3d, 0x7e, 0x11,
  0x73, 0x93, 0x17, 0x2a,
  0xae, 0x2d, 0x8a, 0x57,
  0x1e, 0x03, 0xac, 0x9c,
  0x9e, 0xb7, 0x6f, 0xac,
  0x45, 0xaf, 0x8e, 0x51,
  0x30, 0xc8, 0x1c, 0x46,
  0xa3, 0x5c, 0xe4, 0x11,
};
```

The output is going to be the raw_input encrypted following the AES protocol and it is named MAC. It always has a size of 16 bytes.
```c++
uint8_t mac[16];
```

An instance is made and a CMAC (Cipher-based Message Authentication Code) object is created. The instance name is `aes128` because of the 128-bit AES.
```c++
AESTiny128 aes128;
AES_CMAC cmac(aes128);
```

The `genrateMAC`function uses the `cmac` object to create the MAC (encrypted data output). The required parameters are the `mac`, the `key`, the `data_raw` and the `sizeof(data_raw)`, all needed to compute the output. 
```c++
cmac.generateMAC(mac, key, data_raw, sizeof(data_raw));
```

Finally, the MAC is printed in a right way, showing the 16 uint8_t. If you used the given example AES_CMAC.ino, the result that you have to see in the Serial Monitor has to be this one:
```plaintext
MAC: DF A6 67 47 DE 9A E6 30 30 CA 32 61 14 97 C8 27
```

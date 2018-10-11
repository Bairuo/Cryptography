## Cryptography Tool

[//]: # (Begin current test results)

A cryptographic demonstration tool for _Cryptography Theory and Practice_

Use just like this, more commands see the help document. 

```
> enc -i"plaintext-128.txt" -spn-d"SPN-128"
Hexa of plaintext: 61F6BA430FD7BC0B1E92E6794A423DA3
Binary of plaintext: 01100001111101101011101001000011000011111101011110111100000010110001111010010010111001100111100101001010010000100011110110100011
Binary of ciphertext: 11110111010101101001111011110000000010010100000001110110010011000110110101101111100111000000101110111111001101100110111100010000
Hexa of ciphertext: F7569EF00940764C6D6F9C0BBF366F10
```

### Build
|Win32|Ubuntu16|
|:--:|:--:|
|[![BuildStatus](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/ubuntu_16_debug/badge/icon)](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/ubuntu_16_debug/)|[![BuildStatus](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/mac_debug/badge/icon)](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/mac_debug/)|

For windows, build/Cryptography.workspace can build whole workspace

For _Cryptography_ and _RSAGenerator_ build on linux:


```
cd build/makefiles
make -f cryptography
make -f rsagenerator
```

or

```
make -f cryptography target=release
make -f rsagenerator target=release
```


### Support

* SPN-base
* SPN-base Linear Cryptanalysis
* SPN-base Differential Cryptanalysis
* SPN-strengthen
* SPN-CBC binary file encryption
* RSA Parameters Generate
* RSA Encryption And Decryption
* (Math)Square-and-Multiply
* (Math)Mont-Power-Mode

![Screenshot](https://github.com/Bairuo/Cryptography-dev/raw/master/meta/pic.png)

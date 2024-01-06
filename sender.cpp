#include "MicroBit.h"
#include <ostream>
#include <iostream>
#include "aes128/aes.c"
#include "sha256_c/sha256.c"
#include "md5_c/md5.c"
MicroBit uBit;
#define KEY_SIZE 32
#define AES_BLOCKLEN 16

void generateEncryptionKey(uint8_t* key) {
    // Set the input string for SHA-256
    const char* sha256InputStr = "UWECyber";

    // Perform SHA-256 hashing
    uint8_t sha256Hash[32];
    SHA256_CTX sha256Context;
    sha256_init(&sha256Context);
    sha256_update(&sha256Context, reinterpret_cast<const BYTE*>(sha256InputStr), strlen(sha256InputStr));
    sha256_final(&sha256Context, sha256Hash);

    // Divide the SHA-256 hash into two 128-bit halves
    uint8_t sha256HashHalf1[16];
    uint8_t sha256HashHalf2[16];
    memcpy(sha256HashHalf1, sha256Hash, 16);
    memcpy(sha256HashHalf2, sha256Hash + 16, 16);

    // Perform XOR operation on the two halves
    uint8_t sha256XorResult[16];
    for (int i = 0; i < 16; i++) {
        sha256XorResult[i] = sha256HashHalf1[i] ^ sha256HashHalf2[i];
    }

    // Generate a random number salt from 1 to 1000
    int salt = rand() % 1000 + 1;

    // Convert the random number to a string
    char md5InputStr[32];
    snprintf(md5InputStr, sizeof(md5InputStr), "%d", salt);

    // Perform MD5 hashing
    uint8_t md5Hash[16];
    md5String(md5InputStr, md5Hash);

    // Convert the SHA-256 XOR result and MD5 hash result to hexadecimal strings
    char sha256XorString[33];
    char md5HashString[33];
    for (int i = 0; i < 16; i++) {
        sprintf(&sha256XorString[i * 2], "%02x", sha256XorResult[i]);
        sprintf(&md5HashString[i * 2], "%02x", md5Hash[i]);
    }
    sha256XorString[32] = '\0';
    md5HashString[32] = '\0';

    // Concatenate the SHA-256 XOR result and MD5 hash result
    char concatenatedOutput[65];
    sprintf(concatenatedOutput, "%s%s", sha256XorString, md5HashString);
    
    // Convert the concatenated output to a hexadecimal string
    char keyString[2 * KEY_SIZE + 1];
    for (int i = 0; i < KEY_SIZE; i++) {
        sprintf(&keyString[i * 2], "%02x", concatenatedOutput[i]);
    }
    keyString[2 * KEY_SIZE] = '\0';  // Null-terminate the string

    // Copy the hexadecimal string to the key array
    for (int i = 0; i < KEY_SIZE; i++) {
        key[i] = static_cast<uint8_t>(strtoul(&keyString[i * 2], nullptr, 16));
    }
}


int main() {
    // Initialise the micro:bit runtime
    uBit.init();
    uBit.display.print("S");
    // Enable the radio
    uBit.radio.enable();

    uint8_t key[KEY_SIZE];
    generateEncryptionKey(key);

    while (1) {
            if (uBit.buttonA.isPressed() && uBit.buttonB.isPressed()) {
            uint8_t plaintext[AES_BLOCKLEN] = {'A', '+', 'B'};
            uint8_t ciphertext[AES_BLOCKLEN];
            struct AES_ctx ctx;
            AES_init_ctx(&ctx, key);
            AES_ECB_encrypt(&ctx, plaintext);
            memcpy(ciphertext, plaintext, AES_BLOCKLEN);
            PacketBuffer ciphertextBuffer(ciphertext, AES_BLOCKLEN);
            uBit.serial.printf("Button A+B pressed. Encrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");
            uBit.radio.datagram.send(ciphertextBuffer);

            // Decryption for debugging
            AES_ECB_decrypt(&ctx, ciphertext);
            uBit.serial.printf("Decrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");

        } else if (uBit.buttonA.isPressed()) {
            uint8_t plaintext[AES_BLOCKLEN] = {'A'};
            uint8_t ciphertext[AES_BLOCKLEN];
            struct AES_ctx ctx;
            AES_init_ctx(&ctx, key);
            AES_ECB_encrypt(&ctx, plaintext);
            memcpy(ciphertext, plaintext, AES_BLOCKLEN);
            PacketBuffer ciphertextBuffer(ciphertext, AES_BLOCKLEN);
    		uBit.serial.printf("Button A pressed. Encrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");
    		uBit.radio.datagram.send(ciphertextBuffer);
    		
    		// Decryption for debugging
            AES_ECB_decrypt(&ctx, ciphertext);
            uBit.serial.printf("Decrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");
    		
        } else if (uBit.buttonB.isPressed()) {
            uint8_t plaintext[AES_BLOCKLEN] = {'B'};
            uint8_t ciphertext[AES_BLOCKLEN];
            struct AES_ctx ctx;
            AES_init_ctx(&ctx, key);
            AES_ECB_encrypt(&ctx, plaintext);
            memcpy(ciphertext, plaintext, AES_BLOCKLEN);
            PacketBuffer ciphertextBuffer(ciphertext, AES_BLOCKLEN);
    		uBit.serial.printf("Button B pressed. Encrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");
    		uBit.radio.datagram.send(ciphertextBuffer);
    		
    		// Decryption for debugging
            AES_ECB_decrypt(&ctx, ciphertext);
            uBit.serial.printf("Decrypted value: ");
            for (int i = 0; i < AES_BLOCKLEN; i++) {
                uBit.serial.printf("%c", ciphertext[i]);
            }
            uBit.serial.printf("\n");
        }
        uBit.sleep(500);
    }

    return 0;
}

#include "MicroBit.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <thread>
#include "sha256_c/sha256.c"
#include "md5_c/md5.c"
#include "aes128/aes.c"

#define KEY_SIZE 32 // Define the size of the encryption key
#define AES_BLOCKLEN 16 // Define the block length for AES encryption



MicroBit uBit; // Initialize a MicroBit instance


bool running = false;  // Flag to track if the timer is running
int timeElapsed = 0;  // Variable to store the elapsed time in seconds

void generateEncryptionKey(uint8_t* key) {
    // Set the input string for SHA-256
    const char* sha256InputStr = "UWECyber";

    // Perform SHA-256 hashing
    uint8_t sha256Hash[32];
    SHA256_CTX sha256Context;
    sha256_init(&sha256Context); // Initialize SHA-256 context
    sha256_update(&sha256Context, reinterpret_cast<const BYTE*>(sha256InputStr), strlen(sha256InputStr));// Update context with input string
    sha256_final(&sha256Context, sha256Hash); // Finalize the hash computation

    // Divide the SHA-256 hash into two 128-bit halves
    uint8_t sha256HashHalf1[16];
    uint8_t sha256HashHalf2[16];
    memcpy(sha256HashHalf1, sha256Hash, 16); // Copy first half of the hash
    memcpy(sha256HashHalf2, sha256Hash + 16, 16); // Copy second half of the hash

    // Perform XOR operation on the two halves
    uint8_t sha256XorResult[16];
    for (int i = 0; i < 16; i++) {
        sha256XorResult[i] = sha256HashHalf1[i] ^ sha256HashHalf2[i]; // XOR each byte
    }

    
    // Generate a random number salt from 1 to 1000
    int salt = rand() % 1000 + 1;

    // Convert the random number to a string
    char md5InputStr[32];
    snprintf(md5InputStr, sizeof(md5InputStr), "%d", salt); // Convert integer salt to string

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
    

void onDataV2(MicroBitEvent)
{

    uint8_t buffer[32];
    uBit.radio.datagram.recv(buffer, sizeof(buffer));

    uint8_t key[KEY_SIZE];
    generateEncryptionKey(key); // Generate the encryption key

   // Decrypt the encrypted message
   uint8_t ciphertext[AES_BLOCKLEN];
   memcpy(ciphertext, buffer, AES_BLOCKLEN);

   struct AES_ctx ctx;
   AES_init_ctx(&ctx, key);  // Initialize the AES context with the generated key
   AES_ECB_decrypt(&ctx, ciphertext);
    
    // Print the decrypted value
    uBit.serial.printf("Decrypted value: ");
    for (int i = 0; i < AES_BLOCKLEN; i++) {
        uBit.serial.printf("%c", ciphertext[i]);
    }
    uBit.serial.printf("\r\n");

    // Compare the decrypted message to the if conditions below
    // Convert the decrypted message to a string for comparison
    char decryptedMessage[AES_BLOCKLEN + 1];
    sprintf(decryptedMessage, "%.*s", AES_BLOCKLEN, ciphertext);
    decryptedMessage[AES_BLOCKLEN] = '\0'; // Null-terminate the string

  // Create the arrow image (only needs to be done once)
  MicroBitImage arrow(5, 5);
  arrow.clear();
  arrow.setPixelValue(0, 0, 255);  // Top-left pixel
  arrow.setPixelValue(4, 4, 255);  // Bottom-right pixel

  // Compare the strings
  if (strcmp(decryptedMessage, "A") == 0) {
        int compassHeading = uBit.compass.heading();

        // Display the arrow image
        uBit.display.clear();
        uBit.display.print(arrow);
        
        // Add an indicator for the current heading (using a separate image)
        MicroBitImage indicator(5, 5);  // Create a temporary image for the indicator
        indicator.clear();

        if (compassHeading >= 315 || compassHeading < 45) {
            indicator.setPixelValue(2, 2, 255);  // Center pixel for north
        } else if (compassHeading >= 45 && compassHeading < 135) {
            indicator.setPixelValue(4, 2, 255);  // Right pixel for east
        } else if (compassHeading >= 135 && compassHeading < 225) {
            indicator.setPixelValue(4, 0, 255);  // Bottom pixel for south
        } else {
            indicator.setPixelValue(2, 0, 255);  // Left pixel for west
        }

        uBit.display.print(indicator);  // Display the indicator image

        uBit.sleep(250);  // Delay for 0.25 seconds
  }

  if (strcmp(decryptedMessage, "B") == 0) {
        int temperature = uBit.thermometer.getTemperature();
        uBit.display.scroll(temperature);  // Display the temperature on the LED screen
        uBit.sleep(1000);  // Delay for 1 second
  }


  // new functionality for A+B
    MicroBitImage img("1 0 0 0 0 0 0 0 0 1\n"
                      "0 1 0 0 0 0 0 0 1 0\n"
                      "0 0 1 0 0 0 0 1 0 0\n"
                      "0 0 0 1 0 0 1 0 0 0\n"
                      "0 0 0 0 1 1 0 0 0 0\n");

  if (strcmp(decryptedMessage, "A+B") == 0) {

    // commented code 
    // // Execute the code block only when the "A and B" command is received
    // running = !running;  // Toggle the running state
    // if (!running) {
    //     timeElapsed = 0;  // Reset the elapsed time when stopping
    // }
    // uBit.display.clear();  // Clear the display when starting/stopping
    // uBit.sleep(100);  // Delay for debouncing
    // Display the triangular waveform image when "A+B" command is received
        uBit.display.scroll(img, 200, -1);

        // Optional: additional actions based on the "A+B" command
        // ...
        // Indicate that the image has been displayed in the serial port
        uBit.serial.printf("Triangular waveform displayed\r\n");

        // Delay to prevent immediate repetition
        uBit.sleep(9000);    
  }

  // Output the received string to the serial port.
  uBit.serial.printf("%s\r\n",buffer);
}

int main() {

    // Initialize the micro:bit runtime
    uBit.init();
    uBit.display.print("R");
    uBit.serial.printf("Radio Commands\r\n");
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onDataV2);
    uBit.radio.enable();


    while (!running) {
        // Wait until the "A and B" condition is met
        uBit.sleep(100);  // Adjust the delay as needed
    }

    while (running) {
        timeElapsed++;
        uBit.display.scroll(timeElapsed);  // Display the elapsed time
        uBit.sleep(1000);  // Delay for 1 second (adjust for desired timing)
    }

    uBit.sleep(10);  // Short delay for button responsiveness
}

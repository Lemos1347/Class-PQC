#include "oqs/oqs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Block 1: Function to load data from a binary file
// Takes the filename and buffer for storing data as parameters.
// Opens the file in binary mode and reads its contents into the provided buffer.
// If the file cannot be opened, displays an error message and terminates the program.
void load_from_file(const char *filename, uint8_t *buffer, size_t length) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  fread(buffer, 1, length, file);
  fclose(file);
}

// Block 2: Function to print data in hexadecimal format
// Takes a label, data, and data length as parameters.
// Prints the data byte by byte in hexadecimal with a descriptive label.
void print_hex(const char *label, const uint8_t *data, size_t length) {
  printf("%s: ", label);
  for (size_t i = 0; i < length; i++) {
    printf("%02X", data[i]);
    if ((i + 1) % 16 == 0) {
      printf("\n         ");
    }
  }
  printf("\n");
}

int main() {
  printf("=== Alice: Key Decapsulation ===\n");

  const char *kem_alg = OQS_KEM_alg_ml_kem_1024;
  printf("[INFO] Using algorithm: %s\n", kem_alg);

  // Block 3: Initialize the KEM algorithm
  // Creates and returns an OQS_KEM structure for the specified algorithm.
  // If initialization fails, the program terminates with an error.
  OQS_KEM *kem = OQS_KEM_new(kem_alg);
  if (kem == NULL) {
    fprintf(stderr, "Error initializing KEM.\n");
    return EXIT_FAILURE;
  }

  // Block 4: Allocate memory for the secret key, ciphertext, and shared secret
  // Allocates enough memory to store the secret key, ciphertext, and shared secret.
  uint8_t *secret_key = (uint8_t *)malloc(kem->length_secret_key);
  uint8_t *ciphertext = (uint8_t *)malloc(kem->length_ciphertext);
  uint8_t *shared_secret = (uint8_t *)malloc(kem->length_shared_secret);

  if (secret_key == NULL || ciphertext == NULL || shared_secret == NULL) {
    fprintf(stderr, "Error allocating memory.\n");
    OQS_KEM_free(kem);
    free(secret_key);
    free(ciphertext);
    free(shared_secret);
    return EXIT_FAILURE;
  }

  // Block 5: Load Alice's secret key and Bob's ciphertext from files
  // Reads the secret key from `secret_key_1024.bin` and the ciphertext from `ciphertext.bin`.
  load_from_file("secret_key_1024.bin", secret_key, kem->length_secret_key);
  load_from_file("ciphertext.bin", ciphertext, kem->length_ciphertext);
  printf("[INFO] Loaded Alice's secret key and Bob's ciphertext.\n");

  // Block 6: Decapsulate the shared secret
  // Recovers the shared secret using the secret key and ciphertext.
  // If decapsulation fails, displays an error message and terminates the program.
  if (OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key) != OQS_SUCCESS) {
    fprintf(stderr, "Error during decapsulation.\n");
    free(secret_key);
    free(ciphertext);
    free(shared_secret);
    OQS_KEM_free(kem);
    return EXIT_FAILURE;
  }

  // Block 7: Display the shared secret
  // Prints the recovered shared secret in hexadecimal format.
  printf("[INFO] Key decapsulation successful:\n");
  print_hex("Recovered shared secret", shared_secret, kem->length_shared_secret);

  // Block 8: Release resources
  // Frees the allocated memory for the secret key, ciphertext, and shared secret.
  // Releases the OQS_KEM structure and exits the program successfully.
  free(secret_key);
  free(ciphertext);
  free(shared_secret);
  OQS_KEM_free(kem);

  return EXIT_SUCCESS;
}

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

// Block 2: Function to write data to a binary file
// Takes the filename, data, and data length as parameters.
// Opens the file in binary mode and writes the provided data to the file.
// If the file cannot be opened, displays an error message and terminates the program.
void write_to_file(const char *filename, const uint8_t *data, size_t length) {
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  fwrite(data, 1, length, file);
  fclose(file);
}

// Block 3: Function to print data in hexadecimal format
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
  printf("=== Bob: Key Encapsulation ===\n");

  const char *kem_alg = OQS_KEM_alg_ml_kem_1024;
  printf("[INFO] Using algorithm: %s\n", kem_alg);

  // Block 4: Initialize the KEM algorithm
  // Creates and returns an OQS_KEM structure for the specified algorithm.
  // If initialization fails, the program terminates with an error.
  OQS_KEM *kem = OQS_KEM_new(kem_alg);
  if (kem == NULL) {
    fprintf(stderr, "Error initializing KEM.\n");
    return EXIT_FAILURE;
  }

  // Block 5: Allocate memory for the public key, ciphertext, and shared secret
  // Allocates enough memory to store the public key, ciphertext, and shared secret.
  uint8_t *public_key = (uint8_t *)malloc(kem->length_public_key);
  uint8_t *ciphertext = (uint8_t *)malloc(kem->length_ciphertext);
  uint8_t *shared_secret = (uint8_t *)malloc(kem->length_shared_secret);

  if (public_key == NULL || ciphertext == NULL || shared_secret == NULL) {
    fprintf(stderr, "Error allocating memory.\n");
    OQS_KEM_free(kem);
    free(public_key);
    free(ciphertext);
    free(shared_secret);
    return EXIT_FAILURE;
  }

  // Block 6: Load Alice's public key from file
  // Reads the public key from the file `public_key_1024.bin` into the buffer.
  load_from_file("public_key_1024.bin", public_key, kem->length_public_key);
  printf("[INFO] Loaded Alice's public key.\n");

  // Block 7: Encapsulate a shared secret
  // Generates a shared secret and encapsulates it into a ciphertext using the public key.
  // If encapsulation fails, displays an error message and terminates the program.
  if (OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key) != OQS_SUCCESS) {
    fprintf(stderr, "Error during encapsulation.\n");
    free(public_key);
    free(ciphertext);
    free(shared_secret);
    OQS_KEM_free(kem);
    return EXIT_FAILURE;
  }

  // Block 8: Save the ciphertext to a file
  // Writes the ciphertext to the file `ciphertext.bin` for Alice to decapsulate later.
  write_to_file("ciphertext.bin", ciphertext, kem->length_ciphertext);
  printf("[INFO] Encapsulated ciphertext saved to 'ciphertext.bin'.\n");

  // Block 9: Display the shared secret and ciphertext
  // Prints the shared secret (plaintext) and the encapsulated ciphertext in hexadecimal format.
  printf("[INFO] Key encapsulation successful:\n");
  print_hex("Plaintext shared secret", shared_secret, kem->length_shared_secret);
  print_hex("Encapsulated ciphertext", ciphertext, kem->length_ciphertext);

  // Block 10: Release resources
  // Frees the allocated memory for the public key, ciphertext, and shared secret.
  // Releases the OQS_KEM structure and exits the program successfully.
  free(public_key);
  free(ciphertext);
  free(shared_secret);
  OQS_KEM_free(kem);

  return EXIT_SUCCESS;
}


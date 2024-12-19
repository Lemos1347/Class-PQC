#include "oqs/oqs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Block 1: Function to write data to a binary file
// Takes the filename, data, and data length as parameters.
// Opens the file for binary writing and writes the provided data to the file.
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

// Block 2: Function to print data in hexadecimal format
// Takes a label, data, and data length as parameters.
// Prints the data byte by byte in hexadecimal with a descriptive label.
// Formats the output for readability, breaking lines every 16 bytes.
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
  printf("=== Alice: Key Generation ===\n");

  const char *kem_alg = OQS_KEM_alg_ml_kem_512;
  printf("[INFO] Using algorithm: %s\n", kem_alg);

  // Block 3: Initialize the Key Encapsulation Mechanism (KEM) algorithm
  // Creates and returns an OQS_KEM structure for the specified algorithm.
  // If initialization fails, the program terminates with an error.
  OQS_KEM *kem = OQS_KEM_new(kem_alg);
  if (kem == NULL) {
    fprintf(stderr, "Error initializing KEM.\n");
    return EXIT_FAILURE;
  }

  // Block 4: Allocate memory for the public and secret keys
  // Allocates enough memory to store the keys based on the sizes defined by the KEM algorithm.
  uint8_t *public_key = (uint8_t *)malloc(kem->length_public_key);
  uint8_t *secret_key = (uint8_t *)malloc(kem->length_secret_key);

  // Block 5: Generate the key pair
  // Calls the OQS_KEM_keypair function to generate the public and secret keys.
  // If key generation fails, displays an error message and terminates the program.
  if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
    fprintf(stderr, "Error generating key pair.\n");
    return EXIT_FAILURE;
  }

  // Block 6: Save the keys to binary files
  // Writes the public and secret keys to separate files.
  write_to_file("public_key.bin", public_key, kem->length_public_key);
  write_to_file("secret_key.bin", secret_key, kem->length_secret_key);

  // Block 7: Display information about the generated keys
  // Prints the public and secret keys in hexadecimal format to the terminal.
  // Also informs the user about the names of the generated files.
  printf("[INFO] Generated keys:\n");
  print_hex("Public Key", public_key, kem->length_public_key);
  print_hex("Secret Key", secret_key, kem->length_secret_key);

  printf("[INFO] Generated files:\n");
  printf("  - public_key.bin\n");
  printf("  - secret_key.bin\n");

  // Block 8: Release resources
  // Frees the allocated memory for the keys and the KEM structure.
  // Exits the program successfully.
  free(public_key);
  free(secret_key);
  OQS_KEM_free(kem);

  return EXIT_SUCCESS;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void xorEncrypt(char* message, size_t msgLen, char* key) {
    int keyLen = strlen(key);
	//int msgLen = strlen(message); dont need bc passing in

    for (int i = 0; i<msgLen; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, const char *message, const char *key) {
    FILE *file = fopen(filename, "wb");
    size_t msgLen = strlen(message);

    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    xorEncrypt((char*)message, msgLen, (char*)key);
    fwrite(message, 1, msgLen, file);

    fclose(file);
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

	size_t size = (size_t)fileSize; //unsigned interger version of the filesize
    char *buffer = (char *)malloc((fileSize + 1) * 1);
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    size_t read =fread(buffer, 1, size, file); //save the number of bytes read
    xorEncrypt(buffer, read, (char *)key);

	fwrite(buffer, 1, size, stdout);
    printf("\n");

    free(buffer);
    fclose(file);
}

int main() {
    char choice;
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    scanf(" %c", &choice);

    if (choice == 'e') {
        char message[1024];
        char filename[100];
        char key[50];

        printf("Enter the string to encrypt: ");
        scanf("%s", message);
        // fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        printf("Enter the filename to save the encrypted data: ");
        scanf("%s", filename);

        printf("Enter the encryption key: ");
        scanf("%s", key);

        encryptFile(filename, message, key);
    } else if (choice == 'd') {
        char filename[100];
        char key[50];

        printf("Enter the filename to decrypt: ");
        scanf("%s", filename);

        printf("Enter the decryption key: ");
        scanf("%s", key);

        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
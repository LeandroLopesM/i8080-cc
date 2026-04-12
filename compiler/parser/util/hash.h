#ifndef HASH_H
#define HASH_H


// BSD File checksum modified for strings
// static int hash(char* ce)
// {
//     char* c = ce;
//     int checksum = 0;

//     for (int ch = *c; *c; ch = *c++) {
//         checksum = (checksum >> 1) + ((checksum & 1) << 15);
//         checksum += ch;
//         checksum &= 0xffff;
//     }
//     return checksum;
// }
#include <stdint.h>
#include <string.h>

// Jenkins hash
static uint32_t hash(const char* key) {
  size_t i = 0;
  size_t length = strlen(key);
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

#endif // HASH_H
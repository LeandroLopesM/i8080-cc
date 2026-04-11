#ifndef HASH_H
#define HASH_H


// BSD File checksum modified for strings
static int hash(char* ce)
{
    char* c = ce;
    int checksum = 0;

    for (int ch = *c; *c; ch = *c++) {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += ch;
        checksum &= 0xffff;
    }
    return checksum;
}

#endif // HASH_H
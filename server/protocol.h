#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct {
    uint8_t type;
    uint16_t length;
} TL;

/* 2.2 */
typedef struct {
    TL tl;// type=1, length=5
    uint8_t version; // 1
    uint8_t magic; // 116
} TLV_CONNECT;

/* 2.3 */
typedef TL TLV_ASK_FILES;

/* 2.4 */
typedef struct {
    TL tl; // type=3, length=12
    uint8_t reserved;
    uint8_t entries[8];
} TLV_ENTRIES;

/* #2.5 */
typedef struct {
    TL tl; // type=4, length=?
    uint8_t reserved;
    uint8_t mod_time [8];
    uint8_t size [8];
    char * filename;
} TLV_ENTRY;

/* #2.6 */
typedef struct {
    TL tl; // type=5, length=?
    char * filename;
} TLV_ASK_FILE;

/* #2.7 */
typedef struct {
    TL tl; // type=6, length=?
    uint8_t reserved;
    uint8_t mod_time[8];
    uint8_t size[8];
    uint16_t mode;
    char * filename;
} TLV_META_FILE;

/* #2.8 */
typedef struct {
    TL tl; // type=7, length=?
    char * filename;
} TLV_DELETE_FILE;

#endif

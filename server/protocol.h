#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef struct {
    uint8_t type;
    uint16_t length;
} TL;

/* 2.2 */
typedef struct {
    uint8_t version; // 1
    uint8_t magic; // 116
} TLV_CONNECT;

/* 2.3 */
typedef TL TLV_ASK_FILES;

/* 2.4 */
typedef struct {
    uint8_t reserved;
    uint8_t entries[8];
} TLV_ENTRIES;

/* #2.5 */
typedef struct {
    uint8_t reserved;
    uint8_t mod_time [8];
    uint8_t size [8];
    char * filename;
} TLV_ENTRY;

/* #2.6 */
typedef struct {
    char * filename;
} TLV_ASK_FILE;

/* #2.7 */
typedef struct {
    uint8_t reserved;
    uint8_t mod_time[8];
    uint8_t size[8];
    uint16_t mode;
    char * filename;
} TLV_META_FILE;

/* #2.8 */
typedef struct {
    char * filename;
} TLV_DELETE_FILE;

union VALUE {
    TLV_CONNECT tlv_connect;
    TLV_ASK_FILES tlv_ask_files;
    TLV_ENTRIES tlv_entries;
    TLV_ENTRY tlv_entry;
    TLV_ASK_FILE tlv_ask;
    TLV_META_FILE tlv_meta_file;
    TLV_DELETE_FILE tlv_delete_file;
};

typedef struct {
    TL tl;
    union VALUE value;
} TLV;

#endif

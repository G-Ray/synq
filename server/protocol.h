#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_VERSION 1;

#define TLV_CONNECT_TYPE 1;
#define TLV_ASK_FILES_TYPE 2;
#define TLV_ENTRIES_TYPE 3;
#define TLV_ENTRY_TYPE 4;
#define TLV_ASK_FILE_TYPE 5;
#define TLV_META_TYPE 6;
#define TLV_DELETE_TYPE 7;
#define TLV_ERROR 8;

#define TLV_CONNECT_LENGTH 5;
#define TLV_ASK_FILES_LENGTH 3;
#define TLV_ENTRIES_LENGTH 12;

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
//typedef TL TLV_ASK_FILES;

/* 2.4 */
typedef struct {
    uint8_t reserved;
    uint64_t entries;
} TLV_ENTRIES;

/* #2.5 */
typedef struct {
    uint8_t reserved;
    uint64_t mod_time;
    uint64_t size;
    char * filename;
} TLV_ENTRY;

/* #2.6 */
typedef struct {
    char * filename;
} TLV_ASK_FILE;

/* #2.7 */
typedef struct {
    uint8_t reserved;
    uint64_t mod_time;
    uint64_t size;
    uint16_t mode;
    char * filename;
} TLV_META_FILE;

/* #2.8 */
typedef struct {
    char * filename;
} TLV_DELETE_FILE;

/* #2.9 */
typedef struct {
    uint8_t errno;
    char * filename;
} TLV_ERROR;

union VALUE{
    TLV_CONNECT tlv_connect;
    TLV_ENTRIES tlv_entries;
    TLV_ENTRY tlv_entry;
    TLV_ASK_FILE tlv_ask_file;
    TLV_META_FILE tlv_meta_file;
    TLV_DELETE_FILE tlv_delete_file;
    TLV_ERROR tlv_error;
};

typedef struct {
    TL tl;
    union VALUE value;
} TLV;

#endif

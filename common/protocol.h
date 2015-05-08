#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <string.h>
#include <linux/limits.h>

#pragma pack(1)

#define PROTOCOL_VERSION 1;
#define TLV_CONNECT_LENGTH 5;
#define TLV_ASK_FILES_LENGTH 3;
#define TLV_ENTRIES_LENGTH 12;

static const int TLV_CONNECT_TYPE = 1;
static const int TLV_ASK_FILES_TYPE = 2;
static const int TLV_ENTRIES_TYPE = 3;
static const int TLV_ENTRY_TYPE = 4;
static const int TLV_ASK_FILE_TYPE = 5;
static const int TLV_META_TYPE = 6;
static const int TLV_DELETE_TYPE = 7;
static const int TLV_ERROR_TYPE = 8;

typedef struct {
    uint8_t type;
    uint16_t length;
} TL;

/* 2.2 */
typedef struct {
    uint8_t version;
    uint8_t magic;
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
    uint64_t mtime;
    uint64_t size;
    char filename[PATH_MAX];
} TLV_ENTRY;

/* #2.6 */
typedef struct {
    char filename[PATH_MAX];
} TLV_ASK_FILE;

/* #2.7 */
typedef struct {
    uint8_t reserved;
    uint64_t mtime;
    uint64_t size;
    uint16_t mode;
    char filename[PATH_MAX];
} TLV_META_FILE;

/* #2.8 */
typedef struct {
    char filename[PATH_MAX];
} TLV_DELETE_FILE;

/* #2.9 */
typedef struct {
    uint8_t errno;
    char filename[PATH_MAX];
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

/* Prototypes */
int
init_tlv_connect(TLV * tlv);

int
init_tlv_ask_files(TLV * tlv);

int
init_tlv_entries(TLV * tlv, uint64_t entries);

int
init_tlv_entry(TLV * tlv, uint64_t mtime, uint64_t size, char filename[PATH_MAX]);

int
init_tlv_ask_file(TLV * tlv, char filename[PATH_MAX]);

int
init_tlv_meta_file(TLV * tlv, uint64_t mtime, uint64_t size, uint16_t mode,
                            char filename[PATH_MAX]);

int
init_tlv_delete(TLV *tlv, char filename[PATH_MAX]);

int
init_tlv_error(TLV * tlv, uint8_t errno, char filename[PATH_MAX]);

#endif

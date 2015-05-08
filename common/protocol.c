#include "protocol.h"

/*
TLV_CONNECT tlv_connect;
TLV_ENTRIES tlv_entries;
TLV_ENTRY tlv_entry;
TLV_ASK_FILE tlv_ask_file;
TLV_META_FILE tlv_meta_file;
TLV_DELETE_FILE tlv_delete_file;
TLV_ERROR tlv_error;
*/

int
init_tlv_connect(TLV * tlv) {
    tlv->tl.type = TLV_CONNECT_TYPE;
    tlv->tl.length = TLV_CONNECT_LENGTH;
    tlv->value.tlv_connect.version = PROTOCOL_VERSION;
    tlv->value.tlv_connect.magic = 116;

    return 0;
}

int
init_tlv_ask_files(TLV * tlv) {
    tlv->tl.type = TLV_ASK_FILES_TYPE;
    tlv->tl.length = TLV_ASK_FILES_LENGTH;

    return 0;
}

int
init_tlv_entries(TLV * tlv, uint64_t entries) {
    tlv->tl.type = TLV_ENTRIES_TYPE;
    tlv->tl.length = TLV_ENTRIES_LENGTH;
    tlv->value.tlv_entries.entries = entries;

    return 0;
}

int
init_tlv_entry(TLV * tlv, uint64_t mtime, uint64_t size, char filename[PATH_MAX]) {
    tlv->tl.type = TLV_ENTRY_TYPE;
    tlv->tl.length = 3 + 1 + 16 + strlen(filename);
    tlv->value.tlv_entry.size = 60u;
    tlv->value.tlv_entry.mtime = mtime;
    strncpy(tlv->value.tlv_entry.filename, filename, PATH_MAX);

    return 0;
}

int
init_tlv_ask_file(TLV * tlv, char filename[PATH_MAX]) {
    tlv->tl.type = TLV_ASK_FILE_TYPE;
    tlv->tl.length = 3 + strlen(filename);
    strncpy(tlv->value.tlv_entry.filename, filename, PATH_MAX);

    return 0;
}

int
init_tlv_meta_file(TLV * tlv, uint64_t mtime, uint64_t size, uint16_t mode,
                            char filename[PATH_MAX]) {
    tlv->tl.type = TLV_META_TYPE;
    tlv->tl.length = 3 + 1 + 16 + 2 + 2 + strlen(filename);
    tlv->value.tlv_meta_file.mtime = mtime;
    tlv->value.tlv_meta_file.size = size;
    tlv->value.tlv_meta_file.mode = mode;
    strncpy(tlv->value.tlv_entry.filename, filename, PATH_MAX);

    return 0;
}

int
init_tlv_delete(TLV *tlv, char filename[PATH_MAX]) {
    tlv->tl.type = TLV_DELETE_TYPE;
    tlv->tl.length = 3 + strlen(filename);
    strncpy(tlv->value.tlv_entry.filename, filename, PATH_MAX);

    return 0;
}

int
init_tlv_error(TLV * tlv, uint8_t errno, char filename[PATH_MAX]) {
    tlv->tl.type = TLV_ERROR_TYPE;
    tlv->tl.length = 3 + 1 + strlen(filename);
    tlv->value.tlv_error.errno = errno;
    strncpy(tlv->value.tlv_entry.filename, filename, PATH_MAX);

    return 0;
}

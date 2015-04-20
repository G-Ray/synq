#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>

static inline char *
put_8(char *buf, uint8_t c)
{
    *(uint8_t*)buf = c;
    return buf + 1;
}

static inline char *
put_16(char *buf, uint16_t s)
{
    *((uint16_t*)buf) = htons(s);
    return buf + 2;
}

static inline char *
put_32(char *buf, uint32_t i)
{
    *((uint32_t*)buf) = htonl(i);
    return buf + 4;
}

static inline char *
put_64(char *buf, uint64_t l)
{
    buf = put_32(buf, (l >> 32) & 0xFFFFFFFF);
    buf = put_32(buf, l & 0xFFFFFFFF);
    return buf;
}

static inline char *
put_str(char *buf, const char *src)
{
    strcpy(buf, src);
    return buf + strlen(src) + 1;
}

static inline uint8_t
get_8(const char *buf)
{
    return *(uint8_t*)buf;
}

static inline uint16_t
get_16(const char *buf)
{
    return ntohs(*((uint16_t*)buf));
}

static inline uint32_t
get_32(const char *buf)
{
    return ntohl(*((uint32_t*)buf));
}

static inline uint64_t
get_64(const char *buf)
{
    uint64_t fst= (((uint64_t)ntohl(*((uint32_t*)buf))) << 32);
    uint64_t snd= (uint64_t)ntohl(*((uint32_t*)(buf + 4)));
    return fst | snd;
}

static inline char *
get_str(const char *buf, int len)
{
    int tmp = (int)strnlen(buf, len);
    if (tmp >= len)
        return NULL;
    else
        return strdup(buf);
}

static inline mode_t
mode_from_16(uint16_t mode)
{
    return mode;
}

static inline uint16_t
mode_to_16(mode_t mode)
{
    return mode;
}

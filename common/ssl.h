#ifndef SSL_H
#define SSL_H

#include "openssl/ssl.h"
#include "openssl/err.h"

SSL_CTX* InitServerCTX();
SSL_CTX* InitCTX();
void ShowCerts(SSL* ssl);

#endif

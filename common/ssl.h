#ifndef SSL_H
#define SSL_H

#include "openssl/ssl.h"
#include "openssl/err.h"

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile);
SSL_CTX* InitServerCTX();
SSL_CTX* InitCTX();
void ShowCerts(SSL* ssl);

#endif

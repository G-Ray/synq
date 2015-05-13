# synq

Synqronize your files locally or with a server.

Installation
-------
You need OpenSSL installed on your system.
```
git clone git@github.com:G-Ray/synq.git

cd client
make

cd ../server
make
```
Generate a self-signed certificate on the server with:
```
openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout mycert.pem -out mycert.pem
```

Usage
-------
```
./synq-client [options] <dir1> <dir2>
./synq-client [options] <dir1> <ip> <port>
```

```
./synq-server <dir1> <port>
```

options can be "-d": dry-run, list changes but do not sync

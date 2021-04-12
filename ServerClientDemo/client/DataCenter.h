#ifndef DATACENTER_H
#define DATACENTER_H
#include "contain.h"
#include "CoderDecipher.h"

class AESCoderDecipher;
struct TCPParcel;

class DataCenter
{
public:
	DataCenter(){};
	~DataCenter(){};

	void MessageAnalyse(TCPParcel *, char **, char *);
	void MessageAnalyse(char *, char **, char *);
	void Decipher(char *, char **, AESCoderDecipher *);
	void Decipher(char *, TCPParcel *, AESCoderDecipher *);
	void Coder(char *, char **, AESCoderDecipher *);
	void Coder(TCPParcel *, TCPParcel *, AESCoderDecipher *);
	void Decipher(TCPParcel *, TCPParcel *, AESCoderDecipher *);
};

#endif
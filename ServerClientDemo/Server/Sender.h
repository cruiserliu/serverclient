#ifndef SENDER_H
#define SENDER_H
#include "contain.h"
#include "Errorlog.h"
#include "CoderDecipher.h"
/*****************************************************************SenderModule***********************************************************************************/
class DataCenter
{
public:
	DataCenter()
	{
		m_strTmp = "";
		for (int i = 0; i < MAXLINEREC; i++)
		{
			m_chTmp[i] = (char *)malloc(MAXLINE);
		}
	};
	~DataCenter()
	{
		for (int i = 0; i < MAXLINEREC; i++)
		{
			delete[] m_chTmp[i];
		}
	};

	void Decipher(TCPParcel *, TCPParcel *, AESCoderDecipher *);
	void Decipher(char *, char **, AESCoderDecipher *);
	void Decipher(char *, char (*)[MAXLINE], AESCoderDecipher *);
	void Decipher(char *, TCPParcel *, AESCoderDecipher *);
	void Coder(char *, char **, AESCoderDecipher *);
	void Coder(char *, char (*)[MAXLINE], AESCoderDecipher *);
	void Coder(char *, TCPParcel *, AESCoderDecipher *);
	void Coder(TCPParcel *, TCPParcel *, AESCoderDecipher *);

	void SendExcute(int, ServerType, char *, int, struct sockaddr *, socklen_t);
	void SendExcute(int, ServerType, TCPParcel, int, struct sockaddr *, int);
	void PopenExcute(const char *, char **);
	void PopenExcute(const char *, char (*)[MAXLINEREC]);
	void MergeMessage(string *s1, char **);
	void MergeMessage(string s1, char **);
	void MergeMessage(string s1, char (*)[MAXLINEREC]);
	void MergeMessage(string *s1, char (*)[MAXLINEREC]);

private:
	int nSend;
	char *m_chTmp[MAXLINEREC];
	string m_strTmp;
};

#endif

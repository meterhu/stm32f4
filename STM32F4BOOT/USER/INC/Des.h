//////////////////////////////////////////////////////////////////////////
/*
    Provided by ������, Northeastern University (www.neu.edu.cn)
    Email: blackdrn@sohu.com
	This product is free for use.
*/
//////////////////////////////////////////////////////////////////////////

enum    {ENCRYPT,DECRYPT};
//enum    bool{false,true}; // if bool is not supported,use this or just replace with char
                            // and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type��ENCRYPT:����,DECRYPT:����
// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

_BOOL Des_Go(_BYTE *Out,_BYTE *In,long datalen,const _BYTE *Key,int keylen, _BOOL Type);


extern _BYTE EncryptKey[15];

#define  Encrypt(out, in)  Des_Go( (_BYTE *)out,  (_BYTE *)in, sizeof(out), EncryptKey, sizeof(EncryptKey), ENCRYPT)

#define  Decrypt(out, in)  Des_Go( (_BYTE *)out,  (_BYTE *)in, sizeof(out), EncryptKey, sizeof(EncryptKey), DECRYPT)




//////////////////////////////////////////////////////////////////////////


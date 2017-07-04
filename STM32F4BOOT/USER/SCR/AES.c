/*
**********************************************************************************************************
*  Copyright (C), 2009-2012, ����˼׳�����з�����
*
*  ��Ŀ���ƣ� E2701
*  
*  �ļ�����:  AES.c
*
*  �ļ������� AES���ܽ����㷨Դ�ļ�
*             
*             
*  �� �� ��: �ʺ���
*
*  �������ڣ�2012-11-15 
*
*  �� �� �ţ�V1.0
*
*  �޸ļ�¼�� 
*             
*      1. ��    �ڣ� 
*         �� �� �ˣ� 
*         �����޸ģ� 
*      2. ...
**********************************************************************************************************
*/

#include "AES.h"

// �ֽ��滻
void SubBytes(unsigned char * state)
{
	unsigned char x, y, i;
	for(i = 0; i < 4 * Nb; i++)
	{
		x = (state[i] >> 4) & 0x0F;
		y = state[i] & 0x0F;
		state[i] = Sbox[x][y];
	}
}

// �ֽ����滻
void InvSubBytes(unsigned char * state)
{
	unsigned char x, y, i;
	for(i = 0; i < 4 * Nb; i++)
	{
		x = (state[i] >> 4) & 0x0F;
		y = state[i] & 0x0F;
		state[i] = InvSbox[x][y];
	}
}

// ���滻
unsigned int SubWord(unsigned int w)
{
	unsigned char tmp, i;
	unsigned int res;

	// �����еĸ����ֽڽ����ֽ��滻
	for(res = 0, i = 0; i < 4; i++)
	{
		tmp  = (unsigned char)(w >> i * 8);
		tmp  = Sbox[(tmp >> 4) & 0x0F][tmp & 0x0F];
		res += (unsigned int)(tmp << i * 8);
	}

	// ����S-�����滻���
	return res;
}


// ��Կ��չ
void KeyExpansion(unsigned char * pkey, unsigned int * pw)
{
	unsigned char i;
	unsigned int wtmp;

	// ǰNk(һ����4����128bit)����Կ��ԭʼ��Կ���
	for(i = 0; i < Nk; i++)
	{
		pw[i]  = (unsigned int)(pkey[4 * i + 0] << 24);
		pw[i] += (unsigned int)(pkey[4 * i + 1] << 16);
		pw[i] += (unsigned int)(pkey[4 * i + 2] << 8 );
		pw[i] += (unsigned int)(pkey[4 * i + 3] << 0 );
	}

	// ��չ�����Կ
	for(i = Nk; i < Nb * (Nr + 1); i++)
	{
		wtmp = pw[i - 1];
		if((i % Nk) == 0)
		{
			wtmp = ROTWORD_1BYTE(wtmp);
			wtmp = SubWord(wtmp);
			wtmp = wtmp ^ Rcon[i / Nk];
		}
		else if(Nk > 6 && (i % Nk) == 4)
		{
			wtmp = SubWord(wtmp);
		}

		pw[i] = pw[i - Nk] ^ wtmp;
	}
}

// ��λ�����任(����)  AES�涨���볤��Ϊ128bit ��Կ���ȿ�ѡ��Ϊ128bit��192bit��256bit
void ShiftRows(unsigned char * pstate) 
{
	unsigned char tmp;

	// ��һ�в���
	// �ڶ�������һ���ֽ�
	tmp = pstate[4];
	pstate[4] = pstate[5];
	pstate[5] = pstate[6];
	pstate[6] = pstate[7];
	pstate[7] = tmp;
	// ����������2���ֽ�
	tmp = pstate[8];
	pstate[8]  = pstate[10];
	pstate[10] = tmp;
	tmp = pstate[9];
	pstate[9]  = pstate[11];
	pstate[11] = tmp;
	// ����������3���ֽ�
	tmp = pstate[15];
	pstate[15] = pstate[14];
	pstate[14] = pstate[13];
	pstate[13] = pstate[12];
	pstate[12] = tmp;
}

// ��λ����任(����)  AES�涨���볤��Ϊ128bit ��Կ���ȿ�ѡ��Ϊ128bit��192bit��256bit
void InvShiftRows(unsigned char * pstate) 
{
	unsigned char tmp;

	// ��һ�в���
	// �ڶ�������һ���ֽ�
	tmp = pstate[7];
	pstate[7] = pstate[6];
	pstate[6] = pstate[5];
	pstate[5] = pstate[4];
	pstate[4] = tmp;
	// ����������2���ֽ�
	tmp = pstate[8];
	pstate[8]  = pstate[10];
	pstate[10] = tmp;
	tmp = pstate[9];
	pstate[9]  = pstate[11];
	pstate[11] = tmp;
	// ����������3���ֽ�
	tmp = pstate[12];
	pstate[12] = pstate[13];
	pstate[13] = pstate[14];
	pstate[14] = pstate[15];
	pstate[15] = tmp;
}


// ������GF(28)�ļӷ��ͳ˷� GF����٤�����򡣱��˵�ֵС��0x80����ʱ�˷��Ľ�����Ǹ�
// ֵ����1����λ��������˵�ֵ���ڻ����0x80����ʱ�˷��Ľ����������1����λ����ֵ
// 0x1b�������ֹ�ˡ�������������ֳ˷��ĳ˻��ڷ�Χ���ڡ�
unsigned char xtimes02(unsigned char num)
{
	unsigned char res;

	if((num & 0x80) == 0) 
	{
		res = num << 1;
	}
	else
	{
		res = num << 1;
		res = res ^ 0x1B;
	}

	return res;
}
unsigned char xtimes03(unsigned char num)
{
	// num*0x03 = num*(02 + 01)
	return (xtimes02(num) ^ num);
}
unsigned char xtimes0E(unsigned char num)
{
	unsigned char res, tmp;

	res = 0;

	// num*0x0E = num*(08 + 04 + 02)
	tmp = xtimes02(num);
	res = res ^ tmp;
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	return res;
}
unsigned char xtimes0D(unsigned char num)
{
	unsigned char res, tmp;

	res = 0;

	// num*0x0D = num*(08 + 04 + 01)
	tmp = xtimes02(num);
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	res = res ^ num;
	return res;
}
unsigned char xtimes0B(unsigned char num)
{
	unsigned char res, tmp;

	res = 0;

	// num*0x0B = num*(08 + 02 + 01)
	tmp = xtimes02(num);
	res = res ^ tmp;
	tmp = xtimes02(tmp);
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	res = res ^ num;
	return res;
}
unsigned char xtimes09(unsigned char num)
{
	unsigned char res, tmp;

	res = 0;

	// num*0x09 = num*(08 + 01)
	tmp = xtimes02(num);
	tmp = xtimes02(tmp);
	tmp = xtimes02(tmp);
	res = res ^ tmp;
	res = res ^ num;
	return res;
}

// �л��
// GF(28)��ĳ˷��ͼӷ�����
// ÿ�г��Ծ��� /b00\   /02 03 01 01\ /a00\
//              |b10|   |01 02 03 01| |a01|
//              |b20| = |01 01 02 03| |a02|
//              \b30/   \03 01 01 02/ \a03/
void MixColumns(unsigned char * state)
{
	unsigned char col[4];

	// ��1�� 
	col[0] = state[0];
	col[1] = state[4];
	col[2] = state[8];
	col[3] = state[12];
	state[0]  = xtimes02(col[0]) ^ xtimes03(col[1]) ^ col[2] ^ col[3];
	state[4]  = col[0] ^ xtimes02(col[1]) ^ xtimes03(col[2]) ^ col[3];
	state[8]  = col[0] ^ col[1] ^ xtimes02(col[2]) ^ xtimes03(col[3]);
	state[12] = xtimes03(col[0]) ^ col[1] ^ col[2] ^ xtimes02(col[3]);
	// ��2��
	col[0] = state[1];
	col[1] = state[5];
	col[2] = state[9];
	col[3] = state[13];
	state[1]  = xtimes02(col[0]) ^ xtimes03(col[1]) ^ col[2] ^ col[3];
	state[5]  = col[0] ^ xtimes02(col[1]) ^ xtimes03(col[2]) ^ col[3];
	state[9]  = col[0] ^ col[1] ^ xtimes02(col[2]) ^ xtimes03(col[3]);
	state[13] = xtimes03(col[0]) ^ col[1] ^ col[2] ^ xtimes02(col[3]);
	// ��3��
	col[0] = state[2];
	col[1] = state[6];
	col[2] = state[10];
	col[3] = state[14];
	state[2]  = xtimes02(col[0]) ^ xtimes03(col[1]) ^ col[2] ^ col[3];
	state[6]  = col[0] ^ xtimes02(col[1]) ^ xtimes03(col[2]) ^ col[3];
	state[10] = col[0] ^ col[1] ^ xtimes02(col[2]) ^ xtimes03(col[3]);
	state[14] = xtimes03(col[0]) ^ col[1] ^ col[2] ^ xtimes02(col[3]);
	// ��4��
	col[0] = state[3];
	col[1] = state[7];
	col[2] = state[11];
	col[3] = state[15];
	state[3]  = xtimes02(col[0]) ^ xtimes03(col[1]) ^ col[2] ^ col[3];
	state[7]  = col[0] ^ xtimes02(col[1]) ^ xtimes03(col[2]) ^ col[3];
	state[11] = col[0] ^ col[1] ^ xtimes02(col[2]) ^ xtimes03(col[3]);
	state[15] = xtimes03(col[0]) ^ col[1] ^ col[2] ^ xtimes02(col[3]);

}


// �л��������
// GF(28)��ĳ˷��ͼӷ�����
// ÿ�г��Ծ��� /a00\    /b00\ /0E 0B 0D 09\
//              |a10|    |b01| |09 0E 0B 0D|
//              |a20| =  |b02| |0D 09 0E 0B|
//              \a30/    \b03/ \0B 0D 09 0E/
void InvMixColumns(unsigned char * state)
{
	unsigned char col[4];

	// ��1��
	col[0] = state[0];
	col[1] = state[4];
	col[2] = state[8];
	col[3] = state[12];
	state[0]  = xtimes0E(col[0]) ^ xtimes0B(col[1]) ^ xtimes0D(col[2]) ^ xtimes09(col[3]);
	state[4]  = xtimes09(col[0]) ^ xtimes0E(col[1]) ^ xtimes0B(col[2]) ^ xtimes0D(col[3]);
	state[8]  = xtimes0D(col[0]) ^ xtimes09(col[1]) ^ xtimes0E(col[2]) ^ xtimes0B(col[3]);
	state[12] = xtimes0B(col[0]) ^ xtimes0D(col[1]) ^ xtimes09(col[2]) ^ xtimes0E(col[3]);	

	// ��2��
	col[0] = state[1];
	col[1] = state[5];
	col[2] = state[9];
	col[3] = state[13];
	state[1]  = xtimes0E(col[0]) ^ xtimes0B(col[1]) ^ xtimes0D(col[2]) ^ xtimes09(col[3]);
	state[5]  = xtimes09(col[0]) ^ xtimes0E(col[1]) ^ xtimes0B(col[2]) ^ xtimes0D(col[3]);
	state[9]  = xtimes0D(col[0]) ^ xtimes09(col[1]) ^ xtimes0E(col[2]) ^ xtimes0B(col[3]);
	state[13] = xtimes0B(col[0]) ^ xtimes0D(col[1]) ^ xtimes09(col[2]) ^ xtimes0E(col[3]);	

	// ��3��
	col[0] = state[2];
	col[1] = state[6];
	col[2] = state[10];
	col[3] = state[14];
	state[2]  = xtimes0E(col[0]) ^ xtimes0B(col[1]) ^ xtimes0D(col[2]) ^ xtimes09(col[3]);
	state[6]  = xtimes09(col[0]) ^ xtimes0E(col[1]) ^ xtimes0B(col[2]) ^ xtimes0D(col[3]);
	state[10] = xtimes0D(col[0]) ^ xtimes09(col[1]) ^ xtimes0E(col[2]) ^ xtimes0B(col[3]);
	state[14] = xtimes0B(col[0]) ^ xtimes0D(col[1]) ^ xtimes09(col[2]) ^ xtimes0E(col[3]);	

	// ��4��
	col[0] = state[3];
	col[1] = state[7];
	col[2] = state[11];
	col[3] = state[15];
	state[3]  = xtimes0E(col[0]) ^ xtimes0B(col[1]) ^ xtimes0D(col[2]) ^ xtimes09(col[3]);
	state[7]  = xtimes09(col[0]) ^ xtimes0E(col[1]) ^ xtimes0B(col[2]) ^ xtimes0D(col[3]);
	state[11] = xtimes0D(col[0]) ^ xtimes09(col[1]) ^ xtimes0E(col[2]) ^ xtimes0B(col[3]);
	state[15] = xtimes0B(col[0]) ^ xtimes0D(col[1]) ^ xtimes09(col[2]) ^ xtimes0E(col[3]);	
}

// ����Կ��
void AddKeyRound(unsigned char *state, unsigned int *key)
{
	unsigned char i, keybuf[4];

	for(i = 0; i < Nb; i++)
	{
		keybuf[0] = (unsigned char)(*(key + i) >> 24);
		keybuf[1] = (unsigned char)(*(key + i) >> 16);
		keybuf[2] = (unsigned char)(*(key + i) >> 8 );
		keybuf[3] = (unsigned char)(*(key + i) >> 0 );
		state[4 * 0 + i] ^= keybuf[0];
		state[4 * 1 + i] ^= keybuf[1];
		state[4 * 2 + i] ^= keybuf[2];
		state[4 * 3 + i] ^= keybuf[3];
	}
}

// ����
void Cipher(unsigned char *intext, unsigned char * key, unsigned char * outtext)
{
	unsigned char state[4 * Nb], r, c;
	unsigned int expandedkey[Nb * (Nr + 1)];

	// ���������������ΪABCDEFGHIJKLMNOP�򿽱���Ϊ
	//             A E I M 
	//             B F J N
	//             C G K O
	//             D H L P
	for(r = 0; r < 4; r++)
	{
		for(c = 0; c < Nb; c++)
		{
			state[4 * r + c] = intext[r + 4 * c];
		}
	}

	// ��Կ��չ
	KeyExpansion(key, expandedkey);

	// ��ʼ��������
	// 1 ��������Կ��
	AddKeyRound(state, &expandedkey[0]);
	// round [1 - (Nr - 1)]
	for(r = 1; r < Nr; r++)
	{
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddKeyRound(state, &expandedkey[Nb * r]);
	}
	// final round
	SubBytes(state);
	ShiftRows(state);
	AddKeyRound(state, &expandedkey[Nr * Nb]);

	// ��������
	for(r = 0; r < 4; r++)
	{
		for(c = 0; c < Nb; c++)
		{
			outtext[r + 4 * c] = state[4 * r + c];
		}
	}
}

// ����
void InvCipher(unsigned char *intext, unsigned char * key, unsigned char * outtext)
{
	unsigned char state[4 * Nb], r, c;
	unsigned int expandedkey[Nb * (Nr + 1)];

	// ���������������ΪABCDEFGHIJKLMNOP�򿽱���Ϊ
	//             A E I M 
	//             B F J N
	//             C G K O
	//             D H L P
	for(r = 0; r < 4; r++)
	{
		for(c = 0; c < Nb; c++)
		{
			state[4 * r + c] = intext[r + 4 * c];
		}
	}

	// ��Կ��չ
	KeyExpansion(key, expandedkey);

	// ��ʼ��������
	// 1 ��������Կ��
	AddKeyRound(state, &expandedkey[Nr * Nb]);
	// round [(Nr - 1) - 1]
	for(r = Nr - 1; r > 0  ; r--)
	{
		InvShiftRows(state);
		InvSubBytes(state);
		AddKeyRound(state, &expandedkey[Nb * r]);
		InvMixColumns(state);
	}
	// final round
	InvShiftRows(state);
	InvSubBytes(state);
	AddKeyRound(state, &expandedkey[0]);

	// ��������
	for(r = 0; r < 4; r++)
	{
		for(c = 0; c < Nb; c++)
		{
			outtext[r + 4 * c] = state[4 * r + c];
		}
	}
}





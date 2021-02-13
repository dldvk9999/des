#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __NUMBER_GROUP 1
#define __STRING_GROUP 1

#ifdef __NUMBER_GROUP
	#define Encryption	1
	#define Decryption	2
	#define Round_Count 16
	#define STR_BUFF	100
#endif
#ifdef __STRING_GROUP
	#define USER_INFO		"\n:: Made.by __Park.JG__ ::\n\n"
	#define FIRST_SCANF		"암호화를 원하시면 c, 복호화를 원하시면 p를 입력해주세요 : "
	#define GET_PWD_FILE	"암호화할 패스워드와 파일을 입력해주세요.(ex.123pass test.txt) : "
	#define GET_PWD_FILE2	"복호화할 패스워드와 파일을 입력해주세요.(ex.123pass test.des) : "
	#define ENC_FINISH		"암호화 완료!\n"
	#define DEC_FINISH		"복호화 완료!\n"
	#define DONT_OPEN		"\"%s\" 파일을 열 수 없습니다!!!!\n"
	#define NOT_ENC_FILE	"%s 은 DES 암호화한 파일이 아닙니다!!\n\n"
#endif

int Initial_Permutation[64] = {
	58, 50, 42, 34, 26 ,18 ,10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

int Initial_Permutation_1[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

int Extern_Permutation[48] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

int PERMUTATION[32] = {
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
};

int SBOX[8][4][16] = {{
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 2, 14, 10, 0, 6, 13}},
  
   {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
    {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
    {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
    {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

   {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
    {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
    {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
    {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

   {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
    {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
    {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
    {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    
   {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
    {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
    {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
    {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

   {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
    {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
    {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
    {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

   {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
    {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
    {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
    {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

   {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
    {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
    {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
    {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

int PC_1[56] = {
	57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int PC_2[48] = {
	14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

int KEY[16][48];

void keyGenerate	(char *);
void DES_Start		(int, int, char *, char *);
void Round			(int *, int *);
void CharToBin		(int *, char *, int);
char BinToChar		(int *);
void swap32bit		(int *, int *, int *);
void keyProc		(char *); 

/* DES의 메인함수 */
void DES_Main(int EncOrDec, char *pwd, char *File_Name) {
	FILE	 *fp;
	int		 Key_Tmp[16][48];
	long int dataSize;
	long int readSize, writeSize;
	char	 *File_Buff_1, *File_Buff_2;
	char	 Name_Tmp[STR_BUFF] = { 0, };
	char	 arrDes	 [STR_BUFF] = { 0, };
	
	/* 키 생성 */
	keyGenerate(pwd);
  
	strcpy(Name_Tmp, File_Name);
	if(EncOrDec == Decryption) {
		if(strcmp(".des", File_Name + (strlen(File_Name) - 4))) {
			printf(NOT_ENC_FILE, File_Name); exit(0);
		}
		Name_Tmp[strlen(File_Name) - 4] = '\0';
	} else {
		Name_Tmp[strlen(File_Name) - 4] = '\0';
		sprintf(arrDes, "%s.des", Name_Tmp);
	}
	
	/* 복호화시 키 역순 */
	if(EncOrDec == Decryption) {
		for(int i = 0; i < 16; i++)
			for(int j = 0; j < 48; j++)
				Key_Tmp[15 - i][j] = KEY[i][j];
		for(int i = 0; i < 16; i++)
			for(int j = 0; j < 48; j++)
				KEY[i][j] = Key_Tmp[i][j];
	}

	if((fp = fopen(File_Name, "r+b")) == NULL) {
		printf(DONT_OPEN, File_Name); exit(0);
	}

	fseek(fp, 0, SEEK_END); readSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);	dataSize = readSize / 8;
	dataSize++;
	File_Buff_1 = malloc(dataSize * 8);
	File_Buff_2 = malloc(dataSize * 8);
	memset(File_Buff_1, 0, dataSize * 8);
	memset(File_Buff_2, 0, dataSize * 8);
	
	/* 파일이름 불러오기 */
	if(EncOrDec == Decryption) {
		fread(&writeSize, sizeof(long int), 1, fp);
		fgets(arrDes, STR_BUFF, fp);
		arrDes[strlen(arrDes) - 1] = '\0';
		strcpy(Name_Tmp, arrDes);
		readSize -= sizeof(long int);
	}
	
	fread(File_Buff_1, readSize, 1, fp);
	fclose(fp);
	DES_Start(EncOrDec, dataSize, File_Buff_1, File_Buff_2);
	free(File_Buff_1);

	if((fp = fopen(arrDes, "w+b")) == NULL){
		printf(DONT_OPEN, arrDes); exit(0);
	}
	
	/* 복호화시 파일이름 원래대로 돌려놓기 편하게 하기 위해 암호화시 파일 내용에 원래 파일 이름 저장 */
	if(EncOrDec == Encryption){
		fwrite(&readSize, sizeof(long int), 1, fp);
		fprintf(fp, "%s\n", File_Name);
		fwrite(File_Buff_2, dataSize * 8, 1, fp);
	} else
		fwrite(File_Buff_2, writeSize, 1, fp);

	fclose(fp);
	free(File_Buff_2);
}

/* DES 알고리즘 시작 */
void DES_Start(int EncOrDec, int dataSize, char *Source_File, char *Dest_File) {
	int  Bin_1		  [64]	= { 0, }, 
		 Bin_2		  [64]	= { 0, }, 
		 Bin_Swap_Tmp [64]	= { 0, },
		 Data_1		  [32]	= { 0, }, 
		 Data_2		  [32]	= { 0, }, 
		 Data_Swap_Tmp[32]	= { 0, };
	int  Index				= 0;
	char textBuff	  [9]	= { '\0', };

	for(int i = 0, Index = 0; i < dataSize; i++) {
		memset(textBuff, 0, 9);
		memcpy(textBuff, (Source_File + Index), 8);
		
		/* 문자열을 바이너리로 바꿈 */
		CharToBin(Bin_Swap_Tmp, textBuff, 8);
		
		/* PERMUTATION */
		for(int j = 0; j < 64; j++)
			Bin_1[j] = Bin_Swap_Tmp[Initial_Permutation[j] - 1];

		if(EncOrDec == Encryption)
			for(int j = 0; j < 32; j++) {
				Data_1[j] = Bin_1[j];
				Data_2[j] = Bin_1[j+32];
			}
		else
			for(int j = 0; j < 32; j++) {
				Data_2[j] = Bin_1[j];
				Data_1[j] = Bin_1[j+32];
			}
		
		/* Round 돌림 */
		Round(Data_1, Data_2);
		
		if(EncOrDec == Encryption)
			swap32bit(Data_1, Data_2, Data_Swap_Tmp);

		for(int j = 0; j < 32; j++) {
			Bin_Swap_Tmp[j]	   = Data_2[j];
			Bin_Swap_Tmp[j+32] = Data_1[j];
		}
		
		for(int j = 0; j < 64; j++)
			Bin_2[j] = Bin_Swap_Tmp[Initial_Permutation_1[j] - 1];
		memset(textBuff, 0, 9);
		
		/* 문자열로 다시 바꿈 */
		for(int j = 0, k = 0; j < 64; j += 8, k++)
			textBuff[k] = BinToChar(&Bin_2[j]);
		memcpy((Dest_File + Index), textBuff, 8);
		Index += 8;
	}
}

void Round(int *Data_1, int *Data_2){
	int SBox_Out		[32]	= { 0, }, 
		Final			[32]	= { 0, }, 
		__Buff   		[48]	= { 0, }, 
		__BuffOut		[48]	= { 0, },
		Permutation_Out [32]	= { 0, },
		SBox_Row, SBox_Col, SBox_Val;

	for(int keyIndex = 0; keyIndex < Round_Count; keyIndex++){
		for(int i = 0; i < 32; i++)
			Final[i] = *(Data_2 + i);

		/* XOR */
		for(int i = 0; i < 48; i++) {  					
			__Buff[i]	 = *(Data_2 + (Extern_Permutation[i] - 1));
			__BuffOut[i] = __Buff[i] ^ KEY[keyIndex][i];
		}
		
		/* SBOX */
		for(int i = 0, j = 0, k = 0; i < 8; i++) {
			SBox_Row		= (__BuffOut[j + 0] * 2) + __BuffOut[j + 5];
			SBox_Col		= (__BuffOut[j + 1] * 8) + 
							  (__BuffOut[j + 2] * 4) + 
							  (__BuffOut[j + 3] * 2) + 
							   __BuffOut[j + 4];
			SBox_Val	    = SBOX[i][SBox_Row][SBox_Col];
			SBox_Out[k]     = ((SBox_Val & 8) ? 1 : 0 );
			SBox_Out[k + 1] = ((SBox_Val & 4) ? 1 : 0 );
			SBox_Out[k + 2] = ((SBox_Val & 2) ? 1 : 0 );
			SBox_Out[k + 3] = SBox_Val & 1;
			j += 6;
			k += 4;
		}
		
		/* PERMUTATION */
		for(int i = 0; i < 32; i++) {
			Permutation_Out[i] = SBox_Out[PERMUTATION[i] - 1];  
			*(Data_2 + i)	   = *(Data_1 + i) ^ Permutation_Out[i];
			*(Data_1 + i)	   = Final[i];
		}
	}
}

void swap32bit(int *leftBit, int *rightBit, int *buffBit) {
	for(int i = 0; i < 32; i++) {
		buffBit [i] = leftBit [i];
		leftBit [i] = rightBit[i];
		rightBit[i] = buffBit [i];
	}
}

/* 키 생성 함수 */
void keyGenerate(char *keySrc) {
	int  keySchedule[16]	= { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	int  binaryKey	[64]	= { 0, };
	int  pc1		[56]	= { 0, };
	char Key_Tmp	[9];
	int  tmp1, tmp2, srcLen = strlen(Key_Tmp);

	if((strlen(keySrc)) > 8) keyProc(keySrc);
		
	strncpy(Key_Tmp, keySrc, 8);
	Key_Tmp[8] = '\0';
	CharToBin(binaryKey, Key_Tmp, srcLen);
	memset(Key_Tmp, 0, 8);

	for(int i = 0; i < 56; i++)
		pc1[i] = binaryKey[PC_1[i] - 1];

	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < keySchedule[i]; j++) {
			tmp1 = pc1[0];
			tmp2 = pc1[28];
			for(int k = 1; k < 28; k++) {
				pc1[k - 1]  = pc1[k];
				pc1[k + 27] = pc1[k + 28];
			}
			pc1[27] = tmp1;
			pc1[55] = tmp2;
		}

		for(int j = 0; j < 48; j++)
			KEY[i][j] = pc1[PC_2[j] - 1];
	}
}

/* 키 비트 맞춰주는 함수 */
void keyProc(char *keySrc) {
	int  srcLen = strlen(keySrc), 
		 dev4	= srcLen / 4;
	char *temp  = malloc(srcLen + 1);
	
	memset(temp, 0, srcLen +1);
	strcpy(temp, keySrc);
	memset(keySrc, 0, srcLen);
	srcLen--;

	for(int i = 0, j = 0; i < 4; i++, j += dev4) {
		*(keySrc + i)     = *(temp + srcLen - j);
		*(keySrc + i + 4) = *(temp + j);
	}
}

/* char -> bin */
void CharToBin(int *target, char *src, int srcLen) {
	int  targetIndex = 0;
	char oneChar, temp1, temp2;
	for(int i = 0 ; i < srcLen; i++) {
		oneChar = *(src + i);
		for(int j = 7; j >= 0; j--) {
			temp1 = oneChar >> j;
			temp2 = temp1 & 0x0001;
			*(target + targetIndex) = (temp2 == 1 ? 1 : 0);
			targetIndex++;
		}
	}
}

/* bin -> char */
char BinToChar(int *src) {
	char temp;
	int  inTemp = 1, chTemp = 0;
	for(int i = 0; i < 8; i++) {
		inTemp = 1;
		if(*(src + i)) {
			for(int j = 7; j > i; j--)
				inTemp *= 2;
			chTemp += inTemp;
		}
	}
	return temp = chTemp;
}

int main(void) {
	char answer;
	char text[STR_BUFF];
	char pwd [STR_BUFF];
	
	printf(USER_INFO);
	printf(FIRST_SCANF);
	scanf ("%c", &answer);
	
	if (answer == 'c') {
		printf	(GET_PWD_FILE);
		scanf	("%s %s", pwd, text);
		DES_Main(Encryption, pwd, text);
		printf	(ENC_FINISH);
	}
	else if (answer == 'p') {
		printf	(GET_PWD_FILE2);
		scanf	("%s %s", pwd, text);
		DES_Main(Decryption, pwd, text);
		printf	(DEC_FINISH);
	}
	else printf("c 나 p 중 하나를 입력해주세요!\n");
	
	system("pause");
	return 0;
}   
#define MAXWORD 500
#define ResWordNum 14
#define MAXC 50

int linenum=1;
char word[MAXWORD];
int numberoffunction[100];
int varoffunction[100];
int isback=0;
int syms;
char lastfunction[30];
int lastpcode;
int linenumber=1;
FILE *in ,*out;

char *ReserveWord[]=
{
	"int","char","float","void",
	"if", "while", "switch", "case", "default",
	"scanf", "printf" ,"return" ,"const","else"
};

char *Oprate[]=
{					//������
	"LDA","LDI","JME","JMC","JMP","CAS",
	"RED","EXF","WRR","INF","ADD","SUB","",
	"MUL","DIV","MUS",">","<","==","!=",">=",
	"<=","STO","CAL","MKF","EXP"
};
struct MyStruct
		{
			int adr;//����ջ��ڵ�ַ
			int ref;//ָ����Ӧ�ı�
			int lev;//�ڷֳ���Ĳ��
			int geshu;
		};
union MyUnion
	{
		int valint;
		float valfloat;
		char valchar;
		MyStruct valfunc;
	};
struct Character
{
	char *name;    //���ű�ı�������
	int lev;	   //���ű�Ĳ�������C0�ķ���Ϊ�����еľֲ�������ȫ�ֱ�����	
	int type;		//���ŵ�����
	int kind;		//�ǳ����������Ǻ�������
	MyUnion f;
};
struct PCO
{
	char opr[5];
	int lev;
	float kind;
	char *str;
};
PCO PCODE[1000];
Character fuhaobiao[100][100];
int canshugeshu[100];
int flagerror;
int lev;//���������Ĳ��
int fuhaohang=0,fuhaolie=0;

int interprete();                          //����ջ
int getsyms();									//�ʷ���������
int enter(char* name,int type,int kind);			//�ѱ���������ű�
int constdeclaration();							//��������˵������
int program();									//��������
int charcheck();								//����ʶ���Ƿ��ظ�����
int constdefine( int type );					//������������
int vardefine( int type);						//��������˵������  
int returnfunction( int type)	;				//�з���ֵ�ĺ���
int voidfunction( );							//�޷���ֵ�ĺ����������
int statementArray( );							//�����
int statement( );								//���
int ifstatement( );								//if�������
int whilestatement( );							//whileѭ�����
int switchstatement( );                         //switch���
int scanfstatement( );							//scanf�����
int printfstatement( );							//printfд���
int returnstatement( );							//return���
int assignstatement(int a);						//��ֵ���
int mainfunction( );							//������
int call(int offset);								//�����������
int expression();								//���ʽ
int gen(char *op,int lev,int x);					//����һ�����ʽ
int term();										//��
int factor();									//����
int find(int l);//�ڷ��ű��в��ұ�ʶ���Ƿ����
int find(int l,char *tword);
void error();                                    //������			
/*
	��ʶ��			1				�ڲ��ַ���
	����			2				����ֵ
	������          3               ������
	�ַ�			4				�ַ�
	�ַ���			5				�ַ���
	����            6               ��������
 
    int  			10				---
	char			11				---
	float			12				---
	void			13				---
	if	 	     	14				---
	while			15				---
	switch			16				---
	case        	17				---
	default			18				---
	scanf			19				---
	printf			20				---
	return			21	 			---
	const           22              ---
	else            23              ---
	��				31				---
	��				32				---
	*				33				---
	��				34				---
	}				35				---
	��				36				---
	��				37				---
	��				38				---
	��				39				---
	{				40				---
	"				41				---
 	"				42				---
  	'				43				---
	'				44				---
	>				45				---
	<				46				---
	==				47				---
	!=              48              ---
	>=				49				---
	<=				50				---	
	=				51				---
	[               52              ---
	]               53              ---
	:               54              ---
	*/

/*
opr 0 14		�������ӷ�
opr 0 15		����������
opr 0 16		�������˷�
opr 0 17		����������

prs ��ӡ�ַ���
pri ��ӡ����
prf ��ӡ������
prc ��ӡ��ĸ
rei ��ȡint
ref	��ȡfloat
rec ��ȡchar
*/
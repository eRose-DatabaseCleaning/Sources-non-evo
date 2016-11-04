// 2006.06.30/��뼺/�߰� - SQL ������������ �̿��� ��ŷ����
// SQL �������� ���鶧 ���Ǵ� ���ڿ����� �����Ͽ� Ư�� ���ڸ� �ٸ����ڷ� ��ü�ϰų�
// ��ȯ�Ѵ�.


#include "AlphabetCvt.h"

// ���ĺ��� ' �� �ٲ۴�. a->b, z->a, A->B, Z->A, '->`
char Alphabet2[] = {"                                       `                         BCDEFGHIJKLMNOPQRSTUVWXYZA      bcdefghijklmnopqrstuvwxyza"};
// ���ĺ��� ���󺹱��Ѵ�.
char Alphabet0[] = {"                                                                 ZABCDEFGHIJKLMNOPQRSTUVWXY      zabcdefghijklmnopqrstuvwxy"};

void AlphabetCvt(char *p, int len)
{
	//return;	// Japan

	char *pp;
	for(int i=0; i<len; ++i)
	{
		pp = p + i;

		// America
		if(*pp == '\'' || *pp == '_' || *pp == '!' || *pp == '-' || 
			*pp == '#' || *pp == '$' || *pp == '%' || *pp == '^' || *pp == '&' || 
			*pp == '*' || *pp == '(' || *pp == ')' || *pp == '\\' || *pp == ';' ||
			*pp == '|' || *pp == '{' || *pp == '}' )
		{
			*pp = Alphabet2[*pp];
		}

		// ���ĺ��� ' �� �ٲ۴�. a->b, z->a, A->B, Z->A, '->`
		//if(('a' <= *pp && *pp <= 'z') || ('A' <= *pp && *pp <= 'Z') || *pp == '\'')
		//if(*pp == '\'' || *pp == '-')		// �ӽ÷� ' ���ڸ� `�� �ٲ۴�.
		/*
		if(*pp == '\'' || *pp == '-' || *pp == '_' || *pp == '!' || *pp == '@' || 
			*pp == '#' || *pp == '$' || *pp == '%' || *pp == '^' || *pp == '&' || 
			*pp == '*' || *pp == '(' || *pp == ')' || *pp == '\\' || *pp == ';' ||
			*pp == '|' || *pp == '{' || *pp == '}' )
		{
			*pp = Alphabet2[*pp];
		}
		if(*pp == '\'' || *pp == '-' || *pp == '_')		// �ӽ÷�
		{
			*pp = Alphabet2[*pp];
		}
		*/


		// Korea, Philippine
		//if(*pp == '\'' || *pp == '_' || *pp == '!' || 
		//	*pp == '#' || *pp == '$' || *pp == '%' || *pp == '^' || *pp == '&' || 
		//	*pp == '*' || *pp == '(' || *pp == ')' || *pp == '\\' || *pp == ';' ||
		//	*pp == '|' || *pp == '{' || *pp == '}' )
		//{
		//	*pp = Alphabet2[*pp];
		//}
	}
}

void AlphabetCvt2(char *p, int len)
{
	char *pp;
	for(int i=0; i<len; ++i)
	{
		pp = p + i;

		// ���ĺ��� ���󺹱��Ѵ�.
		if(('a' <= *pp && *pp <= 'z') || ('A' <= *pp && *pp <= 'Z'))
		{
			*pp = Alphabet0[*pp];
		}
	}
}

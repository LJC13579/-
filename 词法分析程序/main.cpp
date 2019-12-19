#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
/*
ʵ����Ŀ��
      ��Ŀ��  �򻯵� C ���ԣ��������߼����ԣ��ʷ�����������ƣ�������Ŀ�� 
������ݼ�Ҫ���Լ�����һ���򵥳���������Եĵ��ʼ����������򣬲��Ѵ�
����������������������ֵ�ʶ�𰴱�ʶ��һ��ʶ��ͨ�����ұ����ֱ������Ǳ����ֻ�
�Ǳ�ʶ���������ܹ����û������Դ�����У�ʶ����ĵ��ʷ��ţ����ö�Ԫʽ��ʾ����ʾ
�����������ļ���
*/
struct Word {
	int syn;//�����ֱ���
	char *ch;//���������ַ�������������
};
struct Word all_word[24];
void init_word(int i, char *ch, int syn) {
	char *new_ch = (char*)malloc(sizeof(char) * 30);
	strcpy(new_ch, ch);
	all_word[i].ch = new_ch;
	all_word[i].syn = syn;
}
void init_all_word() {//��ʼ�����ʱ�
	int i = 0;
	char ch[30];//�ַ���
	int syn;//��¼�����ֱ�
	FILE *fp;
	fp = fopen("all_word.txt", "r");
	int d = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", ch);//�����׼����
		fscanf(fp, "%d\n", &syn);//���뵥���ֱ�
		init_word(i++, ch, syn);//��ʼ��
	}
	fclose(fp);
}
//�ж��ǽ����������
int isDelimiter(char ch) {
	if (ch == '(' || ch == ')' || ch == ';')
		return 1;
	else
		return 0;
}
//�ж�����ĸ��������
int isLetter(char ch) {
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else
		return 0;
}
//�ж������֣�������
int isNumber(char ch) {
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}
//�ж����������������
int isOperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>')
		return 1;
	else return 0;
}

void  check_word_form(char *ch, struct Word *word) {
	int length = (int)(sizeof(all_word) / sizeof(struct Word));//�ʻ��Ĵ�С
	int i;
	for (i = 0; i < length; i++) {
		if (strcmp(ch, all_word[i].ch) == 0) {
			(*word).syn = all_word[i].syn;
			strcpy((*word).ch, all_word[i].ch);
			break;
		}
	}
	if (i == length) {//�ڴʻ����û����������Ӧ�ĵ���
		(*word).syn = 10;
		strcpy((*word).ch, ch);
	}
}
struct Word  scanner(string prog, int *p) {
	struct Word word;
	word.syn = -1;//��ʼ�������ڱ�����ʶ��
	word.ch = (char*)malloc(sizeof(char) * 30);
	char ch;// ����һ���ַ�����ch
	char *token;
	token = (char*)malloc(sizeof(char) * 30);
	char ip = 0;//��¼tokenָ��λ��
	ch = prog[(*p)++];
	while (ch == ' '||ch=='\n')
	{
		ch = prog[(*p)++];
	}
	if (isLetter(ch))//�ж��Ƿ�����ĸ��������
	{
		while (isLetter(ch) || isNumber(ch))//�ж�ch�Ƿ�����ĸ������
		{
			token[ip++] = ch;//д��token�ж��ַ�������
			ch = prog[(*p)++];//����������һ���ַ���
		}
		token[ip] = '\0';
		(*p)--;//����һ���ַ�
		check_word_form(token, &word);//token��ؼ��ֱ���бȽϣ�ȷ��syn��ֵ��
	}
	else
		if (isNumber(ch))
		{
			while (isNumber(ch))//�ж�ch�Ƿ�����
			{
				token[ip++] = ch;//д��token�ж��ַ�������
				ch = prog[(*p)++];//����������һ���ַ���
			}
			token[ip] = '\0';
			(*p)--;//����һ���ַ�
			word.syn = 11;
			strcpy(word.ch, token);
		}
		else if (isDelimiter(ch) || isOperator(ch)) {//�ж�����������Ƿ�Ϊ���
			token[ip++] = ch;//д��token�ж��ַ�������
			token[ip] = '\0';
			check_word_form(token, &word);
		}
		else {
			switch (ch) { //�����ַ����
			case '<':
				token[ip++] = ch;
				ch = prog[(*p)++];//������һ���ַ��������ж�
				switch (ch) {
				case '>':case '='://�ж���һ���ַ��ǡ�����  ��=��
					token[ip++] = ch;
					break;
				default:
					(*p)--;//����һ���ַ�
				}
				token[ip] = '\0';
				check_word_form(token, &word);//���е���ʶ��
				break;
			case '>':case ':':
				token[ip++] = ch;
				ch = prog[(*p)++];
				if (ch == '=')//�ж���һ���ַ��ǡ�=��
					token[ip++] = ch;
				else           //����һ���ַ����ǡ�=��
					(*p)--;//����һ���ַ�
				token[ip] = '\0';
				check_word_form(token, &word);
				break;
			case '#':
				token[ip++] = ch;
				token[ip] = '\0';
				check_word_form(token, &word);
				break;
			}
		}
	return word;
}
void output_digital(struct Word word, FILE *fp) {//������ֵĶ�Ԫ����
	cout << "(" << word.syn << "," << word.ch << ")" << endl;
	fprintf(fp, "(%d,  %s)\n", word.syn, word.ch);
}
void output_letter(struct Word word, FILE *fp) {//����ַ����Ķ�Ԫ����
	cout << "(" << word.syn << "," << word.ch << ")" << endl;
	fprintf(fp, "(%d,  %s)\n", word.syn, word.ch);
}
string input_source_code() {
	//���Ƚ�����txt�ļ����뵽program�ַ�����
	ifstream in("test.txt", ios::in);
	istreambuf_iterator<char> beg(in), end;
	string source_code(beg, end);//����string st;st.assign(beg,end);
	in.close();
	return source_code;
}
int main() {
	int p = 0, length = 10;//pΪ������±�,lengthΪ��¼����ĳ���
	string prog;
	prog = (char*)malloc(sizeof(char) *length);//��ָ������ַ�ռ�
    prog=input_source_code();
	init_all_word();
	/*
	do {
		if (p >= length) {
			length += 10;
			prog = (char*)realloc(prog, sizeof(char)*(length));//���·����ַ
		}
		cin>> ch;
		prog[p++] = ch;//����һ���ַ���
	} while (ch != '#');
	if (p >= length) {
		length += 10;
		prog = (char*)realloc(prog, sizeof(char)*(length));//���·����ַ
	}
	prog[p++] = '\0';//Ϊprog�������\0��������*/
	p = 0;
	struct Word word;
	int error_sign = 0;//����ı�ʶ��0û�д���1���ִ���
	FILE *fw;
	fw = fopen("output_word.txt", "w");
	do {
		word = scanner(prog, &p);//����ɨ���ӳ���
		switch (word.syn)
		{
		case 11:output_digital(word, fw); break;//�������
		case -1:printf("error\n"); error_sign = 1; break;//����
		default:output_letter(word, fw);//�����ʶ��
		}
		if (error_sign == 1) {
			break;//���������˳�����
		}
	} while (word.syn != 0);
	fclose(fw);
	system("pause");
}
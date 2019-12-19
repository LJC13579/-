#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
/*
实验题目：
      题目：  简化的 C 语言（或其它高级语言）词法分析程序设计（个人题目） 
设计内容及要求：自己定义一个简单程序设计语言的单词集及其编码规则，并把词
法分析器构造出来。保留字的识别按标识符一样识别，通过查找保留字表区分是保留字还
是标识符。程序能够从用户输入的源程序中，识别出的单词符号，并用二元式表示，显示
输出或输出到文件中
*/
struct Word {
	int syn;//单词种别码
	char *ch;//单词自身字符串或者整型数
};
struct Word all_word[24];
void init_word(int i, char *ch, int syn) {
	char *new_ch = (char*)malloc(sizeof(char) * 30);
	strcpy(new_ch, ch);
	all_word[i].ch = new_ch;
	all_word[i].syn = syn;
}
void init_all_word() {//初始化单词表
	int i = 0;
	char ch[30];//字符串
	int syn;//记录单词种别
	FILE *fp;
	fp = fopen("all_word.txt", "r");
	int d = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", ch);//读入标准数据
		fscanf(fp, "%d\n", &syn);//读入单词种别
		init_word(i++, ch, syn);//初始化
	}
	fclose(fp);
}
//判端是界符：返回真
int isDelimiter(char ch) {
	if (ch == '(' || ch == ')' || ch == ';')
		return 1;
	else
		return 0;
}
//判断是字母：返回真
int isLetter(char ch) {
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else
		return 0;
}
//判断是数字：返回真
int isNumber(char ch) {
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}
//判断是运算符：返回真
int isOperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>')
		return 1;
	else return 0;
}

void  check_word_form(char *ch, struct Word *word) {
	int length = (int)(sizeof(all_word) / sizeof(struct Word));//词汇表的大小
	int i;
	for (i = 0; i < length; i++) {
		if (strcmp(ch, all_word[i].ch) == 0) {
			(*word).syn = all_word[i].syn;
			strcpy((*word).ch, all_word[i].ch);
			break;
		}
	}
	if (i == length) {//在词汇表中没有搜索到对应的单词
		(*word).syn = 10;
		strcpy((*word).ch, ch);
	}
}
struct Word  scanner(string prog, int *p) {
	struct Word word;
	word.syn = -1;//初始化，用于报错处理识别
	word.ch = (char*)malloc(sizeof(char) * 30);
	char ch;// 读下一个字符送入ch
	char *token;
	token = (char*)malloc(sizeof(char) * 30);
	char ip = 0;//记录token指针位置
	ch = prog[(*p)++];
	while (ch == ' '||ch=='\n')
	{
		ch = prog[(*p)++];
	}
	if (isLetter(ch))//判断是否是字母或者数字
	{
		while (isLetter(ch) || isNumber(ch))//判断ch是否是字母或数字
		{
			token[ip++] = ch;//写入token判断字符串组中
			ch = prog[(*p)++];//继续读入下一个字符串
		}
		token[ip] = '\0';
		(*p)--;//回退一个字符
		check_word_form(token, &word);//token与关键字表进行比较，确定syn的值；
	}
	else
		if (isNumber(ch))
		{
			while (isNumber(ch))//判断ch是否数字
			{
				token[ip++] = ch;//写入token判断字符串组中
				ch = prog[(*p)++];//继续读入下一个字符串
			}
			token[ip] = '\0';
			(*p)--;//回退一个字符
			word.syn = 11;
			strcpy(word.ch, token);
		}
		else if (isDelimiter(ch) || isOperator(ch)) {//判断运算符或者是否为界符
			token[ip++] = ch;//写入token判断字符串组中
			token[ip] = '\0';
			check_word_form(token, &word);
		}
		else {
			switch (ch) { //其他字符情况
			case '<':
				token[ip++] = ch;
				ch = prog[(*p)++];//输入下一个字符，进行判断
				switch (ch) {
				case '>':case '='://判断下一个字符是“》”  “=”
					token[ip++] = ch;
					break;
				default:
					(*p)--;//回退一个字符
				}
				token[ip] = '\0';
				check_word_form(token, &word);//进行单词识别
				break;
			case '>':case ':':
				token[ip++] = ch;
				ch = prog[(*p)++];
				if (ch == '=')//判读下一个字符是‘=’
					token[ip++] = ch;
				else           //当下一个字符不是“=”
					(*p)--;//回退一个字符
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
void output_digital(struct Word word, FILE *fp) {//输出数字的二元数组
	cout << "(" << word.syn << "," << word.ch << ")" << endl;
	fprintf(fp, "(%d,  %s)\n", word.syn, word.ch);
}
void output_letter(struct Word word, FILE *fp) {//输出字符串的二元数组
	cout << "(" << word.syn << "," << word.ch << ")" << endl;
	fprintf(fp, "(%d,  %s)\n", word.syn, word.ch);
}
string input_source_code() {
	//首先将整个txt文件读入到program字符串里
	ifstream in("test.txt", ios::in);
	istreambuf_iterator<char> beg(in), end;
	string source_code(beg, end);//或者string st;st.assign(beg,end);
	in.close();
	return source_code;
}
int main() {
	int p = 0, length = 10;//p为数组的下标,length为记录数组的长度
	string prog;
	prog = (char*)malloc(sizeof(char) *length);//给指针分配地址空间
    prog=input_source_code();
	init_all_word();
	/*
	do {
		if (p >= length) {
			length += 10;
			prog = (char*)realloc(prog, sizeof(char)*(length));//重新分配地址
		}
		cin>> ch;
		prog[p++] = ch;//读入一个字符串
	} while (ch != '#');
	if (p >= length) {
		length += 10;
		prog = (char*)realloc(prog, sizeof(char)*(length));//重新分配地址
	}
	prog[p++] = '\0';//为prog后面加入\0方便输入*/
	p = 0;
	struct Word word;
	int error_sign = 0;//错误的标识，0没有错误，1出现错误
	FILE *fw;
	fw = fopen("output_word.txt", "w");
	do {
		word = scanner(prog, &p);//调用扫描子程序
		switch (word.syn)
		{
		case 11:output_digital(word, fw); break;//输出数字
		case -1:printf("error\n"); error_sign = 1; break;//报错
		default:output_letter(word, fw);//输出标识符
		}
		if (error_sign == 1) {
			break;//遇到报错退出程序
		}
	} while (word.syn != 0);
	fclose(fw);
	system("pause");
}
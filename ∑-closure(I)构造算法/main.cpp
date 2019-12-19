#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
struct trans_func {//状态转换函数的结构
	char *current_state;//当前状态
	char *temp;//转换字符
	char *last_state;//下一状态
};
void init_trans_func(struct trans_func *tf) {
	tf->current_state = (char*)malloc(sizeof(char) * 10);
	tf->temp = (char*)malloc(sizeof(char) * 10);
	tf->last_state = (char*)malloc(sizeof(char) * 10);
}
int initNFA_M(int *state_num, char **state, int *terminator_num, char **terminator, char *first_state, char *last_state, struct trans_func *tf) {
	FILE *fp;
	fp = fopen("NFA M.txt", "r");//文件记录数据的格式
	/*
	状态个数   状态1  状态2.....
	终结符个数 终结符1 终结符2....  其中∑使用null代替
	开始状态
	最终状态
	状态转换函数1
	状态转换函数2
	.....
	*/
	fscanf(fp, "%d ", state_num);
	int i = 0;
	for (i = 0; i < *state_num; i++)
		fscanf(fp, "%s ", state[i]);
	fscanf(fp, "%d ", terminator_num);
	for (i = 0; i < *terminator_num; i++)
		fscanf(fp, "%s  ", terminator[i]);
	fscanf(fp, "%s\n", first_state);
	fscanf(fp, "%s\n", last_state);
	i = 0;
	int length = (int)_msize(tf) / sizeof(struct trans_func);//得到tf的空间大小
	while (!feof(fp))
	{
		init_trans_func(&tf[i]);//分配地址
		fscanf(fp, "%s ", tf[i].current_state);//读入当前状态
		fscanf(fp, "%s ", tf[i].temp);//读入转换字符
		fscanf(fp, "%s \n", tf[i].last_state);//读入下一个状态
		i++;
		if (i >= length) {//判断空间是否足够
			length += 10;
			tf = (struct trans_func*)realloc(tf, length);
		}
	}
	fclose(fp);
	return i;//返回转换函数的个数
}
void input_I(char **input_state_group) {
	int i = 0;
	cout<<"请输入你要求的子集I为(以空格分隔、#结尾）：";
	do {
		cin>>input_state_group[i];
		i++;
	} while (strcmp(input_state_group[i - 1], "#") != 0);//destination中存储着#标记状态结束
}
void search(struct trans_func *tf, int *tag, char *current_state, int state_num, char **state, int trans_func_num) {
	for (int i = 0; i < state_num; i++) {
		if (strcmp(current_state, state[i]) == 0) {
			if (tag[i] == 1)//已经在closure中，不要再次进行遍历
				return;
			tag[i] = 1;//加入closure集中
			for (int j = 0; j < trans_func_num; j++) {
				if (strcmp(tf[j].current_state, current_state) == 0 && strcmp(tf[j].temp, "null") == 0) {
					search(tf, tag, tf[j].last_state, state_num, state, trans_func_num);//递归调用search，搜索last_state有没有通过∑达到其他状态
				}
			}
			break;
		}
	}
}
void get_closure(char **input_state_group, char **destination, int state_num, char **state, struct trans_func *tf, int trans_func_num) {
	int *tag;//0为没有标记，1为标记成功
	tag = (int *)malloc(sizeof(int)*state_num);
	for (int i = 0; i < state_num; i++) {//将tag中的值初始化为0
		tag[i] = 0;
	}
	for (int i = 0; i < _msize(input_state_group) / sizeof(char*) && strcmp(input_state_group[i], "#") != 0; i++) {
		search(tf, tag, input_state_group[i], state_num, state, trans_func_num);//依次搜索输入态中的每一状态通过∑达到的下一个状态
	}
	int j = 0;
	for (int i = 0; i < state_num; i++) {//根据标记，记录closure集
		if (tag[i] == 1) {
			destination[j++] = state[i];
		}
	}
	strcpy(destination[j],"#"); //标记closure集结束
	return;
}
int main() {
	int state_num;//状态个数
	char **state;//状态集合
	int terminator_num;//终结符个数
	char **terminator;//终结符集合
	char *first_state;//开始状态
	char *last_state;//最终状态
	struct trans_func *tf;//存储状态转换函数
	char **input_state_group;//输入的状态子集
	char **destination;//得到的closure状态集
	state = (char**)malloc(sizeof(char*) * 30);//分配30个状态空间
	terminator = (char**)malloc(sizeof(char*) * 30);//分配30个终结符空间
	for (int i = 0; i < 30; i++) {
		state[i] = (char*)malloc(sizeof(char) * 10);//为每个状态分配10个空间记录状态字符
		terminator[i] = (char *)malloc(sizeof(char*) * 10);//为每个终结符设置10个存储空间
	}
	tf = (struct trans_func*)malloc(sizeof(struct trans_func) * 30);//开始为转换函数的个数分配20个空间
	first_state = (char*)malloc(sizeof(char) * 10);//分配30个终结符空间
	last_state = (char*)malloc(sizeof(char) * 10);//分配30个终结符空间
	int trans_func_num = initNFA_M(&state_num, state, &terminator_num, terminator, first_state, last_state, tf);
	input_state_group = (char**)malloc(sizeof(char*) * 30);//分配地址空间
	destination =(char**)malloc(sizeof(char*) * 30);//分配地址空间
	for (int i = 0; i < 30; i++) {
		input_state_group[i] = (char*)malloc(sizeof(char*) * 10);
		destination[i] = (char*)malloc(sizeof(char*) * 10);
	}
	input_I(input_state_group);
	get_closure(input_state_group, destination, state_num, state, tf, trans_func_num);
	cout<<"I状态集的∑-closure(I)为：";
	for (int i = 0; i < _msize(destination) / sizeof(char*) && strcmp(destination[i], "#") != 0; i++) {
		cout << destination[i]<<" ";
	}
	cout << endl;
	system(" pause");
}

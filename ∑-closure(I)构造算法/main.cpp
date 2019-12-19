#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
struct trans_func {//״̬ת�������Ľṹ
	char *current_state;//��ǰ״̬
	char *temp;//ת���ַ�
	char *last_state;//��һ״̬
};
void init_trans_func(struct trans_func *tf) {
	tf->current_state = (char*)malloc(sizeof(char) * 10);
	tf->temp = (char*)malloc(sizeof(char) * 10);
	tf->last_state = (char*)malloc(sizeof(char) * 10);
}
int initNFA_M(int *state_num, char **state, int *terminator_num, char **terminator, char *first_state, char *last_state, struct trans_func *tf) {
	FILE *fp;
	fp = fopen("NFA M.txt", "r");//�ļ���¼���ݵĸ�ʽ
	/*
	״̬����   ״̬1  ״̬2.....
	�ս������ �ս��1 �ս��2....  ���С�ʹ��null����
	��ʼ״̬
	����״̬
	״̬ת������1
	״̬ת������2
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
	int length = (int)_msize(tf) / sizeof(struct trans_func);//�õ�tf�Ŀռ��С
	while (!feof(fp))
	{
		init_trans_func(&tf[i]);//�����ַ
		fscanf(fp, "%s ", tf[i].current_state);//���뵱ǰ״̬
		fscanf(fp, "%s ", tf[i].temp);//����ת���ַ�
		fscanf(fp, "%s \n", tf[i].last_state);//������һ��״̬
		i++;
		if (i >= length) {//�жϿռ��Ƿ��㹻
			length += 10;
			tf = (struct trans_func*)realloc(tf, length);
		}
	}
	fclose(fp);
	return i;//����ת�������ĸ���
}
void input_I(char **input_state_group) {
	int i = 0;
	cout<<"��������Ҫ����Ӽ�IΪ(�Կո�ָ���#��β����";
	do {
		cin>>input_state_group[i];
		i++;
	} while (strcmp(input_state_group[i - 1], "#") != 0);//destination�д洢��#���״̬����
}
void search(struct trans_func *tf, int *tag, char *current_state, int state_num, char **state, int trans_func_num) {
	for (int i = 0; i < state_num; i++) {
		if (strcmp(current_state, state[i]) == 0) {
			if (tag[i] == 1)//�Ѿ���closure�У���Ҫ�ٴν��б���
				return;
			tag[i] = 1;//����closure����
			for (int j = 0; j < trans_func_num; j++) {
				if (strcmp(tf[j].current_state, current_state) == 0 && strcmp(tf[j].temp, "null") == 0) {
					search(tf, tag, tf[j].last_state, state_num, state, trans_func_num);//�ݹ����search������last_state��û��ͨ���ƴﵽ����״̬
				}
			}
			break;
		}
	}
}
void get_closure(char **input_state_group, char **destination, int state_num, char **state, struct trans_func *tf, int trans_func_num) {
	int *tag;//0Ϊû�б�ǣ�1Ϊ��ǳɹ�
	tag = (int *)malloc(sizeof(int)*state_num);
	for (int i = 0; i < state_num; i++) {//��tag�е�ֵ��ʼ��Ϊ0
		tag[i] = 0;
	}
	for (int i = 0; i < _msize(input_state_group) / sizeof(char*) && strcmp(input_state_group[i], "#") != 0; i++) {
		search(tf, tag, input_state_group[i], state_num, state, trans_func_num);//������������̬�е�ÿһ״̬ͨ���ƴﵽ����һ��״̬
	}
	int j = 0;
	for (int i = 0; i < state_num; i++) {//���ݱ�ǣ���¼closure��
		if (tag[i] == 1) {
			destination[j++] = state[i];
		}
	}
	strcpy(destination[j],"#"); //���closure������
	return;
}
int main() {
	int state_num;//״̬����
	char **state;//״̬����
	int terminator_num;//�ս������
	char **terminator;//�ս������
	char *first_state;//��ʼ״̬
	char *last_state;//����״̬
	struct trans_func *tf;//�洢״̬ת������
	char **input_state_group;//�����״̬�Ӽ�
	char **destination;//�õ���closure״̬��
	state = (char**)malloc(sizeof(char*) * 30);//����30��״̬�ռ�
	terminator = (char**)malloc(sizeof(char*) * 30);//����30���ս���ռ�
	for (int i = 0; i < 30; i++) {
		state[i] = (char*)malloc(sizeof(char) * 10);//Ϊÿ��״̬����10���ռ��¼״̬�ַ�
		terminator[i] = (char *)malloc(sizeof(char*) * 10);//Ϊÿ���ս������10���洢�ռ�
	}
	tf = (struct trans_func*)malloc(sizeof(struct trans_func) * 30);//��ʼΪת�������ĸ�������20���ռ�
	first_state = (char*)malloc(sizeof(char) * 10);//����30���ս���ռ�
	last_state = (char*)malloc(sizeof(char) * 10);//����30���ս���ռ�
	int trans_func_num = initNFA_M(&state_num, state, &terminator_num, terminator, first_state, last_state, tf);
	input_state_group = (char**)malloc(sizeof(char*) * 30);//�����ַ�ռ�
	destination =(char**)malloc(sizeof(char*) * 30);//�����ַ�ռ�
	for (int i = 0; i < 30; i++) {
		input_state_group[i] = (char*)malloc(sizeof(char*) * 10);
		destination[i] = (char*)malloc(sizeof(char*) * 10);
	}
	input_I(input_state_group);
	get_closure(input_state_group, destination, state_num, state, tf, trans_func_num);
	cout<<"I״̬���ġ�-closure(I)Ϊ��";
	for (int i = 0; i < _msize(destination) / sizeof(char*) && strcmp(destination[i], "#") != 0; i++) {
		cout << destination[i]<<" ";
	}
	cout << endl;
	system(" pause");
}

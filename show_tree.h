#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AvlTree.hpp"


void btree_visual_print(AvlTree<int> T, const char* elem_fmt, FILE* fp)
{
    if (T.root == NULL)
        return;
    const int _MAX_STR_LEN = 128;        //����Ԫ�ش�ӡ��str��󳤶�
    const int _MAX_NODE_NUM = 1024;      //���������
    const char horiz_conj_char = '_';    //���Ӻ���
    const char vert_conj_char = '|';     //����
    const char left_bracket_char = '(';  //ÿ��Ԫ����ߵ�С����, ��Ҳ���Ի�����ϲ����
    const char right_bracket_char = ')'; //Ԫ���ұߵ�С����

    //˼·: ������������, ��������������, �ýṹ���������Щ��Ϣ�����ͳһ��ӡ
    typedef struct _node_print_info
    {
        AvlNode<int>* address; //���α���ʱ��Ψһ���ֱ�־����key //��hash����ַӳ����ʡʱ��ռ�
        int str_len;
        int depth;
        int left_margin; // horizontal coordintate
    } _Info;

    AvlNode<int>* tree_deque[_MAX_NODE_NUM];
    int depth_queue[_MAX_NODE_NUM]; //��¼��ȵĶ���
    _Info* info_p_arr[_MAX_NODE_NUM];
    int node_count = 0;
    int front = -1, rear = -1;
    AvlNode<int>* p;
    _Info* info_p;
    char temp_str[_MAX_STR_LEN];

    //�ö���ͳ�ƽ��������Ϣ, ˳��ͳ��ÿ��������ݴ�ӡʱ�ĳ���
    tree_deque[++rear] = T.root;
    depth_queue[rear] = 1;
    while (front < rear)
    {
        p = tree_deque[++front];
        info_p = (_Info*)calloc(1, sizeof(_Info));
        info_p->address = p; //��¼��ַ
        memset(temp_str, 0, _MAX_STR_LEN);
        sprintf(temp_str, elem_fmt, p->data);   // TODO: ע��
        info_p->str_len = strlen(temp_str) + 2; //�����ӡ���Ԫ�س���
        info_p->depth = depth_queue[front];
        info_p_arr[node_count++] = info_p;

        if (p->left != NULL)
        {
            tree_deque[++rear] = p->left;
            depth_queue[rear] = depth_queue[front] + 1;
        }
        if (p->right != NULL)
        {
            tree_deque[++rear] = p->right;
            depth_queue[rear] = depth_queue[front] + 1;
        }
    }

    //������ͳ�ƺ�����, ���������
    p = T.root;
    int top = -1, i;
    int horizontal_accumu_cache = 0; //�������ۼƳ���
    do
    {
        while (p != NULL)
        {
            tree_deque[++top] = p;
            p = p->left;
        }
        p = tree_deque[top--];
        for (i = 0; i < node_count; ++i)
        { //������hash������ƾ����ֳ�����
            if (info_p_arr[i]->address == p)
            { //���������
                info_p_arr[i]->left_margin = horizontal_accumu_cache;
                horizontal_accumu_cache += info_p_arr[i]->str_len - 1; //��һ�����ص�һ������,������һ���
            }
        }
        p = p->right;
    } while (!(p == NULL && top == -1));

    //��������ʼ��ӡ
    int horiz_left_start, horiz_right_end, cursor, j, k, cur_depth = 1, end_flag = 0;
    int vert_index_arr[_MAX_NODE_NUM]; //ż���е����ߴ洢����
    i = 0;
    while (i < node_count)
    {
        k = -1;
        cursor = 0;
        while (info_p_arr[i]->depth == cur_depth)
        {
            //��ӡ���
            p = info_p_arr[i]->address;
            if (p->left != NULL)
            { //������˵���к���Ҫ��ӡ
                for (j = 0; j < node_count; ++j)
                { //��������
                    if (info_p_arr[j]->address == p->left)
                    {
                        horiz_left_start = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                        vert_index_arr[++k] = horiz_left_start;
                        break;
                    }
                }
                for (; cursor < horiz_left_start; ++cursor)
                    fprintf(fp, " ");
                for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                    fprintf(fp, "%c", horiz_conj_char);
            }
            else
            { //û������ȫ��ӡ�ո񼴿�
                for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                    fprintf(fp, " ");
            }

            //��ӡԪ��
            fprintf(fp, "%c", left_bracket_char);
            fprintf(fp, elem_fmt, p->data); //����Ҫע��TODO:
            fprintf(fp, "%c", right_bracket_char);
            cursor += info_p_arr[i]->str_len;

            //��ӡ�ұ�
            if (p->right != NULL)
            {
                for (j = 0; j < node_count; ++j)
                {
                    if (info_p_arr[j]->address == p->right)
                    {
                        horiz_right_end = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                        vert_index_arr[++k] = horiz_right_end;
                        break;
                    }
                }
                for (; cursor < horiz_right_end; ++cursor)
                    fprintf(fp, "%c", horiz_conj_char);
            } //�ұ�û��else ,��Ϊֻ���Ǻ��߼���, �հ��㵽ͬ����һ��Ԫ�����

            if (++i >= node_count)
            {
                end_flag = 1; //��ӡ�����һ��Ԫ��, ��һ�в���Ҫ������,��ǰ����
                break;
            }
        }
        fprintf(fp, "\n");

        //��ӡ����
        if (!end_flag)
        {
            cursor = 0;
            for (j = 0; j <= k; ++j)
            {
                for (; cursor < vert_index_arr[j]; ++cursor)
                    fprintf(fp, " ");
                fprintf(fp, "%c", vert_conj_char);
                cursor++;
            }
            fprintf(fp, "\n");
        }
        cur_depth++;
    }
    for (i = 0; i < node_count; ++i)
        free(info_p_arr[i]); //�ͷſռ�
    return;
}



void show_structure(AvlTree<int> tree) {
    FILE* fp = fopen("pic.txt", "w");
    btree_visual_print(tree, "%d", fp);
    fclose(fp);
}




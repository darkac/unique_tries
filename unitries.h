// Last modified: 2013-10-24 13:38:35
 
/**
 * @file: unitries.h
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-10-23 14:14:27
 * @brief: 
 **/
 
#ifndef _UNITRIES_H_
#define _UNITRIES_H_

#define MAX_FACTORS 100

extern uint32_t num_nodes;

enum status {POS = 0, NODE, NUL};

struct node;

union pointer {
	uint64_t pos;
	struct node* next;
};

struct node {
	union pointer slot[256];
	//enum status flag[256];
	uint8_t flag[256]; // 0: null; 1: pos; 2: node
	uint64_t first_appear;
};

struct factor {
	uint64_t pos[MAX_FACTORS];
	uint64_t len[MAX_FACTORS];
	uint32_t nfactors;
};

int32_t case_flag_0(struct node *tnode, char character, uint64_t position);

int32_t case_flag_1(struct node *tnode, char cur_lookat_char,
		uint64_t cur_pos, uint64_t cur_next_pos,
		char *buf, uint64_t buf_len);

int32_t case_flag_2(struct node *tnode, uint64_t cur_pos, uint64_t cur_next_pos,
		char *buf, uint64_t buf_len);

struct node *construct_ut(char *buf, uint64_t buf_len);

void destruct_ut(struct node *root);

struct factor *search_ut(char *buf, uint64_t buf_len,
		struct node *root, char *pattern, uint32_t plen);

void freenodes(struct node *layer);
 
#endif

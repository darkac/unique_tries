// Last modified: 2013-10-24 16:12:36
 
/**
 * @file: unitries.c
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-10-23 14:15:08
 * @brief: 
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "unitries.h"

uint32_t num_nodes = 0;

int32_t case_flag_0(struct node *tnode, char character, uint64_t position)
{
	tnode->slot[(uint8_t)character].pos = position;
	tnode->flag[(uint8_t)character] = 1;
	
	return 0;
}

int32_t case_flag_1(struct node *tnode, char cur_lookat_char,
		uint64_t cur_pos, uint64_t cur_next_pos,
		char *buf, uint64_t buf_len)
{
	uint64_t pre_pos = tnode->slot[(uint8_t)cur_lookat_char].pos;

	struct node *newnode = (struct node *)malloc(sizeof(struct node));
	num_nodes++;
	memset(newnode, 0, sizeof(struct node));
	newnode->first_appear = pre_pos;
	tnode->slot[(uint8_t)cur_lookat_char].next = newnode;
	tnode->flag[(uint8_t)cur_lookat_char] = 2;

	uint32_t exist_next_pos = pre_pos + cur_next_pos - cur_pos;
	char exist_next_char = *(buf + exist_next_pos);
	char cur_next_char = *(buf + cur_next_pos);

	while (cur_next_char == exist_next_char)
	{
		tnode = newnode;
		newnode = (struct node *)malloc(sizeof(struct node));
		num_nodes++;
		memset(newnode, 0, sizeof(struct node));
		newnode->first_appear = pre_pos;
		tnode->slot[(uint8_t)cur_next_char].next = newnode;
		tnode->flag[(uint8_t)cur_next_char] = 2;

		if (cur_next_pos == buf_len - 1)
			break;
		cur_next_char = *(buf + (++cur_next_pos));
		exist_next_char = *(buf + (++exist_next_pos));
	}
	// break from the `if (cur_next_pos == buf_len - 1)`
	//if (cur_next_pos == buf_len - 1 && cur_next_char == exist_next_char)
	if (cur_next_char == exist_next_char)
		return -1;

	case_flag_0(newnode, exist_next_char, pre_pos);
	case_flag_0(newnode, cur_next_char, cur_pos);
	
	return 0;
}

int32_t case_flag_2(struct node *tnode, uint64_t cur_pos, uint64_t cur_next_pos,
		char *buf, uint64_t buf_len)
{
	char cur_char = *(buf + cur_pos);
	tnode = tnode->slot[(uint8_t)cur_char].next;
	char cur_next_char = *(buf + cur_next_pos);
	while (tnode->flag[(uint8_t)cur_next_char] == 2)
	{
		tnode = tnode->slot[(uint8_t)cur_next_char].next;

		if (cur_next_pos == buf_len - 1)
			break;
		cur_next_char = *(buf + (++cur_next_pos));
	}
	//if (cur_next_pos == buf_len - 1 && tnode->flag[(uint8_t)cur_next_char] == 2)
	if (tnode->flag[(uint8_t)cur_next_char] == 2)
		return -1;

	int32_t ret = 0;
	if (tnode->flag[(uint8_t)cur_next_char] == 0)
	{
		ret = case_flag_0(tnode, cur_next_char, cur_pos);
	}
	else // flag == 1
	{
		cur_next_pos++;
		ret = case_flag_1(tnode, cur_next_char, cur_pos, cur_next_pos, buf, buf_len);
	}

	return ret;
}

struct node *construct_ut(char *buf, uint64_t buf_len)
{
	struct node *root = (struct node *)malloc(sizeof(struct node));
	memset(root, 0, sizeof(struct node));

	int32_t ret;
	char cur_char;
	uint64_t cur_pos = 0;
	
	while (cur_pos < buf_len)
	{
		struct node *tmp_node = root;
		cur_char = *(buf + cur_pos);
		if (tmp_node->flag[(uint8_t)cur_char] == 0)
		{
			case_flag_0(tmp_node, cur_char, cur_pos);
			cur_pos++;
		}
		else
		{
			uint64_t cur_next_pos = cur_pos + 1;
			if (cur_next_pos == buf_len)
				break;
			
			if (tmp_node->flag[(uint8_t)cur_char] == 1)
				ret = case_flag_1(tmp_node, cur_char, cur_pos, cur_next_pos, buf, buf_len);
			else // flag == 2
				ret = case_flag_2(tmp_node, cur_pos, cur_next_pos, buf, buf_len);
			
			if (ret == -1)
				break;
			cur_pos++;
		}
	}
	
	return root;
}

void freenodes(struct node *layer)
{
	uint32_t i = 0;
	uint32_t free_avail = 1;
	for (i = 0; i < 256; ++i)
	{
		if (layer->flag[i] == 2)
		{
			free_avail = 0;
			break;
		}
	}
	
	if (free_avail == 1)
	{
		free(layer);
		return ;
	}
	
	for (i = 0; i < 256; ++i)
	{
		if (layer->flag[i] == 2)
			freenodes(layer->slot[i].next);
	}
}

void destruct_ut(struct node *root)
{
	freenodes(root);
}

uint32_t match_forward(char *buf, uint64_t buf_len, uint64_t pos,
		char *pattern, uint32_t plen, uint32_t i)
{
	uint64_t inc_buf = pos;
	uint32_t inc_str = i;
	uint32_t match = 0;
	while (*(buf + inc_buf) == pattern[inc_str] &&
			inc_buf < buf_len && inc_str < plen)
	{
		inc_buf++;
		inc_str++;
		match++;
	}

	return match;
}
struct factor *search_ut(char *buf, uint64_t buf_len,
		struct node *root, char *pattern, uint32_t plen)
{
	struct factor *fac = (struct factor *)malloc(sizeof(struct factor));

	uint32_t i = 0, match = 0;
	uint64_t pos = 0, dep = 0;
	while (i < plen)
	{
		dep = 0;
		match = 0;
		if (root->flag[(uint8_t)pattern[i]] == 0)
		{
			match = 0;
		}
		else if (root->flag[(uint8_t)pattern[i]] == 1)
		{
			pos = root->slot[(uint8_t)pattern[i]].pos;
			match = match_forward(buf, buf_len, pos, pattern, plen, i);
		}
		else // flag == 2
		{
			struct node *tnode = root->slot[(uint8_t)pattern[i]].next;
			dep++;
			while (tnode->flag[(uint8_t)pattern[i + dep]] == 2)
			{
				tnode = tnode->slot[(uint8_t)pattern[i + dep]].next;
				dep++;
				if (i + dep == plen)
					break;
			}
			if (tnode->flag[(uint8_t)pattern[i + dep]] == 0)
			{
				pos = tnode->first_appear;
				match = dep;
			}
			else // flag == 1
			{
				pos = tnode->slot[(uint8_t)pattern[i + dep]].pos;
				match = match_forward(buf, buf_len, pos, pattern, plen, i);
			}
		}
		
		if (match == 0)
			pos = (uint32_t)pattern[i];
		//printf("%" PRId64 "\t%" PRId64 "\n", pos, match);

		fac->pos[fac->nfactors] = pos;
		fac->len[fac->nfactors] = match;
		fac->nfactors++;
		
		if (match == 0)
			i++;
		else
			i += match;
	}

	return fac;
}


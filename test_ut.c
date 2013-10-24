// Last modified: 2013-10-24 13:39:07
 
/**
 * @file: test_ut.c
 * @author: tongjiancong(lingfenghx@gmail.com)
 * @date:   2013-10-24 13:36:26
 * @brief: 
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "unitries.h"
 
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("%s [str] [pattern]\n", argv[0]);
		exit(-1);
	}

	//char buf[10] = "abacbabad";
	uint64_t buf_len = strlen(argv[1]);
	char *buf = (char *)malloc(buf_len);
	memset(buf, 0, buf_len);
	memcpy(buf, argv[1], buf_len);

	struct node *root;
	root = construct_ut(buf, buf_len); // consturct unique string tries

	//char pattern[10] = "ababac";
	uint32_t plen = strlen(argv[2]);
	char *pattern = (char *)malloc(plen);
	memset(pattern, 0, plen);
	memcpy(pattern, argv[2], plen);

	struct factor *fac;
	fac = search_ut(buf, buf_len, root, pattern, plen);
	printf("%d factors:\n", fac->nfactors);
	uint32_t i;
	for (i = 0; i < fac->nfactors; ++i)
		printf("factor [%d]: pos = %" PRId64 ", len = %" PRId64 "\n", i, fac->pos[i], fac->len[i]);
	free(fac);

	destruct_ut(root);
	free(buf);
	free(pattern);

	printf("num_node = %d\n", num_nodes);

    return 0;
}

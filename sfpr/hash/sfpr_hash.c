
#include <string.h>

#include "sfpr_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sfpr_hash_node_s{
	sfpr_int_t hash_value; ///< 哈希值
	sfpr_int_t slot;  ///< 所在槽
	sfpr_void_t *key; ///< 关键字
	sfpr_uint32_t key_size; ///< 关键字长度
	sfpr_void_t *value; ///< 哈希数据
	sfpr_int_t  group;///< 所属分组
	sfpr_uint32_t flags; ///< 标志
	struct sfpr_hash_node_s *next; ///< 下一个节点
};

struct sfpr_hash_s{
	sfpr_char_t name[HASH_MAX_NAME];///< 哈希表名称 
	sfpr_hash_node_t **nodes;///< 哈希表所有节点
	sfpr_uint32_t size;///< 哈希表大小
	sfpr_uint32_t count;///< 哈希表当前节点数
	sfpr_hashfunc_t hash_func;///< 哈希函数
	sfpr_cmpfunc_t cmp_func;///< 哈希比较函数
	sfpr_hashdelete_t del_func;///< 哈希删除函数
	void *userdata;
};

sfpr_hash_t *sfpr_hash_new()
{
	sfpr_hash_t *hash = (sfpr_hash_t*)malloc(sizeof(sfpr_hash_t));
	if(!hash){
		return NULL;
	}
	memset(hash,0,sizeof(sfpr_hash_t));

	return hash;
}

int sfpr_hash_destroy(sfpr_hash_t **hash)
{
	sfpr_hash_t *_hash = *hash;
	if(!hash || !_hash){
		return -1;
	}
	free(_hash);
	*hash = NULL;
	return 0;
}


int sfpr_hashfunc_default(const void *char_key,sfpr_uint32_t key_size)
{
	unsigned int hash = 0;
	const unsigned char *key = (const unsigned char *)char_key;
	const unsigned char *p;
	int i;
  
	/*
	* This is the popular `times 33' hash algorithm which is used by
	* perl and also appears in Berkeley DB. This is one of the best
	* known hash functions for strings because it is both computed
	* very fast and distributes very well.
	*
	* The originator may be Dan Bernstein but the code in Berkeley DB
	* cites Chris Torek as the source. The best citation I have found
	* is "Chris Torek, Hash function for text in C, Usenet message
	* <27038@mimsy.umd.edu> in comp.lang.c , October, 1990." in Rich
	* Salz's USENIX 1992 paper about INN which can be found at
	* <http://citeseer.nj.nec.com/salz92internetnews.html>.
	*
	* The magic of number 33, i.e. why it works better than many other
	* constants, prime or not, has never been adequately explained by
	* anyone. So I try an explanation: if one experimentally tests all
	* multipliers between 1 and 256 (as I did while writing a low-level
	* data structure library some time ago) one detects that even
	* numbers are not useable at all. The remaining 128 odd numbers
	* (except for the number 1) work more or less all equally well.
	* They all distribute in an acceptable way and this way fill a hash
	* table with an average percent of approx. 86%.
	*
	* If one compares the chi^2 values of the variants (see
	* Bob Jenkins ``Hashing Frequently Asked Questions'' at
	* http://burtleburtle.net/bob/hash/hashfaq.html for a description
	* of chi^2), the number 33 not even has the best value. But the
	* number 33 and a few other equally good numbers like 17, 31, 63,
	* 127 and 129 have nevertheless a great advantage to the remaining
	* numbers in the large set of possible multipliers: their multiply
	* operation can be replaced by a faster operation based on just one
	* shift plus either a single addition or subtraction operation. And
	* because a hash function has to both distribute good _and_ has to
	* be very fast to compute, those few numbers should be preferred.
	*
	*                  -- Ralf S. Engelschall <rse@engelschall.com>
	*/

	if (key_size == SPPR_HASH_KEY_STRING) {
		for (p = key; *p; p++) {
			hash = hash * 33 + *p;
		}
	}
	else {
		for (p = key, i = key_size; i; i--, p++) {
			hash = hash * 33 + *p;
		}
	}
	return hash;
}

int sfpr_hash_init(sfpr_hash_t *hash,char *name,int size,
				sfpr_hashfunc_t hash_func,sfpr_cmpfunc_t cmp_func,
				sfpr_hashdelete_t del_func)
{
	if(!hash){
		return -1;
	}
	#ifdef DEBUG
	fprintf(stderr,"hash_init() | hash name: %s  size: %d\n",name,size);
	#endif
	
	hash->count = 0;
	
	if(name){
		strncpy(hash->name,name,sizeof(hash->name));
	}
	if(size<=0){
		hash->size = HASH_DEFAULT_SIZE;
	}else{
		hash->size = size;
	}

	if(!hash_func){
		hash->hash_func = sfpr_hashfunc_default;		
	}else{
		hash->hash_func = hash_func;	
	}

	if(!cmp_func){
		hash->cmp_func = (sfpr_cmpfunc_t)memcmp;
	}else{
		hash->cmp_func = cmp_func;
	}
	hash->del_func = del_func;
	
	hash->nodes = (sfpr_hash_node_t**)malloc(hash->size * sizeof(sfpr_hash_node_t*));
	if(!hash->nodes){
		return -1;
	}
	memset(hash->nodes,0,hash->size * sizeof(sfpr_hash_node_t*));	
	return 0;
}

int sfpr_hash_finalize(sfpr_hash_t *hash)
{
	if(!hash)	return -1;
	free(hash->nodes);
	hash->nodes = NULL;
	return 0;
}


static sfpr_hash_node_t *sfpr_hash_new_node(
			int hash_value,int hash_slot,
			void *key,sfpr_uint32_t key_size,void *value,
			sfpr_int_t group,sfpr_uint32_t flags)
{
	sfpr_hash_node_t *node = NULL;	

	node = (sfpr_hash_node_t*)malloc(sizeof(sfpr_hash_node_t));
	if(!node){
		return NULL;
	}
	memset(node,0,sizeof(sfpr_hash_node_t));
	node->hash_value = hash_value;
	node->slot = hash_slot;
	node->key = key;
	node->key_size = key_size;
	node->value = value;
	node->group = group;
	node->flags = flags;
	node->next = NULL;

	return node;
}


sfpr_int_t sfpr_hash_insert(sfpr_hash_t *hash,void *key,sfpr_uint32_t key_size,
						void *value,sfpr_int_t group,sfpr_uint32_t flags)
{
	sfpr_hash_node_t *node = NULL,*prep = NULL,*new_node = NULL;
	int hash_value,hash_slot;
	
	if(!hash || !key || !value){
		return -1;
	}

	hash_value = hash->hash_func(key,key_size);
	hash_slot = hash_value % hash->size;

	#ifdef DEBUG
	fprintf(stderr,"hash_insert() | key: %s  key size:%d  hash: %d  slot: %d\n",
				(char*)key,key_size,hash_value,hash_slot);
	#endif

	if(!(hash->nodes[hash_slot])){
		new_node = sfpr_hash_new_node(hash_value,hash_slot,key,key_size,value,group,flags);
		if(!new_node)
			return -2;
		hash->nodes[hash_slot] = new_node;
		hash->count++;
		return 0;
	}
	
	node = hash->nodes[hash_slot];
	while(node){
/*
		#ifdef DEBUG
		printf("hash_insert() | node | key: %s  hash: %d key size:%d \n",
			(char*)node->key,node->hash_value,node->key_size);
		#endif
*/
		prep = node;
		if(node->hash_value == hash_value && 
			node->key_size == key_size &&
			!hash->cmp_func(node->key,key,key_size)){
			if(flags){
				if(hash->del_func){
					hash->del_func(node);
				}
				node->value = value;
				return 1;
			}else{
				return 0;
			}
		}
		node = node->next;
	}

	new_node = sfpr_hash_new_node(hash_value,hash_slot,key,key_size,value,group,flags);
	if(!new_node)
		return -1;
	prep->next = new_node;
	hash->count++;
	return 0;
}

void *sfpr_hash_find(sfpr_hash_t *hash,const void *key,sfpr_uint32_t key_size)
{
	sfpr_hash_node_t *node = NULL;
	int hash_value,hash_slot;
	
	hash_value = hash->hash_func(key,key_size);
	hash_slot = hash_value % hash->size;
	
	#ifdef DEBUG
	fprintf(stderr,"hash_find() | key: %s  hash: %d  slot: %d\n",
				(char*)key,hash_value,hash_slot);
	#endif
	
	node = hash->nodes[hash_slot];
	while(node){
		if(node->hash_value == hash_value && 
			node->key_size == key_size &&
			!hash->cmp_func(node->key,key,key_size)){
			return (void*)node->value;
		}
		node = node->next;
	}
	if(node){
		return (void*)node->value;
	}else{
		return NULL;
	}
}

void *sfpr_hash_group_find(sfpr_hash_t *hash,const void *key,sfpr_uint32_t key_size,sfpr_int_t group)
{
	sfpr_hash_node_t *node = NULL;
	int hash_value,hash_slot;
	
	hash_value = hash->hash_func(key,key_size);
	hash_slot = hash_value % hash->size;
	
	#ifdef DEBUG
	fprintf(stderr,"hash_find() | key: %s  hash: %d  slot: %d\n",
				(char*)key,hash_value,hash_slot);
	#endif
	
	node = hash->nodes[hash_slot];
	while(node){
		if(node->hash_value == hash_value && 
			node->key_size == key_size &&
			!hash->cmp_func(node->key,key,key_size)){
			if(node->group == group){
				return (void*)node->value;
			}else{
				return NULL;
			}
		}
		node = node->next;
	}
	if(node){
		if(node->group == group){
			return (void*)node->value;
		}else{
			return NULL;
		}
	}else{
		return NULL;
	}
}

static void hash_delete_cb_default(sfpr_hash_node_t *node)
{
	if(node && node->value)
		free((void*)node->value);
}

int sfpr_hash_delete(sfpr_hash_t *hash,const void *key,sfpr_uint32_t key_size,
					sfpr_hashdelete_t delete_cb)
{
	sfpr_hash_node_t *node = NULL,*prep = NULL;
	int hash_value,hash_slot;
	
	hash_value = hash->hash_func(key,key_size);
	hash_slot = hash_value % hash->size;
	
	#ifdef DEBUG
	fprintf(stderr,"hash_delete() key: %s  hash: %d  slot: %d\n",
				(char*)key,hash_value,hash_slot);
	#endif
	
	if(!(hash->nodes[hash_slot])){
		return -1;
	}
	if(!delete_cb){
		delete_cb = hash->del_func;
	}
	if(!delete_cb){
		delete_cb = hash_delete_cb_default;
	}

	node = hash->nodes[hash_slot];
	if(node->hash_value == hash_value && 
		node->key_size == key_size &&
		!hash->cmp_func(node->key,key,key_size)){
		hash->nodes[hash_slot] = node->next;
		delete_cb(node);
		free(node);
		return 0;
	}
	
	prep = node;
	node = node->next;
	while(node){
/*		
		#ifdef DEBUG
		printf("hash_delete() node: key: %s  hash: %d\n",
			(char*)node->key,node->hash_value);
		#endif
*/
		if(node->hash_value == hash_value && 
			node->key_size == key_size &&
			!hash->cmp_func(node->key,key,key_size)){
			prep->next = node->next;
			delete_cb(node);
			free(node);
			return 0;
		}
		prep = node;
		node = node->next;
	}
	return -1;
}

int sfpr_hash_clear(sfpr_hash_t *hash,sfpr_hashdelete_t delete_cb)
{
	int slot=0;
	sfpr_hash_node_t *node = NULL,*next = NULL;

	if(!delete_cb){
		delete_cb = hash->del_func;
	}
	if(!delete_cb){
		delete_cb = hash_delete_cb_default;
	}
	#ifdef DEBUG
	fprintf(stderr,"hash_clear() hash name: %s\n",hash->name);
	#endif
	
	for(slot=0;slot<hash->size;slot++){
		node = hash->nodes[slot];
		hash->nodes[slot] = NULL;
		while(node)
		{
			next = node->next;
			delete_cb(node);
			free(node);
			node = next;			
		}
	}
	return 0;
}


static void hash_enum_cb_default(sfpr_hash_node_t *node,void *param)
{
	printf("hash_enum_cb(): slot:%-4d hash:%-13d key:%s\n",
				node->slot,node->hash_value,node->key);
}


sfpr_int_t sfpr_hash_enum(sfpr_hash_t *hash,sfpr_hashenum_t enum_cb,void *param)
{
	int slot=0;
	sfpr_hash_node_t *node = NULL;
	
	if(!enum_cb){
		enum_cb = hash_enum_cb_default;
	}

	for(slot=0;slot<hash->size;slot++){
		node = hash->nodes[slot];
		while(node){
			enum_cb(node,param);
			node = node->next;
		}
	}
	return 0;
}

sfpr_int_t sfpr_hash_group_enum(sfpr_hash_t *hash,sfpr_int_t group,sfpr_hashenum_t enum_cb,void *param)
{
	int slot=0;
	sfpr_hash_node_t *node = NULL;
	
	if(!enum_cb){
		enum_cb = hash_enum_cb_default;
	}

	for(slot=0;slot<hash->size;slot++){
		node = hash->nodes[slot];
		while(node){
			if(node->group == group){
				enum_cb(node,param);
			}
			node = node->next;
		}
	}
	return 0;
}

int sfpr_hash_count(sfpr_hash_t *hash)
{
	return hash->count;
}

int sfpr_hash_can_use(sfpr_hash_t *hash)
{
	if(!hash)return -1;
	if(hash->nodes)return 1;
	return -1;
}



sfpr_int_t sfpr_hash_set_userdata(sfpr_hash_t *hash,sfpr_void_t *userdata)
{
	if(!hash)return -1;
	hash->userdata = userdata;
	return 0;
}

sfpr_void_t *sfpr_hash_get_userdata(sfpr_hash_t *hash)
{
	if(!hash)return NULL;
	return hash->userdata;
}

const sfpr_char_t *sfpr_hash_get_name(sfpr_hash_t *hash)
{
	if(!hash)return "";
	return hash->name;
}

const sfpr_int_t sfpr_hash_get_size(sfpr_hash_t *hash)
{
	if(!hash)return 0;
	return hash->size;	
}

sfpr_void_t *sfpr_hash_node_get_key(sfpr_hash_node_t *node)
{
	if(!node)return NULL;
	return node->key;
}

sfpr_int_t sfpr_hash_node_get_key_size(sfpr_hash_node_t *node)
{
	if(!node)return 0;
	return node->key_size;	
}

sfpr_void_t *sfpr_hash_node_get_value(sfpr_hash_node_t *node)
{
	if(!node)return NULL;
	return node->value;	
}

int sfpr_hash_node_set_value(sfpr_hash_node_t *node,sfpr_void_t *new_value)
{
	if(!node)return -1;
	node->value = new_value;
	return 0;
}

sfpr_void_t *sfpr_hash_node_get_value_addr(sfpr_hash_node_t *node)
{
	if(!node)return NULL;
	return &(node->value);	
}

sfpr_int_t sfpr_hash_node_get_slot(sfpr_hash_node_t *node)
{
	if(!node)return -1;
	return node->slot;
}

sfpr_int_t sfpr_hash_node_get_hashvalue(sfpr_hash_node_t *node)
{
	if(!node)return -1;
	return node->hash_value;
}

sfpr_int_t sfpr_hash_node_get_groupid(sfpr_hash_node_t *node)
{
	if(!node)return -1;
	return node->group;	
}

sfpr_int_t sfpr_hash_get_node_size()
{
	return sizeof(sfpr_hash_node_t);
}

#ifdef __cplusplus
}
#endif

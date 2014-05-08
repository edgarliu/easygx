
#ifndef SFEL_PORTABLE_H
#define SFEL_PORTABLE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "mxml.h"

typedef mxml_node_t sfel_document_t;
typedef mxml_node_t sfel_node_t;
typedef mxml_attr_t sfel_attr_t;

typedef mxml_type_t sfel_type_t;


typedef struct sfel_portable_s{
	mxml_type_t (*cb)(mxml_node_t *);	
	void *reserve;
}sfel_portable_t;


typedef mxml_sax_cb_t sfel_sax_cb_t;



#ifdef __cplusplus
}
#endif

#endif

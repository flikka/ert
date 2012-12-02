/*
   Copyright (C) 2012  Statoil ASA, Norway. 
    
   The file 'config_content_node.c' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/

#include <stdbool.h>

#include <type_macros.h>
#include <util.h>
#include <stringlist.h>

#include <config_schema_item.h>
#include <config_content_node.h>



#define CONFIG_CONTENT_NODE_ID 6752887
struct config_content_node_struct {
  UTIL_TYPE_ID_DECLARATION;
  const config_schema_item_type * schema;         
  stringlist_type               * stringlist;              /* The values which have been set. */
  char                          * config_cwd;              /* The currently active cwd (relative or absolute) of the parser when this item is set. */
};


static UTIL_SAFE_CAST_FUNCTION( config_content_node , CONFIG_CONTENT_NODE_ID )

config_content_node_type * config_content_node_alloc( const config_schema_item_type * schema ) {
  config_content_node_type * node = util_malloc(sizeof * node );
  UTIL_TYPE_ID_INIT( node , CONFIG_CONTENT_NODE_ID );
  node->stringlist = stringlist_alloc_new();
  node->config_cwd = NULL;
  node->schema = schema;
  return node;
}


void config_content_node_set(config_content_node_type * node , const stringlist_type * token_list) {
  int argc = stringlist_get_size( token_list ) - 1;
  for (int iarg=0; iarg < argc; iarg++) 
    stringlist_append_copy( node->stringlist , stringlist_iget( token_list , iarg + 1));
}



char * config_content_node_alloc_joined_string(const config_content_node_type * node, const char * sep) {
  return stringlist_alloc_joined_string(node->stringlist , sep);
}



void config_content_node_free(config_content_node_type * node) {
  stringlist_free(node->stringlist);
  util_safe_free(node->config_cwd);
  free(node);
}



void config_content_node_free__(void * arg) {
  config_content_node_type * node = config_content_node_safe_cast( arg );
  config_content_node_free( node );
}



const char * config_content_node_iget(const config_content_node_type * node , int index) {
  return stringlist_iget( node->stringlist , index );
}


const char * config_content_node_safe_iget(const config_content_node_type * node , int index) {
  if (index >= stringlist_get_size( node->stringlist ))
    return NULL;
  else
    return stringlist_iget( node->stringlist , index );
}


bool config_content_node_iget_as_bool(const config_content_node_type * node , int index) {
  bool value;
  config_schema_item_assure_type(node->schema , index , CONFIG_BOOLEAN);
  util_sscanf_bool( config_content_node_iget(node , index) , &value );
  return value;
}


int config_content_node_iget_as_int(const config_content_node_type * node , int index) {
  int value;
  config_schema_item_assure_type(node->schema , index , CONFIG_INT);
  util_sscanf_int( config_content_node_iget(node , index) , &value );
  return value;
}



double config_content_node_iget_as_double(const config_content_node_type * node , double index) {
  double value;
  config_schema_item_assure_type(node->schema , index , CONFIG_FLOAT);
  util_sscanf_double( config_content_node_iget(node , index) , &value );
  return value;
}


const stringlist_type * config_content_node_get_stringlist( const config_content_node_type * node ) {
  return node->stringlist;
}


const char * config_content_node_get_kw( const config_content_node_type * node ) {
  return config_schema_item_get_kw( node->schema );
}


void config_content_node_set_cwd( config_content_node_type * node , const char * config_cwd ) {        
  if (config_cwd != NULL)
    node->config_cwd = util_alloc_string_copy( config_cwd );
  else
    node->config_cwd = util_alloc_cwd(  );  /* For use from external scope. */
}


int config_content_node_get_size( const config_content_node_type * node ) {
  return stringlist_get_size( node->stringlist );
}
/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zh2py.h"
#include "php_zh2py.h"

/* If you declare any globals in php_zh2py.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zh2py)
*/

/* True global resources - no need for thread safety here */
static int le_zh2py;

/* {{{ zh2py_functions[]
 *
 * Every user visible function must have an entry in zh2py_functions[].
 */
const zend_function_entry zh2py_functions[] = {
	PHP_FE(confirm_zh2py_compiled,	NULL)		/* For testing, remove later. */
    PHP_FE(zh2py_table_loaded,NULL)
    PHP_FE(zh2py_unload_table,NULL)
    PHP_FE(zh2py_load_table,NULL)
    PHP_FE(zh2py_get_pinyin,NULL)
    PHP_FE(zh2py_free_last_result,NULL)
	PHP_FE_END	/* Must be the last line in zh2py_functions[] */
};
/* }}} */

/* {{{ zh2py_module_entry
 */
zend_module_entry zh2py_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"zh2py",
	zh2py_functions,
	PHP_MINIT(zh2py),
	PHP_MSHUTDOWN(zh2py),
	PHP_RINIT(zh2py),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zh2py),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zh2py),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZH2PY
ZEND_GET_MODULE(zh2py)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("zh2py.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_zh2py_globals, zh2py_globals)
    STD_PHP_INI_ENTRY("zh2py.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_zh2py_globals, zh2py_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_zh2py_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_zh2py_init_globals(zend_zh2py_globals *zh2py_globals)
{
	zh2py_globals->global_value = 0;
	zh2py_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zh2py)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zh2py)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zh2py)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zh2py)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zh2py)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zh2py support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_zh2py_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_zh2py_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "zh2py", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
PHP_FUNCTION(zh2py_table_loaded)
{
    if(zh2py_res_table_root==NULL){
        RETURN_BOOL(false);
    }else{
        RETURN_BOOL(true);
    }
}
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/
/* {{{ zh2py_unload_table */
PHP_FUNCTION(zh2py_unload_table){
    if(zh2py_res_table_root!=NULL){
        if(zh2py_free_table(zh2py_res_table_root)!=0){
            RETURN_BOOL(false);
        }
        zh2py_res_table_root = NULL;
    }
    RETURN_BOOL(true);
}
/* }}} */
/** {{{ zh2py_load_table */
PHP_FUNCTION(zh2py_load_table){
	char *arg = NULL;
	int arg_len, len;
	char *strg;
    zh2py_table_root * root;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
    root = zh2py_read_transform_table_file(arg);
    if(root==NULL){
        RETURN_BOOL(false);
    }else{
        zh2py_res_table_root = root;
        RETURN_BOOL(true);
    }
}
/** }}} */
/** {{{ zh2py_get_pinyin */
PHP_FUNCTION(zh2py_get_pinyin){
	char *arg = NULL;
	int arg_len, len;
	char *result;
    zh2py_table_root * root;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
    if(zh2py_last_result != NULL){
        free(zh2py_last_result);
    }
    if(zh2py_res_table_root == NULL){
        return ;
    }
    zh2py_last_result = zh2py_transform(zh2py_res_table_root,arg,&len);
    RETURN_STRINGL(zh2py_last_result,len,1);
}
/**  }}} */
PHP_FUNCTION(zh2py_free_last_result){
    if(zh2py_last_result!=NULL){
        free(zh2py_last_result);
        zh2py_last_result = NULL;
        RETURN_BOOL(true);
    }
    RETURN_BOOL(false);
}
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

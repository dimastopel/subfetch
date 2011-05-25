#include "common.h"
#include "utils.h"

void PrintXmlRpcStruct(xmlrpc_env * env, xmlrpc_value * val)
{
    printf("----- Printing XMLRPC struct -----");
    int size = xmlrpc_struct_size(env, val);
    printf("struct size is: [%d]\n",size);
    int counter;
    for (counter = 0; counter < size; counter++)
    {
	
        xmlrpc_value * keyval;
        xmlrpc_value * valval;
	xmlrpc_struct_read_member(env,
                          val,
                          counter,
                          &keyval,
                          &valval);

        const char* string_key;
	const char* string_val;
        xmlrpc_read_string(env, keyval, &string_key);
        xmlrpc_read_string(env, valval, &string_val);
        printf("key [%s] value [%s]\n", string_key, string_val);
    }
}



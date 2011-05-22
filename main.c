/* An XML-RPC client program written in C, as an example of using
   compound XML-RPC values.

   For a simple client program that just deals with integer values,
   see xmlrpc_sample_add_client.c.  This example focuses just on the
   compound XML-RPC values and not the client functions.

   This client invokes the example.divide XML-RPC method that the example
   server program compound_value_server.c provides.  That method takes a
   list of pairs of numbers and returns the list of their quotients.

   Compound XML-RPC values are arrays and structures.  We call them compound
   because they are made up of other XML-RPC values (e.g. an array of XML-RPC
   integers).

   The arguments to the example.divide method are specified as follows:

   There are two arguments:

     Argument 0: Integer.  Version number of this argument protocol.  Must
                 be 1.


     Argument 1: Array.  One element for each pair of numbers you want the
                 server to divide.  Each element is structure, with these
                 members:

                 KEY: "dividend"
                 VALUE: floating point number.  The dividend.

                 KEY: "divisor"
                 VALUE: floating point number.  The divisor.

   The result of the method is an array.  It has one member for each pair of
   numbers in the arguments (So it is the same size as Argument 1).  That
   member is a floating point number.  It is the quotient of the numbers
   in the corresponding element of Argument 1.

   The client sends the RPC to the server running on the local system
   ("localhost"), HTTP Port 8080.
*/

#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

//#include "config.h"  /* information about this build environment */

#define NAME "subfetch"
#define VERSION "0.1"

static void 
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}



struct ratio {
    double dividend;
    double divisor;
};



int 
main(int           const argc, 
     const char ** const argv) {

    const char * const serverUrl = "http://api.opensubtitles.org/xml-rpc";
    const char * const methodName = "LogIn";
    unsigned int const argVersion = 1;
    struct ratio const data[] = {{1,2},{12,3},{10,3},{89,3000}};
    xmlrpc_env env;
    xmlrpc_value * resultP;
    unsigned int i;
    xmlrpc_value * ratioArrayP;
    unsigned int quotientCt;

    if (argc-1 > 0) {
        fprintf(stderr, "This program has no arguments\n");
        exit(1);
    }

    printf("Start!\n");
    xmlrpc_env_init(&env);
    printf("Init successful\n");

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);
    printf("Init2 successful\n");

    xmlrpc_value* s1 = xmlrpc_string_new(&env, "");
    xmlrpc_value* s2 = xmlrpc_string_new(&env, "");
    xmlrpc_value* s3 = xmlrpc_string_new(&env, "eng");
    xmlrpc_value* s4 = xmlrpc_string_new(&env, "oscar v2.13");
    printf("Initialized variables\n");
    
    /* Make the call */

    printf("Making the call\n");
    resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(ssss)",
                                 "","","eng","oscar v2.13");
    dieIfFaultOccurred(&env);
    printf("after call\n");
    
       
    /* Print out the quotients returned */

    printf("the type is: %d", xmlrpc_value_type(resultP));
    
    
    xmlrpc_value * token;
    xmlrpc_value * status;
    xmlrpc_value * seconds;

    xmlrpc_struct_find_value(&env, resultP, "token", &token);
    xmlrpc_struct_find_value(&env, resultP, "status", &status);
    xmlrpc_struct_find_value(&env, resultP, "seconds", &seconds);

    if (token) {
        const char* token_val;
        xmlrpc_read_string(&env, token, &token_val);
        printf("token is %s\n", token_val);
    } 
    else
    {
        printf("There is no member named token\n");
    }

    if (status) {
        const char* status_val;
	size_t str_size;
        xmlrpc_read_string_lp(&env, status, &str_size, &status_val);
        printf("status [%d] is %s\n", str_size, status_val);
    } 
    else
    {
        printf("There is no member named status\n"); 
    }
    if (seconds) {
        xmlrpc_double seconds_val;
        xmlrpc_read_double(&env, seconds, &seconds_val);
        printf("seconds is %f\n", seconds_val);
        //xmlrpc_DECREF(&seconds_val);
    } 
    else
    {
        printf("There is no member named seconds\n");
    }
    
    int size = xmlrpc_struct_size(&env, resultP);
    printf("struct size is: [%d]\n",size);
    int counter;
    for (counter = 0; counter < size; counter++)
    {
	
        xmlrpc_value * keyval;
        xmlrpc_value * valval;
	xmlrpc_struct_read_member(&env,
                          resultP,
                          counter,
                          &keyval,
                          &valval);

        const char* string_val;
        xmlrpc_read_string(&env, keyval, &string_val);
        printf("key [%d] is %s\n", counter,string_val);
	

    }
    
	xmlrpc_DECREF(resultP);

    
    
    /*
    quotientCt = xmlrpc_array_size(&env, resultP);
    dieIfFaultOccurred(&env);
    printf("array size: %d", quotientCt);

    for (i = 0; i < quotientCt; ++i) {
        xmlrpc_value * quotientP;
        xmlrpc_double quotient;

        xmlrpc_array_read_item(&env, resultP, i, &quotientP);
        dieIfFaultOccurred(&env);

        xmlrpc_read_double(&env, quotientP, &quotient);
        dieIfFaultOccurred(&env);

        printf("Server says quotient %u is %f\n", i, quotient);

        xmlrpc_DECREF(quotientP);
    }

    xmlrpc_DECREF(resultP);
	*/
    xmlrpc_env_clean(&env);
    
    xmlrpc_client_cleanup();

    return 0;
}


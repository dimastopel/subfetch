#include "common.h"

#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "utils.h"

//#include "config.h"  /* information about this build environment */

#define PROG_NAME "subfetch"
#define PROG_VERSION "0.1"

static void 
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

int 
main(int           const argc, 
     const char ** const argv) {

    const char * const serverUrl = "http://api.opensubtitles.org/xml-rpc";
    const char * const methodName = "LogIn";
    const char * const defLang = "eng";
    unsigned int const argVersion = 1;
    xmlrpc_env env;
    xmlrpc_value * resultP, * searchSubsReq, *resultP2;
    unsigned int i;
    xmlrpc_value * ratioArrayP;
    unsigned int quotientCt;
    char * token = NULL;


    if (argc-1 > 0) {
        fprintf(stderr, "This program has no arguments\n");
        exit(1);
    }

    printf("Start!\n");
    xmlrpc_env_init(&env);
    printf("Init successful\n");

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, PROG_NAME, PROG_VERSION, NULL, 0);
    dieIfFaultOccurred(&env);
    printf("Init2 successful\n");

    /*
    xmlrpc_value* s1 = xmlrpc_string_new(&env, "");
    xmlrpc_value* s2 = xmlrpc_string_new(&env, "");
    xmlrpc_value* s3 = xmlrpc_string_new(&env, "eng");
    xmlrpc_value* s4 = xmlrpc_string_new(&env, "oscar v2.13");
    */
    
    //
    // Get the token - LogIn
    //

    printf("Making the call\n");
    //resultP = xmlrpc_client_call(&env, serverUrl, methodName, "(ssss)",
    //                             "","","eng","oscar v2.13");
    resultP = xmlrpc_client_call(&env, 
				 serverUrl, 
				 methodName, 
				 "(ssss)",
                                 "",
				 "",
				 defLang,
				 PROG_NAME " " PROG_VERSION);
    dieIfFaultOccurred(&env);
    printf("after call\n");
    
       
    /* Print out the quotients returned */

    printf("the type is: %d\n", xmlrpc_value_type(resultP));
    
    
    xmlrpc_value * token1;
    xmlrpc_value * status;
    xmlrpc_value * seconds;

    xmlrpc_struct_find_value(&env, resultP, "token", &token1);
    xmlrpc_struct_find_value(&env, resultP, "status", &status);
    xmlrpc_struct_find_value(&env, resultP, "seconds", &seconds);



    if (token1) {
        const char* token_val;
        xmlrpc_read_string(&env, token1, &token_val);
        printf("token is %s\n", token_val);
	token = token_val;
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
    
    
    xmlrpc_DECREF(resultP);

    //
    // SErach for subtitles
    //

    //Create struct
    

    //Create request
    searchSubsReq = xmlrpc_build_value(&env, 
                                   "({s:s,s:s,s:i})", 
				   "sublanguageid", "eng",
                                   "moviehash", "68a9ccba52f5b1bf",
                                   "moviebytesize", 585347231);





    resultP2 = xmlrpc_client_call(&env, 
				 serverUrl, 
				 "SearchSubtitles", 
				 "(sA)",
				  token,
				 searchSubsReq);

    dieIfFaultOccurred(&env);
    printf("after call\n");
    
    printf("the type is: %d\n", xmlrpc_value_type(resultP2));
    
    
    const char * status_key, * status_val, * data_key;
    xmlrpc_value * dataArray;
    
    xmlrpc_decompose_value(&env, resultP2, "{s:s,s:A}",
                           &status_key,
                           &status_val,
                           &data_key,
			   &dataArray);

    printf("status: key[%s] val[%s]", status_key, status_val);
    printf("data: key[%s]", data_key);
    
    unsigned int dataSize = xmlrpc_array_size(&env, dataArray);
    for (i = 0; i < dataSize; i++)
      {
	xmlrpc_value * val = NULL;
	xmlrpc_array_read_item(&env, dataArray, i, &val);
	PrintXmlRpcStruct(&env, val);
      }

    xmlrpc_env_clean(&env);
    
    xmlrpc_client_cleanup();

    return 0;
}


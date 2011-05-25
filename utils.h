#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>


void PrintXmlRpcStruct(xmlrpc_env * env, xmlrpc_value * val);

#endif

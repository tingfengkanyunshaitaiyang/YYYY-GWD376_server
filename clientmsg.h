/*
 * clientmsg.h
 *
 *  Created on: Oct 4, 2016
 *      Author: lang
 */

//CLIENTMSG between server and client
#ifndef _clientmsg
#define _clientmsg

//USER MSG EXIT for OP of CLIENTMSG
#define EXIT -1
#define USER 1
#define MSG 2
#define OK 3

#ifndef CMSGLEN
#define CMSGLEN 100
#endif

struct CLIENTMSG
{
    int OP;
    char username[20];
    char buf[CMSGLEN];
};
struct ARG
{
    int sockfd;
    struct CLIENTMSG clientMsg;
};

#endif
///////////////////////////////////end of file//////////////////////////////////

#ifndef __HTTP_REQUEST__H__
#define __HTTP_REQUEST__H__

#include "http/header.h"
#include "http/body.h"

struct HttpRequest{
    const Header header;
    const Body body;
};

#endif  //!__HTTP_REQUEST__H__

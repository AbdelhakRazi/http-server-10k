#ifndef __HTTP_REQUEST__H__
#define __HTTP_REQUEST__H__

#include "../header/header.h"
#include "../body/body.h"

struct HttpRequest{
    const Header header;
    const Body body;
};

#endif  //!__HTTP_REQUEST__H__

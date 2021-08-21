//
// Created by filip on 19. 08. 2021..
//

#ifndef EDITOR_SRC_ERROR_H
#define EDITOR_SRC_ERROR_H

#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

namespace Error
{
	void die(const char* s);
}

#endif //EDITOR_SRC_ERROR_H

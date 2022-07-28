#include "struct.h"
#include "libft.h"
#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

# define ERROR -1

typedef enum e_type
{
	NO = 0,
	SO,
	WE,
	EA,
	F,
	C
} t_type;

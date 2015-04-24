#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../common/protocol.h"
#include "../common/utils.h"

int
main(int argc, char **argv)
{
    explore_dir_rec(".");
    return 0;
}

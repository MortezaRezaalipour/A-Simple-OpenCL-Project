__kernel void hello(__global char* string)
{
   int gid =  get_global_id(0);
   string[gid] = 'H';
   string[gid + 1] = 'e';
   string[gid + 2] = 'l';
   string[gid + 3] = 'l';
   string[gid + 4] = 'o';
   string[gid + 5] = ',';
   string[gid + 6] = ' ';
   string[gid + 7] = 'W';
   string[gid + 8] = 'o';
   string[gid + 9] = 'r';
   string[gid + 10] = 'l';
   string[gid + 11] = 'd';
   string[gid + 12] = '!';
   string[gid + 13] = '\n';
}

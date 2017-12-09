extern int *BMODINITEDGET(void);
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
#pragma inline
#pragma argsused
void rkr(char far *name, unsigned long code)
{
    int *p = BMODINITEDGET();

    *p = !(*p);
}
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

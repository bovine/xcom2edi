extern int *BMODINITEDGET(void);
//����������������������������������������������������������������������������
#pragma inline
#pragma argsused
void rkr(char far *name, unsigned long code)
{
    int *p = BMODINITEDGET();

    *p = !(*p);
}
//����������������������������������������������������������������������������

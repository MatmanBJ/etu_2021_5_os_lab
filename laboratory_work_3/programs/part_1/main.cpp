#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

struct Params1
{
    HANDLE h;
    long double localSUM;
    long double *globalSUM;
    std::size_t begin;
    std::size_t end;
};

size_t BS;
size_t NN;

HANDLE hMutexSELECT;
HANDLE hMutexSUM;

size_t pi_blocks_i;
size_t pi_blocks_n;

DWORD WINAPI piCalc1(LPVOID lpParam)
{
    Params1 *par = (Params1*)lpParam;
    int isuicide;
    do
    {
        //==========GetIterBlock==========BEGIN
        /*
        либо мьютексы (https://eax.me/winapi-threads/):
        CreateMutex
        WaitForSingleObject с INFINITE и ReleaseMutex
        CloseHandle

        либо эвенты (https://docs.microsoft.com/ru-ru/windows/win32/sync/using-event-objects)
        CreateEvent( NULL, TRUE, FALSE, TEXT("WriteEvent"));
        SetEvent(ghWriteEvent) и WaitForSingleObject
        CloseHandle
        */
        DWORD waitError;

        //=====critical SELECT block=====BEGIN
        waitError = WaitForSingleObject(hMutexSELECT, INFINITE);
        if(waitError != WAIT_OBJECT_0 /*Или WAIT_TIMEOUT, если отвалились по таймеру*/)
        {
            cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
        }



        if(pi_blocks_i < pi_blocks_n)
        {
            (*par).begin = pi_blocks_i * BS;
            (*par).end = (pi_blocks_i+1) * BS;
            if((*par).end > NN-1)
            {
                (*par).end = NN-1;
            }
            ++pi_blocks_i;
        }
        else
        {
            (*par).begin = 10;
            (*par).end = 1;
        }



        ReleaseMutex(hMutexSELECT);
        //=====critical SELECT block=====END



        //==========GetIterBlock==========END


        if((*par).begin <= (*par).end)
        {
            //pi!!!!!!!!!!!!!!!!!!!!!!
            long double xi;
            (*par).localSUM = 0;
            for(size_t i = (*par).begin; i <= (*par).end; ++i)
            {
                xi = ((long double)i + 0.5); xi /= (long double)NN;
                (*par).localSUM += (4 / (1 + xi*xi));
            }
            
            //=====critical SUM block=====BEGIN
            waitError = WaitForSingleObject(hMutexSUM, INFINITE);
            if(waitError != WAIT_OBJECT_0 /*Или WAIT_TIMEOUT, если отвалились по таймеру*/)
            {
                cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
            }
            
            *((*par).globalSUM) += (*par).localSUM;

            ReleaseMutex(hMutexSUM);
            //=====critical SUM block=====END

            //Sleep(1);
        }
        else
        {
            isuicide = -1;
        }
        //cout << "th(" << isuicide << ")" << (size_t)((*par).h) << ": b=" << (*par).begin << ", e=" << (*par).end << ", bl_i=" << (*par).pi_block << ", bl=" << ((*par).pi_block==NULL?-1:*(*par).pi_block) << endl;
    }
    while(isuicide != -1);
    
    return 0;
}



//DO=SHAKAL= 3.14159265358980165799319961283941893270821310579776763916015625
//nOCJlE=ETO=3.1415929662565249204549122641338954053935594856739044189453125
//real80    =3.1415926535897932384626433832795028841971693993751058209749445923078164062862089



long double processPI1(const size_t N, const unsigned threadNum, const size_t blocksize, DWORD *milisec)
{
    NN = N;
    BS = blocksize;

    long double respi = 0.0;
    pi_blocks_i = 0;
    pi_blocks_n = NN % blocksize == 0?NN/blocksize:NN/blocksize + 1;


    hMutexSELECT = CreateMutex(NULL, FALSE, NULL);
    if(hMutexSELECT == NULL)
    {
        cout << "Problem with creating hMutexSELECT. Error: " << GetLastError() << endl;
        return -1;
    }

    hMutexSUM = CreateMutex(NULL, FALSE, NULL);
    if(hMutexSUM == NULL)
    {
        cout << "Problem with creating hMutexSUM. Error: " << GetLastError() << endl;
        CloseHandle(hMutexSELECT);
        return -1;
    }

    Params1 *params = new Params1[threadNum];
    HANDLE *ths = new HANDLE[threadNum];
    for(unsigned i = 0; i < threadNum; ++i)
    {
        ths[i] = CreateThread(NULL, 0, piCalc1, &(params[i]), CREATE_SUSPENDED, NULL);
        params[i].h = ths[i];
        if(params[i].h == NULL)
        {
            cout << "Problem with creating thread. Error: " << GetLastError() << endl;
            for(unsigned j = 0; j < i; ++j)
            {
                CloseHandle(params[j].h);
            }
            CloseHandle(hMutexSELECT);
            CloseHandle(hMutexSUM);
            delete params;
            delete ths;
            return -1;
        }
        params[i].localSUM = 0;
        params[i].globalSUM = &respi;

        if(SetThreadPriority(params[i].h, THREAD_PRIORITY_HIGHEST) == 0)
        {
            cout << "Problem with changing thread priority. Error: " << GetLastError() << endl;
        }
    }



    DWORD waitError;
    DWORD startTime;
    DWORD finishTime;
    //Timer up
    startTime = GetTickCount();

    for(unsigned i = 0; i < threadNum; ++i)
    {
        ResumeThread(ths[i]);
    }

    waitError = WaitForMultipleObjects(threadNum, ths, true, INFINITE);

    respi /= N;
    
    finishTime = GetTickCount();
    //Timer down

    if(!(WAIT_OBJECT_0 >= waitError || waitError <= WAIT_OBJECT_0 + threadNum - 1)/*или WAIT_TIMEOUT, если отвал по таймеру*/)
    {
        cout << "Problem with WaitForMultipleObjects (return = " << waitError << "). Error: " << GetLastError() << endl;
    }


    for(unsigned i = 0; i < threadNum; ++i)
    {
        CloseHandle(ths[i]);
    }
    CloseHandle(hMutexSELECT);
    CloseHandle(hMutexSUM);
    delete params;
    delete ths;

    *milisec = finishTime - startTime;
    return respi;
}






const size_t BLOCKSIZE = 10 * 930827;
//const size_t BLOCKSIZE = 930827;
const size_t N = 100000000;

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        const unsigned maxThreads = 32;
        const unsigned attempts = 10;
        DWORD buffMilisec;
        unsigned *reses = new unsigned[maxThreads];
        unsigned *x = new unsigned[maxThreads];
        for(unsigned i = 1; i <= maxThreads; ++i)
        {
            DWORD avgMilisec = 0;
            cout << "For " << i << " threads: " << endl;
            for(unsigned j = 0; j < attempts; ++j)
            {
                cout << setprecision(80) << "Circle " << (j+1) << ": " << processPI1(N, i, BLOCKSIZE, &buffMilisec) << endl;
                avgMilisec += buffMilisec;
            }
            avgMilisec /= attempts;
            reses[i-1] = avgMilisec;
            x[i-1] = i;
            cout << "Threads " << i << ": " << avgMilisec << " milisec (" << (long double)avgMilisec / 1000 << " sec). " << endl;
        }
        //cout << endl << makeCodeForMatLab(x, reses, maxThreads) << endl;
        delete reses;
        delete x;
    }
    else if(argc == 2)
    {
        unsigned threadNum = atoi(argv[1]);
        DWORD milisec = -1;
        long double calculatedpi = processPI1(N, threadNum, BLOCKSIZE, &milisec);
        cout << setprecision(N) << "Calculated pi = " << calculatedpi << endl;
        cout << "Time = " << milisec << " milisec (" << (long double)milisec / 1000 << " sec)." << endl;
        //cout << sizeof(long double) << " " << sizeof(double) << endl; // "16 8"
    }
    else
    {
        cout << "Syntax error! " << endl;
        cout << "Expected: \"" << "> lab3.1.exe threadNum" << "\" or \"" << "> lab3.1.exe" << "\". " << endl;
        return -1;
    }
    return 0;
}
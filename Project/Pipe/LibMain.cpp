//
// Created by hackerl on 10/14/18.
//
#include "Pipe/Pipe.h"
#include "Pipe/PipeDelay.h"

class CTestPipe : public CPipe, public IDelayInstance
{
public:
    CTestPipe()
    {
        m_ID = -1;
    }

public:
    bool PipeOut(const char * Buffer, size_t Length) override
    {
        std::cout << m_ID << "Pipe Out" << std::endl;
    };

    void OnPipeClose() override
    {
        std::cout << m_ID << "Pipe Close" << std::endl;
    };

public:
    bool InitDelay(void * arg) override
    {
        m_ID = * (int *)arg;
        return true;
    }

public:
    int m_ID;
};

void TestPipe()
{
    auto Pipe1 = new CPipeDelay<CTestPipe>;

    int ID1 = 1;
    Pipe1->SetArg(&ID1);

    auto Pipe2 = new CPipeDelay<CTestPipe>;
    int ID2 = 2;

    Pipe2->SetArg(&ID2);

    Pipe2->PipeConnect(Pipe1);
    Pipe1->PipeConnect(Pipe2);

    std::string msg = "hello";

    Pipe2->PipeIn(msg.c_str(), msg.length());

    delete Pipe2->GetPipePort();
    delete Pipe2;
}

int main()
{
    TestPipe();
    return 0;
}
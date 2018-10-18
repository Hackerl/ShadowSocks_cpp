//
// Created by hackerl on 10/14/18.
//
#include "Pipe/Pipe.h"
#include "Pipe/PipeDelay.h"

class CTestPipe : public CPipe, public IDelayInstance<int>
{
public:
    CTestPipe()
    {
        m_ID = -1;
    }

    ~CTestPipe() override
    {
        std::cout << m_ID << "Delete Pipe" << std::endl;
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
    bool InitDelay(int & ID) override
    {
        m_ID = ID;
        return true;
    }

public:
    int m_ID;
};

void TestPipe()
{
    auto Pipe1 = new CPipeDelay<CTestPipe, int>;

    int ID1 = 1;
    Pipe1->SetArg(ID1);

    auto Pipe2 = new CTestPipe;

    Pipe2->m_ID = 2;

    Pipe2->PipeConnect(Pipe1);
    Pipe1->PipeConnect(Pipe2);

    std::string msg = "hello";

    Pipe2->PipeIn(msg.c_str(), msg.length());

    IPipe * Pipe_1 =  Pipe2->GetPipePort();

    Pipe2->PipeClose();
    delete Pipe2;
    delete Pipe_1;
}

int main()
{
    TestPipe();
    return 0;
}
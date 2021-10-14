#include <stdint.h>
#include "echo_canceller.h"
#include "dios_ssp_api.h"

#include "iostream"
using namespace std;

class EchoCancellerImpl : public EchoCanceller
{
public:
    EchoCancellerImpl(int frame_size=128, int sample_rate=16000, int mics=1, int speakers=1);

    ~EchoCancellerImpl();

    std::string process(const std::string& near, const std::string& far);

private:
    objDios_ssp *st;
    short *e;
    int frames;
};

EchoCanceller* EchoCanceller::create(int frame_size, int sample_rate, int mics, int speakers)
{
    return new EchoCancellerImpl(frame_size, sample_rate, mics, speakers);
}

EchoCancellerImpl::EchoCancellerImpl(int frame_size, int sample_rate, int mics, int speakers)
{
    st = dios_ssp_init_api(frame_size, mics, speakers);
    dios_ssp_reset_api(st);

    frames = frame_size * mics;
    e = new short[frames];
}

EchoCancellerImpl::~EchoCancellerImpl()
{
    dios_ssp_uninit_api(st);
    delete e;
}

std::string EchoCancellerImpl::process(const std::string& near, const std::string& far)
{
    const short *y = (const short *)(near.data());
    const short *x = (const short *)(far.data());
    dios_ssp_process_api(st, y, x, e);
    
    return std::string((const char *)e, frames * sizeof(int16_t));
}


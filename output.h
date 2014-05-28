#ifndef OUTPUT_H
#define OUTPUT_H

class Output
{
public:
    static Output* Inst();

    void kprintf(const char* msg, ...);

    void updateStatus(unsigned long long ms, unsigned long long eventInit,
                      unsigned long long internalEvents, unsigned long long externalEvents){

    }

    void clearProgressBar(){

    }

    void progressBar(unsigned long long current, unsigned long long maximum){

    }

private:
    Output();
    static Output* output;
};

#endif // OUTPUT_H

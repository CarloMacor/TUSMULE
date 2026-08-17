#ifndef SAMPLE_BUILDER_H
#define SAMPLE_BUILDER_H

class SampleBuilder
{
public:

    void begin();

    bool acquire();

};

extern SampleBuilder Builder;

#endif
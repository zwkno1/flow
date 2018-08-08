#pragma once

#include <map>
#include <memory>
#include <string>

class Slot
{
public:
	typedef std::shared_ptr<Slot> Ptr;

    virtual std::string type() = 0;

    virtual void addSink(Ptr ) = 0;

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual void flow(const uint8_t * data, size_t size) = 0;

    virtual ~Slot() {}
};

typedef Slot::Ptr SlotPtr;


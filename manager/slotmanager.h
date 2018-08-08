#pragma once

#include <map>
#include <list>

#include <boost/dll.hpp>

#include <flow/slot.h>

class SlotManager : public Slot
{
	typedef SlotPtr (createSlotFunc)(const std::map<std::string, std::string> & );
	typedef boost::dll::detail::import_type<createSlotFunc>::type SlotFactory;

	struct SlotInfo
	{
		std::map<std::string, std::string> args;
		std::vector<std::string> sinks;

		SlotFactory factory;
		SlotPtr slot;
	};
public:
    SlotManager(const std::string & configFile, const std::string & slotDir);

    ~SlotManager();

    std::string type() override;

    bool start() override;

    void stop() override;

    void addSink(SlotPtr ) override;

    void flow(const uint8_t * data, size_t size) override;

private:
    //bool buildSlots();

	bool loadConfig();

    std::string id_;

    std::string configFile_;

	std::string slotDir_;
    
    std::map<std::string, SlotInfo> slots_;

    bool stopped_;

};

#include <flow/factory.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace
{

class FileWriter : public Slot
{
public:
    FileWriter(const std::map<std::string, std::string> & paras)
        : file_("a.txt")
        , started_(false)
    {
		auto iter = paras.find("fileName");
		if(iter != paras.end())
            file_ = iter->second;
    }

    std::string type() override
    {
        return "FileWriter";
    }

    void addSink(SlotPtr) override
    {

    }

    bool start() override
    {
        if(started_)
            return true;
        if(ofs_.is_open())
        {
            ofs_.close();
        }

        ofs_.open(file_, std::ios_base::binary|std::ios_base::out|std::ios_base::app);

        started_ = true;
        return true;
    }

    void flow(const uint8_t * data, size_t size) override
    {
        if(started_)
        {
            ofs_.write((const char *)data, size);
			ofs_.flush();
        }
    }

    void stop() override
    {
        if(!started_)
            return;

        if(ofs_.is_open())
        {
            ofs_.close();
        }
        started_ = false;
    }

    ~FileWriter() override
    {
        stop();
    }

private:
    bool started_;

    std::string file_;

    std::ofstream ofs_;
};

}

SlotPtr createSlot(const std::map<std::string, std::string> & paras)
{
    return std::make_shared<FileWriter>(paras);
}

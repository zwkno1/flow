#include "slotmanager.h"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/graph/graphviz.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/topological_sort.hpp>

SlotManager::SlotManager(const std::string & configFile, const std::string & slotDir)
	: configFile_(configFile)
	, slotDir_(slotDir)
    , stopped_(true)
{
}

SlotManager::~SlotManager()
{

}

std::string SlotManager::type()
{
    return "SlotManager";
}

void SlotManager::addSink(SlotPtr )
{
}

bool SlotManager::start()
{
    loadConfig();
    for(auto & s : slots_)
    {
        s.second.slot->start();
    }
    stopped_ = false;
	return true;
}

void SlotManager::flow(const uint8_t * data, size_t size)
{
}

void SlotManager::stop()
{
    stopped_ = true;
    for(auto & s : slots_)
    {
        s.second.slot->stop();
    }
}

//bool SlotManager::buildSlots()
//{
//	// adjacency_list-based type
//	typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
//	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexProperty> Graph;
//	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
//
//	// Construct an empty graph and prepare the dynamic_property_maps.
//	Graph g(0);
//	std::map<unsigned long, std::map<std::string, std::string> > args;
//	boost::dynamic_properties dp { [& args](const std::string& s, const boost::any& k, const boost::any& v)
//		{
//			args[boost::any_cast<unsigned long>(k)][s] = boost::any_cast<std::string>(v);
//			return boost::shared_ptr<boost::dynamic_property_map>();
//		}
//	};
//
//	boost::property_map<Graph, boost::vertex_name_t>::type name = boost::get(boost::vertex_name, g);
//	dp.property("node_id", name);
//
//	try
//	{
//		std::ifstream ifs(configFile_);
//		bool status = boost::read_graphviz(ifs, g, dp, "node_id");
//		if(!status)
//		  return false;
//		std::vector<Vertex> c;
//		c.reserve(boost::num_vertices(g));
//		topological_sort(g, std::back_inserter(c));
//		for(auto iter = c.rbegin(); iter != c.rend(); ++iter)
//		{
//			std::cout << get(name, *iter) << std::endl;
//			std::cout << "args: " << std::endl;
//			for(auto & i : args[*iter])
//			{
//				std::cout << "\t(" << i.first << ", " << i.second << ")" << std::endl;
//			}
//		}
//	}
//	catch(std::exception & e)
//	{
//		std::cout << "except: " << e.what() << std::endl;
//	}
//	return false;
//}

bool SlotManager::loadConfig()
{
    try
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_json(configFile_, tree);

        for(auto const & i : tree.get_child("slots"))
        {
            //SlotInfo s;
			std::string id;
			std::map<std::string, std::string> args;
			std::vector<std::string> sinks;
			SlotPtr slot;

			id = i.second.get<std::string>("id");
            for(auto const & j : i.second.get_child("sinks"))
            {
                sinks.push_back(j.second.get_value<std::string>());
            }

            for(auto const & j : i.second.get_child("args"))
            {
				args[j.first] = j.second.data();
            }

			std::cout << "creatSlot, id: " << id << ", args:";
			for(auto & i : args)
			{
				std::cout << "(" << i.first << ", " << i.second << ")";
			}
			std::cout << std::endl;

			boost::filesystem::path libPath(slotDir_);
			libPath /= id;
			auto factory = boost::dll::import<createSlotFunc>(
					libPath,
					"createSlot",
					boost::dll::load_mode::append_decorations
					);
			slot = factory(args);

			slots_.insert(std::make_pair<std::string, SlotInfo>(std::move(id), SlotInfo{ std::move(args), std::move(sinks), std::move(factory), std::move(slot)}));
		}
    }
    catch(boost::property_tree::ptree_error & err)
    {
		std::cout << "parse config " << configFile_ << " failed: " << err.what() << std::endl;
        return false;
    }
	catch(std::exception & e)
	{
		std::cout << "load config" << configFile_ << " failed: " << e.what() << std::endl;
		return false;
	}

    for(auto & s : slots_)
    {
        for(auto & sink : s.second.sinks)
        {
            auto iter = slots_.find(sink);
            if(iter == slots_.end())
            {
                std::cout << sink << " not loaded, required by " << s.first << std::endl;
                return false;
            }
			s.second.slot->addSink(iter->second.slot);
        }
    }

    return true;
}

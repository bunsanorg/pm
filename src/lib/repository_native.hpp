#pragma once

#include "bunsan/pm/repository.hpp"
#include "bunsan/pm/depends.hpp"

#include "bunsan/utility/archiver.hpp"
#include "bunsan/utility/builder.hpp"
#include "bunsan/utility/fetcher.hpp"

class bunsan::pm::repository::native: private boost::noncopyable
{
public:
    explicit native(const pm::config &config_);

    void create(const boost::filesystem::path &source, bool strip);
    void fetch_source(const entry &package);
    void update_index(const entry &package);

    /// unpack, configure, compile, pack
    void build(const entry &package);

    void build_installation(const entry &package);

    /// check system directories existance and create them if they are missing
    void check_dirs();

    /// clean caches
    void clean();

private:
    void unpack_source(const entry &package, const boost::filesystem::path &destination, std::map<entry, boost::property_tree::ptree> &snapshot);
    void unpack(const entry &package, const boost::filesystem::path &build_dir);
    void pack(const entry &package, const boost::filesystem::path &build_dir);
    std::string remote_resource(const entry &package, const boost::filesystem::path &name);
    boost::filesystem::path source_resource(const entry &package, const std::string &name);
    boost::filesystem::path package_resource(const entry &package, const std::string &name);
    std::multimap<boost::filesystem::path, std::string> sources(const entry &package);
    std::map<entry, boost::property_tree::ptree> read_snapshot(const boost::filesystem::path &path);
    void write_snapshot(const boost::filesystem::path &path, const std::map<entry, boost::property_tree::ptree> &snapshot);

public:
    bool installation_outdated(const entry &package, const std::map<entry, boost::property_tree::ptree> &snapshot);
    bool build_outdated(const entry &package, const std::map<entry, boost::property_tree::ptree> &snapshot);
    depends read_depends(const entry &package);
    void read_checksum(const entry &package, boost::property_tree::ptree &ptree);
    void extract_build(const entry &package, const boost::filesystem::path &destination);
    void extract_installation(const entry &package, const boost::filesystem::path &destination, bool reset=true);

private:
    const pm::config &m_config;
    const bunsan::utility::resolver m_resolver;
    bunsan::utility::archiver_ptr cache_archiver, source_archiver;
    bunsan::utility::builder_ptr builder;
    bunsan::utility::fetcher_ptr fetcher;
};

namespace bunsan{namespace pm
{
    /// merges first and second map into the first one checking contradictions
    template <typename Key, typename Value>
    void merge_maps(std::map<Key, Value> &a, const std::map<Key, Value> &b)
    {
        for (const auto &i: b)
        {
            auto iter = a.find(i.first);
            if (iter != a.end())
                BOOST_ASSERT(iter->second == i.second);
            else
                a[i.first] = i.second;
        }
    }
}}

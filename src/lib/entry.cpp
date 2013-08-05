#include <bunsan/pm/entry.hpp>

#include <bunsan/pm/error.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assert.hpp>

bool bunsan::pm::entry::is_allowed_symbol(char c)
{
    return ('0' <= c && c <= '9') ||
           ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           c == '_' ||
           c == '-' ||
           c == '.';
}

bool bunsan::pm::entry::is_allowed_subpath(const std::string &subpath)
{
    return !subpath.empty() &&
           subpath != "." &&
           subpath != ".." &&
           boost::algorithm::all(subpath, bunsan::pm::entry::is_allowed_symbol);
}

bunsan::pm::entry::entry(const std::string &name_, char delim)
{
    build(name_, delim);
}

bunsan::pm::entry::entry(const char *name_, char delim)
{
    build(name_, delim);
}

void bunsan::pm::entry::build(const std::string &name_, char delim)
{
    m_location.clear();
    boost::algorithm::split(m_location,
                            name_,
                            [delim](char c){return delim == c;},
                            boost::algorithm::token_compress_on);
    for (const std::string &i: m_location)
        if (!is_allowed_subpath(i))
            BOOST_THROW_EXCEPTION(invalid_entry_name_error() << invalid_entry_name_error::entry_name(name_));
    BOOST_ASSERT(!m_location.empty());
}

bunsan::pm::entry &bunsan::pm::entry::operator=(bunsan::pm::entry &&e) noexcept
{
    swap(e);
    return *this;
}

bunsan::pm::entry &bunsan::pm::entry::operator=(const bunsan::pm::entry &e)
{
    bunsan::pm::entry ent(e);
    swap(ent);
    return *this;
}

bunsan::pm::entry &bunsan::pm::entry::operator=(const std::string &name_)
{
    bunsan::pm::entry ent(name_);
    swap(ent);
    return *this;
}

bool bunsan::pm::entry::operator == (const bunsan::pm::entry &e) const
{
    return m_location == e.m_location;
}

bool bunsan::pm::entry::operator<(const bunsan::pm::entry &e) const
{
    return m_location < e.m_location;
}

bunsan::pm::entry bunsan::pm::entry::operator/(const entry &e) const
{
    entry ent(*this);
    ent.m_location.insert(ent.m_location.end(), e.m_location.begin(), e.m_location.end());
    return ent;
}

boost::filesystem::path bunsan::pm::entry::location() const
{
    boost::filesystem::path loc;
    for (const std::string &i: m_location)
        loc /= i;
    return loc;
}

const std::vector<std::string> &bunsan::pm::entry::decomposition() const
{
    return m_location;
}

boost::property_tree::ptree::path_type bunsan::pm::entry::ptree_path() const
{
    return boost::property_tree::ptree::path_type(name('/'), '/');
}

std::string bunsan::pm::entry::name(const std::string &delim) const
{
    BOOST_ASSERT(!m_location.empty());
    return boost::algorithm::join(m_location, delim);
}

std::string bunsan::pm::entry::name(char delim) const
{
    std::string d = {delim};
    return name(d);
}

void bunsan::pm::entry::swap(bunsan::pm::entry &e) noexcept
{
    m_location.swap(e.m_location);
}

std::string bunsan::pm::entry::remote_resource(
    const std::string &repository,
    const boost::filesystem::path &name_) const
{
    std::string full = repository;
    if (!full.empty() && full.back() != '/')
        full.push_back('/');
    full += name('/');
    for (const boost::filesystem::path &i: name_)
    {
        full.push_back('/');
        full += i.string();
    }
    return full;
}

boost::filesystem::path bunsan::pm::entry::local_resource(
    const boost::filesystem::path &dir,
    const boost::filesystem::path &name_) const
{
    return dir / location() / name_;
}

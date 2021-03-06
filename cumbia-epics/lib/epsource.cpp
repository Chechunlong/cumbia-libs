#include <stdio.h>
#include <algorithm>
#include <regex>
#include <cumacros.h>
#include "epsource.h"

EpSource::EpSource()
{

}

EpSource::EpSource(const string s)
{
    m_s = s;
}

EpSource::EpSource(const EpSource &other)
{
    this->m_s = other.m_s;
}

string EpSource::getPV() const
{
    size_t pos = m_s.find("(");
    if(pos == string::npos)
        return m_s;
    return m_s.substr(0, pos); /* exclude args */
}

std::vector<string> EpSource::getArgs() const
{
    std::string a = getArgsString();
    std::string delim = ",";
    std::regex re(delim);
    std::vector<std::string> ret;
    size_t pos = m_s.find('(');
    if(pos != string::npos)
        a = m_s.substr(pos + 1, m_s.rfind(')') - pos - 1);
    std::sregex_token_iterator iter(a.begin(), a.end(), re, -1);
    std::sregex_token_iterator end;
    for ( ; iter != end; ++iter)
        ret.push_back((*iter));
    return ret;
}

std::string EpSource::getArgsString() const
{
    std::string a;
    size_t pos = m_s.find('(');
    if(pos != string::npos)
        a = m_s.substr(pos + 1, m_s.rfind(')') - pos - 1);
    return a;
}

string EpSource::getName() const
{
    return m_s;
}

EpSource::Type EpSource::getType() const
{
    if(m_s.rfind(":") == string::npos)
        return PV;
    return Field;
}

EpSource &EpSource::operator=(const EpSource &other)
{
    if(this != &other)
        m_s = other.m_s;
    return *this;
}

bool EpSource::operator ==(const EpSource &other) const
{
    return m_s == other.m_s;
}

std::string EpSource::toString() const
{
    char repr[512];
    char type[8];
    getType() == PV ? snprintf(type, 8, "pv") :  snprintf(type, 8, "field");
    snprintf(repr, 512, "TSource [%p] [name:\"%s\"] [pv:\"%s\"] [type:%s] [args:\"%s\"]",
             this, m_s.c_str(), getPV().c_str(), type, getArgsString().c_str());
    return std::string(repr);
}


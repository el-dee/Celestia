
#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <string>
#include <celutil/util.h>
#include <celutil/utf8.h>

class AstroObject;

class Name
{
protected:
    std::shared_ptr<std::string> m_ptr { nullptr };
//     size_t m_hash { 0 };
//     void makeHash();
    const static std::string m_empty;
public:
    Name(const char *s) { m_ptr = std::make_shared<std::string>(s); }
    Name(const std::string& s) { m_ptr = std::make_shared<std::string>(s); }
    Name() : m_ptr { nullptr } {}
    Name(const Name& n)
    {
        m_ptr = n.ptr();
//         m_hash = n.m_hash;
    }
    const std::shared_ptr<std::string>& ptr() const { return m_ptr; }
//     size_t hash() const { return m_hash; }
    bool empty() const { return null() ? true : m_ptr->empty(); }
    const std::string& str() const { return null() ? m_empty : *m_ptr; }
    Name & operator=(const Name &n);
    Name & operator=(const std::string &n) { m_ptr = std::make_shared<std::string>(n); return *this; }
    Name & operator=(const char *n) { m_ptr = std::make_shared<std::string>(n); return *this; }
    operator const std::string() const { return str(); }
    bool null() const { return !m_ptr; }
    size_t length() const { return null() ? 0 : m_ptr->length(); }
    const char *c_str() const { return str().c_str(); }
    static const Name getEmpty() { return m_empty; }
};

bool inline operator==(const Name& n1, const Name& n2)
{
    return n1.ptr() == n2.ptr() || n1.str() == n2.str();
}

bool inline operator!=(const Name& n1, const Name& n2)
{
    return !(n1 == n2);
}

bool inline operator<(const Name &n1, const Name &n2)
{
    return n1.str() < n2.str();
}

bool inline operator>(const Name &n1, const Name &n2)
{
    return n1.str() > n2.str();
}

struct NameGreaterThanPredicate
{
    bool operator()(const Name &n1, const Name &n2)
    {
        return n1.ptr() < n2.ptr();
    }
};
/*
struct NameHash
{
    size_t operator()(const Name& h) const { return h.hash(); }
};

struct NameComp
{
    size_t operator()(const Name & n1, const Name &n2) const { return n1.hash() == n2.hash(); }
};
*/
class NameInfo
{
    Name m_canonical;
    Name m_localized;
    Name m_domain;
    AstroObject *m_object = { nullptr };
 public:
    NameInfo() = default;
    NameInfo(const std::string& val, const Name& domain)
    {
        std::string _val = ReplaceGreekLetterAbbr(val);
        m_canonical = _val;
        m_domain = domain;
    }
    NameInfo(const Name& val, const Name& domain)
    {
        m_canonical = val;
        m_domain = domain;
    }
    NameInfo(const NameInfo &other)
    {
        m_canonical = other.m_canonical;
        m_localized = other.m_localized;
        m_domain = other.m_domain;
        m_object = other.m_object;
    }
    bool hasLocalized() const;
    const Name getCanon() const;
    const Name getLocalized(bool = true) const;
    void translate();
    const AstroObject *getObject() const { return m_object; }
    AstroObject* getObject() { return m_object; }
    void setObject(AstroObject *o) { m_object = o; }
};

bool inline operator==(const NameInfo &n1, const NameInfo &n2)
{
    return n1.getCanon().str() == n2.getCanon().str();
}

bool inline operator!=(const NameInfo &n1, const NameInfo &n2)
{
    return n1.getCanon().str() != n2.getCanon().str();
}

bool inline operator<(const NameInfo &n1, const NameInfo &n2)
{
    return n1.getCanon().str() < n2.getCanon().str();
}

bool inline operator>(const NameInfo &n1, const NameInfo &n2)
{
    return n1.getCanon().str() > n2.getCanon().str();
}

typedef std::set<NameInfo> NameInfoSet;
typedef std::set<Name> NameSet;
typedef std::map<Name, NameInfo> NameMap;

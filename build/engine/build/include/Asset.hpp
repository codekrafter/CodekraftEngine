#pragma once

#include <string>
namespace ck {
class Asset
{
  protected:
    int version;
    std::string type;

  public:
    /// Create Asset from string data
    virtual void deserialize(std::string data) = 0;
    /// Get string representation of object
    virtual std::string serialize() = 0;
    /// Get type, should contain no spaces and no symbols. Used in header
    virtual std::string getType() = 0;
    /// Get version, should be an integer. Used in header
    virtual int getVersion() = 0;
};
}
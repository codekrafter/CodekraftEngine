#pragma once

#include "Engine.hpp"
#include "Asset.hpp"

namespace ck
{


CKClass();
class TestAsset : public Asset
{
public:
    TestAsset()
    {
        version = 1;
        type = "TEST_ASSET";

        let = 'S';
    };

    CKProperty();
    char let;
    //std::string string = "test_string";

    virtual void Ninit()
    {
        //let = 'X';
        //strings.clear();

        //strings.push_back("test1");
        //strings.push_back("test2");
        //strings.push_back("test3");
    }
};

}
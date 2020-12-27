//
// Created by Roberts Kalnins on 26/12/2020.
//

#ifndef SC_CONFIG_H
#define SC_CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace sc {

// Short alias for this namespace
namespace pt = boost::property_tree;


class Config {
  public:
    static Config &GetInstance () {
        static Config instance;

        return instance;
    }

    Config () {
        ReadConfig();
    };

    template < typename T >
    T GetConfig ( const std::string &name, T default_value ) {
        return config.get< T >(name, default_value);
    }


    Config ( Config const & ) = delete;

    void operator= ( Config const & ) = delete;

  private:

    void ReadConfig () {
        pt::read_json("config.json", config);
    }

    pt::ptree config;
};

}

#endif //SC_CONFIG_H

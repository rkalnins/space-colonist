//
// Created by Roberts Kalnins on 26/12/2020.
//

#ifndef SC_CONFIG_H
#define SC_CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <iostream>

#include "../logging/logging.h"


namespace sc {

namespace pt = boost::property_tree;


struct Range {
    Range ( int min, int max ) : min(min), max(max) {}

    Range () = default;

    int min { 0 };
    int max { 0 };
};


class Config {
  public:
    static Config &GetInstance () {
        static Config instance;

        return instance;
    }

    template < typename T >
    T GetValue ( const std::string &path, T default_value ) {
        auto res = config_.get< T >(path, default_value);

        logger_->debug("{}: {}", path, res);

        return res;
    }

    template < typename T >
    std::vector< T > GetList ( const std::string &path ) {
        std::vector< T > result;

        pt::ptree t = config_.get_child(path);
        result.reserve(t.size());

        for ( auto &item : t ) {
            result.push_back(item.second.get_value< T >());
        }

        return result;
    }

    Range GetRange ( const std::string &path ) {
        pt::ptree t = config_.get_child(path);

        auto min = t.begin();

        Range r(min->second.get_value< int >(),
                ( ++min )->second.get_value< int >());

        logger_->debug("{}: {}, {}", path, r.min, r.max);

        return r;
    }

    Config ( Config const & ) = delete;


    void operator= ( Config const & ) = delete;

  private:

    Config () {
        logger_ = CreateLogger("config");
        ReadConfig();
    };

    void ReadConfig () {
        try {
            pt::read_json("config.json", config_);
        }
        catch ( boost::wrapexcept< pt::json_parser::json_parser_error > &e ) {
            pt::read_json(std::cin, config_);
        };
    }

  private:

    pt::ptree config_;
    logger_t  logger_;
};

}

#endif //SC_CONFIG_H

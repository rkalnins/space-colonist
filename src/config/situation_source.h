//
// Created by Roberts Kalnins on 28/12/2020.
//

#ifndef SPACE_COLONIST_SITUATION_SOURCE_H
#define SPACE_COLONIST_SITUATION_SOURCE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../logging/logging.h"


namespace sc {

namespace pt = boost::property_tree;


class SituationSource {
  public:

    static SituationSource &GetInstance () {
        static SituationSource instance;

        return instance;
    }

    SituationSource ( SituationSource const & ) = delete;

    void operator= ( SituationSource const & ) = delete;

    template < typename T >
    T GetValue ( const std::string &path, T default_value ) {
        auto res = situation_tree_.get< T >(path, default_value);

        logger_->debug("{}: {}", path, res);

        return res;
    }

    template < typename T >
    std::vector< T > GetList ( const std::string &path ) {
        std::vector< T > result;

        pt::ptree t = situation_tree_.get_child(path);
        result.reserve(t.size());

        for ( auto &item : t ) {
            result.push_back(item.second.get_value< T >());
        }

        return result;
    }

    Range GetRange ( const std::string &path ) {
        pt::ptree t = situation_tree_.get_child(path);

        auto min = t.begin();

        Range r(min->second.get_value< int >(),
                ( ++min )->second.get_value< int >());

        logger_->debug("{}: {}, {}", path, r.min, r.max);

        return r;
    }

  private:
    SituationSource () {
        logger_ = CreateLogger("sitsrc");
        pt::read_json("situations.json", situation_tree_);
    }

  private:

    pt::ptree situation_tree_;
    logger_t  logger_;
};

}

#endif //SPACE_COLONIST_SITUATION_SOURCE_H

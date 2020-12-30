//
// Created by Roberts Kalnins on 27/12/2020.
//

#ifndef SPACE_COLONIST_ITEM_SOURCE_H
#define SPACE_COLONIST_ITEM_SOURCE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../logging/logging.h"
#include "../play/item.h"


namespace sc {

namespace pt = boost::property_tree;

using play::Item;
using items_vector_ptr_t = std::unique_ptr< std::vector< std::shared_ptr< Item>> >;
using cat_vec_t = std::unique_ptr< std::vector< std::string>>;


class ItemSource {
  public:

    static ItemSource &GetInstance () {
        static ItemSource instance;

        return instance;
    }

    ItemSource ( ItemSource const & ) = delete;

    void operator= ( ItemSource const & ) = delete;

    cat_vec_t GetCategories ( const std::string &source ) {
        cat_vec_t result = std::make_unique< std::vector< std::string>>();

        logger_->debug("Getting category children");
        pt::ptree t = items_.get_child(source + ".categories");
        logger_->debug("Got category children");
        result->reserve(t.size());

        for ( auto &c : t ) {
            result->push_back(c.second.get_value< std::string >());
            logger_->debug("Category: {}",
                           c.second.get_value< std::string >());
        }

        return result;
    }


    items_vector_ptr_t
    GetItems ( const std::string &source, const std::string &category ) {
        items_vector_ptr_t vec = std::make_unique< std::vector< std::shared_ptr< Item>> >();

        pt::ptree t = items_.get_child(source + "." + category);
        vec->reserve(t.size());

        std::string name;

        int quantity;
        int weight;
        int cost;

        for ( auto &item : t ) {
            name     = item.second.get< std::string >("name");
            quantity = item.second.get< int >("quantity");
            weight   = item.second.get< int >("weight");
            cost     = item.second.get< int >("cost");

            vec->emplace_back(
                    std::make_unique< Item >(category, name, quantity,
                                             weight, cost));
        }


        return vec;
    }

  private:

    ItemSource () {
        logger_ = CreateLogger("itemsrc");
        pt::read_json("items.json", items_);
    }

  private:

    pt::ptree items_;
    logger_t  logger_;


};

}

#endif //SPACE_COLONIST_ITEM_SOURCE_H

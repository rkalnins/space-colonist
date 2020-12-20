//
// Created by Roberts Kalnins on 15/12/2020.
//

#pragma once

#include <vector>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "../logging/logging.h"
#include "task.h"


namespace sc {

using task_pool_t = std::vector< std::shared_ptr< Task > >;


class TaskPool {

  public:

    [[maybe_unused]] TaskPool ( const std::string &name );

    void AddTask ( const std::shared_ptr< Task > &task );

    using iterator = task_pool_t::iterator;
    using const_iterator = task_pool_t::const_iterator;

    GameState Loop ( GameState state );

    void SetNewState ( GameState newState );


    inline iterator begin () noexcept { return pool_.begin(); }

    [[nodiscard]] inline const_iterator
    cbegin () const noexcept { return pool_.cbegin(); }

    inline iterator end () noexcept { return pool_.end(); }

    [[nodiscard]] inline const_iterator
    cend () const noexcept { return pool_.cend(); }


  private:
    task_pool_t pool_;
    logger_t    logger_;
    GameState   new_state_;

};


}

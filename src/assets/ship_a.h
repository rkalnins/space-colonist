//
// Created by Roberts Kalnins on 29/12/2020.
//

#ifndef SPACE_COLONIST_SHIP_A_H
#define SPACE_COLONIST_SHIP_A_H

namespace assets {

struct ShipA {
    const std::vector< const std::vector< const std::string>> ship {
            {
                    { R"(/\~~~~~~~~^\__/^\)" },
                    { R"(|| <%%> [  ]__  <)" },
                    { R"(\/~~~~~~~~~/  \v/)" }
            },
            {       { R"( /\~~~~~~^\_/^\)" },
                    { R"( || <%> [ ]_  <)" },
                    { R"( \/~~~~~~~/ \v/)" }
            },
            {       { R"(  /\~~~^\_/^\)" },
                    { R"(  || <%>[]_ <)" },
                    { R"(  \/~~~~/ \v/)" }
            },
            {       { R"(   /-\~^\/^\)" },
                    { R"(   |:|<>[] |)" },
                    { R"(   \-/~~/\v/)" }
            },
            {       { R"(    /--\^\)" },
                    { R"(    |:.| |)" },
                    { R"(    \--/~/)" }
            },
            {       { R"(    /--\\)" },
                    { R"(    |:.||)" },
                    { R"(    \--//)" }
            },
            {       { R"(    /--\)" },
                    { R"(    |:.|)" },
                    { R"(    \--/)" }
            },
            {       { R"(    //--\)" },
                    { R"(    ||:.|)" },
                    { R"(    \\--/)" }
            },
            {       { R"(    /^/--\)" },
                    { R"(    | |:.|)" },
                    { R"(    \~\--/)" }
            },
            {       { R"(   /^\^~~/-\)" },
                    { R"(   | []<>|:|)" },
                    { R"(   \v/~~~\-/)" }
            },
            {       { R"(    /^\_/^~~~/\)" },
                    { R"(    > _[]<%> ||)" },
                    { R"(    \v/ \~~~~\/)" }
            },
            {       { R"(   /^\_/^~~~~~~/\)" },
                    { R"(   >  _[ ] <%> ||)" },
                    { R"(   \v/ \~~~~~~~\/)" }
            },
            {       { R"(  /^\__/^~~~~~~~~/\)" },
                    { R"(  >  __[  ] <%%> ||)" },
                    { R"(  \v/  \~~~~~~~~~\/)" }
            },
    };
};

}

#endif //SPACE_COLONIST_SHIP_A_H

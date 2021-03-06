#ifndef METASHELL_READLINE_MDB_SHELL_HPP
#define METASHELL_READLINE_MDB_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/colored_string.hpp>
#include <metashell/mdb_shell.hpp>
#include <metashell/readline_environment.hpp>

namespace metashell {

class readline_mdb_shell : public mdb_shell {
public:

  readline_mdb_shell(
      const config& conf,
      const environment& env);

  virtual void run();

  virtual void add_history(const std::string& str);

  virtual void display(
      const colored_string& cs,
      colored_string::size_type first,
      colored_string::size_type length) const;

  virtual unsigned width() const;
private:

  readline_environment readline_env;
};

}

#endif

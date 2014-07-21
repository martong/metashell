#ifndef METASHELL_PRAGMA_TEMPLIGHT_HPP
#define METASHELL_PRAGMA_TEMPLIGHT_HPP

#include <metashell/pragma_handler_interface.hpp>

#include <string>

namespace metashell
{
  class shell;

  class pragma_templight : public pragma_handler_interface
  {
  public:
    explicit pragma_templight(shell& shell_);

    virtual pragma_handler_interface* clone() const;

    virtual std::string arguments() const;
    virtual std::string description() const;

    virtual void run(
      const token_iterator& args_begin_,
      const token_iterator& args_end_
    ) const;
  private:
    shell& _shell;
  };
}

#endif

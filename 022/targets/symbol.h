#ifndef __SIMPLE_TARGETS_SYMBOL_H__
#define __SIMPLE_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace til {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;
    int _qualifier;
    int _value = 0;
    bool _is_main = false;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name, int qualifier = 0) :
        _type(type), _name(name), _qualifier(qualifier) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }
    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }
    const std::string &name() const {
      return _name;
    }
    int value() const {
      return _value;
    }
    int value(int v) {
      return _value = v;
    }
    int qualifier() const {
      return _qualifier;
    }
    bool is_main() const {
      return _is_main;
    }
    bool is_main(bool v) {
      return _is_main = v;
    }
  };

} // til

#endif

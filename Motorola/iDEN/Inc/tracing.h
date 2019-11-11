#pragma once

namespace tracing
{
    const long SUCCEEDED  = 1;
  const long FAILED   = 0;

  class __method__
  {
  public:
    __method__(char* name, unsigned long* status);
    virtual ~__method__();

  protected:
    unsigned long*  status;
    std::string   name;
  };
}

#ifdef _LOG

#define __tracing_method__(variable) \
  unsigned long variable = tracing::FAILED; \
  tracing::__method__ __tracer__(__FUNCTION__, &variable);

#else

#define __tracing_method__(variable) \
    unsigned long variable = tracing::FAILED;

#endif
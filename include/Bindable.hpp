#pragma once

struct Bindable {

  virtual void bind() = 0;
  virtual void unbind() = 0;

  virtual ~Bindable() = default;
};

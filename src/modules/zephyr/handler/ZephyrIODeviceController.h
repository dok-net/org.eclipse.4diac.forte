#ifndef ZEPHYRIO_DEVICE_CONTROLLER_H
#define ZEPHYRIO_DEVICE_CONTROLLER_H

#include "core/io/device/io_controller.h"
#include "core/io/mapper/io_handle.h"
#include "extevhan.h"

class ZephyrIODeviceController : public forte::core::io::IODeviceController {
public:
  using HandleDescriptor = forte::core::io::IODeviceController::HandleDescriptor;
  using IOMapper = forte::core::io::IOMapper;
  using IOHandle = forte::core::io::IOHandle;

  DECLARE_HANDLER(ZephyrIODeviceController);

  struct Config : forte::core::io::IODeviceController::Config {
    unsigned int updateInterval = 40; // Sets the period for the data update cycle. The default value is 40 ms.
  };

  enum HandleType {
    Bit,
  };

  class ZephyrIOHandleDescriptor
    : public forte::core::io::IODeviceController::HandleDescriptor {
  public:
    HandleType mType;

    ZephyrIOHandleDescriptor(std::string const &paId,
      forte::core::io::IOMapper::Direction paDirection, HandleType paType)
      : HandleDescriptor(paId, paDirection), mType(paType) {}
  };

  IOHandle* initHandle(HandleDescriptor* paHandleDescriptor) override;

  void setConfig(struct forte::core::io::IODeviceController::Config* paConfig) override;

protected:
  const char *init() override {
    DEVLOG_INFO("ZephyrIODeviceController::init\n");
    return nullptr;
  }
  void runLoop() override;
  void deInit() override {
    DEVLOG_INFO("ZephyrIODeviceController::deInit\n");
  }

  struct Config mConfig;
};

#endif /* ifndef ZEPHYRIO_DEVICE_CONTROLLER_H */

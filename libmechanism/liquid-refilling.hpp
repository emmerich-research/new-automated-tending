#ifndef LIB_MECHANISM_LIQUID_REFILLING_HPP_
#define LIB_MECHANISM_LIQUID_REFILLING_HPP_

/** @file liquid-refilling.hpp
 *  @brief Liquid refilling mechanism class definition
 *
 * Liquid refilling mechanism
 */

#include <memory>
#include <string>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace mechanism {
// forward declaration
namespace impl {
class LiquidRefillingImpl;
}

namespace liquid {
using status = device::float_sensor::status;
}

using LiquidRefilling = StaticObj<impl::LiquidRefillingImpl>;

namespace impl {
class LiquidRefillingImpl : public StackObj {
  template <class LiquidRefillingImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<LiquidRefillingImpl>::create(Args&&... args);

 public:
  /**
   * Get active status
   *
   * @return active status
   */
  inline const bool& active() const { return active_; }
  /**
   * Setup water device
   *
   * @param level_device_id        level device id
   * @param in_device_id           in device id
   * @param out_device_id          out device id
   */
  void setup_water_device(const std::string& level_device_id,
                          const std::string& in_device_id,
                          const std::string& out_device_id);
  /**
   * Setup disinfectant device
   *
   * @param level_device_id level device id
   * @param in_device_id    in device id
   * @param out_device_id   out device id
   */
  void setup_disinfectant_device(const std::string& level_device_id,
                                 const std::string& in_device_id,
                                 const std::string& out_device_id);
  /**
   * Setup draining time
   *
   * @param water_draining_time        water draining time
   * @param disinfectant_draining_time disinfectant draining time
   */
  void setup_draining_time(unsigned int water_draining_time,
                           unsigned int disinfectant_draining_time);
  /**
   * Get water level status
   *
   * @return water level status
   */
  liquid::status water_level() const;
  /**
   * Get disinfectant level status
   *
   * @return disinfectant level status
   */
  liquid::status disinfectant_level() const;
  /**
   * Exchange water
   */
  void exchange_water() const;
  /**
   * Exchange disinfectant
   */
  void exchange_disinfectant() const;

 private:
  /**
   * Liquid Refilling constructor
   */
  LiquidRefillingImpl();
  /**
   * Liquid Refilling destructor
   */
  ~LiquidRefillingImpl();
  /**
   * Get water level device id
   *
   * @return water level device id
   */
  inline const std::shared_ptr<device::FloatDevice>& water_level_device()
      const {
    return water_level_device_;
  }
  /**
   * Get water in device id
   *
   * @return water in device id
   */
  inline const std::string& water_in_device_id() const {
    return water_in_device_id_;
  }
  /**
   * Get water out device id
   *
   * @return water out device id
   */
  inline const std::string& water_out_device_id() const {
    return water_out_device_id_;
  }
  /**
   * Get disinfectant level device
   *
   * @return disinfectant level device
   */
  inline const std::shared_ptr<device::FloatDevice>& disinfectant_level_device()
      const {
    return disinfectant_level_device_;
  }
  /**
   * Get disinfectant in device id
   *
   * @return disinfectant in device id
   */
  inline const std::string& disinfectant_in_device_id() const {
    return disinfectant_in_device_id_;
  }
  /**
   * Get disinfectant out device id
   *
   * @return disinfectant out device id
   */
  inline const std::string& disinfectant_out_device_id() const {
    return disinfectant_out_device_id_;
  }

 private:
  /**
   * Active status
   */
  bool active_;
  /**
   * Water level device
   */
  std::shared_ptr<device::FloatDevice> water_level_device_;
  /**
   * Water in device id in Shift Register
   */
  std::string water_in_device_id_;
  /**
   * Water out device id in Shift Register
   */
  std::string water_out_device_id_;
  /**
   * Water draining time
   */
  unsigned int water_draining_time_;
  /**
   * Disinfectant level device
   */
  std::shared_ptr<device::FloatDevice> disinfectant_level_device_;
  /**
   * Disinfectant in device id in Shift Register
   */
  std::string disinfectant_in_device_id_;
  /**
   * Disinfectant out device id in Shift Register
   */
  std::string disinfectant_out_device_id_;
  /**
   * Disinfectant draining time
   */
  unsigned int disinfectant_draining_time_;
};
}  // namespace impl
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_HPP_

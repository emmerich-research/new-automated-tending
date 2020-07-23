#include "mechanism.hpp"

#include "movement.hpp"

#include <cmath>
#include <thread>

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace mechanism {
namespace impl {
MovementBuilderImpl::MovementBuilderImpl() {}

MovementBuilderImpl::~MovementBuilderImpl() {}

ATM_STATUS MovementBuilderImpl::setup_x(
    const std::string&           stepper_x_id,
    const device::stepper::step& steps_per_mm,
    const std::string&           limit_switch_x_id) {
  if (!device::StepperRegistry::get()->exist(stepper_x_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(limit_switch_x_id)) {
    return ATM_ERR;
  }

  if (steps_per_mm == 0) {
    return ATM_ERR;
  }

  stepper_x_id_ = stepper_x_id;
  limit_switch_x_id_ = limit_switch_x_id;
  steps_per_mm_x_ = steps_per_mm;
  return ATM_OK;
}

ATM_STATUS MovementBuilderImpl::setup_y(
    const std::string&           stepper_y_id,
    const device::stepper::step& steps_per_mm,
    const std::string&           limit_switch_y_id) {
  if (!device::StepperRegistry::get()->exist(stepper_y_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(limit_switch_y_id)) {
    return ATM_ERR;
  }

  if (steps_per_mm == 0) {
    return ATM_ERR;
  }

  stepper_y_id_ = stepper_y_id;
  limit_switch_y_id_ = limit_switch_y_id;
  steps_per_mm_y_ = steps_per_mm;
  return ATM_OK;
}

ATM_STATUS MovementBuilderImpl::setup_z(
    const std::string&           stepper_z_id,
    const device::stepper::step& steps_per_mm,
    const std::string&           limit_switch_z_top_id,
    const std::string&           limit_switch_z_bottom_id) {
  if (!device::StepperRegistry::get()->exist(stepper_z_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(
          limit_switch_z_top_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(
          limit_switch_z_bottom_id)) {
    return ATM_ERR;
  }

  if (steps_per_mm == 0) {
    return ATM_ERR;
  }

  stepper_z_id_ = stepper_z_id;
  limit_switch_z_top_id_ = limit_switch_z_top_id;
  limit_switch_z_bottom_id_ = limit_switch_z_bottom_id;
  steps_per_mm_z_ = steps_per_mm;
  return ATM_OK;
}

ATM_STATUS MovementBuilderImpl::setup_finger(
    const std::string& finger_id,
    const std::string& finger_infrared_id) {
  if (!device::PWMDeviceRegistry::get()->exist(finger_id) ||
      !device::DigitalInputDeviceRegistry::get()->exist(finger_infrared_id)) {
    return ATM_ERR;
  }

  finger_id_ = finger_id;
  finger_infrared_id_ = finger_infrared_id;

  return ATM_OK;
}

std::shared_ptr<Movement>& MovementBuilderImpl::build() {
  if (movement_mechanism_instance_ == nullptr) {
    movement_mechanism_instance_ = Movement::create(this);
  }

  massert(movement_mechanism_instance_ != nullptr, "sanity");

  return movement_mechanism_instance_;
}
}  // namespace impl

Movement::Movement(const impl::MovementBuilderImpl* builder)
    : builder_{builder} {
  active_ = true;
  ready_ = true;
  next_move_interval_ = 0;
  last_move_end_ = 0;
  event_timer_x_ = 0;
  event_timer_y_ = 0;
  event_timer_z_ = 0;

  setup_stepper();
  if (active()) {
    setup_limit_switch();
  }

  if (active()) {
    setup_finger();
  }
}

Movement::~Movement() {}

void Movement::setup_stepper() {
  auto*  stepper_registry = device::StepperRegistry::get();
  auto&& stepper_x = stepper_registry->get(builder()->stepper_x_id());

  if (!stepper_x) {
    active_ = false;
    return;
  }

  stepper_x_ = stepper_x;

  auto&& stepper_y = stepper_registry->get(builder()->stepper_y_id());

  if (!stepper_y) {
    active_ = false;
    return;
  }

  stepper_y_ = stepper_y;

  auto&& stepper_z = stepper_registry->get(builder()->stepper_z_id());

  if (!stepper_z) {
    active_ = false;
    return;
  }

  stepper_z_ = stepper_z;
}

void Movement::setup_limit_switch() {
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  auto* digital_input_registry = device::DigitalInputDeviceRegistry::get();

  auto&& limit_switch_x =
      digital_input_registry->get(builder()->limit_switch_x_id());

  if (!limit_switch_x) {
    active_ = false;
    return;
  }

  limit_switch_x_ = limit_switch_x;

  auto&& limit_switch_y =
      digital_input_registry->get(builder()->limit_switch_y_id());

  if (!limit_switch_y) {
    active_ = false;
    return;
  }

  limit_switch_y_ = limit_switch_y;

  auto&& limit_switch_z_top =
      digital_input_registry->get(builder()->limit_switch_z_top_id());

  if (!limit_switch_z_top) {
    active_ = false;
    return;
  }

  limit_switch_z_top_ = limit_switch_z_top;

  auto&& limit_switch_z_bottom =
      digital_input_registry->get(builder()->limit_switch_z_bottom_id());

  if (!limit_switch_z_bottom) {
    active_ = false;
    return;
  }

  limit_switch_z_bottom_ = limit_switch_z_bottom;
}

void Movement::setup_finger() {
  massert(device::PWMRDeviceRegistry::get() != nullptr, "sanity");
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");

  auto* pwm_registry = device::PWMDeviceRegistry::get();
  auto* digital_input_registry = device::DigitalInputDeviceRegistry::get();

  auto&& finger = pwm_registry->get(builder()->finger_id());
  auto&& finger_infrared =
      digital_input_registry->get(builder()->limit_switch_z_top_id());

  if (!finger || !finger_infrared) {
    active_ = false;
    return;
  }

  finger_ = finger;
  finger_infrared_ = finger_infrared;
}

device::stepper::step Movement::stop_x(void) {
  return stepper_x()->stop();
}

device::stepper::step Movement::stop_y(void) {
  return stepper_y()->stop();
}

device::stepper::step Movement::stop_z(void) {
  return stepper_z()->stop();
}

void Movement::stop(void) {
  [[maybe_unused]] auto step_x = stop_x();
  [[maybe_unused]] auto step_y = stop_y();
  [[maybe_unused]] auto step_z = stop_z();
  ready_ = true;
}

void Movement::start_move(const long& x, const long& y, const long& z) {
  massert(State::get() != nullptr, "sanity");

  auto* state = State::get();

  if (state->fault() && !state->manual_mode()) {
    return;
  }

#if defined(SYNC_DRIVER)
  const time_unit time_x = stepper_x()->time_for_move(x);
  const time_unit time_y = stepper_y()->time_for_move(y);
  const time_unit time_z = stepper_z()->time_for_move(z);

  // find which motor would take the longest to finish,
  const time_unit move_time = std::max(time_x, std::max(time_y, time_z));

  LOG_DEBUG("Will move about {} micros", move_time);

  // start moving x
  if (x == 0) {
    event_timer_x_ = 0;
  } else {
    stepper_x()->start_move(x, move_time);
    event_timer_x_ = 1;
  }

  // start moving y
  if (y == 0) {
    event_timer_y_ = 0;
  } else {
    stepper_y()->start_move(y, move_time);
    event_timer_y_ = 1;
  }

  // start moving z
  if (z == 0) {
    event_timer_z_ = 0;
  } else {
    stepper_z()->start_move(z, move_time);
    event_timer_z_ = 1;
  }
#else

  // start moving x
  if (x == 0) {
    event_timer_x_ = 0;
  } else {
    stepper_x()->start_move(x);
    event_timer_x_ = 1;
  }

  // start moving y
  if (y == 0) {
    event_timer_y_ = 0;
  } else {
    stepper_y()->start_move(y);
    event_timer_y_ = 1;
  }

  // start moving z
  if (z == 0) {
    event_timer_z_ = 0;
  } else {
    stepper_z()->start_move(z);
    event_timer_z_ = 1;
  }
#endif

  ready_ = false;
  last_move_end_ = 0;
  next_move_interval_ = 1;
}

void Movement::update_x() const {
  auto steps = stepper_x()->step_count();
  auto remaining_steps = stepper_x()->remaining_steps();

  if (remaining_steps == 0) {
    return;
  }

  if ((steps % builder()->steps_per_mm_x()) == 0) {
    if (stepper_x()->direction() == device::stepper::direction::forward) {
      State::get()->inc_x();
    } else {
      State::get()->dec_x();
    }
  }
}

void Movement::update_y() const {
  auto steps = stepper_y()->step_count();
  auto remaining_steps = stepper_y()->remaining_steps();

  if (remaining_steps == 0) {
    return;
  }

  if ((steps % builder()->steps_per_mm_y()) == 0) {
    if (stepper_y()->direction() == device::stepper::direction::forward) {
      State::get()->inc_y();
    } else {
      State::get()->dec_y();
    }
  }
}

void Movement::update_z() const {
  auto steps = stepper_z()->step_count();
  auto remaining_steps = stepper_z()->remaining_steps();

  if (remaining_steps == 0) {
    return;
  }

  if ((steps % builder()->steps_per_mm_z()) == 0) {
    if (stepper_z()->direction() == device::stepper::direction::forward) {
      State::get()->inc_z();
    } else {
      State::get()->dec_z();
    }
  }
}

void Movement::update_position() const {
  update_x();
  update_y();
  update_z();
}

float Movement::progress() const {
  const auto& step_remain_x = stepper_x()->remaining_steps();
  const auto& step_remain_y = stepper_y()->remaining_steps();
  const auto& step_remain_z = stepper_z()->remaining_steps();

  const auto& step_count_x = stepper_x()->step_count();
  const auto& step_count_y = stepper_y()->step_count();
  const auto& step_count_z = stepper_z()->step_count();

  const auto f = [](const device::stepper::step& remain,
                    const device::stepper::step& count) {
    return static_cast<float>(count) / static_cast<float>(remain + count);
  };

  float remainder = 0.0f;
  float percentage = 0.0f;

  float percentage_x = f(step_remain_x, step_count_x);
  float percentage_y = f(step_remain_y, step_count_y);
  float percentage_z = f(step_remain_z, step_count_z);

  if (percentage_x < 1.0f) {
    remainder += 1.0f;
    percentage += percentage_x;
  }

  if (percentage_y < 1.0f) {
    remainder += 1.0f;
    percentage += percentage_y;
  }

  if (percentage_z < 1.0f) {
    remainder += 1.0f;
    percentage += percentage_z;
  }

  if (percentage == 0.0f) {
    percentage = 1.0f;
  } else {
    percentage /= remainder;
  }

  return percentage;
}

time_unit Movement::next() {
  while ((micros() - last_move_end()) < next_move_interval()) {
    // not yet running
  }

  // bool next_x = false;
  // bool next_y = false;
  // bool next_z = false;

  // std::future<time_unit> timer_x;
  // std::future<time_unit> timer_y;
  // std::future<time_unit> timer_z;

  if (event_timer_x() <= next_move_interval()) {
    // with thread version
    // next_x = true;
    // timer_x = thread_pool().enqueue([this] { return stepper_x()->next(); });

    // without thread version
    event_timer_x_ = stepper_x()->next();
  } else {
    event_timer_x_ -= next_move_interval();
  }

  if (event_timer_y() <= next_move_interval()) {
    // with thread version
    // next_y = true;
    // timer_y = thread_pool().enqueue([this] { return stepper_y()->next(); });

    // without thread version
    event_timer_y_ = stepper_y()->next();
  } else {
    event_timer_y_ -= next_move_interval();
  }

  if (event_timer_z() <= next_move_interval()) {
    // with thread version
    // next_z = true;
    // timer_z = thread_pool().enqueue([this] { return stepper_z()->next(); });

    // without thread version
    event_timer_z_ = stepper_z()->next();
  } else {
    event_timer_z_ -= next_move_interval();
  }

  // if (next_x) {
  //   event_timer_x_ = timer_x.get();
  // }

  // if (next_y) {
  //   event_timer_y_ = timer_y.get();
  // }

  // if (next_z) {
  //   event_timer_z_ = timer_z.get();
  // }

  update_position();
  // auto x = State::get()->x();
  // auto y = State::get()->y();
  // auto z = State::get()->z();
  // LOG_DEBUG("Position X {} Y {} Z {}", x, y, z);

  last_move_end_ = micros();
  next_move_interval_ = 0;

  // Find the time when the next pulse needs to fire
  // this is the smallest non-zero timer value from all active motorsA
  if (event_timer_x() > 0 &&
      (event_timer_x() < next_move_interval() || next_move_interval() == 0)) {
    next_move_interval_ = event_timer_x();
  }

  if (event_timer_y() > 0 &&
      (event_timer_y() < next_move_interval() || next_move_interval() == 0)) {
    next_move_interval_ = event_timer_y();
  }

  if (event_timer_z() > 0 &&
      (event_timer_z() < next_move_interval() || next_move_interval() == 0)) {
    next_move_interval_ = event_timer_z();
  }

  ready_ = (next_move_interval() == 0);

  return next_move_interval();
}

void Movement::move_to_spraying_position() {
  LOG_DEBUG("Move to spraying position...");
  const auto& iter = Config::get()->spraying_position();
  move<movement::unit::mm>(iter.first, iter.second, 0.0);
  // reset position so imaginary homing equals tending position
  State::get()->coordinate({0.0, 0.0, 0.0});
}

void Movement::move_to_tending_position() {
  LOG_DEBUG("Move to tending position...");
  const auto& iter = Config::get()->tending_position();
  move<movement::unit::mm>(iter.first, iter.second, 0.0);
  // reset position so imaginary homing equals tending position
  State::get()->coordinate({0.0, 0.0, 0.0});
}

void Movement::follow_spraying_paths() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  motor_profile(config->spraying_speed_profile(state->speed_profile()));

  LOG_DEBUG("Following spraying paths...");
  for (const auto& iter : Config::get()->spraying_path()) {
    if (state->fault())
      return;
    LOG_DEBUG("Move to x={}mm y={}mm", iter.first, iter.second);
    move<movement::unit::mm>(iter.first, iter.second, 0.0);
  }

  revert_motor_params();
}

void Movement::follow_tending_paths_edge() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  motor_profile(config->tending_speed_profile(state->speed_profile()));

  LOG_DEBUG("Following tending paths edge...");

  for (const auto& iter : config->tending_path_edge()) {
    if (state->fault())
      return;
    LOG_DEBUG("Move to x={}mm y={}mm", iter.first, iter.second);
    move<movement::unit::mm>(iter.first, iter.second, state->z());
  }

  revert_motor_params();
}

void Movement::follow_tending_paths_zigzag() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  motor_profile(config->tending_speed_profile(state->speed_profile()));

  LOG_DEBUG("Following tending paths zigzag...");

  for (const auto& iter : config->tending_path_zigzag()) {
    if (state->fault())
      return;
    LOG_DEBUG("Move to x={}mm y={}mm", iter.first, iter.second);
    move<movement::unit::mm>(iter.first, iter.second, state->z());
  }

  revert_motor_params();
}

void Movement::motor_profile(
    const config::MechanismSpeed& speed_profile) const {
  LOG_INFO("Changing motors' parameters...");

  stepper_x()->rpm(speed_profile.x.rpm);
  stepper_x()->acceleration(speed_profile.x.acceleration);
  stepper_x()->deceleration(speed_profile.x.deceleration);

  stepper_y()->rpm(speed_profile.y.rpm);
  stepper_y()->acceleration(speed_profile.y.acceleration);
  stepper_y()->deceleration(speed_profile.y.deceleration);

  stepper_z()->rpm(speed_profile.z.rpm);
  stepper_z()->acceleration(speed_profile.z.acceleration);
  stepper_z()->deceleration(speed_profile.z.deceleration);
}

void Movement::revert_motor_params() const {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  LOG_INFO("Reverting to default motors' parameters (homing)...");

  motor_profile(config->homing_speed_profile(state->speed_profile()));
}

void Movement::move_finger_up() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  // set speed profile
  motor_profile(config->homing_speed_profile(state->speed_profile()));

  LOG_DEBUG("Lifting finger...");

  enable_motors();

  bool z_completed =
      limit_switch_z_top()->read().value_or(device::digital::value::low) ==
      device::digital::value::high;
  while (!z_completed) {
    z_completed =
        limit_switch_z_top()->read().value_or(device::digital::value::low) ==
        device::digital::value::high;
    if (z_completed) {
      stepper_z()->stop();
    } else {
      start_move(0, 0, -1200);
      while (!ready()) {
        if (limit_switch_z_top()->read().value_or(
                device::digital::value::low) == device::digital::value::high) {
          stepper_z()->stop();
          z_completed = true;
          ready_ = true;
        } else {
          next();
        }
      }
    }
  }

  disable_motors();

  State::get()->z(0.0);
}

void Movement::move_finger_down() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  // set speed profile
  motor_profile(config->homing_speed_profile(state->speed_profile()));

  LOG_DEBUG("Lowering finger...");

  enable_motors();

  bool z_completed =
      limit_switch_z_bottom()->read().value_or(device::digital::value::low) ==
      device::digital::value::high;

  while (!z_completed) {
    z_completed =
        limit_switch_z_bottom()->read().value_or(device::digital::value::low) ==
        device::digital::value::high;
    if (z_completed) {
      stepper_z()->stop();
    } else {
      start_move(0, 0, 1200);
      while (!ready()) {
        if (limit_switch_z_bottom()->read().value_or(
                device::digital::value::low) == device::digital::value::high) {
          stepper_z()->stop();
          z_completed = true;
          ready_ = true;
        } else {
          next();
        }
      }
    }
  }

  disable_motors();

  State::get()->z(52.0);
}

void Movement::rotate_finger() const {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  const auto& speed_profile =
      config->tending_speed_profile(state->speed_profile());
  LOG_DEBUG("Rotating finger...");
  if (finger()->duty_cycle(speed_profile.duty_cycle) == ATM_ERR) {
    LOG_DEBUG("Cannot set finger duty cycle...");
  }
}

void Movement::stop_finger() const {
  LOG_DEBUG("Stopping finger...");
  finger()->write(device::digital::value::low);
}

void Movement::homing_finger() const {
  massert(Config::get() != nullptr, "sanity");

  auto* config = Config::get();

  const auto& speed_profile =
      config->homing_speed_profile(State::get()->speed_profile());

  LOG_DEBUG("Starting to homing finger");
  LOG_DEBUG("Setting to homing duty cycle");
  finger()->duty_cycle(speed_profile.duty_cycle);
  while (true) {
    if (finger_infrared()->read().value_or(device::digital::value::low) ==
        device::digital::value::high) {
      stop_finger();
      break;
    }
    // add delay
    sleep_for<time_units::micros>(50);
  }
  LOG_DEBUG("Homing finger is finished");
}

void Movement::homing() {
  massert(Config::get() != nullptr, "sanity");
  massert(State::get() != nullptr, "sanity");

  auto* config = Config::get();
  auto* state = State::get();

  LOG_DEBUG("Homing is started...");

  state->homing(true);

  // set speed profile
  motor_profile(config->homing_speed_profile(state->speed_profile()));

  // homing z
  move_finger_up();

  // enabling motor
  enable_motors();

  // homing x and y
  bool is_x_completed =
      limit_switch_x()->read().value_or(device::digital::value::low) ==
      device::digital::value::high;
  bool is_y_completed =
      limit_switch_y()->read().value_or(device::digital::value::low) ==
      device::digital::value::high;

  while (!is_x_completed || !is_y_completed) {
    long steps_x = 0;
    long steps_y = 0;

    if (limit_switch_x()->read().value_or(device::digital::value::low) ==
        device::digital::value::high) {
      is_x_completed = true;
      stepper_x()->stop();
    } else {
      steps_x = convert_length_to_steps<movement::unit::mm>(
          -1500.0, builder()->steps_per_mm_x());
    }

    if (limit_switch_y()->read().value_or(device::digital::value::low) ==
        device::digital::value::high) {
      is_y_completed = true;
      stepper_y()->stop();
    } else {
      steps_y = convert_length_to_steps<movement::unit::mm>(
          -1200.0, builder()->steps_per_mm_y());
    }

    start_move(steps_x, steps_y, 0);
    while (!ready()) {
      bool x_complete =
          limit_switch_x()->read().value_or(device::digital::value::low) ==
          device::digital::value::high;
      bool y_complete =
          limit_switch_y()->read().value_or(device::digital::value::low) ==
          device::digital::value::high;
      if (x_complete) {
        is_x_completed = true;
        stepper_x()->stop();
      }
      if (y_complete) {
        is_y_completed = true;
        stepper_y()->stop();
      }
      if (x_complete && y_complete) {
        ready_ = true;
      } else {
        next();
      }
    }
  }

  // set state to 0,0,0
  state->reset_coordinate();

  // move a bit (5mm for each axis)
  move<movement::unit::mm>(5.0, 5.0, 5.0);

  // set state to 0,0,0
  state->reset_coordinate();

  // disabling motor
  disable_motors();

  state->homing(false);

  LOG_DEBUG("Homing is finished...");
}

void Movement::enable_motors() const {
  LOG_DEBUG("Enabling motors...");
  stepper_x()->enable();
  stepper_y()->enable();
  stepper_z()->enable();
}

void Movement::disable_motors() const {
  LOG_DEBUG("Disabling motors...");
  stepper_x()->disable();
  stepper_y()->disable();
  stepper_z()->disable();
}

bool Movement::is_home() const {
  return util::and_(limit_switch_x()->read_bool(),
                    limit_switch_y()->read_bool(),
                    limit_switch_z_top()->read_bool());
}
}  // namespace mechanism

NAMESPACE_END

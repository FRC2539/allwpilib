/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "AnalogPotentiometer.h"

#include "ControllerPower.h"

using namespace frc;

/**
 * Construct an Analog Potentiometer object from a channel number.
 *
 * @param channel   The channel number on the roboRIO to represent. 0-3 are
 *                  on-board 4-7 are on the MXP port.
 * @param fullRange The angular value (in desired units) representing the full
 *                  0-5V range of the input.
 * @param offset    The angular value (in desired units) representing the
 *                  angular output at 0V.
 */
AnalogPotentiometer::AnalogPotentiometer(int channel, double fullRange,
                                         double offset)
    : m_analog_input(std::make_unique<AnalogInput>(channel)),
      m_fullRange(fullRange),
      m_offset(offset) {}

/**
 * Construct an Analog Potentiometer object from an existing Analog Input
 * pointer.
 *
 * @param channel   The existing Analog Input pointer
 * @param fullRange The angular value (in desired units) representing the full
 *                  0-5V range of the input.
 * @param offset    The angular value (in desired units) representing the
 *                  angular output at 0V.
 */
AnalogPotentiometer::AnalogPotentiometer(AnalogInput* input, double fullRange,
                                         double offset)
    : m_analog_input(input, NullDeleter<AnalogInput>()),
      m_fullRange(fullRange),
      m_offset(offset) {}

/**
 * Construct an Analog Potentiometer object from an existing Analog Input
 * pointer.
 *
 * @param channel   The existing Analog Input pointer
 * @param fullRange The angular value (in desired units) representing the full
 *                  0-5V range of the input.
 * @param offset    The angular value (in desired units) representing the
 *                  angular output at 0V.
 */
AnalogPotentiometer::AnalogPotentiometer(std::shared_ptr<AnalogInput> input,
                                         double fullRange, double offset)
    : m_analog_input(input), m_fullRange(fullRange), m_offset(offset) {}

/**
 * Get the current reading of the potentiometer.
 *
 * @return The current position of the potentiometer (in the units used for
 *         fullRange and offset).
 */
double AnalogPotentiometer::Get() const {
  return (m_analog_input->GetVoltage() / ControllerPower::GetVoltage5V()) *
             m_fullRange +
         m_offset;
}

/**
 * Implement the PIDSource interface.
 *
 * @return The current reading.
 */
double AnalogPotentiometer::PIDGet() { return Get(); }

/**
 * @return the Smart Dashboard Type
 */
std::string AnalogPotentiometer::GetSmartDashboardType() const {
  return "Analog Input";
}

/**
 * Live Window code, only does anything if live window is activated.
 */
void AnalogPotentiometer::InitTable(
    std::shared_ptr<nt::NetworkTable> subtable) {
  if (subtable) {
    m_valueEntry = subtable->GetEntry("Value");
    UpdateTable();
  } else {
    m_valueEntry = nt::NetworkTableEntry();
  }
}

void AnalogPotentiometer::UpdateTable() {
  if (m_valueEntry) m_valueEntry.SetDouble(Get());
}

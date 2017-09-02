/*----------------------------------------------------------------------------*/
/* Copyright (c) 2014-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "PowerDistributionPanel.h"

#include <HAL/HAL.h>
#include <HAL/PDP.h>
#include <HAL/Ports.h>
#include <llvm/SmallString.h>
#include <llvm/raw_ostream.h>

#include "LiveWindow/LiveWindow.h"
#include "WPIErrors.h"

using namespace frc;

PowerDistributionPanel::PowerDistributionPanel() : PowerDistributionPanel(0) {}

/**
 * Initialize the PDP.
 */
PowerDistributionPanel::PowerDistributionPanel(int module) : m_module(module) {
  int32_t status = 0;
  HAL_InitializePDP(m_module, &status);
  if (status != 0) {
    wpi_setErrorWithContextRange(status, 0, HAL_GetNumPDPModules(), module,
                                 HAL_GetErrorMessage(status));
    m_module = -1;
    return;
  }
}

/**
 * Query the input voltage of the PDP.
 *
 * @return The voltage of the PDP in volts
 */
double PowerDistributionPanel::GetVoltage() const {
  int32_t status = 0;

  double voltage = HAL_GetPDPVoltage(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return voltage;
}

/**
 * Query the temperature of the PDP.
 *
 * @return The temperature of the PDP in degrees Celsius
 */
double PowerDistributionPanel::GetTemperature() const {
  int32_t status = 0;

  double temperature = HAL_GetPDPTemperature(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return temperature;
}

/**
 * Query the current of a single channel of the PDP.
 *
 * @return The current of one of the PDP channels (channels 0-15) in Amperes
 */
double PowerDistributionPanel::GetCurrent(int channel) const {
  int32_t status = 0;

  if (!CheckPDPChannel(channel)) {
    llvm::SmallString<32> str;
    llvm::raw_svector_ostream buf(str);
    buf << "PDP Channel " << channel;
    wpi_setWPIErrorWithContext(ChannelIndexOutOfRange, buf.str());
  }

  double current = HAL_GetPDPChannelCurrent(m_module, channel, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return current;
}

/**
 * Query the total current of all monitored PDP channels (0-15).
 *
 * @return The the total current drawn from the PDP channels in Amperes
 */
double PowerDistributionPanel::GetTotalCurrent() const {
  int32_t status = 0;

  double current = HAL_GetPDPTotalCurrent(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return current;
}

/**
 * Query the total power drawn from the monitored PDP channels.
 *
 * @return The the total power drawn from the PDP channels in Watts
 */
double PowerDistributionPanel::GetTotalPower() const {
  int32_t status = 0;

  double power = HAL_GetPDPTotalPower(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return power;
}

/**
 * Query the total energy drawn from the monitored PDP channels.
 *
 * @return The the total energy drawn from the PDP channels in Joules
 */
double PowerDistributionPanel::GetTotalEnergy() const {
  int32_t status = 0;

  double energy = HAL_GetPDPTotalEnergy(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }

  return energy;
}

/**
 * Reset the total energy drawn from the PDP.
 *
 * @see PowerDistributionPanel#GetTotalEnergy
 */
void PowerDistributionPanel::ResetTotalEnergy() {
  int32_t status = 0;

  HAL_ResetPDPTotalEnergy(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }
}

/**
 * Remove all of the fault flags on the PDP.
 */
void PowerDistributionPanel::ClearStickyFaults() {
  int32_t status = 0;

  HAL_ClearPDPStickyFaults(m_module, &status);

  if (status) {
    wpi_setWPIErrorWithContext(Timeout, "");
  }
}

void PowerDistributionPanel::UpdateTable() {
  for (size_t i = 0; i < sizeof(m_chanEntry) / sizeof(m_chanEntry[0]); ++i) {
    if (m_chanEntry[i]) m_chanEntry[i].SetDouble(GetCurrent(i));
  }
  if (m_voltageEntry) m_voltageEntry.SetDouble(GetVoltage());
  if (m_totalCurrentEntry) m_totalCurrentEntry.SetDouble(GetTotalCurrent());
}

void PowerDistributionPanel::StartLiveWindowMode() {}

void PowerDistributionPanel::StopLiveWindowMode() {}

std::string PowerDistributionPanel::GetSmartDashboardType() const {
  return "PowerDistributionPanel";
}

void PowerDistributionPanel::InitTable(
    std::shared_ptr<nt::NetworkTable> subTable) {
  m_table = subTable;
  if (m_table != nullptr) {
    for (size_t i = 0; i < sizeof(m_chanEntry) / sizeof(m_chanEntry[0]); ++i) {
      llvm::SmallString<32> buf;
      llvm::raw_svector_ostream oss(buf);
      oss << "Chan" << i;
      m_chanEntry[i] = m_table->GetEntry(oss.str());
    }
    m_voltageEntry = m_table->GetEntry("Voltage");
    m_totalCurrentEntry = m_table->GetEntry("TotalCurrent");
    UpdateTable();
  } else {
    for (size_t i = 0; i < sizeof(m_chanEntry) / sizeof(m_chanEntry[0]); ++i) {
      m_chanEntry[i] = nt::NetworkTableEntry();
    }
    m_voltageEntry = nt::NetworkTableEntry();
    m_totalCurrentEntry = nt::NetworkTableEntry();
  }
}

std::shared_ptr<nt::NetworkTable> PowerDistributionPanel::GetTable() const {
  return m_table;
}
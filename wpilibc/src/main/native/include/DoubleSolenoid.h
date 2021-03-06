/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <memory>
#include <string>

#include <HAL/Types.h>

#include "LiveWindow/LiveWindowSendable.h"
#include "SolenoidBase.h"
#include "networktables/NetworkTableEntry.h"

namespace frc {

/**
 * DoubleSolenoid class for running 2 channels of high voltage Digital Output
 * (PCM).
 *
 * The DoubleSolenoid class is typically used for pneumatics solenoids that
 * have two positions controlled by two separate channels.
 */
class DoubleSolenoid : public SolenoidBase, public LiveWindowSendable {
 public:
  enum Value { kOff, kForward, kReverse };

  explicit DoubleSolenoid(int forwardChannel, int reverseChannel);
  DoubleSolenoid(int moduleNumber, int forwardChannel, int reverseChannel);
  virtual ~DoubleSolenoid();
  virtual void Set(Value value);
  virtual Value Get() const;
  bool IsFwdSolenoidBlackListed() const;
  bool IsRevSolenoidBlackListed() const;

  void UpdateTable();
  void StartLiveWindowMode();
  void StopLiveWindowMode();
  std::string GetSmartDashboardType() const;
  void InitTable(std::shared_ptr<nt::NetworkTable> subTable);

 private:
  int m_forwardChannel;  ///< The forward channel on the module to control.
  int m_reverseChannel;  ///< The reverse channel on the module to control.
  int m_forwardMask;     ///< The mask for the forward channel.
  int m_reverseMask;     ///< The mask for the reverse channel.
  HAL_SolenoidHandle m_forwardHandle = HAL_kInvalidHandle;
  HAL_SolenoidHandle m_reverseHandle = HAL_kInvalidHandle;

  nt::NetworkTableEntry m_valueEntry;
  NT_EntryListener m_valueListener = 0;
};

}  // namespace frc

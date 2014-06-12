/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "SensorBase.h"

#include "NetworkCommunication/LoadOut.h"
#include "WPIErrors.h"

const uint32_t SensorBase::kDigitalChannels;
const uint32_t SensorBase::kAnalogInputs;
const uint32_t SensorBase::kAnalogModules;
const uint32_t SensorBase::kDigitalModules;
const uint32_t SensorBase::kSolenoidChannels;
const uint32_t SensorBase::kSolenoidModules;
const uint32_t SensorBase::kPwmChannels;
const uint32_t SensorBase::kRelayChannels;
const uint32_t SensorBase::kPDPChannels;
const uint32_t SensorBase::kChassisSlots;
SensorBase *SensorBase::m_singletonList = NULL;

/**
 * Creates an instance of the sensor base and gets an FPGA handle
 */
SensorBase::SensorBase()
{
}

/**
 * Frees the resources for a SensorBase.
 */
SensorBase::~SensorBase()
{
}

/**
 * Add sensor to the singleton list.
 * Add this sensor to the list of singletons that need to be deleted when
 * the robot program exits. Each of the sensors on this list are singletons,
 * that is they aren't allocated directly with new, but instead are allocated
 * by the static GetInstance method. As a result, they are never deleted when
 * the program exits. Consequently these sensors may still be holding onto
 * resources and need to have their destructors called at the end of the program.
 */
void SensorBase::AddToSingletonList()
{
	m_nextSingleton = m_singletonList;
	m_singletonList = this;
}

/**
 * Delete all the singleton classes on the list.
 * All the classes that were allocated as singletons need to be deleted so
 * their resources can be freed.
 */
void SensorBase::DeleteSingletons()
{
	for (SensorBase *next = m_singletonList; next != NULL;)
	{
		SensorBase *tmp = next;
		next = next->m_nextSingleton;
		delete tmp;
	}
	m_singletonList = NULL;
}

/**
 * Check that the analog module number is valid.
 *
 * @return Analog module is valid and present
 */
bool SensorBase::CheckAnalogModule(uint8_t moduleNumber)
{
	if (nLoadOut::getModulePresence(nLoadOut::kModuleType_Analog, moduleNumber - 1))
		return true;
	return false;
}

/**
 * Check that the digital module number is valid.
 *
 * @return Digital module is valid and present
 */
bool SensorBase::CheckDigitalModule(uint8_t moduleNumber)
{
	if (nLoadOut::getModulePresence(nLoadOut::kModuleType_Digital, moduleNumber - 1))
		return true;
	return false;
}

/**
 * Check that the digital module number is valid.
 *
 * @return Digital module is valid and present
 */
bool SensorBase::CheckPWMModule(uint8_t moduleNumber)
{
	return CheckDigitalModule(moduleNumber);
}

/**
 * Check that the digital module number is valid.
 *
 * @return Digital module is valid and present
 */
bool SensorBase::CheckRelayModule(uint8_t moduleNumber)
{
	return CheckDigitalModule(moduleNumber);
}

/**
 * Check that the solenoid module number is valid.
 *
 * @return Solenoid module is valid and present
 */
bool SensorBase::CheckSolenoidModule(uint8_t moduleNumber)
{
	if (nLoadOut::getModulePresence(nLoadOut::kModuleType_Solenoid, moduleNumber - 1))
		return true;
	return false;
}

/**
 * Check that the digital channel number is valid.
 * Verify that the channel number is one of the legal channel numbers. Channel numbers are
 * 1-based.
 *
 * @return Digital channel is valid
 */
bool SensorBase::CheckDigitalChannel(uint32_t channel)
{
	if (channel < kDigitalChannels)
		return true;
	return false;
}

/**
 * Check that the digital channel number is valid.
 * Verify that the channel number is one of the legal channel numbers. Channel numbers are
 * 1-based.
 *
 * @return Relay channel is valid
 */
bool SensorBase::CheckRelayChannel(uint32_t channel)
{
	if (channel < kRelayChannels)
		return true;
	return false;
}

/**
 * Check that the digital channel number is valid.
 * Verify that the channel number is one of the legal channel numbers. Channel numbers are
 * 1-based.
 *
 * @return PWM channel is valid
 */
bool SensorBase::CheckPWMChannel(uint32_t channel)
{
	if (channel < kPwmChannels)
		return true;
	return false;
}

/**
 * Check that the analog input number is value.
 * Verify that the analog input number is one of the legal channel numbers. Channel numbers
 * are 0-based.
 *
 * @return Analog channel is valid
 */
bool SensorBase::CheckAnalogInput(uint32_t channel)
{
	if (channel < kAnalogInputs)
		return true;
	return false;
}

/**
 * Check that the analog output number is value.
 * Verify that the analog output number is one of the legal channel numbers. Channel numbers
 * are 0-based.
 *
 * @return Analog channel is valid
 */
bool SensorBase::CheckAnalogOutput(uint32_t channel)
{
    if (channel < kAnalogOutputs)
        return true;
    return false;
}

/**
 * Verify that the solenoid channel number is within limits.
 *
 * @return Solenoid channel is valid
 */
bool SensorBase::CheckSolenoidChannel(uint32_t channel)
{
	if (channel > 0 && channel <= kSolenoidChannels)
		return true;
	return false;
}

/**
 * Verify that the power distribution channel number is within limits.
 *
 * @return Solenoid channel is valid
 */
bool SensorBase::CheckPDPChannel(uint32_t channel)
{
	if (channel > 0 && channel <= kPDPChannels)
		return true;
	return false;
}

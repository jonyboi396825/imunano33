/**
 * @file
 * @brief File containing the imunano33::Climate class
 */

#ifndef INCLUDE_IMUNANO33_CLIMATE_HPP_
#define INCLUDE_IMUNANO33_CLIMATE_HPP_

#include "imunano33/unit.hpp"

namespace imunano33 {
/**
 * @brief Handles climate data from Nano 33 (or other) sensors
 *
 * Objects of this class take in data from a temperature, humidity, and pressure
 * sensor (HTS221 for temperature and humidity and LPS22HB for pressure on the
 * Nano 33 BLE Sense), which can be converted to other units.
 *
 * This class is meant to be default initialized, then updated through the use
 * of the update() method.
 *
 * If a temperature sensor is not available, this can still be initialized, but
 * dataExists() will be false.
 */
class Climate {
public:
  /**
   * @brief Default constructor
   *
   * Initializes with no climate data. To update climate data, call update().
   */
  Climate() = default;

  /**
   * @brief Copy constructor
   */
  Climate(const Climate &other) = default;

  /**
   * @brief Assignment operator
   */
  Climate &operator=(const Climate &other) = default;

  /**
   * @brief Destructor
   */
  ~Climate() = default;

  /**
   * @brief Move constructor
   */
  Climate(Climate &&) = default;

  /**
   * @brief Move assignment
   */
  Climate &operator=(Climate &&) = default;

  /**
   * @brief Determines if climate data exists.
   *
   * This only returns false if initialized but update() has not been called
   * yet.
   *
   * @returns If data exists.
   */
  bool dataExists() const { return m_dataExists; }

  /**
   * @brief Gets temperature
   *
   * Check that this temperature measurement is valid with dataExists() first.
   *
   * @tparam U Temperature unit.
   *
   * @returns Temperature in given unit.
   */
  template <TempUnit U> num_t getTemp() const {
    num_t res = 0;

    switch (U) {
    case FAHRENHEIT:
#ifdef IMUNANO33_EMBED
      res = m_temp * (9.0F / 5.0F) + 32.0F;
#else
      res = m_temp * (9.0 / 5.0) + 32.0;
#endif
      break;
    case CELSIUS:
      res = m_temp;
      break;
    case KELVIN:
#ifdef IMUNANO33_EMBED
      res = m_temp + 273.15F;
#else
      res = m_temp + 273.15;
#endif
      break;
    default:
      res = m_temp;
    }

    return res;
  }

  /**
   * @brief Gets pressure
   *
   * Check that this pressure measurement is valid with dataExists() first.
   *
   * @tparam U Pressure unit.
   *
   * @returns Pressure in given unit.
   */
  template <PressureUnit U> num_t getPressure() const {
    num_t res = 0;

    switch (U) {
    case KPA:
      res = m_pressure;
      break;
    case ATM:
#ifdef IMUNANO33_EMBED
      res = m_pressure * 0.00986923266716F;
#else
      res = m_pressure * 0.00986923266716;
#endif
      break;
    case MMHG:
#ifdef IMUNANO33_EMBED
      res = m_pressure * 7.500617F;
#else
      res = m_pressure * 7.500617;
#endif
      break;
    case PSI:
#ifdef IMUNANO33_EMBED
      res = m_pressure * 0.1450377377F;
#else
      res = m_pressure * 0.1450377377;
#endif
      break;
    }

    return res;
  }

  /**
   * @brief Gets relative humidity
   *
   * Check that this humidity measurement is valid with dataExists() first.
   *
   * Unit is percent humidity.
   *
   * @returns Relative humidity
   */
  num_t getHumidity() const { return m_humid; }

  /**
   * @brief Updates climate data
   *
   * @param temp Temperature, in C
   * @param humid Relative humidity, in percent
   * @param pressure Pressure, in kPa
   */
  void update(const num_t temp, const num_t humid, const num_t pressure) {
    m_dataExists = true;
    m_temp = temp;
    m_humid = humid;
    m_pressure = pressure;
  }

  /**
   * @brief Resets climate data
   *
   * dataExists() will be false after this is called.
   */
  void reset() { m_dataExists = false; }

private:
  bool m_dataExists{false};

  num_t m_temp = 0.0;
  num_t m_humid = 0.0;
  num_t m_pressure = 0.0;
};
} // namespace imunano33

#endif

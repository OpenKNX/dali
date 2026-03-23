#pragma once
#include "Arduino.h"

namespace Dali {
    /** DALI commands */
    enum Command {
        OFF = 0,
        UP = 1,
        DOWN = 2,
        STEP_UP = 3,
        STEP_DOWN = 4,
        RECALL_MAX = 5,
        RECALL_MIN = 6,
        STEP_DOWN_AND_OFF = 7,
        ON_AND_STEP_UP = 8,
        GO_TO_LAST = 10, // DALI-2
        // 11-15 not used
        GO_TO_SCENE = 16,
        // 17-31 go to scene +1 to +15
        // <-- Configuration Commands
        DEVICE_RESET = 32,
        ARC_TO_DTR = 33,
        SAVE_VARS = 34,
        SET_OPMODE = 35,
        RESET_MEM = 36,
        IDENTIFY = 37, // DALI-2
        // 38-41 not used
        DTR_AS_MAX = 42,
        DTR_AS_MIN = 43,
        DTR_AS_FAIL = 44,
        DTR_AS_POWER_ON = 45,
        DTR_AS_FADE_TIME = 46,
        DTR_AS_FADE_RATE = 47,
        DTR_AS_EXT_FADE_TIME = 48, // DALI-2
        // 49-63 not used
        DTR_AS_SCENE = 64,
        REMOVE_FROM_SCENE = 80,
        ADD_TO_GROUP = 96,
        REMOVE_FROM_GROUP = 112,
        DTR_AS_SHORT = 128,
        ENABLE_WRITE_MEMORY = 129,
        // --> Configuration Commands
        // 130-143 not used
        QUERY_STATUS = 144,
        QUERY_CONTROL_GEAR = 145,
        QUERY_LAMP_FAILURE = 146,
        QUERY_LAMP_POWER_ON = 147,
        QUERY_LIMIT_ERROR = 148,
        QUERY_RESET_STATE = 149,
        QUERY_MISSING_SHORT = 150,
        QUERY_VERSION = 151,
        QUERY_DTR = 152,
        QUERY_DEVICE_TYPE = 153,
        QUERY_PHYS_MIN = 154,
        QUERY_POWER_FAILURE = 155,
        QUERY_DTR1 = 156,
        QUERY_DTR2 = 157,
        QUERY_OPMODE = 158,
        QUERY_LIGHTTYPE = 159, // DALI-2
        QUERY_ACTUAL_LEVEL = 160,
        QUERY_MAX_LEVEL = 161,
        QUERY_MIN_LEVEL = 162,
        QUERY_POWER_ON_LEVEL = 163,
        QUERY_FAIL_LEVEL = 164,
        QUERY_FADE_SPEEDS = 165,
        QUERY_SPECMODE = 166,
        QUERY_NEXT_DEVTYPE = 167,
        QUERY_EXT_FADE_TIME = 168,
        QUERY_CTRL_GEAR_FAIL = 169, // DALI-2
        // 170-175 not used
        QUERY_SCENE_LEVEL = 176,
        // 177-191 query scene level +1 to +15
        QUERY_GROUPS_0_7 = 192,
        QUERY_GROUPS_8_15 = 193,
        QUERY_ADDRH = 194,
        QUERY_ADDRM = 195,
        QUERY_ADDRL = 196,
        READ_MEMORY_LOCATION = 197,
        // 198-223 not used
        // 224-254 Used as Extended Commands
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI special commands */
    enum SpecialCommand {
        TERMINATE = 16,
        SET_DTR = 17,
        INITIALISE = 18,
        RANDOMISE = 19,
        COMPARE = 20,
        WITHDRAW = 21,
        // 22-23 not used
        SEARCHADDRH = 24,
        SEARCHADDRM = 25,
        SEARCHADDRL = 26,
        PROGRAMSHORT = 27,
        VERIFYSHORT = 28,
        QUERY_SHORT = 29,
        PHYSICAL_SELECTION = 30,
        // 31 not used
        ENABLE_DT = 32,
        SET_DTR1 = 33,
        SET_DTR2 = 34,
        WRITE_MEMORY_LOCATION = 35,
        WRITE_MEMORY_LOCATION_NOREPLY = 36, // DALI-2
        // 37-255 not used
    };

    /** DALI Extended Commands for DT0 -  */
    enum class ExtendedCommandDT0 : uint8_t {
        // 0-223 not used
        // TODO: 224-254
    };

    /** DALI Extended Commands for DT1 - Self-contained emergency lighting */
    enum class ExtendedCommandDT1 : uint8_t {
        // 0-223 not used
        REST = 224,
        INHIBIT = 225,
        RESET_INHIBIT = 226,
        REQUEST_FUNCTION_TEST = 227,
        REQUEST_DURATION_TEST = 228,
        STOP_TEST = 229,
        RESET_FUNCTION_TEST_DONE_FLAG = 230,
        RESET_DURATION_TEST_DONE_FLAG = 231,
        RESET_LAMP_TIME = 232,
        STORE_EMERGENCY_LEVEL = 233,
        STORE_TEST_DELAY_TIME_HB = 234,
        STORE_TEST_DELAY_TIME_LB = 235,
        STORE_FUNCTION_TEST_INTERVAL = 236,
        STORE_DURATION_TEST_INTERVAL = 237,
        STORE_TEST_EXCECUTION_TIMEOUT = 238,
        STORE_PROLONG_TIME = 239,
        START_IDENTIFICATION = 240,
        QUERY_BATTERY_CHARGE = 241,
        QUERY_SELECTED_VARIABLE = 242,
        QUERY_DURATION_TEST_RESULT = 243,
        QUERY_LAMP_EMERGENCY_TIME = 244,
        QUERY_LAMP_TOTAL_OPERATION_TIME = 245,
        QUERY_EMERGENCY_LEVEL = 246,
        QUERY_EMERGENCY_PHYSICAL_MIN_LEVEL = 247,
        QUERY_EMERGENCY_PHYSICAL_MAX_LEVEL = 248,
        QUERY_RATED_DURATION = 249,
        QUERY_EMERGENCY_MODE = 250,
        QUERY_FEATURES = 251,
        QUERY_FAILURE_STATUS = 252,
        QUERY_EMERGENCY_STATUS = 253,
        PERFORM_SELECTED_FUNCTION = 254
    };

    /** DALI Extended Commands for DT2 - Discharge lamp */
    enum class ExtendedCommandDT2 {
        // 0-223 not used
        RESET_STORED_HID_FAILURE = 224,
        // 225-239 not used
        START_IDENTIFICATION = 240,
        QUERY_THERMAL_LOAD = 241,
        QUERY_THERMAL_OVERLOAD_TIME_HB = 242,
        QUERY_THERMAL_OVERLOAD_TIME_LB = 243,
        // 244-249 not used
        QUERY_HID_FEATURES = 250,
        QUERY_ACTUAL_HID_FAILURE = 251,
        QUERY_STORED_HID_FAILURE = 252,
        QUERY_HID_STATUS = 253,
        // 254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT3 - Low voltage halogen lamp */
    enum class ExtendedCommandDT3 {
        // 0-223 not used
        REFERENCE_SYSTEM_POWER = 224,
        ENABLE_CURRENT_PROTECTOR = 225,
        DISABLE_CURRENT_PROTECTOR = 226,
        // 227-239 not used
        QUERY_FEATURES = 240,
        QUERY_FAILURE_STATUS = 241,
        QUERY_SHORT_CIRCUIT = 242,
        QUERY_OPEN_CIRCUIT = 243,
        QUERY_LOAD_DECREASE = 244,
        QUERY_LOAD_INCREASE = 245,
        QUERY_CURRENT_PROTECTOR_ACTIVE = 246,
        QUERY_THERMAL_SHUTDOWN = 247,
        QUERY_THERMAL_OVERLOAD = 248,
        QUERY_REFERENCE_RUNNING = 249,
        QUERY_REFERENCE_MEASUREMENT_FAILED = 250,
        QUERY_CURRENT_PROTECTOR_ENABLED = 251,
        // 252-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT4 - Incandescent lamp */
    enum class ExtendedCommandDT4 {
        // 0-223 not used
        REFERENCE_SYSTEM_POWER = 224,
        SELECT_DIMMING_CURVE = 225,
        // 226-237 not used
        QUERY_DIMMING_CURVE = 238,
        QUERY_DIMMER_STATUS = 239,
        QUERY_FEATURES = 240,
        QUERY_FAILURE_STATUS = 241,
        QUERY_DIMMER_TEMPERATURE = 242,
        QUERY_RMS_SUPPLY_VOLTAGE = 243,
        QUERY_SUPPLY_FREQUENCY = 244,
        QUERY_RMS_LOAD_VOLTAGE = 245,
        QUERY_RMS_LOAD_CURRENT = 246,
        QUERY_REAL_LOAD_POWER = 247,
        QUERY_LOAD_RATING = 248,
        QUERY_REFERENCE_RUNNING = 249,
        QUERY_REFERENCE_MEASUREMENT_FAILED = 250,
        // 251-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };
    
    /** DALI Extended Commands for DT5 - Conversion to 1-10V */
    enum class ExtendedCommandDT5 {
        // 0-223 not used
        SET_OUTPUT_RANGE_TO_1_10V = 224,
        SET_OUTPUT_RANGE_TO_0_10V = 225,
        SWITCH_ON_INTERNAL_PULL_UP = 226,
        SWITCH_OFF_INTERNAL_PULL_UP = 227,
        STORE_DTR_AS_PHYSICAL_MINIMUM = 228,
        SELECT_DIMMING_CURVE = 229,
        RESET_CONVERTER_SETTINGS = 230,
        // 231-237 not used
        QUERY_DIMMING_CURVE = 238,
        QUERY_OUTPUT_LEVEL = 239,
        QUERY_CONVERTER_FEATURES = 240,
        QUERY_FAILURE_STATUS = 241,
        QUERY_CONVERTER_STATUS = 242,
        // 243-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT6 - LED Modules */
    enum class ExtendedCommandDT6 {
        // 0-223 not used
        REFERENCE_SYSTEM_POWER = 224,
        // 225-226 not used
        SELECT_DIMMING_CURVE = 227,
        SET_FAST_FADE_TIME = 228,
        // 229-236 not used
        QUERY_CONTROL_GEAR_TYPE = 237,
        QUERY_DIMMING_CURVE = 238,
        // 239 not used
        QUERY_FEATURES = 240,
        QUERY_FAILURE_STATUS = 241,
        // 242-243 not used
        QUERY_LOAD_DECREASE = 244,
        QUERY_LOAD_INCREASE = 245,
        // 246 not used
        QUERY_THERMAL_SHUTDOWN = 247,
        QUERY_THERMAL_OVERLOAD = 248,
        QUERY_REFERENCE_RUNNING = 249,
        QUERY_REFERENCE_MEASUREMENT_FAILED = 250,
        // 251-252 not used
        QUERY_FAST_FADE_TIME = 253,
        QUERY_MIN_FAST_FADE_TIME = 254,
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT7 - Switching function */
    enum class ExtendedCommandDT7 {
        // 0-223 not used
        REFERENCE_SYSTEM_POWER = 224,
        STORE_DTR_AS_UP_SWITCH_ON_THRESHOLD = 225,
        STORE_DTR_AS_UP_SWITCH_OFF_THRESHOLD = 226,
        STORE_DTR_AS_DOWN_SWITCH_ON_THRESHOLD = 227,
        STORE_DTR_AS_DOWN_SWITCH_OFF_THRESHOLD = 228,
        STORE_DTR_AS_ERROR_HOLD_OFF_TIME = 229,
        // 230-239 not used
        QUERY_FEATURES = 240,
        QUERY_SWITCH_STATUS = 241,
        QUERY_UP_SWITCH_ON_THRESHOLD = 242,
        QUERY_UP_SWITCH_OFF_THRESHOLD = 243,
        QUERY_DOWN_SWITCH_ON_THRESHOLD = 244,
        QUERY_DOWN_SWITCH_OFF_THRESHOLD = 245,
        QUERY_ERROR_HOLD_OFF_TIME = 246,
        QUERY_GEAR_TYPE = 247,
        // 248 not used
        QUERY_REFERENCE_RUNNING = 249,
        QUERY_REFERENCE_MEASUREMENT_FAILED = 250,
        // 251-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    // TODO set back to class and see why it doesnt work
    /** DALI Extended Commands for DT8 - Colour */
    enum ExtendedCommandDT8 : uint8_t {
        // 0-223 not used
        SET_COORDINATE_X = 224,
        SET_COORDINATE_Y = 225,
        ACTIVATE = 226,
        STEP_UP_COORDINATE_X = 227,
        STEP_DOWN_COORDINATE_X = 228,
        STEP_UP_COORDINATE_Y = 229,
        STEP_DOWN_COORDINATE_Y = 230,
        SET_TEMP_COLOUR_TEMPERATURE = 231,
        STEP_COOLER_COLOUR_TEMPERATURE = 232,
        STEP_WARMER_COLOUR_TEMPERATURE = 233,
        SET_TEMP_PRIMARY_LEVEL = 234,
        SET_TEMP_RGB_LEVEL = 235,
        SET_TEMP_WAF_LEVEL = 236,
        SET_TEMP_RGBWAF_LEVEL = 237,
        COPY_REPORT_TO_TEMP = 238,
        // 239 not used
        STORE_TY_PRIMARY = 240,
        STORE_XY_COORDINATE_PRIMARY = 241,
        STORE_COLOUR_TEMPERATURE_LIMIT = 242,
        STORE_GEAR_FEATURES = 243,
        // 244 not used
        ASSIGN_COLOUR_TO_LINKED_CHANNEL = 245,
        START_AUTO_CALIBRATION = 246,
        QUERY_GEAR_FEATURES = 247,
        QUERY_COLOUR_STATUS = 248,
        QUERY_COLOUR_TYPE_FEATURES = 249,
        QUERY_COLOUR_VALUE = 250,
        QUERY_COLOUR_RGBWAF_CONTROL = 251,
        QUERY_COLOUR_ASSIGNED_COLOUR = 252,
        // 253-254 not used
    };

    /** DALI Extended Commands for DT15 - Load referencing */
    enum class ExtendedCommandDT15 {
        // 0-223 not used
        REFERENCE_SYSTEM_POWER = 224,
        // 225-240 not used
        QUERY_FAILURE_STATUS = 241,
        // 242-243 not used
        QUERY_LOAD_DEVIATION = 244,
        // 245-248 not used
        QUERY_REFERENCE_RUNNING = 249,
        QUERY_REFERENCE_MEASUREMENT_FAILED = 250,
        // 251-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT16 - Thermal gear protection */
    enum class ExtendedCommandDT16 {
        // 0-223 not used
        RESET_THERMAL_GEAR_COUNT = 224,
        // 225-240 not used
        QUERY_FAILURE_STATUS = 241,
        // 242-246 not used
        QUERY_THERMAL_GEAR_SHUTDOWN = 247,
        QUERY_THERMAL_GEAR_OVERLOAD = 248,
        QUERY_THERMAL_GEAR_SHUTDOWN_COUNTER = 249,
        QUERY_THERMAL_GEAR_OVERLOAD_COUNTER = 250,
        // 251-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT17 - Centrally supplied emergency operation */
    enum class ExtendedCommandDT17 {
        // 0-226 not used
        SELECT_DIMMING_CURVE = 227,
        // 228-237 not used
        QUERY_DIMMING_CURVE = 238,
        // 239-254 not used
        QUERY_EXTENDED_VERSION_NUMBER = 255
    };

    /** DALI Extended Commands for DT19 - Load shedding */
    enum class ExtendedCommandDT19 {
        // 0-223 not used
        SET_EMERGENCY_LEVEL = 224,
        SET_EMERGENCY_CONDITION_SUPPLY = 225,
        SET_EMERGENCY_CONDITION_BUS = 226,
        TEST_EMERGENCY_LEVEL = 227,
        SET_EMERGENCY_PARAMETER_LOCK = 228,
        // 229-249 not used
        QUERY_EMERGENCY_LEVEL = 250,
        QUERY_EMERGENCY_CONDITION_SUPPLY = 251,
        QUERY_EMERGENCY_CONDITION_BUS = 252,
        QUERY_EMERGENCY_PHYSICAL_MAXIMUM = 253,
        QUERY_EMERGENCY_STATUS = 254
    };

    /** DALI Extended Commands for DT20 - Thermal lamp protection */
    enum class ExtendedCommandDT20 {
        // 0-223 not used
        SET_LOAD_SHEDDING_CONDITION = 224,
        SET_REDUCTION_FACTOR1 = 225,
        SET_REDUCTION_FACTOR2 = 226,
        SET_REDUCTION_FACTOR3 = 227,
        // 228-248 not used
        QUERY_LOAD_SHEDDING_CONDITION = 249,
        QUERY_REDUCTION_FACTOR1 = 250,
        QUERY_REDUCTION_FACTOR2 = 251,
        QUERY_REDUCTION_FACTOR3 = 252,
        QUERY_ACTUAL_FACTOR = 253,
        QUERY_LOAD_SHEDDING_FADE_RUNNING = 254
    };

    /** DALI Extended Commands for DT49 - Integrated Bus power */
    enum ExtendedCommandDT49 {
        // 0-254 not used
        QUERY_ACTIVE_POWER_SUPPLY = 255
    };

    /** DALI device types */
    enum class DeviceType {
        FLUORESCENT_LAMP,
        EMERGENCY_LIGHT,
        DISCHARGE_LAMP,
        HALOGEN_LAMP,
        INCANDESCENT_LAMP,
        DC_CONVERTER,
        LED_MODULE,
        SWITCH,
        COLOUR_CTRL,
        SEQUENCER,
        OPTICAL_CTRL,
        CENTRALLY_SUPPLIED_EMERGENCY_OPERATION = 17,
        LOAD_SHEDDING = 19,
        THERMAL_LAMP_PROTECTION = 20,
        INTEGRATED_LIGHT_SOURCE = 23,
        INTEGRATED_BUS_POWER = 49
    };
}; // namespace Dali
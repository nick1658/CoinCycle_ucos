#ifndef CCTALK_H
#define CCTALK_H

#define CCTALK_ADDR (0x37)


enum ccTalk_method_t {
    CCTALK_METHOD_RESPONSE = 0,
    CCTALK_METHOD_RESET_DEVICE = 1,
    CCTALK_METHOD_REQUEST_COMMS_STATUS_VAR = 2,
    CCTALK_METHOD_CLEAR_COMMS_STATUS_VAR = 3,
    CCTALK_METHOD_REQUEST_COMMS_REVISION = 4,
    CCTALK_METHOD_REQUEST_COIN_IN_BY_TYPE = 82,
    CCTALK_METHOD_REQUEST_HOPPER_HIGH_LOW_LEVEL_SENSOR_STATUS = 84,
    CCTALK_METHOD_REQUEST_HOPPER_PATTERN = 85,
    CCTALK_METHOD_REQUEST_SERVICE_STATUS = 104,
    CCTALK_METHOD_DATA_STREAM = 105,
    CCTALK_METHOD_REQUEST_ESCROW_STATUS = 106,
    CCTALK_METHOD_OPERATE_ESCROW = 107,
    CCTALK_METHOD_REQUEST_ENCRYPTED_MONETARY_ID = 108,
    CCTALK_METHOD_REQUEST_ENCRYPTED_HOPPER_STATUS = 109,
    CCTALK_METHOD_SWITCH_ENCRYPTION_KEY = 110,
    CCTALK_METHOD_REQUEST_ENCRYPTION_SUPPORT = 111,
    CCTALK_METHOD_READ_ENCRYPTED_EVENTS = 112,
    CCTALK_METHOD_SWITCH_BAUD_RATE = 113,
    CCTALK_METHOD_REQUEST_USB_ID = 114,
    CCTALK_METHOD_REQUEST_REAL_TIME_CLOCK = 115,
    CCTALK_METHOD_MODIFY_REAL_TIME_CLOCK = 116,
    CCTALK_METHOD_REQUEST_CASHBOX_VALUE = 117,
    CCTALK_METHOD_MODIFY_CASHBOX_VALUE = 118,
    CCTALK_METHOD_REQUEST_HOPPER_BALANCE = 119,
    CCTALK_METHOD_MODIFY_HOPPER_BALANCE = 120,
    CCTALK_METHOD_PURGE_HOPPER = 121,
    CCTALK_METHOD_REQUEST_ERROR_STATUS = 122,
    CCTALK_METHOD_REQUEST_ACTIVITY_REGISTER = 123,
    CCTALK_METHOD_VERIFY_MONEY_OUT = 124,
    CCTALK_METHOD_PAY_MONEY_OUT = 125,
    CCTALK_METHOD_CLEAR_MONEY_COUNTERS = 126,
    CCTALK_METHOD_REQUEST_MONEY_OUT = 127,
    CCTALK_METHOD_REQUEST_MONEY_IN = 128,
    CCTALK_METHOD_READ_BARCODE_DATA = 129,
    CCTALK_METHOD_REQUEST_INDEXED_HOPPER_DISPENSE_COUNT = 130,
    CCTALK_METHOD_REQUEST_HOPPER_COIN_VALUE = 131,
    CCTALK_METHOD_EMERGENCY_STOP_VALUE = 132,
    CCTALK_METHOD_REQUEST_HOPPER_POLLING_VALUE = 133,
    CCTALK_METHOD_DISPENSE_HOPPER_VALUE = 134,
    CCTALK_METHOD_SET_ACCEPT_LIMIT = 135,
    CCTALK_METHOD_STORE_ENCRYPTION_CODE = 136,
    CCTALK_METHOD_SWITCH_ENCRYPTION_CODE = 137,
    CCTALK_METHOD_FINISH_FIRMWARE_UPGRADE = 138,
    CCTALK_METHOD_BEGIN_FIRMWARE_UPGRADE = 139,
    CCTALK_METHOD_UPLOAD_FIRMWARE = 140,
    CCTALK_METHOD_REQUEST_FIRMWARE_UPGRADE_CAPABILITY = 141,
    CCTALK_METHOD_FINISH_BILL_TABLE_UPGRADE = 142,
    CCTALK_METHOD_BEGIN_BILL_TABLE_UPGRADE = 143,
    CCTALK_METHOD_UPLOAD_BILL_TABLES = 144,
    CCTALK_METHOD_REQUEST_CURRENCY_REVISION = 145,
    CCTALK_METHOD_OPERATE_BI_DIRECTIONAL_MOTORS = 146,
    CCTALK_METHOD_PERFORM_STACKER_CYCLE = 147,
    CCTALK_METHOD_READ_OPTO_VOLTAGES = 148,
    CCTALK_METHOD_REQUEST_INDIVIDUAL_ERROR_COUNTER = 149,
    CCTALK_METHOD_REQUEST_INDIVIDUAL_ACCEPT_COUNTER = 150,
    CCTALK_METHOD_TEST_LAMPS = 151,
    CCTALK_METHOD_REQUEST_BILL_OPERATING_MODE = 152,
    CCTALK_METHOD_MODIFY_BILL_OPERATING_MODE = 153,
    CCTALK_METHOD_ROUTE_BILL = 154,
    CCTALK_METHOD_REQUEST_BILL_POSITION = 155,
    CCTALK_METHOD_REQUEST_COUNTRY_SCALING_FACTOR = 156,
    CCTALK_METHOD_REQUEST_BILL_ID = 157,
    CCTALK_METHOD_MODIFY_BILL_ID = 158,
    CCTALK_METHOD_READ_BUFFERED_BILL_EVENTS = 159,
    CCTALK_METHOD_REQUEST_CIPHER_KEY = 160,
    CCTALK_METHOD_PUMP_RNG = 161,
    CCTALK_METHOD_MODIFY_INHIBIT_AND_OVERRIDE_REGISTERS = 162,
    CCTALK_METHOD_TEST_HOPPER = 163,
    CCTALK_METHOD_ENABLE_HOPPER = 164,
    CCTALK_METHOD_MODIFY_VARIABLE_SET = 165,
    CCTALK_METHOD_REQUEST_HOPPER_STATUS = 166,
    CCTALK_METHOD_DISPENSE_HOPPER_COINS = 167,
    CCTALK_METHOD_REQUEST_HOPPER_DISPENSE_COUNT = 168,
    CCTALK_METHOD_REQUEST_ADDRESS_MODE = 169,
    CCTALK_METHOD_REQUEST_BASE_YEAR = 170,
    CCTALK_METHOD_REQUEST_HOPPER_COIN = 171,
    CCTALK_METHOD_EMERGENCY_STOP = 172,
    CCTALK_METHOD_REQUEST_THERMISTOR_READING = 173,
    CCTALK_METHOD_REQUEST_PAYOUT_FLOAT = 174,
    CCTALK_METHOD_MODIFY_PAYOUT_FLOAT = 175,
    CCTALK_METHOD_REQUEST_ALARM_COUNTER = 176,
    CCTALK_METHOD_HANDHELD_FUNCTION = 177,
    CCTALK_METHOD_REQUEST_BANK_SELECT = 178,
    CCTALK_METHOD_MODIFY_BANK_SELECT = 179,
    CCTALK_METHOD_REQUEST_SECURITY_SETTING = 180,
    CCTALK_METHOD_MODIFY_SECURITY_SETTING = 181,
    CCTALK_METHOD_DOWNLOAD_CALIBRATION_INFO = 182,
    CCTALK_METHOD_UPLOAD_WINDOW_DATA = 183,
    CCTALK_METHOD_REQUEST_COIN_ID = 184,
    CCTALK_METHOD_MODIFY_COIN_ID = 185,
    CCTALK_METHOD_REQUEST_PAYOUT_CAPACITY = 186,
    CCTALK_METHOD_MODIFY_PAYOUT_CAPACITY = 187,
    CCTALK_METHOD_REQUEST_DEFAULT_SORTER_PATH = 188,
    CCTALK_METHOD_MODIFY_DEFAULT_SORTER_PATH = 189,
    CCTALK_METHOD_KEYPAD_CONTROL = 191,
    CCTALK_METHOD_REQUEST_BUILD_CODE = 192,
    CCTALK_METHOD_REQUEST_FRAUD_COUNTER = 193,	
    CCTALK_METHOD_REQUEST_REJECT_COUNTER = 194,
    CCTALK_METHOD_REQUEST_LAST_MODIFICATION_DATE = 195,
    CCTALK_METHOD_REQUEST_CREATION_DATE = 196,
    CCTALK_METHOD_CALCULATE_ROM_CHECKSUM = 197,
    CCTALK_METHOD_COUNTERS_TO_EEPROM = 198,
    CCTALK_METHOD_CONFIGURATION_TO_EEPROM = 199,
    CCTALK_METHOD_ACMI_UNENCRYPTED_PRODUCT_ID = 200,
    CCTALK_METHOD_REQUEST_TEACH_STATUS = 201,
    CCTALK_METHOD_TEACH_MODE_CONTROL_=202,
    CCTALK_METHOD_DISPLAY_CONTROL = 203,
    CCTALK_METHOD_METER_CONTROL = 204,
    
    CCTALK_METHOD_REQUEST_PAYOUT_ABSOLUTE_COUNT = 207,
    CCTALK_METHOD_MODIFY_PAYOUT_ABSOLUTE_COUNT = 208,
    CCTALK_METHOD_REQUEST_SORTER_PATHS = 209,
    CCTALK_METHOD_MODIFY_SORTER_PATHS = 210,
    CCTALK_METHOD_POWER_MANAGEMENT_CONTROL = 211,
    CCTALK_METHOD_REQUEST_COIN_POSITION = 212,
    CCTALK_METHOD_REQUEST_OPTION_FLAGS = 213,
    CCTALK_METHOD_WRITE_DATA_BLOCK = 214,
    CCTALK_METHOD_READ_DATA_BLOCK = 215,
    CCTALK_METHOD_REQUEST_DATA_STORAGE_AVAILABILITY = 216,
    CCTALK_METHOD_REQUEST_PAYOUT_HIGH_LOW_STATUS = 217,
    CCTALK_METHOD_ENTER_PIN_NUMBER = 218,
    CCTALK_METHOD_ENTER_NEW_PIN_NUMBER = 219,
    CCTALK_METHOD_ACMI_ENCRYPTED_DATA = 220,
    CCTALK_METHOD_REQUEST_SORTER_OVERRIDE_STATUS = 221,
    CCTALK_METHOD_MODIFY_SORTER_OVERRIDE_STATUS = 222,
    CCTALK_METHOD_MODIFY_ENCRYPTED_INHIBIT_AND_OVERRIDE_REGISTERS = 223,
    CCTALK_METHOD_REQUEST_ENCRYPTED_PRODUCT_ID = 224,
    CCTALK_METHOD_REQUEST_ACCEPT_COUNTER = 225,
    CCTALK_METHOD_REQUEST_INSERTION_COUNTER = 226,
    CCTALK_METHOD_REQUEST_MASTER_INHIBIT_STATUS = 227,
    CCTALK_METHOD_MODIFY_MASTER_INHIBIT_STATUS = 228,
    CCTALK_METHOD_READ_BUFFERED_CREDIT_OR_ERROR_CODES = 229,
    CCTALK_METHOD_REQUEST_INHIBIT_STATUS = 230,
    CCTALK_METHOD_MODIFY_INHIBIT_STATUS = 231,
    CCTALK_METHOD_PERFORM_SELF_CHECK = 232,
    CCTALK_METHOD_LATCH_OUTPUT_LINES = 233,
    CCTALK_METHOD_SEND_DH_PUBLIC_KEY = 234,
    CCTALK_METHOD_READ_DH_PUBLIC_KEY = 235,
    CCTALK_METHOD_READ_OPTO_STATES = 236,
    CCTALK_METHOD_READ_INPUT_LINES = 237,
    CCTALK_METHOD_TEST_OUTPUT_LINES = 238,
    CCTALK_METHOD_OPERATE_MOTORS = 239,
    CCTALK_METHOD_TEST_SOLENOIDS = 240,
    CCTALK_METHOD_REQUEST_SOFTWARE_REVISION = 241,
    CCTALK_METHOD_REQUEST_SERIAL_NUMBER = 242,
    CCTALK_METHOD_REQUEST_DATABASE_VERSION = 243,
    CCTALK_METHOD_REQUEST_PRODUCT_CODE = 244,
    CCTALK_METHOD_REQUEST_EQUIPMENT_CATEGORY_ID = 245,
    CCTALK_METHOD_REQUEST_MANUFACTURER_ID = 246,
    CCTALK_METHOD_REQUEST_VAR_SET = 247,
    CCTALK_METHOD_REQUEST_STATUS = 248,
    CCTALK_METHOD_REQUEST_POLLING_PRIOTRITY = 249,
    CCTALK_METHOD_ADDRESS_RANDOM = 250,
    CCTALK_METHOD_ADDRESS_CHANGE = 251,
    CCTALK_METHOD_ADDRESS_CLASH = 252,
    CCTALK_METHOD_ADDRESS_POLL = 253,
    CCTALK_METHOD_SIMPLE_POLL = 254,
    CCTALK_METHOD_FACTORY_SET_UP_AND_TEST = 255
}; 

enum cctalk_crc_mode  {
	/* Simple 256-complement, 1 byte checksum after every message. */
	CCTALK_CRC_SIMPLE = 0,

	/* Full CRC-16-CCITT checksum using source address field and one
	 * more byte after every message. */
	CCTALK_CRC_CCITT = 1,
}; 

#define HOPPER_STATUS_LOW (0x02)
#define HOPPER_STATUS_HIGH (0x04)


#define ACT_L_R_SINGULATOR_RUNNING 		(0x01)
#define ACT_L_R_ESCALATOR_RUNNING			(0x02)
#define ACT_L_R_ACCEPTING_COIN 				(0x04)
#define ACT_L_R_DISPENSING_COIN 			(0x08)
#define ACT_L_R_FAULT_DETECTED		 		(0x10)
#define ACT_L_R_AVALANCHE_DETECTED		(0x20)
#define ACT_L_R_INITIALISHING 				(0x40)
#define ACT_L_R_ENTRYFLAPOPEN	 				(0x80)

#define ACT_H_R_COINTINUOUS_REJECTS		(0x01)
#define ACT_H_R_HOPPER_CONF_CHANGED		(0x02)
#define ACT_H_R_REJECTDIVERT_ACTIVE 	(0x04)
#define ACT_H_R_EXIT_CUP_FULL 				(0x08)
#define ACT_H_R_NORMAL_FAULT_DETECTED	(0x10)
#define ACT_H_R_IN_TEST_MODE					(0x20)


typedef struct
{
	uint8_t dispense_event_ctr;
	uint8_t act_resister_l;
	uint8_t act_resister_h;
	uint8_t hopper_status[HOPPER_NUM];
	uint16_t coin_master_inhibit_status;
	uint16_t coin_inhibit_status;
	uint32_t cctalk_version;
	uint32_t payed_money_out;
	uint32_t unpayed_money_out;
}s_cctalk_env;




extern s_cctalk_env cctalk_env;

void cctalk_env_init (void);
int cctalk_protocol (char *buf, uint32_t len);
void update_hopper_status (void);
void set_active_resister (uint8_t reg_l, uint8_t reg_h);
void reset_active_resister (uint8_t reg_l, uint8_t reg_h);





#endif

 

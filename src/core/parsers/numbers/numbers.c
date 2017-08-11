
/**
 * @file /libcore/core/parsers/numbers/numbers.c
 *
 * @brief	Functions for converting different string types into binary numbers.
 */

#include "core.h"

/**
 * @brief	Convert a managed string to a float.
 * @param	s		the managed string to be converted.
 * @param	number	a pointer to a float where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t float_conv(stringer_t *s, float_t *number) {

	chr_t *end;
	float_t output;

	if (!number || st_empty(s)) {
		return false;
	}

	*number = 0.0f;
	end = st_char_get(s) + st_length_get(s);
	output = strtof(st_char_get(s), &end);

	if (st_char_get(s) == end) {
		log_pedantic("The provided string could not be converted to a float. {s = %.*s}", st_length_int(s), st_char_get(s));
		return false;
	}
	else if (output == HUGE_VALF && errno == ERANGE) {
		log_pedantic("Numeric overflow. {s = %.*s}", st_length_int(s), st_char_get(s));
		return false;
	}

	*number = output;
	return true;
}

/**
 * @brief	Convert a managed string to a a double.
 * @param	s		the managed string to be converted.
 * @param	number	a pointer to a double where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t double_conv(stringer_t *s, double_t *number) {

	chr_t *end;
	double_t output;

	if (!number || st_empty(s)) {
		return false;
	}

	*number = 0.0f;
	end = st_char_get(s) + st_length_get(s);
	output = strtod(st_char_get(s), &end);

	if (st_char_get(s) == end) {
		log_pedantic("The provided string could not be converted to a double. {s = %.*s}", st_length_int(s), st_char_get(s));
		return false;
	}
	else if (output == HUGE_VALF && errno == ERANGE) {
		log_pedantic("Numeric overflow. {s = %.*s}", st_length_int(s), st_char_get(s));
		return false;
	}

	*number = output;
	return true;
}

/**
 * @brief	Convert a numerical string into a size_t value.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to an size_t value where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t size_conv_bl(void *block, size_t length, size_t *number) {
	return uint64_conv_bl(block, length, number);
}

/**
 * @brief	Convert a numerical string into an ssize_t value.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to an ssize_t value where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t ssize_conv_bl(void *block, size_t length, ssize_t *number) {
	return int64_conv_bl(block, length, number);
}

/**
 * @brief	Convert a numerical string into an unsigned 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to an unsigned 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint64_conv_bl(void *block, size_t length, uint64_t *number) {

	char *data;
	uint64_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}

		before = *number;
		*number += (*data-- - '0') * add;

		if (*number < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}

		before = add;
		add *= 10;

		if (add < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into an unsigned 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to an unsigned 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint64_conv_ns(char *string, uint64_t *number) {
	return uint64_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into an unsigned 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to an unsigned 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint64_conv_st(stringer_t *string, uint64_t *number) {
	return uint64_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a placer into an unsigned 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the placer to be converted.
 * @param	number	a pointer to an unsigned 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint64_conv_pl(placer_t string, uint64_t *number) {
	return uint64_conv_bl(pl_data_get(string), pl_length_get(string), number);
}

/**
 * @brief	Convert a numerical string into an unsigned 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to an unsigned 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint32_conv_bl(void *block, size_t length, uint32_t *number) {

	char *data;
	uint32_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}

		before = *number;
		*number += (*data-- - '0') * add;

		if (*number < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}

		before = add;
		add *= 10;

		if (add < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into an unsigned 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to an unsigned 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint32_conv_ns(char *string, uint32_t *number) {
	return uint32_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into an unsigned 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to an unsigned 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t uint32_conv_st(stringer_t *string, uint32_t *number) {
	return uint32_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string to a 16-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the block of memory to be converted.
 * @param	length	the length, in bytes, of the numerical string.
 * @param	number	a pointer to an unsigned 16-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint16_conv_bl(void *block, size_t length, uint16_t *number) {

	char *data;
	uint16_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}

		before = *number;
		*number += (*data-- - '0') * add;

		if (*number < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}

		before = add;
		add *= 10;

		if (add < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}
	}

	return true;
}

/**
 * @brief	Convert a numerical string to a 16-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to a null-terminated string containing the data to be converted.
 * @param	number	a pointer to an unsigned 16-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint16_conv_ns(char *string, uint16_t *number) {
	return uint16_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a numerical string to a 16-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to a managed string containing the data to be converted.
 * @param	number	a pointer to an unsigned 16-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint16_conv_st(stringer_t *string, uint16_t *number) {
	return uint16_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string to an unsigned 8-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the block of memory to be converted.
 * @param	length	the length, in bytes, of the numerical string.
 * @param	number	a pointer to an unsigned 8-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint8_conv_bl(void *block, size_t length, uint8_t *number) {

	char *data;
	uint8_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}

		before = *number;
		*number += (*data-- - '0') * add;

		if (*number < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}

		before = add;
		add *= 10;

		if (add < before) {
			log_pedantic("Numeric overflow.");
			return false;
		}
	}

	return true;
}

/**
 * @brief	Convert a numerical string to an 8-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to a null-terminated string containing the data to be converted.
 * @param	number	a pointer to an unsigned 8-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint8_conv_ns(char *string, uint8_t *number) {
	return uint8_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a numerical string to an 8-bit unsigned integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to a managed string containing the data to be converted.
 * @param	number	a pointer to an unsigned 8-bit integer that will hold the result of the conversion.
 * @return	true on success, or false on failure.
 */
bool_t uint8_conv_st(stringer_t *string, uint8_t *number) {
	return uint8_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string into a signed 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to a signed 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int64_conv_bl(void *block, size_t length, int64_t *number) {

	char *data;
	int64_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		// Check to see if the number leads with a negative sign. If so, multiply by -1.
		if (i == (length - 1) && (*data == '+' || *data == '-')) {
			if (*data == '-') {
				*number *= -1;
			}
		}
		else if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}
		else {

			before = *number;
			*number += (*data-- - '0') * add;

			if (*number < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}

			before = add;
			add *= 10;

			if (add < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into a signed 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to a signed 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int64_conv_ns(char *string, int64_t *number) {
	return int64_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into a signed 64-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to a signed 64-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int64_conv_st(stringer_t *string, int64_t *number) {
	return int64_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string into a signed 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to a signed 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int32_conv_bl(void *block, size_t length, int32_t *number) {

	char *data;
	int32_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		// Check to see if the number leads with a negative sign. If so, multiply by -1.
		if (i == (length - 1) && (*data == '+' || *data == '-')) {
			if (*data == '-') {
				*number *= -1;
			}
		}
		else if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}
		else {

			before = *number;
			*number += (*data-- - '0') * add;

			if (*number < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}

			before = add;
			add *= 10;

			if (add < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into a signed 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to a signed 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int32_conv_ns(char *string, int32_t *number) {
	return int32_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into a signed 32-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to a signed 32-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int32_conv_st(stringer_t *string, int32_t *number) {
	return int32_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string into a signed 16-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to a signed 16-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int16_conv_bl(void *block, size_t length, int16_t *number) {

	char *data;
	int16_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		// Check to see if the number leads with a negative sign. If so, multiply by -1.
		if (i == (length - 1) && (*data == '+' || *data == '-')) {
			if (*data == '-') {
				*number *= -1;
			}
		}
		else if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}
		else {

			before = *number;
			*number += (*data-- - '0') * add;

			if (*number < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}

			before = add;
			add *= 10;

			if (add < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into a signed 16-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to a signed 16-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int16_conv_ns(char *string, int16_t *number) {
	return int16_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into a signed 16-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to a signed 16-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int16_conv_st(stringer_t *string, int16_t *number) {
	return int16_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Convert a numerical string into a signed 8-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	block	a pointer to the numerical string to be converted.
 * @param	length	the length, in bytes, of the numerical data.
 * @param	number	a pointer to a signed 8-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int8_conv_bl(void *block, size_t length, int8_t *number) {

	char *data;
	int16_t add = 1, before;

	if (!block || !length || !number) {
		log_pedantic("A NULL parameter was passed in.");
		return false;
	}

	*number = 0;
	data = block;
	data += length - 1;

	for (size_t i = 0; i < length; i++) {

		// Check to see if the number leads with a negative sign. If so, multiply by -1.
		if (i == (length - 1) && (*data == '+' || *data == '-')) {
			if (*data == '-') {
				*number *= -1;
			}
		}
		else if (*data < '0' || *data > '9') {
			log_pedantic("Non numeric data found. {%c}", *data);
			return false;
		}
		else {

			before = *number;
			*number += (*data-- - '0') * add;

			if (*number < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}

			before = add;
			add *= 10;

			if (add < before) {
				log_pedantic("Numeric overflow.");
				return false;
			}
		}
	}

	return true;
}

/**
 * @brief	Convert a null-terminated string into a signed 8-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the null-terminated string to be converted.
 * @param	number	a pointer to a signed 8-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int8_conv_ns(char *string, int8_t *number) {
	return int8_conv_bl(string, ns_length_get(string), number);
}

/**
 * @brief	Convert a managed string into a signed 8-bit integer.
 * @note	This function checks for both numeric underflows and overflows.
 * @param	string	a pointer to the managed string to be converted.
 * @param	number	a pointer to a signed 8-bit number where the result of the conversion will be stored.
 * @return	true on success or false on failure.
 */
bool_t int8_conv_st(stringer_t *string, int8_t *number) {
	return int8_conv_bl(st_data_get(string), st_length_get(string), number);
}

/**
 * @brief	Create a stringer, storing a 4-byte value in network byte order.
 * @param	s		a pointer to the data buffer where the value will be inserted.
 * @param	val		the 4 byte value to be placed in the specified buffer, in network byte order.
 * @return	Pointer to stringer on success, NULL on failure.
 */
stringer_t * uint32_put_no(uint32_t val) {

	unsigned char *data;
	stringer_t *result;

	if(!(result = st_alloc(4))) {
		log_pedantic("Failed to allocate stringer");
		return NULL;
	}

	if(!(data = st_data_get(result))) {
		log_pedantic("Failed to retrieve data pointer from stringer");
		return NULL;
	}

	data[0] = (val & 0xff000000) >> 24;
	data[1] = (val & 0x00ff0000) >> 16;
	data[2] = (val & 0x0000ff00) >> 8;
	data[3] = val & 0x000000ff;

	st_length_set(result, 4);
	return result;
}

/**
 * @brief	Create a stringer, storing a 3-byte value in network byte order.
 * @param	s		a pointer to the data buffer where the value will be inserted.
 * @param	val		the 3 byte value to be placed in the specified buffer, in network byte order.
 * @return	Pointer to stringer on success, NULL on failure.
 */
stringer_t * uint24_put_no(uint32_t val) {

	unsigned char *data;
	stringer_t *result;

	if(!(result = st_alloc(3))) {
		log_pedantic("Failed to allocate stringer");
		return NULL;
	}

	if(!(data = st_data_get(result))) {
		log_pedantic("Failed to retrieve data pointer from stringer");
		return NULL;
	}

	data[0] = (val & 0x00ff0000) >> 16;
	data[1] = (val & 0x0000ff00) >> 8;
	data[2] = val & 0x000000ff;

	st_length_set(result, 3);
	return result;
}

/**
 * @brief	Create a stringer, storing a 2-byte value in network byte order.
 * @param	s		a pointer to the data buffer where the value will be inserted.
 * @param	val		the 2 byte value to be placed in the specified buffer, in network byte order.
 * @return	Pointer to stringer on success, NULL on failure.
 */
stringer_t * uint16_put_no(uint16_t val) {

	unsigned char *data;
	stringer_t *result;

	if(!(result = st_alloc(2))) {
		log_pedantic("Failed to allocate stringer");
		return NULL;
	}

	if(!(data = st_data_get(result))) {
		log_pedantic("Failed to retrieve data pointer from stringer");
		return NULL;
	}

	data[0] = (val & 0x0000ff00) >> 8;
	data[1] = val & 0x000000ff;

	st_length_set(result, 2);
	return result;
}

/**
 * @brief	Fetch a 4 byte network order value from a stringer and return it in host byte order.
 * @param	buf	a pointer to the data buffer from which the bytes will be read.
 * @return	the value of the first 4 network order bytes in the buffer in host byte order. 0 on failure.
 */
uint32_t uint32_get_no(stringer_t *s) {

	unsigned char *data;
	uint32_t result = 0;

	if(!s || st_empty(s) || st_length_get(s) < 4) {
		log_pedantic("Empty or invalid stringer was passed.");
		return 0;
	}
	else if (!(data = st_data_get(s))) {
		log_pedantic("No data buffer found in stringer.");
	}

	result |= (data[0] << 24);
	result |= (data[1] << 16);
	result |= (data[2] << 8);
	result |= data[3];

	return result;
}

/**
 * @brief	Fetch a 3 byte network order value from a stringer and return it in host byte order.
 * @param	buf	a pointer to the data buffer from which the bytes will be read.
 * @return	the value of the first 3 network order bytes in the buffer in host byte order. 0 on failure.
 */
uint32_t uint24_get_no(stringer_t *s) {

	unsigned char *data;
	uint32_t result = 0;

	if(!s || st_empty(s) || st_length_get(s) < 3) {
		log_pedantic("Empty or invalid stringer was passed.");
		return 0;
	}
	else if (!(data = st_data_get(s))) {
		log_pedantic("No data buffer found in stringer.");
	}

	result |= (data[0] << 16);
	result |= (data[1] << 8);
	result |= data[2];

	return result;
}

/**
 * @brief	Fetch a 2 byte network order value from a stringer and return it in host byte order.
 * @param	buf	a pointer to the data buffer from which the bytes will be read.
 * @return	the value of the first 2 network order bytes in the buffer in host byte order. 0 on failure.
 */
uint16_t uint16_get_no(stringer_t *s) {

	unsigned char *data;
	uint16_t result = 0;

	if(!s || st_empty(s) || st_length_get(s) < 2) {
		log_pedantic("Empty or invalid stringer was passed.");
		return 0;
	}
	else if (!(data = st_data_get(s))) {
		log_pedantic("No data buffer found in stringer.");
	}

	result |= (data[0] << 8);
	result |= data[1];

	return result;
}


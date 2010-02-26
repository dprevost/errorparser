/*
 * This file was generated by the program errorParser
 * using the input file ../tests/xml/generic.xml.
 * Date: Wed Dec 31 17:38:21 2008.
 *
 * The version of the XML error file is 0.3.
 *
 * Copyright (C) 2006-2008 Daniel Prevost
 *
 * This file is part of Photon.
 *
 * This file may be distributed and/or modified under the terms of the 
 * GNU General Public License version 2 as published by the Free 
 * Software Foundation and appearing in the file COPYING included in 
 * the packaging of this library.
 *
 * This library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#ifndef HEADER_H
#define HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

enum my_enum
{
    /**
     * No error...
     */
    MY_PROG_OK = 0,

    /**
     * Abnormal internal error. It should not happen!
     */
    MY_PROG_INTERNAL_ERROR = 666,

    /**
     * Cannot get a lock on a system object, the engine is "busy".
     *
     * This might be the result of either a very busy system where 
     * unused cpu cycles are rare or a lock might be held by a crashed 
     * process.
     */
    MY_PROG_ENGINE_BUSY = 1,

    /**
     * Not enough memory in the PSO.
     */
    MY_PROG_NOT_ENOUGH_PSO_MEMORY = 2,

    /**
     * Not enough heap memory (non-PSO memory).
     */
    MY_PROG_NOT_ENOUGH_HEAP_MEMORY = 3,

    /**
     * There are not enough resources to correctly process the call.
     *
     * Possible explanations: it might be a lack of POSIX semaphores on 
     * systems where atomic locks are implemented that way or a failure 
     * in initializing a pthread_mutex (or on Windows, a critical 
     * section).
     */
    MY_PROG_NOT_ENOUGH_RESOURCES = 4,

    /**
     * The provided handle is of the wrong type (C API). This could 
     * happen if you provide a queue handle to access a hash map or 
     * something similar. It can also occur if you try to access an 
     * object after closing it.
     *
     * If you are seeing this error for the C++ API (or some other 
     * object-oriented interface), you've just found an internal 
     * error... (the handle is encapsulated and cannot be modified 
     * using the public interface).
     */
    MY_PROG_WRONG_TYPE_HANDLE = 5,

    /**
     * The provided handle is NULL.
     */
    MY_PROG_NULL_HANDLE = 6,

    /**
     * One of the arguments of an API function is an invalid NULL 
     * pointer.
     */
    MY_PROG_NULL_POINTER = 7,

    /**
     * An invalid length was provided as an argument to an API 
     * function.
     *
     * This invalid length will usually indicate that the length value 
     * is set to zero.
     */
    MY_PROG_INVALID_LENGTH = 8,

    /**
     * The process was already initialized. One possibility: was 
     * psoInit() called for a second time?
     */
    MY_PROG_PROCESS_ALREADY_INITIALIZED = 21,

    /**
     * The process was not properly initialized. One possibility: was 
     * psoInit() called?
     */
    MY_PROG_PROCESS_NOT_INITIALIZED = 22,

    /**
     * The watchdog address is invalid (empty string, NULL pointer, 
     * etc.).
     */
    MY_PROG_INVALID_WATCHDOG_ADDRESS = 23,

    /**
     * API - memory-file version mismatch.
     */
    MY_PROG_INCOMPATIBLE_VERSIONS = 24,

    /**
     * Generic socket error.
     */
    MY_PROG_SOCKET_ERROR = 25,

    /**
     * Socket error when trying to connect to the watchdog.
     */
    MY_PROG_CONNECT_ERROR = 26,

    /**
     * Socket error when trying to send a request to the watchdog.
     */
    MY_PROG_SEND_ERROR = 27,

    /**
     * Socket error when trying to receive a reply from the watchdog.
     */
    MY_PROG_RECEIVE_ERROR = 28,

    /**
     * The shared-memory backstore file is missing. The name of this 
     * file is provided by the server (quasar) - if it is missing, 
     * something really weird is going on.
     */
    MY_PROG_BACKSTORE_FILE_MISSING = 29,

    /**
     * Generic i/o error when attempting to open the shared memory.
     */
    MY_PROG_ERROR_OPENING_MEMORY = 30,

    /**
     * Error accessing the directory for the log files or error opening 
     * the log file itself.
     */
    MY_PROG_LOGFILE_ERROR = 41,

    /**
     * Cannot get a lock on the session (a pthread_mutex or a critical 
     * section on Windows).
     */
    MY_PROG_SESSION_CANNOT_GET_LOCK = 42,

    /**
     * An attempt was made to use a session object (a session handle) 
     * after this session was terminated.
     */
    MY_PROG_SESSION_IS_TERMINATED = 43,

    /**
     * Permitted characters for names are alphanumerics, spaces (' '), 
     * dashes ('-') and underlines ('_'). The first character must be 
     * alphanumeric.
     */
    MY_PROG_INVALID_OBJECT_NAME = 51,

    /**
     * The object was not found (but its folder does exist).
     */
    MY_PROG_NO_SUCH_OBJECT = 52,

    /**
     * One of the parent folder of an object does not exist.
     */
    MY_PROG_NO_SUCH_FOLDER = 53,

    /**
     * Attempt to create an object which already exists.
     */
    MY_PROG_OBJECT_ALREADY_PRESENT = 54,

    /**
     * The object (data container) is empty.
     */
    MY_PROG_IS_EMPTY = 55,

    /**
     * Attempt to create an object of an unknown object type or to open 
     * an object of the wrong type.
     */
    MY_PROG_WRONG_OBJECT_TYPE = 56,

    /**
     * Cannot get lock on the object. This might be the result of 
     * either a very busy system where unused cpu cycles are rare or a 
     * lock might be held by a crashed process.
     */
    MY_PROG_OBJECT_CANNOT_GET_LOCK = 57,

    /**
     * The search/iteration reached the end without finding a new 
     * item/record.
     */
    MY_PROG_REACHED_THE_END = 58,

    /**
     * An invalid value was used for a psoIteratorType parameter.
     */
    MY_PROG_INVALID_ITERATOR = 59,

    /**
     * The name of the object is too long. The maximum length of a name 
     * cannot be more than PSO_MAX_NAME_LENGTH (or 
     * PSO_MAX_FULL_NAME_LENGTH for the fully qualified name).
     */
    MY_PROG_OBJECT_NAME_TOO_LONG = 60,

    /**
     * You cannot delete a folder if there are still undeleted objects 
     * in it.
     *
     * Technical: a folder does not need to be empty to be deleted but 
     * all objects in it must be "marked as deleted" by the current 
     * session. This enables writing recursive deletions
     */
    MY_PROG_FOLDER_IS_NOT_EMPTY = 61,

    /**
     * An item with the same key was found.
     */
    MY_PROG_ITEM_ALREADY_PRESENT = 62,

    /**
     * The item was not found in the hash map.
     */
    MY_PROG_NO_SUCH_ITEM = 63,

    /**
     * The object is scheduled to be deleted soon. Operations on this 
     * data container are not permitted at this time.
     */
    MY_PROG_OBJECT_IS_DELETED = 64,

    /**
     * Object must be open first before you can access them.
     */
    MY_PROG_OBJECT_NOT_INITIALIZED = 65,

    /**
     * The data item is scheduled to be deleted soon or was just 
     * created and is not committed. Operations on this data item are 
     * not permitted at this time.
     */
    MY_PROG_ITEM_IS_IN_USE = 66,

    /**
     * The data item is scheduled to be deleted soon. Operations on 
     * this data container are not permitted at this time.
     */
    MY_PROG_ITEM_IS_DELETED = 67,

    /**
     * The object is scheduled to be deleted soon or was just created 
     * and is not committed. Operations on this object are not 
     * permitted at this time.
     */
    MY_PROG_OBJECT_IS_IN_USE = 69,

    /**
     * The object is read-only and update operations 
     * (delete/insert/replace) on it are not permitted. at this time.
     */
    MY_PROG_OBJECT_IS_READ_ONLY = 70,

    /**
     * All read-only objects open for updates (as temporary objects) 
     * must be closed prior to doing a commit on the session.
     */
    MY_PROG_NOT_ALL_EDIT_ARE_CLOSED = 71,

    /**
     * Read-only objects are not updated very frequently and therefore 
     * only a single editing copy is allowed. To allow concurrent 
     * editors (either all working on the same copy or each working 
     * with its own copy would have been be possible but was deemed 
     * unnecessary.
     */
    MY_PROG_A_SINGLE_UPDATER_IS_ALLOWED = 72,

    /**
     * The number of fields in the data definition is invalid - either 
     * zero or greater than PSO_MAX_FIELDS (defined in 
     * photon/psoCommon.h).
     */
    MY_PROG_INVALID_NUM_FIELDS = 101,

    /**
     * The data type of the field definition does not correspond to one 
     * of the data type defined in the enum psoFieldType 
     * (photon/psoCommon.h). or you've used PSO_VAR_STRING or 
     * PSO_VAR_BINARY at the wrong place.
     *
     * Do not forget that PSO_VAR_STRING and PSO_VAR_BINAR can only be 
     * used for the last field of your data definition.
     */
    MY_PROG_INVALID_FIELD_TYPE = 102,

    /**
     * The length of an integer field (PSO_INTEGER) is invalid. Valid 
     * values are 1, 2, 4 and 8.
     */
    MY_PROG_INVALID_FIELD_LENGTH_INT = 103,

    /**
     * The length of a field (string or binary) is invalid. Valid 
     * values are all numbers greater than zero and less than 
     * 4294967296 (4 Giga).
     */
    MY_PROG_INVALID_FIELD_LENGTH = 104,

    /**
     * The name of the field contains invalid characters. Valid 
     * characters are the standard ASCII alphanumerics ([a-zA-Z0-9]) 
     * and the underscore ('_'). The first character of the name must 
     * be letter.
     */
    MY_PROG_INVALID_FIELD_NAME = 105,

    /**
     * The name of the field is already used by another field in the 
     * current definition.
     *
     * Note: at the moment field names are case sensitive (for example 
     * "account_id" and "Account_Id" are considered different). This 
     * might be changed eventually so this practice should be avoided.
     */
    MY_PROG_DUPLICATE_FIELD_NAME = 106,

    /**
     * The precision of a PSO_DECIMAL field is either zero or over the 
     * limit for this type (set at 30 currently). Note: precision is 
     * the number of digits in a number.
     */
    MY_PROG_INVALID_PRECISION = 107,

    /**
     * The scale of a PSO_DECIMAL field is invalid (greater than the 
     * value of precision. Note: scale is the number of digits to the 
     * right of the decimal separator in a number.
     */
    MY_PROG_INVALID_SCALE = 108,

    /**
     * The key definition for a hash map is either invalid or missing.
     */
    MY_PROG_INVALID_KEY_DEF = 109,

    /*
     * XML Errors
     *
     * A group of XML related errors (XML can be used to define data 
     * containers).
     */

    /**
     * Error reading the XML buffer stream.
     *
     * No validation is done at this point. Therefore the error is 
     * likely something like a missing end-tag or some other 
     * non-conformance to the XML's syntax rules.
     *
     * A simple Google search for "well-formed xml" returns many web 
     * sites that describe the syntax rules for XML. You can also use 
     * the program xmllint (included in the distribution of libxm2) to 
     * pinpoint the issue.
     */
    MY_PROG_XML_READ_ERROR = 201,

    /**
     * The root element is not the expected root, <folder> and similar.
     */
    MY_PROG_XML_INVALID_ROOT = 202,

    /**
     * The root element must have an attribute named schemaLocation (in 
     * the namespace "http://www.w3.org/2001/XMLSchema-instance") to 
     * point to the schema use for the xml buffer stream.
     *
     * This attribute is in two parts separated by a space. The code 
     * expects the file name of the schema in the second element of 
     * this attribute.
     */
    MY_PROG_XML_NO_SCHEMA_LOCATION = 203,

    /**
     * The creation of a new schema parser context failed. There might 
     * be multiple reasons for this, for example, a memory-allocation 
     * failure in libxml2. However, the most likely reason is that the 
     * schema file is not at the location indicated by the attribute 
     * schemaLocation of the root element of the buffer stream.
     */
    MY_PROG_XML_PARSER_CONTEXT_FAILED = 204,

    /**
     * The parse operation of the schema failed. Most likely, there is 
     * an error in the schema. To debug this you can use xmllint (part 
     * of the libxml2 package).
     */
    MY_PROG_XML_PARSE_SCHEMA_FAILED = 205,

    /**
     * The creation of a new schema validation context failed. There 
     * might be multiple reasons for this, for example, a 
     * memory-allocation failure in libxml2.
     */
    MY_PROG_XML_VALID_CONTEXT_FAILED = 206,

    /**
     * Document validation for the xml buffer failed. To debug this 
     * problem you can use xmllint (part of the libxml2 package).
     */
    MY_PROG_XML_VALIDATION_FAILED = 207,

    /**
     * Abnormal internal error with sem_destroy. It should not happen! 
     * It could indicate that the memory allocated for the semaphore 
     * was corrupted (errno = EINVAL) or that the reference counting is 
     * wrong (errno = EBUSY). Please contact us.
     */
    MY_PROG_SEM_DESTROY_ERROR = 601
};

typedef enum my_enum my_enum;

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */

#ifdef __cplusplus
}
#endif

#endif /* HEADER_H */

/* --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- */


# This file was generated by the program errorParser
# using the input file ../tests/xml/generic.xml.
# Date: Tue Dec 30 18:16:09 2008.

# The version of the XML error file is 0.3.

# Copyright (C) 2006-2008 Daniel Prevost
#
# This file is part of Photon.
#
# This file may be distributed and/or modified under the terms of the 
# GNU General Public License version 2 as published by the Free 
# Software Foundation and appearing in the file COPYING included in the 
# packaging of this library.
#
# This library is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

def add_errors():

    errors = dict()
    error_names = dict()

    # No error...
    errors['OK'] = 0
    error_names[0] = 'OK'

    # Abnormal internal error. It should not happen!
    errors['INTERNAL_ERROR'] = 666
    error_names[666] = 'INTERNAL_ERROR'

    # Cannot get a lock on a system object, the engine is "busy".
    #
    # This might be the result of either a very busy system where 
    # unused cpu cycles are rare or a lock might be held by a crashed 
    # process.
    errors['ENGINE_BUSY'] = 1
    error_names[1] = 'ENGINE_BUSY'

    # Not enough memory in the PSO.
    errors['NOT_ENOUGH_PSO_MEMORY'] = 2
    error_names[2] = 'NOT_ENOUGH_PSO_MEMORY'

    # Not enough heap memory (non-PSO memory).
    errors['NOT_ENOUGH_HEAP_MEMORY'] = 3
    error_names[3] = 'NOT_ENOUGH_HEAP_MEMORY'

    # There are not enough resources to correctly process the call.
    #
    # Possible explanations: it might be a lack of POSIX semaphores on 
    # systems where atomic locks are implemented that way or a failure 
    # in initializing a pthread_mutex (or on Windows, a critical 
    # section).
    errors['NOT_ENOUGH_RESOURCES'] = 4
    error_names[4] = 'NOT_ENOUGH_RESOURCES'

    # The provided handle is of the wrong type (C API). This could 
    # happen if you provide a queue handle to access a hash map or 
    # something similar. It can also occur if you try to access an 
    # object after closing it.
    #
    # If you are seeing this error for the C++ API (or some other 
    # object-oriented interface), you've just found an internal 
    # error... (the handle is encapsulated and cannot be modified using 
    # the public interface).
    errors['WRONG_TYPE_HANDLE'] = 5
    error_names[5] = 'WRONG_TYPE_HANDLE'

    # The provided handle is NULL.
    errors['NULL_HANDLE'] = 6
    error_names[6] = 'NULL_HANDLE'

    # One of the arguments of an API function is an invalid NULL 
    # pointer.
    errors['NULL_POINTER'] = 7
    error_names[7] = 'NULL_POINTER'

    # An invalid length was provided as an argument to an API function.
    #
    # This invalid length will usually indicate that the length value 
    # is set to zero.
    errors['INVALID_LENGTH'] = 8
    error_names[8] = 'INVALID_LENGTH'

    # The process was already initialized. One possibility: was 
    # psoInit() called for a second time?
    errors['PROCESS_ALREADY_INITIALIZED'] = 21
    error_names[21] = 'PROCESS_ALREADY_INITIALIZED'

    # The process was not properly initialized. One possibility: was 
    # psoInit() called?
    errors['PROCESS_NOT_INITIALIZED'] = 22
    error_names[22] = 'PROCESS_NOT_INITIALIZED'

    # The watchdog address is invalid (empty string, NULL pointer, 
    # etc.).
    errors['INVALID_WATCHDOG_ADDRESS'] = 23
    error_names[23] = 'INVALID_WATCHDOG_ADDRESS'

    # API - memory-file version mismatch.
    errors['INCOMPATIBLE_VERSIONS'] = 24
    error_names[24] = 'INCOMPATIBLE_VERSIONS'

    # Generic socket error.
    errors['SOCKET_ERROR'] = 25
    error_names[25] = 'SOCKET_ERROR'

    # Socket error when trying to connect to the watchdog.
    errors['CONNECT_ERROR'] = 26
    error_names[26] = 'CONNECT_ERROR'

    # Socket error when trying to send a request to the watchdog.
    errors['SEND_ERROR'] = 27
    error_names[27] = 'SEND_ERROR'

    # Socket error when trying to receive a reply from the watchdog.
    errors['RECEIVE_ERROR'] = 28
    error_names[28] = 'RECEIVE_ERROR'

    # The shared-memory backstore file is missing. The name of this 
    # file is provided by the server (quasar) - if it is missing, 
    # something really weird is going on.
    errors['BACKSTORE_FILE_MISSING'] = 29
    error_names[29] = 'BACKSTORE_FILE_MISSING'

    # Generic i/o error when attempting to open the shared memory.
    errors['ERROR_OPENING_MEMORY'] = 30
    error_names[30] = 'ERROR_OPENING_MEMORY'

    # Error accessing the directory for the log files or error opening 
    # the log file itself.
    errors['LOGFILE_ERROR'] = 41
    error_names[41] = 'LOGFILE_ERROR'

    # Cannot get a lock on the session (a pthread_mutex or a critical 
    # section on Windows).
    errors['SESSION_CANNOT_GET_LOCK'] = 42
    error_names[42] = 'SESSION_CANNOT_GET_LOCK'

    # An attempt was made to use a session object (a session handle) 
    # after this session was terminated.
    errors['SESSION_IS_TERMINATED'] = 43
    error_names[43] = 'SESSION_IS_TERMINATED'

    # Permitted characters for names are alphanumerics, spaces (' '), 
    # dashes ('-') and underlines ('_'). The first character must be 
    # alphanumeric.
    errors['INVALID_OBJECT_NAME'] = 51
    error_names[51] = 'INVALID_OBJECT_NAME'

    # The object was not found (but its folder does exist).
    errors['NO_SUCH_OBJECT'] = 52
    error_names[52] = 'NO_SUCH_OBJECT'

    # One of the parent folder of an object does not exist.
    errors['NO_SUCH_FOLDER'] = 53
    error_names[53] = 'NO_SUCH_FOLDER'

    # Attempt to create an object which already exists.
    errors['OBJECT_ALREADY_PRESENT'] = 54
    error_names[54] = 'OBJECT_ALREADY_PRESENT'

    # The object (data container) is empty.
    errors['IS_EMPTY'] = 55
    error_names[55] = 'IS_EMPTY'

    # Attempt to create an object of an unknown object type or to open 
    # an object of the wrong type.
    errors['WRONG_OBJECT_TYPE'] = 56
    error_names[56] = 'WRONG_OBJECT_TYPE'

    # Cannot get lock on the object. This might be the result of either 
    # a very busy system where unused cpu cycles are rare or a lock 
    # might be held by a crashed process.
    errors['OBJECT_CANNOT_GET_LOCK'] = 57
    error_names[57] = 'OBJECT_CANNOT_GET_LOCK'

    # The search/iteration reached the end without finding a new 
    # item/record.
    errors['REACHED_THE_END'] = 58
    error_names[58] = 'REACHED_THE_END'

    # An invalid value was used for a psoIteratorType parameter.
    errors['INVALID_ITERATOR'] = 59
    error_names[59] = 'INVALID_ITERATOR'

    # The name of the object is too long. The maximum length of a name 
    # cannot be more than PSO_MAX_NAME_LENGTH (or 
    # PSO_MAX_FULL_NAME_LENGTH for the fully qualified name).
    errors['OBJECT_NAME_TOO_LONG'] = 60
    error_names[60] = 'OBJECT_NAME_TOO_LONG'

    # You cannot delete a folder if there are still undeleted objects 
    # in it.
    #
    # Technical: a folder does not need to be empty to be deleted but 
    # all objects in it must be "marked as deleted" by the current 
    # session. This enables writing recursive deletions
    errors['FOLDER_IS_NOT_EMPTY'] = 61
    error_names[61] = 'FOLDER_IS_NOT_EMPTY'

    # An item with the same key was found.
    errors['ITEM_ALREADY_PRESENT'] = 62
    error_names[62] = 'ITEM_ALREADY_PRESENT'

    # The item was not found in the hash map.
    errors['NO_SUCH_ITEM'] = 63
    error_names[63] = 'NO_SUCH_ITEM'

    # The object is scheduled to be deleted soon. Operations on this 
    # data container are not permitted at this time.
    errors['OBJECT_IS_DELETED'] = 64
    error_names[64] = 'OBJECT_IS_DELETED'

    # Object must be open first before you can access them.
    errors['OBJECT_NOT_INITIALIZED'] = 65
    error_names[65] = 'OBJECT_NOT_INITIALIZED'

    # The data item is scheduled to be deleted soon or was just created 
    # and is not committed. Operations on this data item are not 
    # permitted at this time.
    errors['ITEM_IS_IN_USE'] = 66
    error_names[66] = 'ITEM_IS_IN_USE'

    # The data item is scheduled to be deleted soon. Operations on this 
    # data container are not permitted at this time.
    errors['ITEM_IS_DELETED'] = 67
    error_names[67] = 'ITEM_IS_DELETED'

    # The object is scheduled to be deleted soon or was just created 
    # and is not committed. Operations on this object are not permitted 
    # at this time.
    errors['OBJECT_IS_IN_USE'] = 69
    error_names[69] = 'OBJECT_IS_IN_USE'

    # The object is read-only and update operations 
    # (delete/insert/replace) on it are not permitted. at this time.
    errors['OBJECT_IS_READ_ONLY'] = 70
    error_names[70] = 'OBJECT_IS_READ_ONLY'

    # All read-only objects open for updates (as temporary objects) 
    # must be closed prior to doing a commit on the session.
    errors['NOT_ALL_EDIT_ARE_CLOSED'] = 71
    error_names[71] = 'NOT_ALL_EDIT_ARE_CLOSED'

    # Read-only objects are not updated very frequently and therefore 
    # only a single editing copy is allowed. To allow concurrent 
    # editors (either all working on the same copy or each working with 
    # its own copy would have been be possible but was deemed 
    # unnecessary.
    errors['A_SINGLE_UPDATER_IS_ALLOWED'] = 72
    error_names[72] = 'A_SINGLE_UPDATER_IS_ALLOWED'

    # The number of fields in the data definition is invalid - either 
    # zero or greater than PSO_MAX_FIELDS (defined in 
    # photon/psoCommon.h).
    errors['INVALID_NUM_FIELDS'] = 101
    error_names[101] = 'INVALID_NUM_FIELDS'

    # The data type of the field definition does not correspond to one 
    # of the data type defined in the enum psoFieldType 
    # (photon/psoCommon.h). or you've used PSO_VAR_STRING or 
    # PSO_VAR_BINARY at the wrong place.
    #
    # Do not forget that PSO_VAR_STRING and PSO_VAR_BINAR can only be 
    # used for the last field of your data definition.
    errors['INVALID_FIELD_TYPE'] = 102
    error_names[102] = 'INVALID_FIELD_TYPE'

    # The length of an integer field (PSO_INTEGER) is invalid. Valid 
    # values are 1, 2, 4 and 8.
    errors['INVALID_FIELD_LENGTH_INT'] = 103
    error_names[103] = 'INVALID_FIELD_LENGTH_INT'

    # The length of a field (string or binary) is invalid. Valid values 
    # are all numbers greater than zero and less than 4294967296 (4 
    # Giga).
    errors['INVALID_FIELD_LENGTH'] = 104
    error_names[104] = 'INVALID_FIELD_LENGTH'

    # The name of the field contains invalid characters. Valid 
    # characters are the standard ASCII alphanumerics ([a-zA-Z0-9]) and 
    # the underscore ('_'). The first character of the name must be 
    # letter.
    errors['INVALID_FIELD_NAME'] = 105
    error_names[105] = 'INVALID_FIELD_NAME'

    # The name of the field is already used by another field in the 
    # current definition.
    #
    # Note: at the moment field names are case sensitive (for example 
    # "account_id" and "Account_Id" are considered different). This 
    # might be changed eventually so this practice should be avoided.
    errors['DUPLICATE_FIELD_NAME'] = 106
    error_names[106] = 'DUPLICATE_FIELD_NAME'

    # The precision of a PSO_DECIMAL field is either zero or over the 
    # limit for this type (set at 30 currently). Note: precision is the 
    # number of digits in a number.
    errors['INVALID_PRECISION'] = 107
    error_names[107] = 'INVALID_PRECISION'

    # The scale of a PSO_DECIMAL field is invalid (greater than the 
    # value of precision. Note: scale is the number of digits to the 
    # right of the decimal separator in a number.
    errors['INVALID_SCALE'] = 108
    error_names[108] = 'INVALID_SCALE'

    # The key definition for a hash map is either invalid or missing.
    errors['INVALID_KEY_DEF'] = 109
    error_names[109] = 'INVALID_KEY_DEF'

    # Error reading the XML buffer stream.
    #
    # No validation is done at this point. Therefore the error is 
    # likely something like a missing end-tag or some other 
    # non-conformance to the XML's syntax rules.
    #
    # A simple Google search for "well-formed xml" returns many web 
    # sites that describe the syntax rules for XML. You can also use 
    # the program xmllint (included in the distribution of libxm2) to 
    # pinpoint the issue.
    errors['XML_READ_ERROR'] = 201
    error_names[201] = 'XML_READ_ERROR'

    # The root element is not the expected root, <folder> and similar.
    errors['XML_INVALID_ROOT'] = 202
    error_names[202] = 'XML_INVALID_ROOT'

    # The root element must have an attribute named schemaLocation (in 
    # the namespace "http://www.w3.org/2001/XMLSchema-instance") to 
    # point to the schema use for the xml buffer stream.
    #
    # This attribute is in two parts separated by a space. The code 
    # expects the file name of the schema in the second element of this 
    # attribute.
    errors['XML_NO_SCHEMA_LOCATION'] = 203
    error_names[203] = 'XML_NO_SCHEMA_LOCATION'

    # The creation of a new schema parser context failed. There might 
    # be multiple reasons for this, for example, a memory-allocation 
    # failure in libxml2. However, the most likely reason is that the 
    # schema file is not at the location indicated by the attribute 
    # schemaLocation of the root element of the buffer stream.
    errors['XML_PARSER_CONTEXT_FAILED'] = 204
    error_names[204] = 'XML_PARSER_CONTEXT_FAILED'

    # The parse operation of the schema failed. Most likely, there is 
    # an error in the schema. To debug this you can use xmllint (part 
    # of the libxml2 package).
    errors['XML_PARSE_SCHEMA_FAILED'] = 205
    error_names[205] = 'XML_PARSE_SCHEMA_FAILED'

    # The creation of a new schema validation context failed. There 
    # might be multiple reasons for this, for example, a 
    # memory-allocation failure in libxml2.
    errors['XML_VALID_CONTEXT_FAILED'] = 206
    error_names[206] = 'XML_VALID_CONTEXT_FAILED'

    # Document validation for the xml buffer failed. To debug this 
    # problem you can use xmllint (part of the libxml2 package).
    errors['XML_VALIDATION_FAILED'] = 207
    error_names[207] = 'XML_VALIDATION_FAILED'

    # Abnormal internal error with sem_destroy. It should not happen! 
    # It could indicate that the memory allocated for the semaphore was 
    # corrupted (errno = EINVAL) or that the reference counting is 
    # wrong (errno = EBUSY). Please contact us.
    errors['SEM_DESTROY_ERROR'] = 601
    error_names[601] = 'SEM_DESTROY_ERROR'


    return tuple(errors, error_names)

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--


/**
 * @file cppp/reiconv.hpp
 * @author ChenPi11
 * @brief C++ Plus cppp-reiconv package.
 * @version 2.1.0
 * @date 2023-7-28
 * @copyright Copyright (C) 1999-2023 Free Software Foundation, Inc.
 */
/* Copyright (C) 1999-2023 Free Software Foundation, Inc.
   This file is part of the cppp-reiconv library.

   The cppp-reiconv library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either version 3
   of the License, or (at your option) any later version.

   The cppp-reiconv library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the cppp-reiconv library; see the file COPYING.
   If not, see <https://www.gnu.org/licenses/>.  */

/* When installed, this file is called "cppp/reiconv.hpp". */

#ifndef _CPPP_REICONV_HPP
#define _CPPP_REICONV_HPP

#if _MSC_VER >= 1600 
/* Use UTF-8 to decode this file. */
#pragma execution_character_set("utf-8")
#endif

#include <exception>
#include <iostream>
#include <errno.h>
#include <time.h>
#include <vector>
#include <wchar.h>

extern "C++"
{
    namespace cppp
    {
    namespace base
    {
    namespace reiconv
    {
    /**
     * @brief Version infomation. (major<<8) + minor
     */
    extern __declspec(dllexport) int reiconv_version;

#undef iconv_t
    /**
     * @typedef void*
     * @brief Identifier for conversion method from one codeset to another.
     */
    typedef void *iconv_t;

#undef iconv_open
    /**
     * @brief Allocates descriptor for code conversion from encoding 'fromcode' to encoding 'tocode'.
     * @param tocode Target encoding.
     * @param fromcode From encoding.
     * @return Allocated iconv conversation buffer's pointer.
     */
    extern __declspec(dllexport) iconv_t iconv_open(const char *tocode, const char *fromcode);

    /**
     * @brief Allocates descriptor for code conversion from codepage 'tocode_cp' to codepage 'fromcode_cp'.
     * @param tocode_cp Target codepage.
     * @param fromcode_cp From codepage.
     * @param strict Strict mode, if false, will ignore the invalid characters.
     * @note Some encoding may don't have codepage.
     * @return Allocated iconv conversation buffer's pointer.
     */
    extern __declspec(dllexport) iconv_t iconv_open(int tocode_cp, int fromcode_cp, bool strict = true);

#undef iconv
    /**
     * @brief Converts, using conversion descriptor 'cd', at most '*inbytesleft' bytes starting at '*inbuf', writing at
     * most '*outbytesleft' bytes starting at '*outbuf'.
     * @attention Decrements '*inbytesleft' and increments '*inbuf' by the same amount.
     * @attention Decrements '*outbytesleft' and increments '*outbuf' by the same amount.
     * @param cd Conversion descriptor.
     * @param inbuf Input buffer.
     * @param inbytesleft Input bytes left.
     * @param outbuf Output buffer.
     * @param outbytesleft Output bytes left.
     * @return Number of converted characters (excluding terminators, -1 means error).
     */
    extern __declspec(dllexport) size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf,
                                       size_t *outbytesleft);

#undef iconv_close
    /**
     * @brief Frees resources allocated for conversion descriptor 'cd'.
     * @param cd Conversion descriptor you want to close.
     * @return Operation status. (-1 means error, 0 means success).
     */
    extern __declspec(dllexport) int iconv_close(iconv_t cd);

    /**
     * @brief Control of attributes.
     * @param cd Conversion descriptor.
     * @param request Attribute request id.
     * @param argument Attribute argument.
     * @return Operation status (>=0 means success, <0 means error).
     */
    extern __declspec(dllexport) int iconvctl(iconv_t cd, int request, void *argument);

    /**
     * @typedef void (*) (unsigned int, void*)
     * @brief Hook performed after every successful conversion of a Unicode character.
     */
    typedef void (*iconv_unicode_char_hook)(unsigned int uc, void *data);

    /**
     * @brief Set of hooks.
     */
    struct iconv_hooks
    {
        iconv_unicode_char_hook uc_hook;
        void *data;
    };

    /**
     * @brief Fallback function.  Invoked when a small number of bytes could not be
     *        converted to a Unicode character.  This function should process all
     *        bytes from inbuf and may produce replacement Unicode characters by calling
     *        the write_replacement callback repeatedly.
     */
    typedef void (*iconv_unicode_mb_to_uc_fallback)(const char *inbuf, size_t inbufsize,
                                                    void (*write_replacement)(const unsigned int *buf, size_t buflen,
                                                                              void *callback_arg),
                                                    void *callback_arg, void *data);

    /**
     * @brief Fallback function.  Invoked when a Unicode character could not be converted
     *        to the target encoding.  This function should process the character and
     *        may produce replacement bytes (in the target encoding) by calling the
     *        write_replacement callback repeatedly.
     */
    typedef void (*iconv_unicode_uc_to_mb_fallback)(unsigned int code,
                                                    void (*write_replacement)(const char *buf, size_t buflen,
                                                                              void *callback_arg),
                                                    void *callback_arg, void *data);

    /**
     * @brief Set of fallbacks.
     */
    struct iconv_fallbacks
    {
        iconv_unicode_mb_to_uc_fallback mb_to_uc_fallback;
        iconv_unicode_uc_to_mb_fallback uc_to_mb_fallback;
        void *data;
    };

    /**
     * @brief Surfaces. The concept of surfaces is described in the 'recode' manual.
     */
    constexpr int ICONV_SURFACE_NONE = 0;
    /**
     * @brief In EBCDIC encodings, 0x15 (which encodes the "newline function", see the
     *        Unicode standard, chapter 5) maps to U+000A instead of U+0085.  This is
     *        for interoperability with C programs and Unix environments on z/OS.
     */
    constexpr int ICONV_SURFACE_EBCDIC_ZOS_UNIX = 1;

    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_TRIVIALP = 0; /* int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_GET_DISCARD_ILSEQ = 3; /* int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_SET_DISCARD_ILSEQ = 4; /* const int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_SET_HOOKS = 5; /* const struct iconv_hooks *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_SET_FALLBACKS = 6; /* const struct iconv_fallbacks *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_GET_FROM_SURFACE = 7; /* unsigned int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_SET_FROM_SURFACE = 8; /* const unsigned int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_GET_TO_SURFACE = 9; /* unsigned int *argument */
    /**
     * @brief Request for iconvctl.
     */
    constexpr int ICONV_SET_TO_SURFACE = 10; /* const unsigned int *argument */

    /**
     * @brief Listing of locale independent encodings.
     * @param do_one One encoding fallback function.
     * @param data Data.
     */
    extern __declspec(dllexport) void iconvlist(int (*do_one)(unsigned int namescount, const char *const *names, void *data),
                                          void *data);

    /**
     * @brief Converts an entire string from one encoding to another, using iconv. Easier to use than iconv() itself.
     * @note This function does not treat zero characters specially.
     * @param cd Conversion descriptor.
     * @param start Source string start pointer.
     * @param end Source string end pointer.
     * @param resultp Result memory pointer.
     * @param lengthp Pointer to a variable that stores the length of the result.
     * @return Return 0 if successful, otherwise return errno and errno set. Particular errno values are EILSEQ and ENOMEM.
     */
    extern __declspec(dllexport) int iconv_string(const iconv_t& cd, const char *start,
                                            const char *end, char **resultp, size_t *lengthp);
    
    /**
     * @brief Converts an entire string from one encoding to another,
     * and supports autodetect encodings on input.
     *
     * @note Converts a memory region given in encoding FROMCODE to a new memory
     * region in encoding TOCODE. FROMCODE and TOCODE are as for iconv_open(3),
     * except that FROMCODE may be one of the values.
     *
     * @note "autodetect_utf8"          supports ISO-8859-1 and UTF-8
     * @note "autodetect_jp"            supports EUC-JP, ISO-2022-JP-2 and SHIFT_JIS
     * @note "autodetect_kr"            supports EUC-KR and ISO-2022-KR
     * @note The input is in the memory region between start (inclusive) and end
     * (exclusive). If resultp is not NULL, the output string is stored in
     * *resultp; malloc/realloc is used to allocate the result.
     *
     * @note This function does not treat zero characters specially.
     * @param tocode Target encoding.
     * @param fromcode From encoding.
     * @param start Source string start pointer.
     * @param end Source string end pointer.
     * @param resultp Result memory pointer.
     * @param lengthp Pointer to a variable that stores the length of the result.
     * @return Return 0 if successful, otherwise return errno and errno set. Particular errno values: EILSEQ and ENOMEM.
     *
     * @example
     *   const char* s = ...;
     *   char* result = NULL;
     *   if (iconv_string("UCS-4-INTERNAL", "GBK",
     *                    s, s+strlen(s)+1, &result, NULL) != 0)
     *   perror("iconv_string");
     *
     */
    extern __declspec(dllexport) int iconv_string(const char* tocode, const char* fromcode, const char* start,
                                            const char* end, char** resultp, size_t* lengthp);

    /**
     * @brief Converts an entire string from one encoding to another.
     * @note This function does not treat zero characters specially.
     * @param tocode_cp Target encoding codepage.
     * @param fromcode_cp From encoding codepage.
     * @param start Source string start pointer.
     * @param end Source string end pointer.
     * @param resultp Result memory pointer.
     * @param lengthp Pointer to a variable that stores the length of the result.
     * @param strict Strict mode, if false, will ignore the invalid characters.
     * @return Return 0 if successful, otherwise return errno and errno set. Particular errno values: EILSEQ and ENOMEM.
     *
     * @example
     *   const char* s = ...;
     *   char* result = NULL;
     *   if (iconv_string(65001, 936,
     *                    s, s+strlen(s)+1, &result, NULL) != 0)
     *   perror("iconv_string");
     *
     */
    extern __declspec(dllexport) int iconv_string(int tocode_cp, int fromcode_cp, const char* start,
                                            const char* end, char** resultp, size_t* lengthp, bool strict=true);

    /**
     * @brief Encode source string to another encoding.
     * @param from From encoding.
     * @param to Target encoding.
     * @param data Input string data.
     * @param ignore Ignore invalid characters.
     * @throw int: when conversation error, throw the 'errno', you can catch the errno for more infomation.
     */
    inline std::vector<char> encode(const std::string& from, const std::string& to, const std::vector<char>& data,
                                    bool ignore = false)
    {
        char *result = NULL;
        size_t length = 0;
        std::string ignstr = ignore ? "//IGNORE" : "";
        if (iconv_string((to + ignstr).c_str(), from.c_str(), data.data(), data.data() + data.size(), &result,
                         &length) < 0)
        {
            if (!ignore)
            {
                /* When conversation error, throw the errno, you can catch the errno for more infomation. */
                throw(int)(errno);
            }
        }
        return std::vector<char>(result, result + length);
    }

    } // namespace reiconv
    } // namespace base
    } // namespace cppp
}

#endif /* _CPPP_REICONV_HPP */

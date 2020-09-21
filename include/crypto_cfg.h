/******************************************************************************
 *
 * SiFive Cryptographic Library (SCL)
 *
 ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

/**
 * @file crypto_cfg.h
 * @brief define default section for crypto function and crypto data
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_CRYPTO_CFG_H
#define SCL_CRYPTO_CFG_H

#define CRYPTO_FUNCTION __attribute__((section("crypto_functions")))
#define CRYPTO_DATA __attribute__((section("crypto_data")))

/**
 * @brief Macro to add section qualifier to some contant data section.
 * @details This allow user to place constants (ecdsa curves parameters) in RAM
 * by doing so we speed up computation. But that put constant data in a
 * writable section and might cause security issue (i.e. the memory zone should
 * be set not writable/executable with pmp to avoid any issue).
 */
#define CRYPTO_CONST_DATA __attribute__((section("crypto_const_data")))

#endif /* SCL_CRYPTO_CFG_H */

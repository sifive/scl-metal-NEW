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
 * @file scl_soft.h
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SCL_SOFT_H
#define SCL_BACKEND_SCL_SOFT_H

#include <stdint.h>
#include <stdio.h>

#include <backend/software/asymmetric/ecc/soft_ecc.h>
#include <backend/software/asymmetric/ecc/soft_ecc_keygen.h>
#include <backend/software/asymmetric/ecc/soft_ecdh.h>
#include <backend/software/asymmetric/ecc/soft_ecdsa.h>
#include <backend/software/bignumbers/soft_bignumbers.h>
#include <backend/software/hash/sha/soft_sha.h>
#include <backend/software/key_derivation_functions/soft_kdf_x963.h>
#include <backend/software/message_auth/soft_hmac.h>

#endif /* SCL_BACKEND_SCL_SOFT_H */

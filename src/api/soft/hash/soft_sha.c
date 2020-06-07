/******************************************************************************
 *
 * SiFive Cryptographic Library (SCL)
 *
 ******************************************************************************
 * @file soft_sha.c
 * @author Pierre-Henry Moussay (pierre-henry.moussay@sifive.com)
 * @brief
 * @version 0.1
 * @date 2020-05-28
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
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

#include <scl/scl_retdefs.h>

#include <api/hash/sha.h>
#include <api/soft/hash/soft_sha.h>

#include <api/soft/hash/soft_sha224.h>
#include <api/soft/hash/soft_sha256.h>
#include <api/soft/hash/soft_sha384.h>
#include <api/soft/hash/soft_sha512.h>

int32_t sha_init_soft(metal_scl_t *const scl, sha_ctx_t *const ctx,
                      hash_mode_t hash_mode, endianness_t data_endianness)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ctx->mode = hash_mode;

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (sha224_init_soft(&(ctx->ctx.sha224), data_endianness));
        break;
    case SCL_HASH_SHA256:
        return (sha256_init_soft(&(ctx->ctx.sha256), data_endianness));
        break;
    case SCL_HASH_SHA384:
        return (sha384_init_soft(&(ctx->ctx.sha384), data_endianness));
        break;
    case SCL_HASH_SHA512:
        return (sha512_init_soft(&(ctx->ctx.sha512), data_endianness));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

int32_t sha_core_soft(metal_scl_t *const scl, sha_ctx_t *const ctx,
                      const uint8_t *const data, size_t data_byte_len)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (sha224_core_soft(&(ctx->ctx.sha224), data, data_byte_len));
        break;
    case SCL_HASH_SHA256:
        return (sha256_core_soft(&(ctx->ctx.sha256), data, data_byte_len));
        break;
    case SCL_HASH_SHA384:
        return (sha384_core_soft(&(ctx->ctx.sha384), data, data_byte_len));
        break;
    case SCL_HASH_SHA512:
        return (sha512_core_soft(&(ctx->ctx.sha512), data, data_byte_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

int32_t sha_finish_soft(metal_scl_t *const scl, sha_ctx_t *const ctx,
                        uint8_t *const hash, size_t *const hash_len)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (sha224_finish_soft(&(ctx->ctx.sha224), hash, hash_len));
        break;
    case SCL_HASH_SHA256:
        return (sha256_finish_soft(&(ctx->ctx.sha256), hash, hash_len));
        break;
    case SCL_HASH_SHA384:
        return (sha384_finish_soft(&(ctx->ctx.sha384), hash, hash_len));
        break;
    case SCL_HASH_SHA512:
        return (sha512_finish_soft(&(ctx->ctx.sha512), hash, hash_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

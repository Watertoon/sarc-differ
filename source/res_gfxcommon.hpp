 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

namespace dd::res {

    enum class GfxMemoryPoolFlags : u32 {
        CpuNoAccess  = (1 << 0),
        CpuUncached  = (1 << 1),
        CpuCached    = (1 << 2),
        GpuNoAccess  = (1 << 3),
        GpuUncached  = (1 << 4),
        GpuCached    = (1 << 5),
        ShaderCode   = (1 << 6),
        Compressible = (1 << 7),
        Physical     = (1 << 8),
        Virtual      = (1 << 9),
    };
    
    enum class GfxGpuAccessFlags : u32 {
        
        VertexBuffer        = (1 << 2),
        IndexBuffer         = (1 << 3),
        UniformBuffer       = (1 << 4),
        Texture             = (1 << 5),
        TransferDestination = (1 << 6),
        RenderTargetColor   = (1 << 7),
        RenderTargetDepth   = (1 << 8),
        IndirectDraw        = (1 << 9),
        DisplayTexture      = (1 << 10),
        Counter             = (1 << 11),
    };

    struct ResGfxMemoryPoolInfo {
        u32   memory_pool_flags;
        u32   size;
        void *storage;
    };
    static_assert(sizeof(ResGfxMemoryPoolInfo) == 0x10);

    struct ResGfxBufferInfo {
        u32 size;
        u32 gpu_access_flags;
        u32 reserve0;
        u32 reserve1;
    };
    static_assert(sizeof(ResGfxBufferInfo) == 0x10);

    enum class GfxChannelSource : u8 {
        Zero  = 0x0,
        One   = 0x1,
        Red   = 0x2,
        Green = 0x3,
        Blue  = 0x4,
        Alpha = 0x5
    };

    enum class GfxChannelFormat : u8 {
        None         = 0x1,
        R8           = 0x2,
        R4G4B4A4     = 0x3,
        R5G5B5A1     = 0x5,
        A1B5G5R5     = 0x6,
        R5G6B5       = 0x7,
        B5G6R5       = 0x8,
        R8G8         = 0x9,
        R16          = 0xa,
        R8G8B8A8     = 0xb,
        B8G8R8A8     = 0xc,
        R9G9B9E5F    = 0xd,
        R10G10B10A2  = 0xe,
        R11G11B10F   = 0xf,
        R16G16       = 0x12,
        D24S8        = 0x13,
        R32          = 0x14,
        R16G16B16A16 = 0x15,
        D32FS8       = 0x16,
        R32G32       = 0x17,
        R32G32B32    = 0x18,
        R32G32B32A32 = 0x19,
        BC1          = 0x1a,
        BC3          = 0x1b,
        BC5          = 0x1c,
        BC4          = 0x1d,
        //BC5          = 0x1e,
        BC6H         = 0x1f,
        BC7U         = 0x20,
        ASTC_4x4     = 0x2d,
        ASTC_5x4     = 0x2e,
        ASTC_5x5     = 0x2f,
        ASTC_6x5     = 0x30,
        ASTC_6x6     = 0x31,
        ASTC_8x5     = 0x32,
        ASTC_8x6     = 0x33,
        ASTC_8x8     = 0x34,
        ASTC_10x5    = 0x35,
        ASTC_10x6    = 0x36,
        ASTC_10x8    = 0x37,
        ASTC_10x10   = 0x38,
        ASTC_12x10   = 0x39,
        ASTC_12x12   = 0x3a,
        B5G5R5A1     = 0x3b,
    };

    static constexpr inline u32 VariableBlockWidthTable[] = {
        0x10, 0x08, 0x10, 0x08,
        0x08, 0x04, 0x04, 0x05,
        0x05, 0x06, 0x06, 0x08,
        0x08, 0x08, 0x0a, 0x0a,
        0x0a, 0x0a, 0x0c, 0x0c,
    };
    static constexpr inline u32 VariableBlockHeightTable[] = {
        0x08, 0x08, 0x08, 0x08,
        0x04, 0x04, 0x04, 0x04,
        0x05, 0x05, 0x06, 0x05,
        0x06, 0x08, 0x05, 0x06,
        0x08, 0x0a, 0x0a, 0x0c,
    };
    
    static constexpr inline u32 PackagedTextureTexelSizeTable[] = {
        0x08, 0x10, 0x10, 0x08,
        0x10, 0x10, 0x10, 0x08,
        0x10, 0x08, 0x08, 0x08,
        0x10, 0x20, 0x20, 0x20,
        0x20, 0x08, 0x08, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10,
        0x10, 0x02
    };

    constexpr ALWAYS_INLINE u32 CalculateGfxImageSize(GfxChannelFormat channel_format, u32 width, u32 height, u32 depth) {
        u32 format = static_cast<u32>(channel_format);

        /* Adjust width and height for compressed formats */
        u32 package_format = format - 0x1a;
        if (package_format < 0x21) {
            u32 variable_format = format - 0x27;
            u32 block_width     = 4;
            u32 block_height    = 4;
            if (variable_format < 0x14) {
                block_width  = VariableBlockWidthTable[variable_format];
                block_height = VariableBlockHeightTable[variable_format];
            }

            width  = (block_width != 0)  ? (width  + block_width  - 1) / block_width  : 0;
            height = (block_height != 0) ? (height + block_height - 1) / block_height : 0;
        }

        /* Find texel size */
        u32 texel_size = 0;
        if (package_format < 0x22) {
            texel_size = PackagedTextureTexelSizeTable[package_format];
        } else if (format < 0x3) {
            texel_size = 1;
        } else if (format < 0xb) {
            texel_size = 2;
        } else if (format < 0x15) {
            texel_size = 4;
        } else if (format < 0x18) {
            texel_size = 8;
        } else {
            texel_size = 12;
            if (format != 0x18) {
                texel_size = 16;
            }
        }

        /* Calculate */
        return texel_size * width * height * depth;
    }

    enum class GfxTypeFormat : u8 {
        Unorm   = 1,
        Snorm   = 2,
        UInt    = 3,
        SInt    = 4,
        Float   = 5,
        SRGB    = 6,
        Depth   = 7, /* (Unorm) */
        UScaled = 8,
        SScaled = 9,
        UFloat  = 0xa
    };

    #define GFX_MAKE_IMAGE_FORMAT(channel_format, type_format) \
        (((static_cast<u32>(channel_format) & 0xFF) << 8) | (static_cast<u32>(type_format) & 0xFF))

    enum class GfxImageFormat : u32 {
        R8_Unorm              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Unorm),
        R8_Snorm              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Snorm),
        R8_UInt               = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::UInt),
        R8_SInt               = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::SInt),
        R4G4B4A4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R4G4B4A4,     GfxTypeFormat::Unorm),
        R5G5B5A1_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R5G5B5A1,     GfxTypeFormat::Unorm),
        A1B5G5R5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::A1B5G5R5,     GfxTypeFormat::Unorm),
        R5G6B5_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R5G6B5,       GfxTypeFormat::Unorm),
        B5G6R5_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B5G6R5,       GfxTypeFormat::Unorm),
        R8G8_Unorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Unorm),
        R8G8_Snorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Snorm),
        R8G8_UInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::UInt),
        R8G8_SInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::SInt),
        R16_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Unorm),
        R16_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Snorm),
        R16_UInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::UInt),
        R16_SInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::SInt),
        R16_Float             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Float),
        Z16_Depth             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Depth),
        R8G8B8A8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Unorm),
        R8G8B8A8_Snorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Snorm),
        R8G8B8A8_UInt         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::UInt),
        R8G8B8A8_SInt         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SInt),
        R8G8B8A8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SRGB),
        B8G8R8A8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B8G8R8A8,     GfxTypeFormat::Unorm),
        B8G8R8A8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B8G8R8A8,     GfxTypeFormat::SRGB),
        R9G9B9E5F_SharedFloat = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R9G9B9E5F,    GfxTypeFormat::Float),
        R10G10B10A2_Unorm     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::Unorm),
        R10G10B10A2_UInt      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::UInt),
        R11G11B10F_Float      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R11G11B10F,   GfxTypeFormat::Float),
        R16G16_Unorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Unorm),
        R16G16_Snorm          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Snorm),
        R16G16_UInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::UInt),
        R16G16_SInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::SInt),
        R16G16_Float          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,       GfxTypeFormat::Float),
        D24S8_Depth           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::D24S8,        GfxTypeFormat::Depth),
        R32_UInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::UInt),
        R32_SInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::SInt),
        R32_Float             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::Float),
        D32F_Depth            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,          GfxTypeFormat::Depth),
        R16G16B16A16_Unorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Unorm),
        R16G16B16A16_Snorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Snorm),
        R16G16B16A16_UInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::UInt),
        R16G16B16A16_SInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::SInt),
        R16G16B16A16_Float    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16, GfxTypeFormat::Float),
        D32FS8_Depth          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::D32FS8,       GfxTypeFormat::Depth),
        R32G32_UInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::UInt),
        R32G32_SInt           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::SInt),
        R32G32_Float          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,       GfxTypeFormat::Float),
        R32G32B32_UInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::UInt),
        R32G32B32_SInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::SInt),
        R32G32B32_Float       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,    GfxTypeFormat::Float),
        R32G32B32A32_UInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::UInt),
        R32G32B32A32_SInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::SInt),
        R32G32B32A32_Float    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32, GfxTypeFormat::Float),
        BC1_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC1,          GfxTypeFormat::Unorm),
        BC1_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC1,          GfxTypeFormat::SRGB),
        //BC2_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC2,          GfxTypeFormat::Unorm),
        //BC2_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC2,          GfxTypeFormat::SRGB),
        BC3_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC3,          GfxTypeFormat::Unorm),
        BC3_SRGB              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC3,          GfxTypeFormat::SRGB),
        BC4_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC4,          GfxTypeFormat::Unorm),
        BC4_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC4,          GfxTypeFormat::Snorm),
        BC5_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC5,          GfxTypeFormat::Unorm),
        BC5_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC5,          GfxTypeFormat::Snorm),
        BC6H_SF16             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC6H,         GfxTypeFormat::Float),
        BC6H_UF16             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC6H,         GfxTypeFormat::UFloat),
        BC7U_Unorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC7U,         GfxTypeFormat::Unorm),
        BC7U_SRGB             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::BC7U,         GfxTypeFormat::SRGB),
        ASTC_4X4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_4x4,     GfxTypeFormat::Unorm),
        ASTC_4X4_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_4x4,     GfxTypeFormat::SRGB),
        ASTC_5X4_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x4,     GfxTypeFormat::Unorm),
        ASTC_5X4_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x4,     GfxTypeFormat::SRGB),
        ASTC_5X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x5,     GfxTypeFormat::Unorm),
        ASTC_5X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_5x5,     GfxTypeFormat::SRGB),
        ASTC_6X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x5,     GfxTypeFormat::Unorm),
        ASTC_6X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x5,     GfxTypeFormat::SRGB),
        ASTC_6X6_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x6,     GfxTypeFormat::Unorm),
        ASTC_6X6_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_6x6,     GfxTypeFormat::SRGB),
        ASTC_8X5_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x5,     GfxTypeFormat::Unorm),
        ASTC_8X5_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x5,     GfxTypeFormat::SRGB),
        ASTC_8X6_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x6,     GfxTypeFormat::Unorm),
        ASTC_8X6_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x6,     GfxTypeFormat::SRGB),
        ASTC_8X8_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x8,     GfxTypeFormat::Unorm),
        ASTC_8X8_SRGB         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_8x8,     GfxTypeFormat::SRGB),
        ASTC_10X5_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x5,    GfxTypeFormat::Unorm),
        ASTC_10X5_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x5,    GfxTypeFormat::SRGB),
        ASTC_10X6_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x6,    GfxTypeFormat::Unorm),
        ASTC_10X6_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x6,    GfxTypeFormat::SRGB),
        ASTC_10X8_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x8,    GfxTypeFormat::Unorm),
        ASTC_10X8_SRGB        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x8,    GfxTypeFormat::SRGB),
        ASTC_10X10_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x10,   GfxTypeFormat::Unorm),
        ASTC_10X10_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_10x10,   GfxTypeFormat::SRGB),
        ASTC_12X10_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x10,   GfxTypeFormat::Unorm),
        ASTC_12X10_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x10,   GfxTypeFormat::SRGB),
        ASTC_12X12_Unorm      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x12,   GfxTypeFormat::Unorm),
        ASTC_12X12_SRGB       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::ASTC_12x12,   GfxTypeFormat::SRGB),
        B5G5R5A1_Unorm        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::B5G5R5A1,     GfxTypeFormat::Unorm)
    };

    enum class GfxAttributeFormat : u32 {
        None_Unorm           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::None,         GfxTypeFormat::Unorm),
        R8_Unorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Unorm),
        R8_Snorm             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::Snorm),
        R8_UInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::UInt),
        R8_SInt              = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::SInt),
        R8_UScaled           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::UScaled),
        R8_SScaled           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8,           GfxTypeFormat::SScaled),
        R8G8_Unorm           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Unorm),
        R8G8_Snorm           = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::Snorm),
        R8G8_UInt            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::UInt),
        R8G8_SInt            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::SInt),
        R8G8_UScaled         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::UScaled),
        R8G8_SScaled         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8,         GfxTypeFormat::SScaled),
        R16_Unorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Unorm),
        R16_Snorm            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Snorm),
        R16_UInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::UInt),
        R16_SInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::SInt),
        R16_Float            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::Float),
        R16_UScaled          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::UScaled),
        R16_SScaled          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16,          GfxTypeFormat::SScaled),
        R8G8B8A8_Unorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Unorm),
        R8G8B8A8_Snorm       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::Snorm),
        R8G8B8A8_UInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::UInt),
        R8G8B8A8_SInt        = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SInt),
        R8G8B8A8_UScaled     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::UScaled),
        R8G8B8A8_SScaled     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R8G8B8A8,     GfxTypeFormat::SScaled),
        R10G10B10A2_Unorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::Unorm),
        R10G10B10A2_Snorm    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::Snorm),
        R10G10B10A2_UInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::UInt),
        R10G10B10A2_SInt     = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R10G10B10A2,  GfxTypeFormat::SInt),
        R16G16_Unorm         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::Unorm),
        R16G16_Snorm         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::Snorm),
        R16G16_UInt          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::UInt),
        R16G16_SInt          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::SInt),
        R16G16_Float         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::Float),
        R16G16_UScaled       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::UScaled),
        R16G16_SScaled       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16,        GfxTypeFormat::SScaled),
        R32_UInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,           GfxTypeFormat::UInt),
        R32_SInt             = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,           GfxTypeFormat::SInt),
        R32_Float            = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32,           GfxTypeFormat::Float),
        R16G16B16A16_Unorm   = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::Unorm),
        R16G16B16A16_Snorm   = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::Snorm),
        R16G16B16A16_UInt    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::UInt),
        R16G16B16A16_SInt    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::SInt),
        R16G16B16A16_Float   = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::Float),
        R16G16B16A16_UScaled = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::UScaled),
        R16G16B16A16_SScaled = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R16G16B16A16,  GfxTypeFormat::SScaled),
        R32G32_UInt          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,        GfxTypeFormat::UInt),
        R32G32_SInt          = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,        GfxTypeFormat::SInt),
        R32G32_Float         = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32,        GfxTypeFormat::Float),
        R32G32B32_UInt       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,     GfxTypeFormat::UInt),
        R32G32B32_SInt       = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,     GfxTypeFormat::SInt),
        R32G32B32_Float      = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32,     GfxTypeFormat::Float),
        R32G32B32A32_UInt    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32,  GfxTypeFormat::UInt),
        R32G32B32A32_SInt    = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32,  GfxTypeFormat::SInt),
        R32G32B32A32_Float   = GFX_MAKE_IMAGE_FORMAT(GfxChannelFormat::R32G32B32A32,  GfxTypeFormat::Float),
    };

    enum class GfxImageStorageDimension : u8 { 
        Type1D = 1,
        Type2D = 2,
        Type3D = 3
    };

    enum class GfxImageDimension : u8 {
        Type1D                      = 0,
        Type2D                      = 1,
        Type3D                      = 2,
        TypeCube                    = 3,
        TypeCube1DArray             = 4,
        TypeCube2DArray             = 5,
        TypeCube2DMultisample       = 6,
        TypeCube2DMultisampleArray  = 7,
        TypeCubeArray               = 8
    };

    struct ResGfxTextureInfo {
        u8  is_packaged         : 1;
        u8  is_sparse_binding   : 1;
        u8  is_sparse           : 1;
        u8  is_res_texture      : 1;
        u8  reserve0            : 4;
        u8  storage_dimension;
        u16 tile_mode;
        u16 swizzle;
        u16 mip_levels;
        u16 sample_count;
        u16 reserve1;
        u32 image_format;
        u32 gpu_access_flags;
        u32 width;
        u32 height;
        u32 depth;
        u32 texture_dimension;
        u32 packaged_texture_layout;
    };
    static_assert(sizeof(ResGfxTextureInfo) == 0x28);

    enum class GfxWrapMode : u8 {
        Repeat            = 0,
        MirrorRepeat      = 1,
        ClampToEdge       = 2,
        ClampToBorder     = 3,
        MirrorClampToEdge = 4
    };
    
    enum class GfxCompareOp : u8 {
        Never            = 0,
        LessThan         = 1,
        Equal            = 2,
        LessThanEqual    = 3,
        GreaterThan      = 4,
        NotEqual         = 5,
        GreaterThanEqual = 6,
        Always           = 7
    };

    enum class GfxBorderColor : u8 {
        White            = 0,
        TransparentBlack = 1,
        Black            = 2,
    };

    enum class GfxMipMapFilter : u8 {
        None    = 0,
        Nearest = 1,
        Linear  = 2
    };

    enum class GfxMinFilter : u8 {
        Invalid = 0,
        Nearest = 1,
        Linear  = 2
    };

    enum class GfxMagFilter : u8 {
        Invalid = 0,
        Nearest = 1,
        Linear  = 2
    };

    enum GfxReductionFilter : u8 {
        Average = 0,
        Min     = 1,
        Max     = 2
    };

    struct ResGfxSamplerInfo {
        u8    wrap_mode_u;
        u8    wrap_mode_v;
        u8    wrap_mode_w;
        u8    compare_op;
        u8    border_color;
        u8    max_anisotropy;
        u16   mip_map_filter    : 2;
        u16   mag_filter        : 2;
        u16   min_filter        : 2;
        u16   enable_anisotropy : 1;
        u16   enable_compare_op : 1;
        u16   reduction_filter  : 2;
        u16   reserve0          : 6;
        float lod_clamp_min;
        float lod_clamp_max;
        float lod_bias;
        u32   reserve1;
        u32   reserve2;
        u32   reserve3;
    };
    static_assert(sizeof(ResGfxSamplerInfo) == 0x20);

    enum class GfxPrimitiveTopology {
        Points                 = 0,
        Lines                  = 1,
        LineStrip              = 2,
        Triangles              = 3,
        TrianglesStrip         = 4,
        LinesAdjacency         = 5,
        LineStripAdjacency     = 6,
        TrianglesAdjacency     = 7,
        TriangleStripAdjacency = 8,
        Patches                = 9,
    };

    enum class GfxPolygonMode {
        Point = 0,
        Fill  = 1,
        Line  = 2
    };

    enum class GfxIndexFormat {
        U8  = 0,
        U16 = 1,
        U32 = 2
    };

    struct ResGfxEmbedFile {
        void *file_offset;
        u32   file_size;
        u32   reserve0;
    };
    static_assert(sizeof(ResGfxEmbedFile) == 0x10);

    enum class GfxUserDataType : u8 {
        S32    = 0,
        Float  = 1,
        String = 2,
        Byte   = 3
    };

    struct ResGfxUserData {
        const char *user_data_name;
        void       *user_data;
        s32         data_entries;
        u8          data_type;
        u8          reserve0[43];
    };
    static_assert(sizeof(ResGfxUserData) == 0x40);
    
    constexpr ALWAYS_INLINE const char *GfxWrapModeToString(GfxWrapMode wrap_mode) {
        switch (wrap_mode) {
            case GfxWrapMode::Repeat:
                return "Repeat";
            case GfxWrapMode::MirrorRepeat:
                return "MirrorRepeat";
            case GfxWrapMode::ClampToEdge:
                return "ClampToEdge";
            case GfxWrapMode::ClampToBorder:
                return "ClamptToBorder";
            case GfxWrapMode::MirrorClampToEdge:
                return "MirrorClampToEdge";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxMipMapFilterToString(GfxMipMapFilter mip_map_filter) {
        switch (mip_map_filter) {
            case GfxMipMapFilter::None:
                return "None";
            case GfxMipMapFilter::Nearest:
                return "Nearest";
            case GfxMipMapFilter::Linear:
                return "Linear";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxMagFilterToString(GfxMagFilter mag_filter) {
        switch (mag_filter) {
            case GfxMagFilter::Nearest:
                return "Nearest";
            case GfxMagFilter::Linear:
                return "Linear";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxMinFilterToString(GfxMinFilter min_filter) {
        switch (min_filter) {
            case GfxMinFilter::Nearest:
                return "Nearest";
            case GfxMinFilter::Linear:
                return "Linear";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxReductionFilterToString(GfxReductionFilter reduction_filter) {
        switch (reduction_filter) {
            case GfxReductionFilter::Average:
                return "Average";
            case GfxReductionFilter::Min:
                return "Min";
            case GfxReductionFilter::Max:
                return "Max";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxCompareOpToString(GfxCompareOp compare_op) {
        switch (compare_op) {
            case GfxCompareOp::Never:
                return "Never";
            case GfxCompareOp::LessThan:
                return "LessThan";
            case GfxCompareOp::Equal:
                return "Equal";
            case GfxCompareOp::LessThanEqual:
                return "LessThanEqual";
            case GfxCompareOp::GreaterThan:
                return "GreaterThan";
            case GfxCompareOp::NotEqual:
                return "NotEqual";
            case GfxCompareOp::GreaterThanEqual:
                return "GreaterThanEqual";
            case GfxCompareOp::Always:
                return "Always";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxBorderColorToString(GfxBorderColor border_color) {
        switch (border_color) {
            case GfxBorderColor::White:
                return "White";
            case GfxBorderColor::TransparentBlack:
                return "TransparentBlack";
            case GfxBorderColor::Black:
                return "Black";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxPrimitiveTopologyToString(GfxPrimitiveTopology primitive_topology) {
        switch (primitive_topology) {
            case GfxPrimitiveTopology::Points:
                return "Points";
            case GfxPrimitiveTopology::Lines:
                return "Lines";
            case GfxPrimitiveTopology::LineStrip:
                return "LineStrip";
            case GfxPrimitiveTopology::Triangles:
                return "Triangles";
            case GfxPrimitiveTopology::TrianglesStrip:
                return "TrianglesStrip";
            case GfxPrimitiveTopology::LinesAdjacency:
                return "LinesAdjacency";
            case GfxPrimitiveTopology::LineStripAdjacency:
                return "LineStripAdjacency";
            case GfxPrimitiveTopology::TrianglesAdjacency:
                return "TrianglesAdjacency";
            case GfxPrimitiveTopology::TriangleStripAdjacency:
                return "TriangleStripAdjacency";
            case GfxPrimitiveTopology::Patches:
                return "Patches";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxIndexFormatToString(GfxIndexFormat index_format) {
        switch (index_format) {
            case GfxIndexFormat::U8:
                return "U8";
            case GfxIndexFormat::U16:
                return "U16";
            case GfxIndexFormat::U32:
                return "U32";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxImageFormatToString(GfxImageFormat image_format) {
        switch (image_format) {
            case GfxImageFormat::R8_Unorm:
                return "R8_Unorm";
            case GfxImageFormat::R8_Snorm:
                return "R8_Snorm";
            case GfxImageFormat::R8_UInt:
                return "R8_UInt";
            case GfxImageFormat::R8_SInt:
                return "R8_SInt";
            case GfxImageFormat::R4G4B4A4_Unorm:
                return "R4G4B4A4_Unorm";
            case GfxImageFormat::R5G5B5A1_Unorm:
                return "R5G5B5A1_Unorm";
            case GfxImageFormat::A1B5G5R5_Unorm:
                return "A1B5G5R5_Unorm";
            case GfxImageFormat::R5G6B5_Unorm:
                return "R5G6B5_Unorm";
            case GfxImageFormat::B5G6R5_Unorm:
                return "B5G6R5_Unorm";
            case GfxImageFormat::R8G8_Unorm:
                return "R8G8_Unorm";
            case GfxImageFormat::R8G8_Snorm:
                return "R8G8_Snorm";
            case GfxImageFormat::R8G8_UInt:
                return "R8G8_UInt";
            case GfxImageFormat::R8G8_SInt:
                return "R8G8_SInt";
            case GfxImageFormat::R16_Unorm:
                return "R16_Unorm";
            case GfxImageFormat::R16_Snorm:
                return "R16_Snorm";
            case GfxImageFormat::R16_UInt:
                return "R16_UInt";
            case GfxImageFormat::R16_SInt:
                return "R16_SInt";
            case GfxImageFormat::R16_Float:
                return "R16_Float";
            case GfxImageFormat::Z16_Depth:
                return "Z16_Depth";
            case GfxImageFormat::R8G8B8A8_Unorm:
                return "R8G8B8A8_Unorm";
            case GfxImageFormat::R8G8B8A8_Snorm:
                return "R8G8B8A8_Snorm";
            case GfxImageFormat::R8G8B8A8_UInt:
                return "R8G8B8A8_UInt";
            case GfxImageFormat::R8G8B8A8_SInt:
                return "R8G8B8A8_SInt";
            case GfxImageFormat::R8G8B8A8_SRGB:
                return "R8G8B8A8_SRGB";
            case GfxImageFormat::B8G8R8A8_Unorm:
                return "B8G8R8A8_Unorm";
            case GfxImageFormat::B8G8R8A8_SRGB:
                return "B8G8R8A8_SRGB";
            case GfxImageFormat::R9G9B9E5F_SharedFloat:
                return "R9G9B9E5F_SharedFloat";
            case GfxImageFormat::R10G10B10A2_Unorm:
                return "R10G10B10A2_Unorm";
            case GfxImageFormat::R10G10B10A2_UInt:
                return "R10G10B10A2_UInt";
            case GfxImageFormat::R11G11B10F_Float:
                return "R11G11B10F_Float";
            case GfxImageFormat::R16G16_Unorm:
                return "R16G16_Unorm";
            case GfxImageFormat::R16G16_Snorm:
                return "R16G16_Snorm";
            case GfxImageFormat::R16G16_UInt:
                return "R16G16_UInt";
            case GfxImageFormat::R16G16_SInt:
                return "R16G16_SInt";
            case GfxImageFormat::R16G16_Float:
                return "R16G16_Float";
            case GfxImageFormat::D24S8_Depth:
                return "D24S8_Depth";
            case GfxImageFormat::R32_UInt:
                return "R32_UInt";
            case GfxImageFormat::R32_SInt:
                return "R32_SInt";
            case GfxImageFormat::R32_Float:
                return "R32_Float";
            case GfxImageFormat::D32F_Depth:
                return "D32F_Depth";
            case GfxImageFormat::R16G16B16A16_Unorm:
                return "R16G16B16A16_Unorm";
            case GfxImageFormat::R16G16B16A16_Snorm:
                return "R16G16B16A16_Snorm";
            case GfxImageFormat::R16G16B16A16_UInt:
                return "R16G16B16A16_UInt";
            case GfxImageFormat::R16G16B16A16_SInt:
                return "R16G16B16A16_SInt";
            case GfxImageFormat::R16G16B16A16_Float:
                return "R16G16B16A16_Float";
            case GfxImageFormat::D32FS8_Depth:
                return "D32FS8_Depth";
            case GfxImageFormat::R32G32_UInt:
                return "R32G32_UInt";
            case GfxImageFormat::R32G32_SInt:
                return "R32G32_SInt";
            case GfxImageFormat::R32G32_Float:
                return "R32G32_Float";
            case GfxImageFormat::R32G32B32_UInt:
                return "R32G32B32_UInt";
            case GfxImageFormat::R32G32B32_SInt:
                return "R32G32B32_SInt";
            case GfxImageFormat::R32G32B32_Float:
                return "R32G32B32_Float";
            case GfxImageFormat::R32G32B32A32_UInt:
                return "R32G32B32A32_UInt";
            case GfxImageFormat::R32G32B32A32_SInt:
                return "R32G32B32A32_SInt";
            case GfxImageFormat::R32G32B32A32_Float:
                return "R32G32B32A32_Float";
            case GfxImageFormat::BC1_Unorm:
                return "BC1_Unorm";
            case GfxImageFormat::BC1_SRGB:
                return "BC1_SRGB";
            case GfxImageFormat::BC3_Unorm:
                return "BC3_Unorm";
            case GfxImageFormat::BC3_SRGB:
                return "BC3_SRGB";
            case GfxImageFormat::BC4_Unorm:
                return "BC4_Unorm";
            case GfxImageFormat::BC4_Snorm:
                return "BC4_Snorm";
            case GfxImageFormat::BC5_Unorm:
                return "BC5_Unorm";
            case GfxImageFormat::BC5_Snorm:
                return "BC5_Snorm";
            case GfxImageFormat::BC6H_SF16:
                return "BC6H_SF16";
            case GfxImageFormat::BC6H_UF16:
                return "BC6H_UF16";
            case GfxImageFormat::BC7U_Unorm:
                return "BC7U_Unorm";
            case GfxImageFormat::BC7U_SRGB:
                return "BC7U_SRGB";
            case GfxImageFormat::ASTC_4X4_Unorm:
                return "ASTC_4X4_Unorm";
            case GfxImageFormat::ASTC_4X4_SRGB:
                return "ASTC_4X4_SRGB";
            case GfxImageFormat::ASTC_5X4_Unorm:
                return "ASTC_5X4_Unorm";
            case GfxImageFormat::ASTC_5X4_SRGB:
                return "ASTC_5X4_SRGB";
            case GfxImageFormat::ASTC_5X5_Unorm:
                return "ASTC_5X5_Unorm";
            case GfxImageFormat::ASTC_5X5_SRGB:
                return "ASTC_5X5_SRGB";
            case GfxImageFormat::ASTC_6X5_Unorm:
                return "ASTC_6X5_Unorm";
            case GfxImageFormat::ASTC_6X5_SRGB:
                return "ASTC_6X5_SRGB";
            case GfxImageFormat::ASTC_6X6_Unorm:
                return "ASTC_6X6_Unorm";
            case GfxImageFormat::ASTC_6X6_SRGB:
                return "ASTC_6X6_SRGB";
            case GfxImageFormat::ASTC_8X5_Unorm:
                return "ASTC_8X5_Unorm";
            case GfxImageFormat::ASTC_8X5_SRGB:
                return "ASTC_8X5_SRGB";
            case GfxImageFormat::ASTC_8X6_Unorm:
                return "ASTC_8X6_Unorm";
            case GfxImageFormat::ASTC_8X6_SRGB:
                return "ASTC_8X6_SRGB";
            case GfxImageFormat::ASTC_8X8_Unorm:
                return "ASTC_8X8_Unorm";
            case GfxImageFormat::ASTC_8X8_SRGB:
                return "ASTC_8X8_SRGB";
            case GfxImageFormat::ASTC_10X5_Unorm:
                return "ASTC_10X5_Unorm";
            case GfxImageFormat::ASTC_10X5_SRGB:
                return "ASTC_10X5_SRGB";
            case GfxImageFormat::ASTC_10X6_Unorm:
                return "ASTC_10X6_Unorm";
            case GfxImageFormat::ASTC_10X6_SRGB:
                return "ASTC_10X6_SRGB";
            case GfxImageFormat::ASTC_10X8_Unorm:
                return "ASTC_10X8_Unorm";
            case GfxImageFormat::ASTC_10X8_SRGB:
                return "ASTC_10X8_SRGB";
            case GfxImageFormat::ASTC_10X10_Unorm:
                return "ASTC_10X10_Unorm";
            case GfxImageFormat::ASTC_10X10_SRGB:
                return "ASTC_10X10_SRGB";
            case GfxImageFormat::ASTC_12X10_Unorm:
                return "ASTC_12X10_Unorm";
            case GfxImageFormat::ASTC_12X10_SRGB:
                return "ASTC_12X10_SRGB";
            case GfxImageFormat::ASTC_12X12_Unorm:
                return "ASTC_12X12_Unorm";
            case GfxImageFormat::ASTC_12X12_SRGB:
                return "ASTC_12X12_SRGB";
            case GfxImageFormat::B5G5R5A1_Unorm:
                return "B5G5R5A1_Unorm";
            default:
                break;
        }
        return "Invalid";
    }

    constexpr ALWAYS_INLINE const char *GfxAttributeFormatToString(GfxAttributeFormat attribute_format) {
        switch (attribute_format) {
            case GfxAttributeFormat::None_Unorm:
                return "None_Unorm";
            case GfxAttributeFormat::R8_Unorm:
                return "R8_Unorm";
            case GfxAttributeFormat::R8_Snorm:
                return "R8_Snorm";
            case GfxAttributeFormat::R8_UInt:
                return "R8_UInt";
            case GfxAttributeFormat::R8_SInt:
                return "R8_SInt";
            case GfxAttributeFormat::R8_UScaled:
                return "R8_UScaled";
            case GfxAttributeFormat::R8_SScaled:
                return "R8_SScaled";
            case GfxAttributeFormat::R8G8_Unorm:
                return "R8G8_Unorm";
            case GfxAttributeFormat::R8G8_Snorm:
                return "R8G8_Snorm";
            case GfxAttributeFormat::R8G8_UInt:
                return "R8G8_UInt";
            case GfxAttributeFormat::R8G8_SInt:
                return "R8G8_SInt";
            case GfxAttributeFormat::R8G8_UScaled:
                return "R8G8_UScaled";
            case GfxAttributeFormat::R8G8_SScaled:
                return "R8G8_SScaled";
            case GfxAttributeFormat::R16_Unorm:
                return "R16_Unorm";
            case GfxAttributeFormat::R16_Snorm:
                return "R16_Snorm";
            case GfxAttributeFormat::R16_UInt:
                return "R16_UInt";
            case GfxAttributeFormat::R16_SInt:
                return "R16_SInt";
            case GfxAttributeFormat::R16_Float:
                return "R16_Float";
            case GfxAttributeFormat::R16_UScaled:
                return "R16_UScaled";
            case GfxAttributeFormat::R16_SScaled:
                return "R16_SScaled";
            case GfxAttributeFormat::R8G8B8A8_Unorm:
                return "R8G8B8A8_Unorm";
            case GfxAttributeFormat::R8G8B8A8_Snorm:
                return "R8G8B8A8_Snorm";
            case GfxAttributeFormat::R8G8B8A8_UInt:
                return "R8G8B8A8_UInt";
            case GfxAttributeFormat::R8G8B8A8_SInt:
                return "R8G8B8A8_SInt";
            case GfxAttributeFormat::R8G8B8A8_UScaled:
                return "R8G8B8A8_UScaled";
            case GfxAttributeFormat::R8G8B8A8_SScaled:
                return "R8G8B8A8_SScaled";
            case GfxAttributeFormat::R10G10B10A2_Unorm:
                return "R10G10B10A2_Unorm";
            case GfxAttributeFormat::R10G10B10A2_Snorm:
                return "R10G10B10A2_Snorm";
            case GfxAttributeFormat::R10G10B10A2_UInt:
                return "R10G10B10A2_UInt";
            case GfxAttributeFormat::R10G10B10A2_SInt:
                return "R10G10B10A2_SInt";
            case GfxAttributeFormat::R16G16_Unorm:
                return "R16G16_Unorm";
            case GfxAttributeFormat::R16G16_Snorm:
                return "R16G16_Snorm";
            case GfxAttributeFormat::R16G16_UInt:
                return "R16G16_UInt";
            case GfxAttributeFormat::R16G16_SInt:
                return "R16G16_SInt";
            case GfxAttributeFormat::R16G16_Float:
                return "R16G16_Float";
            case GfxAttributeFormat::R16G16_UScaled:
                return "R16G16_UScaled";
            case GfxAttributeFormat::R16G16_SScaled:
                return "R16G16_SScaled";
            case GfxAttributeFormat::R32_UInt:
                return "R32_UInt";
            case GfxAttributeFormat::R32_SInt:
                return "R32_SInt";
            case GfxAttributeFormat::R32_Float:
                return "R32_Float";
            case GfxAttributeFormat::R16G16B16A16_Unorm:
                return "R16G16B16A16_Unorm";
            case GfxAttributeFormat::R16G16B16A16_Snorm:
                return "R16G16B16A16_Snorm";
            case GfxAttributeFormat::R16G16B16A16_UInt:
                return "R16G16B16A16_UInt";
            case GfxAttributeFormat::R16G16B16A16_SInt:
                return "R16G16B16A16_SInt";
            case GfxAttributeFormat::R16G16B16A16_Float:
                return "R16G16B16A16_Float";
            case GfxAttributeFormat::R16G16B16A16_UScaled:
                return "R16G16B16A16_UScaled";
            case GfxAttributeFormat::R16G16B16A16_SScaled:
                return "R16G16B16A16_SScaled";
            case GfxAttributeFormat::R32G32_UInt:
                return "R32G32_UInt";
            case GfxAttributeFormat::R32G32_SInt:
                return "R32G32_SInt";
            case GfxAttributeFormat::R32G32_Float:
                return "R32G32_Float";
            case GfxAttributeFormat::R32G32B32_UInt:
                return "R32G32B32_UInt";
            case GfxAttributeFormat::R32G32B32_SInt:
                return "R32G32B32_SInt";
            case GfxAttributeFormat::R32G32B32_Float:
                return "R32G32B32_Float";
            case GfxAttributeFormat::R32G32B32A32_UInt:
                return "R32G32B32A32_UInt";
            case GfxAttributeFormat::R32G32B32A32_SInt:
                return "R32G32B32A32_SInt";
            case GfxAttributeFormat::R32G32B32A32_Float:
                return "R32G32B32A32_Float";
            default:
                break;
        }
        return "Invalid";
    }
}
